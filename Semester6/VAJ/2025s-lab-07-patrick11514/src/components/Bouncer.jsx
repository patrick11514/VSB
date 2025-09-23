import { useEffect } from "react";
import { useParams, useNavigate } from "react-router-dom";

export default function Bouncer() {
    const { name } = useParams();
    const navigate = useNavigate();

    useEffect(() => {
        const timer = setTimeout(() => {
            navigate("/lab");
        }, 2000);
        return () => clearTimeout(timer);
    }, [navigate]);

    return (
        <div>
            Hello, <strong>{name}</strong>!
        </div>
    );
}
