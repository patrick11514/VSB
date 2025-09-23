import { Link } from "react-router-dom";

export default function NotFound() {
    return (
        <div>
            <h1>404 Not Found</h1>
            <p>
                Go back <Link to="/">Home</Link>
            </p>
        </div>
    );
}
