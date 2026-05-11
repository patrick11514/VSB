# Project: StellaDB-CLI (Rust Ratatui TUI)
# Goal: Rebuild Application Architecture from Scratch

You are an expert Rust developer specializing in terminal user interfaces using `ratatui` and `crossterm`. We are wiping the previous UI implementation and building a clean, state-driven application from scratch. 

The application is a MongoDB-backed astronomical database explorer that heavily utilizes spatial queries (`$geoNear`) and calculates real-time celestial mechanics.

## 1. Core Architecture (Mandatory)
Do not use boolean flags to hide/show UI components. You must implement a strict State Machine using an `enum AppState` and a central event loop. Separate your render logic (`fn render(frame, state)`) from your event handling logic (`fn handle_events(event, state)`).

### Required Dependencies:
* `ratatui` & `crossterm` (UI and Events)
* `mongodb` & `tokio` (Database interaction)
* `chrono` (Absolute time manipulation)
* `reqwest` & `serde_json` (API calls)

---

## 2. State Machine & UI Specifications

### State A: `SetupLocation` (Entry Point)
**Purpose:** Establish the observer's Latitude and Longitude.
* **UI Layout:** A vertically and horizontally centered modal popup (`Clear` widget background).
* **Interactions:**
  * Press `I`: Auto-resolve coordinates based on IP.
  * Press `M`: Open a text input field to type a city name (e.g., "Prague, Czech Republic").
* **API Constraint:** When querying the OpenStreetMap/Nominatim API for the city search, you **must** use `reqwest` and set the `User-Agent` header exactly to: `StellaDB-CLI (patrik.mintel.st@vsb.cz)`. If you do not include this, the API will reject the request.
* **Transition:** Once `Lat` and `Lon` are successfully stored in the global app state, transition automatically to `SearchByName`.

### State B: `SearchByName`
**Purpose:** Find an object in the database to act as our primary target.
* **UI Layout:** * Top (10%): A text input field acting as a search bar.
  * Bottom (90%): A scrollable `List` or `Table` widget.
* **Logic:** As the user types, query the MongoDB collection using a text index or regex match on the designation/name fields.
* **Transition:** The user scrolls the list with `Up/Down` arrows. Pressing `Enter` selects the object, stores it as the `active_target`, and transitions to `ObjectDetails`.

### State C: `ObjectDetails` (The Core Observatory View)
**Purpose:** Display detailed telemetry and the 24-hour altitude transit graph.
* **UI Layout:**
  * **Left Pane (30% width):** Display the parsed BSON document details (Name, Designation, Magnitude, Type, RA, Dec).
  * **Right Pane (70% width):** The `Chart` widget displaying the Altitude Graph.
  * **Bottom Pane (3 fixed lines):** Time control readout showing the current simulated `chrono::DateTime<Utc>` and Local Sidereal Time (LST).
* **The Altitude Graph Math & Visuals:**
  * **Y-Axis:** Fixed bounds from `0.0` to `90.0` degrees.
  * **X-Axis:** A 24-hour period starting from the current simulated time. Display time labels (e.g., `18:00`, `00:00`, `06:00`).
  * **Calculation:** For every pixel/step on the X-axis, calculate the altitude ($a$) using: `sin(a) = sin(δ) * sin(φ) + cos(δ) * cos(φ) * cos(H)`
    * `δ` (Declination): From the database object.
    * `φ` (Latitude): From the `SetupLocation` state.
    * `H` (Hour Angle): Calculated as `LST - RA` for that specific time step.
  * **Visual Markers:** Draw a horizontal line at `y = 0` (The Horizon). Draw a vertical marker representing the exact current time selected by the user.
* **Absolute Time Scrubbing:**
  * Bind `Left/Right` arrow keys to subtract/add exactly 15 minutes to the simulated `DateTime`.
  * Bind `Up/Down` arrow keys to subtract/add exactly 1 day.
  * The Altitude Graph and LST must dynamically recalculate when the time is scrubbed.
* **Transition:** Pressing `S` opens a small prompt to enter a search radius and magnitude limit, then transitions to `SearchAround`.

### State D: `SearchAround` (Spatial Proximity)
**Purpose:** Visualize objects near the `active_target` using MongoDB spatial indexes.
* **Database Logic:** Execute a `$geoNear` aggregation pipeline centered on the `active_target` coordinates, constrained by the user's input radius and magnitude.
* **UI Layout:** A full-screen view with a toggle mechanic.
  * **Toggle:** Pressing `Tab` switches between "List Mode" and "Map Mode".
  * **List Mode:** A standard `Table` widget sorting the results by distance.
  * **Map Mode:** Use the ratatui `Canvas` widget. Place the `active_target` perfectly in the center `(0,0)`. Map the returned neighbors onto the canvas relative to the center based on their RA/Dec offsets. Render the target as a distinct character (e.g., `@`) and neighbors as dots (`.`). Draw text labels for the top 3 brightest neighbors.
* **Transition:** Selecting a neighbor from the List Mode and pressing `Enter` sets it as the new `active_target` and transitions back to `ObjectDetails`.

## Database connection
* 10.10.10.232 with default port