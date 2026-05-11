use std::error::Error;
use std::io::{self, Stdout};
use std::time::Duration;

use chrono::{DateTime, Duration as ChronoDuration, Utc};
use crossterm::cursor::{Hide, Show};
use crossterm::event::{self, Event, KeyCode, KeyEvent};
use crossterm::execute;
use crossterm::terminal::{
    EnterAlternateScreen, LeaveAlternateScreen, disable_raw_mode, enable_raw_mode,
};
use mongodb::{Client, Collection};
use ratatui::backend::CrosstermBackend;
use ratatui::prelude::*;
use ratatui::symbols::Marker;
use ratatui::widgets::canvas::{Canvas, Points};
use ratatui::widgets::{
    Axis, Block, Borders, Cell, Chart, Clear, Dataset, GraphType, List, ListItem, ListState,
    Paragraph, Row, Table, TableState,
};

use reqwest::header::USER_AGENT;
use serde::Deserialize;
use tokio::task::JoinHandle;

use project::{SkyObject, altitude_deg, lst_deg_at_unix_seconds, search_objects};

const NOMINATIM_USER_AGENT: &str = "StellaDB-CLI (patrik.mintel.st@vsb.cz)";
const DEFAULT_MONGO_URI: &str = "mongodb://10.10.10.232:27017";
const DEFAULT_DATABASE: &str = "celestial_data";
const DEFAULT_COLLECTION: &str = "objects";

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum AppState {
    SetupLocation,
    TownInput,
    Resolving,
    SearchByName,
    ObjectDetails,
    SearchAround,
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
    collection: Collection<SkyObject>,
    search_query: String,
    search_results: Vec<SkyObject>,
    search_selected: usize,
    search_task: Option<JoinHandle<Result<Vec<SkyObject>, String>>>,
    active_target: Option<SkyObject>,
    simulated_time: DateTime<Utc>,
    search_around_radius_input: String,
    search_around_magnitude_limit: Option<f64>,
    search_around_results: Vec<(SkyObject, f64)>,
    search_around_selected: usize,
    search_around_task: Option<JoinHandle<Result<Vec<(SkyObject, f64)>, String>>>,
    search_around_is_map_mode: bool,
    search_around_radius_hint: f64,
    search_around_mag_input: String,
    search_around_catalog_input: String,
    search_around_focus: u8, // 0 = Radius, 1 = Magnitude, 2 = Results
}

impl App {
    fn new(collection: Collection<SkyObject>) -> Self {
        Self {
            state: AppState::SetupLocation,
            selected_choice: LocationChoice::Ip,
            town_input: String::new(),
            location: None,
            status: String::from("Choose how to resolve the observer location."),
            lookup_task: None,
            collection,
            search_query: String::new(),
            search_results: Vec::new(),
            search_selected: 0,
            search_task: None,
            active_target: None,
            simulated_time: Utc::now(),
            search_around_radius_input: "10.0".to_string(),
            search_around_magnitude_limit: None,
            search_around_results: Vec::new(),
            search_around_selected: 0,
            search_around_task: None,
            search_around_is_map_mode: false,
            search_around_radius_hint: 10.0,
            search_around_mag_input: String::new(),
            search_around_catalog_input: String::new(),
            search_around_focus: 0,
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

    fn set_active_target(&mut self, target: SkyObject) {
        self.active_target = Some(target);
        self.simulated_time = Utc::now();
        self.state = AppState::ObjectDetails;
        self.status = String::from("Object selected.");
    }

    fn start_search(&mut self) {
        if let Some(task) = self.search_task.take() {
            task.abort();
        }

        let query = self.search_query.trim().to_string();
        if query.is_empty() {
            self.search_results.clear();
            self.search_selected = 0;
            self.status = String::from("Type to search the MongoDB object collection.");
            return;
        }

        self.status = format!("Searching for '{query}'...");
        let collection = self.collection.clone();
        self.search_task = Some(tokio::spawn(async move {
            let results = search_objects(&collection, &query, None, 50)
                .await
                .map_err(|error| error.to_string())?;
            Ok(results)
        }));
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
                self.location = Some(location);
                self.state = AppState::SearchByName;
                self.status = String::from("Type to search the MongoDB object collection.");
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

    async fn poll_search(&mut self) {
        let Some(task) = self.search_task.as_ref() else {
            return;
        };

        if !task.is_finished() {
            return;
        }

        let Some(task) = self.search_task.take() else {
            return;
        };

        match task.await {
            Ok(Ok(results)) => {
                self.search_results = results;
                self.search_selected = 0;
                self.status = format!(
                    "Found {} object(s) for '{}'.",
                    self.search_results.len(),
                    self.search_query.trim()
                );
            }
            Ok(Err(error)) => {
                self.search_results.clear();
                self.search_selected = 0;
                self.status = error;
            }
            Err(error) => {
                self.search_results.clear();
                self.search_selected = 0;
                self.status = format!("Search task failed: {error}");
            }
        }
    }

    fn selected_observer_longitude(&self) -> f64 {
        self.location
            .as_ref()
            .map(|location| location.longitude)
            .unwrap_or(0.0)
    }

    fn selected_observer_latitude(&self) -> f64 {
        self.location
            .as_ref()
            .map(|location| location.latitude)
            .unwrap_or(0.0)
    }

    fn current_lst_deg(&self) -> f64 {
        lst_deg_at_unix_seconds(
            self.selected_observer_longitude(),
            datetime_to_unix_seconds(self.simulated_time),
        )
    }

    fn start_search_around(&mut self, radius: f64, magnitude_limit: Option<f64>) {
        let Some(target) = self.active_target.as_ref() else {
            self.status = String::from("No object selected.");
            return;
        };

        if radius <= 0.0 || radius > 180.0 {
            self.status = String::from("Radius must be between 0° and 180°.");
            return;
        }

        self.status = format!("Searching nearby objects within {:.1}°...", radius);
        self.search_around_radius_hint = radius;
        self.search_around_magnitude_limit = magnitude_limit;

        // WE REMOVED the lines that reset the map mode and selection here!

        let collection = self.collection.clone();
        let center_lon = target.normalized_longitude();
        let center_lat = target.dec_deg;
        let target_designation = target.designation.clone();
        let input = self.search_around_catalog_input.trim().to_string();

        self.search_around_task = Some(tokio::spawn(async move {
            use project::geonear_objects;
            let results = geonear_objects(
                &collection,
                center_lon,
                center_lat,
                radius,
                magnitude_limit,
                100,
                Some(&target_designation),
                if input.is_empty() {
                    None
                } else {
                    Some(input.as_str())
                },
            )
            .await
            .map_err(|error| error.to_string())?;
            Ok(results)
        }));

        self.state = AppState::SearchAround;
    }

    async fn poll_search_around(&mut self) {
        let Some(task) = self.search_around_task.as_ref() else {
            return;
        };
        if !task.is_finished() {
            return;
        }
        let Some(task) = self.search_around_task.take() else {
            return;
        };

        match task.await {
            Ok(Ok(results)) => {
                self.search_around_results = results;

                // Prevent out-of-bounds crash if the new search has fewer results
                if self.search_around_selected >= self.search_around_results.len() {
                    self.search_around_selected =
                        self.search_around_results.len().saturating_sub(1);
                }

                self.status = format!(
                    "Found {} object(s) within {:.1}°.",
                    self.search_around_results.len(),
                    self.search_around_radius_hint
                );
            }
            Ok(Err(error)) => {
                self.search_around_results.clear();
                self.search_around_selected = 0;
                self.status = error;
            }
            Err(error) => {
                self.search_around_results.clear();
                self.search_around_selected = 0;
                self.status = format!("Search around task failed: {error}");
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
    let client = Client::with_uri_str(DEFAULT_MONGO_URI).await?;
    let collection = client
        .database(DEFAULT_DATABASE)
        .collection::<SkyObject>(DEFAULT_COLLECTION);

    let _guard = setup_terminal()?;
    let mut terminal = create_terminal()?;
    let mut app = App::new(collection);

    loop {
        app.poll_lookup().await;
        app.poll_search().await;
        app.poll_search_around().await;

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
        AppState::SearchByName => render_search_by_name(frame, area, app),
        AppState::ObjectDetails => render_object_details(frame, area, app),
        AppState::SearchAround => render_search_around(frame, area, app),
    }
}

fn render_setup_location(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let popup = centered_rect(60, 40, area);
    frame.render_widget(Clear, popup);

    let inner = Block::default()
        .title(Span::styled(
            "Observer Location",
            Style::default()
                .fg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        ))
        .border_style(Style::default().fg(Color::Cyan))
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
            .alignment(Alignment::Center)
            .style(
                Style::default()
                    .fg(Color::White)
                    .add_modifier(Modifier::BOLD),
            ),
        chunks[0],
    );

    let items = [
        ListItem::new("IP geolocation"),
        ListItem::new("Town search via Nominatim"),
    ];
    let mut list_state = ListState::default();
    list_state.select(Some(app.selected_choice.index()));

    let list = List::new(items)
        .block(
            Block::default()
                .borders(Borders::ALL)
                .title(Span::styled(
                    "Lookup mode",
                    Style::default()
                        .fg(Color::Magenta)
                        .add_modifier(Modifier::BOLD),
                ))
                .border_style(Style::default().fg(Color::Blue)),
        )
        .highlight_style(
            Style::default()
                .fg(Color::Black)
                .bg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        )
        .highlight_symbol("> ");
    frame.render_stateful_widget(list, chunks[1], &mut list_state);

    let help = Line::from(vec![
        Span::styled(
            "Enter ",
            Style::default()
                .fg(Color::Green)
                .add_modifier(Modifier::BOLD),
        ),
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
        .title(Span::styled(
            "Town Lookup",
            Style::default()
                .fg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        ))
        .border_style(Style::default().fg(Color::Cyan))
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
            .alignment(Alignment::Center)
            .style(Style::default().fg(Color::White)),
        chunks[0],
    );

    let input = Paragraph::new(app.town_input.as_str())
        .style(Style::default().fg(Color::Cyan))
        .block(
            Block::default()
                .borders(Borders::ALL)
                .title(Span::styled(
                    "City / town",
                    Style::default()
                        .fg(Color::Magenta)
                        .add_modifier(Modifier::BOLD),
                ))
                .border_style(Style::default().fg(Color::Blue)),
        );
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
        .title(Span::styled(
            "Resolving Location",
            Style::default()
                .fg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        ))
        .border_style(Style::default().fg(Color::Green))
        .borders(Borders::ALL);
    frame.render_widget(block, popup);

    let text = match app.selected_choice {
        LocationChoice::Ip => "Looking up your location from IP address...",
        LocationChoice::Town => "Searching the town in Nominatim...",
    };

    frame.render_widget(
        Paragraph::new(text)
            .alignment(Alignment::Center)
            .style(
                Style::default()
                    .fg(Color::Green)
                    .add_modifier(Modifier::BOLD),
            )
            .block(Block::default()),
        popup.inner(Margin::new(1, 1)),
    );
}

fn render_search_by_name(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let block = Block::default()
        .title(Span::styled(
            "Search by Name",
            Style::default()
                .fg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        ))
        .border_style(Style::default().fg(Color::Cyan))
        .borders(Borders::ALL);
    frame.render_widget(block, area);

    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .constraints([Constraint::Percentage(10), Constraint::Percentage(90)])
        .margin(2)
        .split(area);

    let observer_line = if let Some(location) = &app.location {
        format!(
            "Observer: {} | {:.4}, {:.4} | {}",
            location.label, location.latitude, location.longitude, location.source
        )
    } else {
        String::from("Observer location unavailable")
    };

    let search_bar = Paragraph::new(vec![
        Line::from(Span::styled(
            observer_line,
            Style::default().fg(Color::Cyan),
        )),
        Line::from(vec![
            Span::styled(
                "Search: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(app.search_query.as_str(), Style::default().fg(Color::White)),
        ]),
    ])
    .block(
        Block::default()
            .title(Span::styled(
                "Search objects",
                Style::default()
                    .fg(Color::Green)
                    .add_modifier(Modifier::BOLD),
            ))
            .border_style(Style::default().fg(Color::Blue))
            .borders(Borders::ALL),
    );
    frame.render_widget(search_bar, chunks[0]);

    let mut list_state = ListState::default();
    if !app.search_results.is_empty() {
        let selected = app.search_selected.min(app.search_results.len() - 1);
        list_state.select(Some(selected));
    }

    let items: Vec<ListItem<'_>> = if app.search_results.is_empty() {
        vec![ListItem::new(Span::styled(
            "Type a few letters to search the database.",
            Style::default().fg(Color::DarkGray),
        ))]
    } else {
        app.search_results
            .iter()
            .map(|object| {
                let label = if object.aliases.is_empty() {
                    object.designation.clone()
                } else {
                    format!("{}  [{}]", object.designation, object.aliases.join(", "))
                };
                ListItem::new(Span::styled(label, Style::default().fg(Color::White)))
            })
            .collect()
    };

    let list = List::new(items)
        .block(
            Block::default()
                .title(Span::styled(
                    "Matching objects",
                    Style::default()
                        .fg(Color::Magenta)
                        .add_modifier(Modifier::BOLD),
                ))
                .border_style(Style::default().fg(Color::Cyan))
                .borders(Borders::ALL),
        )
        .highlight_style(
            Style::default()
                .fg(Color::Black)
                .bg(Color::Cyan)
                .add_modifier(Modifier::BOLD),
        )
        .highlight_symbol("> ");
    frame.render_stateful_widget(list, chunks[1], &mut list_state);

    frame.render_widget(
        Paragraph::new(app.status.as_str()).alignment(Alignment::Center),
        Rect {
            x: area.x,
            y: area.bottom().saturating_sub(1),
            width: area.width,
            height: 1,
        },
    );
}

fn render_object_details(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let outer = Layout::default()
        .direction(Direction::Vertical)
        .constraints([Constraint::Min(10), Constraint::Length(3)])
        .margin(1)
        .split(area);

    let body = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([Constraint::Percentage(30), Constraint::Percentage(70)])
        .split(outer[0]);

    let target = match app.active_target.as_ref() {
        Some(target) => target,
        None => {
            frame.render_widget(
                Paragraph::new("No object selected yet.")
                    .alignment(Alignment::Center)
                    .style(Style::default().fg(Color::DarkGray)),
                outer[0],
            );
            frame.render_widget(
                Paragraph::new("Press q to quit or r to return to search.")
                    .alignment(Alignment::Center)
                    .style(Style::default().fg(Color::DarkGray)),
                outer[1],
            );
            return;
        }
    };

    let object_kind = object_type_label(target);
    let detail_lines = vec![
        Line::from(vec![
            Span::styled(
                "Name: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                target.designation.as_str(),
                Style::default().fg(Color::White),
            ),
        ]),
        Line::from(vec![
            Span::styled(
                "Designation: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                target.designation.as_str(),
                Style::default().fg(Color::White),
            ),
        ]),
        Line::from(vec![
            Span::styled(
                "Type: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(object_kind, Style::default().fg(Color::Cyan)),
        ]),
        Line::from(vec![
            Span::styled(
                "Magnitude: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                target
                    .magnitude
                    .map(|value| format!("{value:.2}"))
                    .unwrap_or_else(|| String::from("n/a")),
                Style::default().fg(Color::Yellow),
            ),
        ]),
        Line::from(vec![
            Span::styled(
                "RA: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                format!("{:.6}°", target.ra_deg),
                Style::default().fg(Color::White),
            ),
        ]),
        Line::from(vec![
            Span::styled(
                "Dec: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                format!("{:.6}°", target.dec_deg),
                Style::default().fg(Color::White),
            ),
        ]),
        Line::from(vec![
            Span::styled(
                "Aliases: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                if target.aliases.is_empty() {
                    String::from("-")
                } else {
                    target.aliases.join(", ")
                },
                Style::default().fg(Color::DarkGray),
            ),
        ]),
    ];

    let details_block = Block::default()
        .title(Span::styled(
            "Object Details",
            Style::default()
                .fg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        ))
        .border_style(Style::default().fg(Color::Magenta))
        .borders(Borders::ALL);
    frame.render_widget(details_block, body[0]);
    frame.render_widget(
        Paragraph::new(detail_lines)
            .alignment(Alignment::Left)
            .style(Style::default().fg(Color::White))
            .block(Block::default()),
        body[0].inner(Margin::new(1, 1)),
    );

    let chart_data = build_altitude_plot_data(app, target);
    let chart = Chart::new(vec![
        Dataset::default()
            .name("Altitude")
            .marker(Marker::Braille)
            .graph_type(GraphType::Line)
            .style(Style::default().fg(Color::Yellow))
            .data(&chart_data.altitude_points),
        Dataset::default()
            .name("Horizon")
            .marker(Marker::Block)
            .graph_type(GraphType::Line)
            .style(Style::default().fg(Color::DarkGray))
            .data(&chart_data.horizon_points),
        Dataset::default()
            .name("Now")
            .marker(Marker::Block)
            .graph_type(GraphType::Line)
            .style(Style::default().fg(Color::Cyan))
            .data(&chart_data.marker_points),
    ])
    .block(
        Block::default()
            .title(Span::styled(
                format!("Altitude Graph - {}", target.designation),
                Style::default()
                    .fg(Color::Cyan)
                    .add_modifier(Modifier::BOLD),
            ))
            .border_style(Style::default().fg(Color::Blue))
            .borders(Borders::ALL),
    )
    .x_axis(
        Axis::default()
            .title(Span::styled(
                "24h window",
                Style::default().fg(Color::DarkGray),
            ))
            .style(Style::default().fg(Color::Gray))
            .bounds([0.0, 24.0])
            .labels(chart_data.x_labels),
    )
    .y_axis(
        Axis::default()
            .title(Span::styled(
                "Altitude (deg)",
                Style::default().fg(Color::DarkGray),
            ))
            .style(Style::default().fg(Color::Gray))
            .bounds([-30.0, 90.0])
            .labels(vec![
                Span::styled("-30", Style::default().fg(Color::DarkGray)),
                Span::styled("0", Style::default().fg(Color::DarkGray)),
                Span::styled("45", Style::default().fg(Color::DarkGray)),
                Span::styled("90", Style::default().fg(Color::DarkGray)),
            ]),
    );
    frame.render_widget(chart, body[1]);

    let lst_deg = app.current_lst_deg();
    let footer_text = vec![
        Line::from(vec![
            Span::styled(
                "Simulated time: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                app.simulated_time
                    .format("%Y-%m-%d %H:%M:%S UTC")
                    .to_string(),
                Style::default().fg(Color::White),
            ),
        ]),
        Line::from(vec![
            Span::styled(
                "LST: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(format_lst_deg(lst_deg), Style::default().fg(Color::Cyan)),
            Span::styled(
                format!(
                    "  |  Alt at now: {:.1}°",
                    altitude_deg(
                        target.ra_deg,
                        target.dec_deg,
                        lst_deg,
                        app.selected_observer_latitude()
                    )
                ),
                Style::default().fg(Color::Yellow),
            ),
        ]),
        Line::from(vec![
            Span::styled(
                "Controls: ",
                Style::default()
                    .fg(Color::Magenta)
                    .add_modifier(Modifier::BOLD),
            ),
            Span::styled(
                "Left/Right = 15 min, Up/Down = 1 day, r = back to search",
                Style::default().fg(Color::DarkGray),
            ),
        ]),
    ];

    frame.render_widget(
        Paragraph::new(footer_text)
            .style(Style::default().fg(Color::White))
            .block(
                Block::default()
                    .borders(Borders::ALL)
                    .border_style(Style::default().fg(Color::Blue)),
            ),
        outer[1],
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
                    app.start_lookup(tokio::spawn(
                        async move { lookup_town_location(&query).await },
                    ));
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
        AppState::SearchByName => match key.code {
            KeyCode::Char('q') | KeyCode::Esc => return Ok(true),
            KeyCode::Up => {
                if !app.search_results.is_empty() {
                    app.search_selected = app.search_selected.saturating_sub(1);
                }
            }
            KeyCode::Down => {
                if !app.search_results.is_empty() {
                    app.search_selected =
                        (app.search_selected + 1).min(app.search_results.len() - 1);
                }
            }
            KeyCode::Backspace => {
                app.search_query.pop();
                app.start_search();
            }
            KeyCode::Enter => {
                if let Some(target) = app.search_results.get(app.search_selected).cloned() {
                    app.set_active_target(target);
                }
            }
            KeyCode::Char(c) if !c.is_control() => {
                app.search_query.push(c);
                app.start_search();
            }
            _ => {}
        },
        AppState::ObjectDetails => match key.code {
            KeyCode::Char('q') | KeyCode::Char('Q') => return Ok(true), // Only Q quits now
            KeyCode::Esc | KeyCode::Backspace => {
                // Esc goes back
                app.state = AppState::SearchByName;
                app.status = String::from("Type to search the MongoDB object collection.");
            }
            KeyCode::Left => {
                app.simulated_time -= ChronoDuration::minutes(15);
            }
            KeyCode::Right => {
                app.simulated_time += ChronoDuration::minutes(15);
            }
            KeyCode::Up => {
                app.simulated_time -= ChronoDuration::days(1);
            }
            KeyCode::Down => {
                app.simulated_time += ChronoDuration::days(1);
            }
            KeyCode::Char('r') | KeyCode::Char('R') => {
                app.state = AppState::SearchByName;
                app.status = String::from("Type to search the MongoDB object collection.");
            }
            KeyCode::Char('s') | KeyCode::Char('S') => {
                app.state = AppState::SearchAround;
                app.search_around_focus = 0; // Start with focus on Radius
                let radius = app
                    .search_around_radius_input
                    .parse::<f64>()
                    .unwrap_or(10.0);
                let mag = app.search_around_mag_input.parse::<f64>().ok();
                app.start_search_around(radius, mag);
            }
            _ => {}
        },
        AppState::SearchAround => {
            // Helper closure to trigger the search reactively
            let trigger_search = |app: &mut App| {
                let radius = app
                    .search_around_radius_input
                    .parse::<f64>()
                    .unwrap_or(10.0);
                let mag = app.search_around_mag_input.parse::<f64>().ok();
                app.start_search_around(radius, mag);
            };

            match key.code {
                KeyCode::Esc => {
                    app.state = AppState::ObjectDetails;
                    app.status = String::from("Returned to Object Details.");
                }
                KeyCode::Tab => {
                    // 2. Cycle through 4 states now (0, 1, 2, 3)
                    app.search_around_focus = (app.search_around_focus + 1) % 4;
                }
                KeyCode::Char('m') | KeyCode::Char('M') if app.search_around_focus != 2 => {
                    app.search_around_is_map_mode = !app.search_around_is_map_mode;
                }

                // --- RESULT NAVIGATION (Global, no focus check required) ---
                KeyCode::Up if app.search_around_focus == 3 => {
                    app.search_around_selected = app.search_around_selected.saturating_sub(1);
                }
                KeyCode::Down if app.search_around_focus == 3 => {
                    if !app.search_around_results.is_empty() {
                        app.search_around_selected = (app.search_around_selected + 1)
                            .min(app.search_around_results.len() - 1);
                    }
                }
                KeyCode::Enter if app.search_around_focus == 3 => {
                    if let Some((target, _)) = app
                        .search_around_results
                        .get(app.search_around_selected)
                        .cloned()
                    {
                        // Set the new target, which automatically drops you back into ObjectDetails!
                        app.set_active_target(target);
                    }
                }

                // --- LIVE TYPING (Only when focus is 0 or 1) ---
                KeyCode::Backspace => {
                    if app.search_around_focus == 0 {
                        app.search_around_radius_input.pop();
                        trigger_search(app);
                    } else if app.search_around_focus == 1 {
                        app.search_around_mag_input.pop();
                        trigger_search(app);
                    } else if app.search_around_focus == 2 {
                        app.search_around_catalog_input.pop();
                        trigger_search(app);
                    }
                }
                KeyCode::Char(c) => {
                    // For Radius and Mag, only allow numbers/decimals
                    if (app.search_around_focus == 0 || app.search_around_focus == 1)
                        && (c.is_ascii_digit() || c == '.' || c == '-')
                    {
                        if app.search_around_focus == 0 {
                            app.search_around_radius_input.push(c);
                        } else {
                            app.search_around_mag_input.push(c);
                        }
                        trigger_search(app);
                    }
                    // For Catalog, allow letters, numbers, spaces, and hyphens!
                    else if app.search_around_focus == 2
                        && (c.is_alphanumeric() || c == ' ' || c == '-')
                    {
                        app.search_around_catalog_input.push(c);
                        trigger_search(app);
                    }
                }
                _ => {}
            }
        }
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
        return Err(payload
            .message
            .unwrap_or_else(|| String::from("IP lookup service reported failure")));
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

struct AltitudePlotData {
    altitude_points: Vec<(f64, f64)>,
    horizon_points: Vec<(f64, f64)>,
    marker_points: Vec<(f64, f64)>,
    x_labels: Vec<Span<'static>>,
}

fn build_altitude_plot_data(app: &App, target: &SkyObject) -> AltitudePlotData {
    let observer_latitude = app.selected_observer_latitude();
    let observer_longitude = app.selected_observer_longitude();
    let base_unix_seconds = datetime_to_unix_seconds(app.simulated_time);

    let mut altitude_points = Vec::with_capacity(97);
    for step in 0..=96 {
        let hour_offset = step as f64 * 24.0 / 96.0;
        // Shift by -12 hours so that "now" (hour 0) appears at the center (x=12)
        let sample_unix_seconds = base_unix_seconds + (hour_offset - 12.0) * 3600.0;
        let lst_deg = lst_deg_at_unix_seconds(observer_longitude, sample_unix_seconds);
        let altitude = altitude_deg(target.ra_deg, target.dec_deg, lst_deg, observer_latitude);
        altitude_points.push((hour_offset, altitude));
    }

    let horizon_points = vec![(0.0, 0.0), (24.0, 0.0)];
    // Place marker at x=12 (center) showing the full altitude range including below horizon
    let marker_points = vec![(12.0, -30.0), (12.0, 90.0)];

    let labels = [-12_i64, -6, 0, 6, 12]
        .into_iter()
        .map(|hours| {
            Span::styled(
                (app.simulated_time + ChronoDuration::hours(hours))
                    .format("%H:%M")
                    .to_string(),
                Style::default().fg(Color::DarkGray),
            )
        })
        .collect();

    AltitudePlotData {
        altitude_points,
        horizon_points,
        marker_points,
        x_labels: labels,
    }
}

fn datetime_to_unix_seconds(datetime: DateTime<Utc>) -> f64 {
    datetime.timestamp() as f64 + f64::from(datetime.timestamp_subsec_nanos()) / 1_000_000_000.0
}

fn format_lst_deg(lst_deg: f64) -> String {
    let total_seconds = ((lst_deg.rem_euclid(360.0) / 15.0) * 3600.0).round() as i64;
    let hours = total_seconds / 3600;
    let minutes = (total_seconds % 3600) / 60;
    let seconds = total_seconds % 60;
    format!(
        "{:02}:{:02}:{:02} ({:.2}°)",
        hours,
        minutes,
        seconds,
        lst_deg.rem_euclid(360.0)
    )
}

fn object_type_label(object: &SkyObject) -> String {
    if let Ok(kind) = object.metadata.get_str("hubble") {
        return kind.to_string();
    }

    if let Ok(kind) = object.metadata.get_str("spectral_type") {
        return kind.to_string();
    }

    if let Ok(kind) = object.metadata.get_str("constellation") {
        if !kind.is_empty() {
            return kind.to_string();
        }
    }

    object.source.as_deref().unwrap_or("Unknown").to_string()
}

fn render_search_around(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let chunks = Layout::default()
        .direction(Direction::Vertical)
        .constraints([Constraint::Length(3), Constraint::Min(5)])
        .split(area);

    // Split into 3 equal columns
    let top_chunks = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([
            Constraint::Percentage(33),
            Constraint::Percentage(33),
            Constraint::Percentage(34),
        ])
        .split(chunks[0]);

    let rad_style = if app.search_around_focus == 0 {
        Style::default().fg(Color::Black).bg(Color::Cyan)
    } else {
        Style::default().fg(Color::White)
    };
    let mag_style = if app.search_around_focus == 1 {
        Style::default().fg(Color::Black).bg(Color::Cyan)
    } else {
        Style::default().fg(Color::White)
    };
    let cat_style = if app.search_around_focus == 2 {
        Style::default().fg(Color::Black).bg(Color::Cyan)
    } else {
        Style::default().fg(Color::White)
    };

    let rad_p = Paragraph::new(app.search_around_radius_input.as_str())
        .style(rad_style)
        .block(Block::default().title(" Radius (°) ").borders(Borders::ALL));

    let mag_p = Paragraph::new(if app.search_around_mag_input.is_empty() {
        "No limit"
    } else {
        &app.search_around_mag_input
    })
    .style(mag_style)
    .block(Block::default().title(" Max Mag ").borders(Borders::ALL));

    let cat_p = Paragraph::new(if app.search_around_catalog_input.is_empty() {
        "All"
    } else {
        &app.search_around_catalog_input
    })
    .style(cat_style)
    .block(
        Block::default()
            .title(" Catalog Filter ")
            .borders(Borders::ALL),
    );

    frame.render_widget(rad_p, top_chunks[0]);
    frame.render_widget(mag_p, top_chunks[1]);
    frame.render_widget(cat_p, top_chunks[2]);

    // 3. Render the Map or List in the bottom chunk
    if app.search_around_is_map_mode {
        render_search_around_map(frame, chunks[1], app);
    } else {
        render_search_around_list(frame, chunks[1], app);
    }
    let footer_area = Rect {
        x: area.x,
        y: area.bottom().saturating_sub(1),
        width: area.width,
        height: 1,
    };
    frame.render_widget(
        Paragraph::new(
            "M: Toggle Map/List | Tab: Switch Input | Enter: Select | Esc: Back | Up/Down: Scroll",
        )
        .alignment(Alignment::Center)
        .style(Style::default().fg(Color::DarkGray)),
        footer_area,
    );
}

fn render_search_around_list(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let block = Block::default()
        .title(Span::styled(
            format!(
                "Nearby Objects (Radius: {:.1}°)",
                app.search_around_radius_hint
            ),
            Style::default()
                .fg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        ))
        .border_style(Style::default().fg(Color::Cyan))
        .borders(Borders::ALL);

    let mut table_state = TableState::default();
    if !app.search_around_results.is_empty() {
        table_state.select(Some(app.search_around_selected));
    }

    let header = Row::new(vec!["Distance", "Designation", "Magnitude", "Type"])
        .style(
            Style::default()
                .fg(Color::Magenta)
                .add_modifier(Modifier::BOLD),
        )
        .bottom_margin(1);

    let rows: Vec<Row> = app
        .search_around_results
        .iter()
        .map(|(obj, dist)| {
            let mag_str = obj
                .magnitude
                .map(|m| format!("{:.2}", m))
                .unwrap_or_else(|| "-".to_string());
            Row::new(vec![
                Cell::from(format!("{:.3}°", dist)),
                Cell::from(obj.designation.clone()),
                Cell::from(mag_str),
                Cell::from(object_type_label(obj)),
            ])
        })
        .collect();

    let table = Table::new(
        rows,
        [
            Constraint::Length(12),
            Constraint::Min(20),
            Constraint::Length(10),
            Constraint::Length(20),
        ],
    )
    .header(header)
    .block(block)
    .highlight_style(
        Style::default()
            .bg(Color::Cyan)
            .fg(Color::Black)
            .add_modifier(Modifier::BOLD),
    )
    .highlight_symbol(">> ");

    frame.render_stateful_widget(table, area, &mut table_state);

    // Render footer help
    let footer_area = Rect {
        x: area.x,
        y: area.bottom().saturating_sub(1),
        width: area.width,
        height: 1,
    };
    frame.render_widget(
        Paragraph::new("Tab: Map View | Enter: Select Object | Esc/q: Back | Up/Down: Scroll")
            .alignment(Alignment::Center)
            .style(Style::default().fg(Color::DarkGray)),
        footer_area,
    );

    let footer_area = Rect {
        x: area.x,
        y: area.bottom().saturating_sub(1),
        width: area.width,
        height: 1,
    };
    frame.render_widget(
        Paragraph::new(
            "M: Toggle Map/List | Tab: Switch Input | Enter: Select | Esc: Back | Up/Down: Scroll",
        )
        .alignment(Alignment::Center)
        .style(Style::default().fg(Color::DarkGray)),
        footer_area,
    );
}

fn render_search_around_map(frame: &mut Frame<'_>, area: Rect, app: &App) {
    let target = app.active_target.as_ref().unwrap();
    let radius = app.search_around_radius_hint;

    let block = Block::default()
        .title(Span::styled(
            format!(
                "Spatial Map - Centered on {} (Radius: {:.1}°)",
                target.designation, radius
            ),
            Style::default()
                .fg(Color::Yellow)
                .add_modifier(Modifier::BOLD),
        ))
        .border_style(Style::default().fg(Color::Green))
        .borders(Borders::ALL);

    let canvas = Canvas::default()
        .block(block)
        .x_bounds([-radius, radius])
        .y_bounds([-radius, radius])
        .paint(|ctx| {
            for (idx, (neighbor, _dist)) in app.search_around_results.iter().enumerate() {
                // Calculate angular offset
                let mut ra_diff = neighbor.ra_deg - target.ra_deg;
                // Handle spherical wrapping
                if ra_diff > 180.0 {
                    ra_diff -= 360.0;
                }
                if ra_diff < -180.0 {
                    ra_diff += 360.0;
                }

                // Apply cosine correction for converging RA lines at poles
                let x = ra_diff * target.dec_deg.to_radians().cos();
                let y = neighbor.dec_deg - target.dec_deg;

                if idx == app.search_around_selected {
                    // Highlight selected neighbor
                    ctx.print(
                        x,
                        y,
                        Span::styled(
                            "*",
                            Style::default()
                                .fg(Color::Yellow)
                                .add_modifier(Modifier::BOLD),
                        ),
                    );
                    ctx.print(
                        x + (radius * 0.02),
                        y + (radius * 0.02),
                        Span::styled(
                            neighbor.designation.clone(),
                            Style::default().fg(Color::Yellow),
                        ),
                    );
                } else {
                    ctx.print(
                        x,
                        y,
                        Span::styled(".", Style::default().fg(Color::DarkGray)),
                    );
                }
            }

            ctx.print(
                0.0,
                0.0,
                Span::styled(
                    "@",
                    Style::default()
                        .fg(Color::Cyan)
                        .add_modifier(Modifier::BOLD),
                ),
            );
        });

    frame.render_widget(canvas, area);

    // Render footer help
    let footer_area = Rect {
        x: area.x,
        y: area.bottom().saturating_sub(1),
        width: area.width,
        height: 1,
    };
    frame.render_widget(
        Paragraph::new(
            "Tab: List View | Enter: Select Object | Esc/q: Back | Up/Down: Cycle Target",
        )
        .alignment(Alignment::Center)
        .style(Style::default().fg(Color::DarkGray)),
        footer_area,
    );

    let footer_area = Rect {
        x: area.x,
        y: area.bottom().saturating_sub(1),
        width: area.width,
        height: 1,
    };
    frame.render_widget(
        Paragraph::new(
            "M: Toggle Map/List | Tab: Switch Input | Enter: Select | Esc: Back | Up/Down: Scroll",
        )
        .alignment(Alignment::Center)
        .style(Style::default().fg(Color::DarkGray)),
        footer_area,
    );
}
