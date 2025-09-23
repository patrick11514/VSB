import { Outlet, Link } from "react-router-dom";

function App() {
    return (
        <div className="App">
            <Link to="/">Home</Link>
            <Link to="/lab">Lab</Link>
            <Outlet />
        </div>
    );
}

export default App;
