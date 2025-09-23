import { Link } from "react-router-dom";
import { randomName } from "../data/index.js";

export default function Trigger() {
    const name = randomName();

    return (
        <div>
            <Link to={`bouncer/${name}`}>Go to Bouncer with name: {name}</Link>
        </div>
    );
}
