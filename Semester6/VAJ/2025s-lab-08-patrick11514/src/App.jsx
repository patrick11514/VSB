import React from "react";
import { Outlet, Link } from "react-router-dom";

function App() {
  return (
    <div className="App">
      <h1>App</h1>
      <ul>
        <li>
          <Link to="/">Home</Link>
        </li>
        <li>
          <Link to="/characters">Characters</Link>
        </li>
        <li>
          <Link to="/episodes">Episodes</Link>
        </li>
        <li>
          <Link to="/locations">Locations</Link>
        </li>
      </ul>
      <Outlet />
    </div>
  );
}

export default App;
