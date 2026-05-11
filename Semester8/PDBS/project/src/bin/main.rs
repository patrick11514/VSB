use std::error::Error;
use std::io::{self, Stdout};
use std::time::Duration;

use crossterm::cursor::{Hide, Show};
use crossterm::event::{self, Event, KeyCode, KeyEvent};
use crossterm::execute;
use crossterm::terminal::{
    disable_raw_mode, enable_raw_mode, EnterAlternateScreen, LeaveAlternateScreen,
};
use ratatui::backend::CrosstermBackend;
use ratatui::prelude::*;
use ratatui::widgets::{Block, Borders, Clear, List, ListItem, ListState, Paragraph};
use reqwest::header::USER_AGENT;
use serde::Deserialize;
use tokio::task::JoinHandle;

const NOMINATIM_USER_AGENT: &str = "StellaDB-CLI (patrik.mintel.st@vsb.cz)";

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum AppState {
    SetupLocation,
    TownInput,
    Resolving,
    Ready,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum LocationChoice {
    Ip,
    Town,
}

impl LocationChoice {
    fn toggle(self) -> Self {
        match self {
            Self::Ip => Self::Town,
            Self::Town => Self::Ip,
        }
    }

    fn index(self) -> usize {
        match self {
            Self::Ip => 0,
            Self::Town => 1,
        }
    }
}

#[derive(Debug, Clone)]
struct ObserverLocation {
    label: String,
    latitude: f64,
    longitude: f64,
    source: String,
}

#[derive(Debug)]
struct App {
    state: AppState,
    selected_choice: LocationChoice,
    town_input: String,
    location: Option<ObserverLocation>,
    status: String,
    lookup_task: Option<JoinHandle<Result<ObserverLocation, String>>>,
}

impl App {
    fn new() -> Self {
        Self {
            state: AppState::SetupLocation,
            selected_choice: LocationChoice::Ip,
            town_input: String::new(),
            location: None,
            status: String::from("Choose how to resolve the observer location."),
            lookup_task: None,
        }
    }

    fn start_lookup(&mut self, task: JoinHandle<Result<ObserverLocation, String>>) {
        self.lookup_task = Some(task);
        self.state = AppState::Resolving;
    }

    fn reset_to_setup(&mut self) {
        self.state = AppState::SetupLocation;
        self.lookup_task = None;
    }

    async fn poll_lookup(&mut self) {
        let Some(task) = self.lookup_task.as_ref() else {
            return;
        };

        if !task.is_finished() {
            return;
        }

        let Some(task) = self.lookup_task.take() else {
            return;
        };

        match task.await {
            Ok(Ok(location)) => {
                self.status = format!(
                    "Resolved {} at lat {:.4}, lon {:.4}.",
                    location.label, location.latitude, location.longitude
                );
                self.location = Some(location);
                self.state = AppState::Ready;
            }
            Ok(Err(error)) => {
                self.status = error;
                self.state = AppState::SetupLocation;
            }
            Err(error) => {
                self.status = format!("Lookup task failed: {error}");
                self.state = AppState::SetupLocation;
            }
        }
    }
}

#[derive(Debug, Deserialize)]
struct IpLookupResponse {
    success: bool,
    latitude: f64,
    longitude: f64,
    #[serde(default)]
    city: Option<String>,
    #[serde(default)]
    region: Option<String>,
    #[serde(default)]
    country: Option<String>,
    #[serde(default)]
    message: Option<String>,
}

#[derive(Debug, Deserialize)]
struct NominatimResult {
    lat: String,
    lon: String,
    display_name: String,
}

struct TerminalGuard;

impl Drop for TerminalGuard {
    fn drop(&mut self) {
        let _ = disable_raw_mode();
        let mut stdout = io::stdout();
        let _ = execute!(stdout, LeaveAlternateScreen, Show);
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let _guard = setup_terminal()?;
    let mut terminal = create_terminal()?;
    let mut app = App::new();

    loop {
        app.poll_lookup().await;

        terminal.draw(|frame| render(frame, &app))?;

        if event::poll(Duration::from_millis(100))? {
            let event = event::read()?;
            if handle_events(event, &mut app).await? {
                break;
            }
        }
    }

    Ok(())
}

fn setup_terminal() -> Result<TerminalGuard, Box<dyn Error>> {
    enable_raw_mode()?;
    let mut stdout = io::stdout();
    execute!(stdout, EnterAlternateScreen, Hide)?;
    Ok(TerminalGuard)
}

fn create_terminal() -> Result<Terminal<CrosstermBackend<Stdout>>, Box<dyn Error>> {
    let stdout = io::stdout();
    let backend = CrosstermBackend::new(stdout);
    Ok(Terminal::new(backend)?)
}

fn render(frame: &mut Frame<'_>, app: &App) {
    let area = frame.area();

    match app.state {
        AppState::SetupLocation => render_setup_location(frame, area, app),
        AppState::TownInput => render_town_input(frame, area, app),
        AppState::Resolving => render_resolving(frame, area, app),
        AppState::Ready => render_ready(frame, area, app),
    }
}

fn render_setup_location(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let popup = centered_rect(60, 40, area);
    frame.render_widget(Clear, popup);

    let inner = Block::default()
        .title("Observer Location")
        .borders(Borders::ALL);
    frame.render_widget(inner, popup);

    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .constraints([
            Constraint::Length(3),
            Constraint::Min(4),
            Constraint::Length(3),
        ])
        .margin(1)
        .split(popup);

    frame.render_widget(
        Paragraph::new("Choose how to resolve the observer location.")
            .alignment(Alignment::Center),
        chunks[0],
    );

    let items = [
        ListItem::new("IP geolocation"),
        ListItem::new("Town search via Nominatim"),
    ];
    let mut list_state = ListState::default();
    list_state.select(Some(app.selected_choice.index()));

    let list = List::new(items)
        .block(Block::default().borders(Borders::ALL).title("Lookup mode"))
        .highlight_style(Style::default().fg(Color::Yellow).add_modifier(Modifier::BOLD))
        .highlight_symbol("> ");
    frame.render_stateful_widget(list, chunks[1], &mut list_state);

    let help = Line::from(vec![
        Span::styled("Enter ", Style::default().fg(Color::Green)),
        Span::raw("to confirm, arrows/tab to switch, q to quit"),
    ]);
    frame.render_widget(Paragraph::new(help).alignment(Alignment::Center), chunks[2]);

    if !app.status.is_empty() {
        let status_area = Rect {
            x: popup.x,
            y: popup.bottom().saturating_add(1),
            width: popup.width,
            height: 2,
        };
        frame.render_widget(
            Paragraph::new(app.status.as_str())
                .alignment(Alignment::Center)
                .style(Style::default().fg(Color::DarkGray)),
            status_area,
        );
    }
}

fn render_town_input(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let popup = centered_rect(72, 34, area);
    frame.render_widget(Clear, popup);

    let block = Block::default()
        .title("Town Lookup")
        .borders(Borders::ALL);
    frame.render_widget(block, popup);

    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .constraints([
            Constraint::Length(3),
            Constraint::Length(3),
            Constraint::Min(2),
        ])
        .margin(1)
        .split(popup);

    frame.render_widget(
        Paragraph::new("Type a town or city name, then press Enter.")
            .alignment(Alignment::Center),
        chunks[0],
    );

    let input = Paragraph::new(app.town_input.as_str())
        .block(Block::default().borders(Borders::ALL).title("City / town"));
    frame.render_widget(input, chunks[1]);

    let help = Paragraph::new("Enter to search, Esc to go back, Backspace to edit")
        .alignment(Alignment::Center)
        .style(Style::default().fg(Color::DarkGray));
    frame.render_widget(help, chunks[2]);
}

fn render_resolving(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let popup = centered_rect(50, 24, area);
    frame.render_widget(Clear, popup);

    let block = Block::default()
        .title("Resolving Location")
        .borders(Borders::ALL);
    frame.render_widget(block, popup);

    let text = match app.selected_choice {
        LocationChoice::Ip => "Looking up your location from IP address...",
        LocationChoice::Town => "Searching the town in Nominatim...",
    };

    frame.render_widget(
        Paragraph::new(text)
            .alignment(Alignment::Center)
            .block(Block::default()),
        popup.inner(Margin::new(1, 1)),
    );
}

fn render_ready(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let block = Block::default()
        .title("Location Ready")
        .borders(Borders::ALL);
    frame.render_widget(block, area);

    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .constraints([
            Constraint::Length(5),
            Constraint::Length(5),
            Constraint::Min(2),
        ])
        .margin(2)
        .split(area);

    if let Some(location) = &app.location {
        let details = vec![
            Line::from(format!("Label: {}", location.label)),
            Line::from(format!("Latitude: {:.6}", location.latitude)),
            Line::from(format!("Longitude: {:.6}", location.longitude)),
            Line::from(format!("Source: {}", location.source)),
        ];
        frame.render_widget(
            Paragraph::new(details).block(Block::default().title("Stored coordinates").borders(Borders::ALL)),
            chunks[0],
        );
    }

    frame.render_widget(
        Paragraph::new(app.status.as_str())
            .block(Block::default().title("Status").borders(Borders::ALL)),
        chunks[1],
    );

    frame.render_widget(
        Paragraph::new("Press r to choose a different location lookup or q to quit.")
            .alignment(Alignment::Center)
            .style(Style::default().fg(Color::DarkGray)),
        chunks[2],
    );
}

async fn handle_events(event: Event, app: &mut App) -> Result<bool, Box<dyn Error>> {
    match event {
        Event::Key(key) => return handle_key_event(key, app).await,
        Event::Resize(_, _) => {}
        _ => {}
    }

    Ok(false)
}

async fn handle_key_event(key: KeyEvent, app: &mut App) -> Result<bool, Box<dyn Error>> {
    match app.state {
        AppState::SetupLocation => match key.code {
            KeyCode::Char('q') | KeyCode::Esc => return Ok(true),
            KeyCode::Up | KeyCode::Down | KeyCode::Left | KeyCode::Right | KeyCode::Tab => {
                app.selected_choice = app.selected_choice.toggle();
            }
            KeyCode::Char('i') | KeyCode::Char('I') => {
                app.selected_choice = LocationChoice::Ip;
                app.status = String::from("Resolving location from IP address...");
                app.start_lookup(tokio::spawn(async { lookup_ip_location().await }));
            }
            KeyCode::Char('m') | KeyCode::Char('M') => {
                app.selected_choice = LocationChoice::Town;
                app.state = AppState::TownInput;
                app.status = String::from("Type a town name and press Enter.");
            }
            KeyCode::Enter => match app.selected_choice {
                LocationChoice::Ip => {
                    app.status = String::from("Resolving location from IP address...");
                    app.start_lookup(tokio::spawn(async { lookup_ip_location().await }));
                }
                LocationChoice::Town => {
                    app.state = AppState::TownInput;
                    app.status = String::from("Type a town name and press Enter.");
                }
            },
            _ => {}
        },
        AppState::TownInput => match key.code {
            KeyCode::Esc => {
                app.reset_to_setup();
                app.status = String::from("Choose how to resolve the observer location.");
            }
            KeyCode::Enter => {
                let query = app.town_input.trim().to_string();
                if query.is_empty() {
                    app.status = String::from("Enter a town or city name first.");
                } else {
                    app.status = format!("Searching for {query}...");
                    app.start_lookup(tokio::spawn(async move { lookup_town_location(&query).await }));
                }
            }
            KeyCode::Backspace => {
                app.town_input.pop();
            }
            KeyCode::Char(c) if !c.is_control() => {
                app.town_input.push(c);
            }
            _ => {}
        },
        AppState::Resolving => match key.code {
            KeyCode::Char('q') | KeyCode::Esc => return Ok(true),
            _ => {}
        },
        AppState::Ready => match key.code {
            KeyCode::Char('q') | KeyCode::Esc => return Ok(true),
            KeyCode::Char('r') | KeyCode::Char('R') => {
                app.location = None;
                app.town_input.clear();
                app.selected_choice = LocationChoice::Ip;
                app.reset_to_setup();
                app.status = String::from("Choose how to resolve the observer location.");
            }
            _ => {}
        },
    }

    Ok(false)
}

async fn lookup_ip_location() -> Result<ObserverLocation, String> {
    let client = reqwest::Client::builder()
        .timeout(Duration::from_secs(10))
        .build()
        .map_err(|error| format!("Failed to build HTTP client: {error}"))?;

    let response = client
        .get("https://ipwho.is/")
        .send()
        .await
        .map_err(|error| format!("IP lookup request failed: {error}"))?;

    if !response.status().is_success() {
        return Err(format!("IP lookup returned HTTP {}", response.status()));
    }

    let payload: IpLookupResponse = response
        .json()
        .await
        .map_err(|error| format!("Failed to parse IP lookup response: {error}"))?;

    if !payload.success {
        return Err(
            payload
                .message
                .unwrap_or_else(|| String::from("IP lookup service reported failure")),
        );
    }

    let mut label = payload.city.unwrap_or_else(|| String::from("IP location"));

    if let Some(region) = payload.region.filter(|region| !region.is_empty()) {
        label = format!("{label}, {region}");
    }

    if let Some(country) = payload.country.filter(|country| !country.is_empty()) {
        label = format!("{label}, {country}");
    }

    Ok(ObserverLocation {
        label,
        latitude: payload.latitude,
        longitude: payload.longitude,
        source: String::from("ipwho.is"),
    })
}

async fn lookup_town_location(query: &str) -> Result<ObserverLocation, String> {
    let client = reqwest::Client::builder()
        .timeout(Duration::from_secs(10))
        .build()
        .map_err(|error| format!("Failed to build HTTP client: {error}"))?;

    let response = client
        .get("https://nominatim.openstreetmap.org/search")
        .header(USER_AGENT, NOMINATIM_USER_AGENT)
        .query(&[("format", "jsonv2"), ("limit", "1"), ("q", query)])
        .send()
        .await
        .map_err(|error| format!("Town search request failed: {error}"))?;

    if !response.status().is_success() {
        return Err(format!("Town search returned HTTP {}", response.status()));
    }

    let results: Vec<NominatimResult> = response
        .json()
        .await
        .map_err(|error| format!("Failed to parse town search response: {error}"))?;

    let first = results
        .into_iter()
        .next()
        .ok_or_else(|| format!("No town match found for '{query}'"))?;

    let latitude = first
        .lat
        .parse::<f64>()
        .map_err(|error| format!("Failed to parse latitude: {error}"))?;
    let longitude = first
        .lon
        .parse::<f64>()
        .map_err(|error| format!("Failed to parse longitude: {error}"))?;

    Ok(ObserverLocation {
        label: first.display_name,
        latitude,
        longitude,
        source: String::from("OpenStreetMap Nominatim"),
    })
}

fn centered_rect(percent_x: u16, percent_y: u16, area: Rect) -> Rect {
    let vertical = Layout::default()
        .direction(Direction::Vertical)
        .constraints([
            Constraint::Percentage((100 - percent_y) / 2),
            Constraint::Percentage(percent_y),
            Constraint::Percentage((100 - percent_y) / 2),
        ])
        .split(area);

    Layout::default()
        .direction(Direction::Horizontal)
        .constraints([
            Constraint::Percentage((100 - percent_x) / 2),
            Constraint::Percentage(percent_x),
            Constraint::Percentage((100 - percent_x) / 2),
        ])
        .split(vertical[1])[1]
}
