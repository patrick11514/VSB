import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { createBrowserRouter, RouterProvider } from "react-router-dom";
import App from "./App.jsx";
import Home from "./components/Home.jsx";
import Lab, { todoCreate, todoLoader } from "./components/Lab.jsx";
import Trigger from "./components/Trigger.jsx";
import Bouncer from "./components/Bouncer.jsx";
import ErrorBoundary from "./components/ErrorBoundary.jsx";
import NotFound from "./components/NotFound.jsx";

const router = createBrowserRouter([
    {
        path: "/",
        element: <App />,
        children: [
            {
                index: true,
                element: <Home />,
            },
            {
                path: "lab",
                element: <Lab />,
                loader: todoLoader,
                action: todoCreate,
                ErrorBoundary,
                children: [
                    {
                        index: true,
                        element: <Trigger />,
                    },
                    {
                        path: "bouncer/:name",
                        element: <Bouncer />,
                    },
                ],
            },
            {
                path: "*",
                element: <NotFound />,
            },
        ],
    },
]);

createRoot(document.getElementById("root")).render(
    <StrictMode>
        <RouterProvider router={router} />
    </StrictMode>,
);
