import React from "react";
import ReactDOM from "react-dom/client";
import { createBrowserRouter, RouterProvider } from "react-router-dom";
import App from "./App";
import {
  Characters,
  loader as charactersLoader,
} from "./components/Characters";
import {
  CharacterDetail,
  loader as characterDetailLoader,
} from "./components/CharacterDetail";
import {
  CharacterComments,
  loader as characterCommentsLoader,
  action as characterCommentsAction,
} from "./components/CharacterComments";
import { Episodes, loader as episodesLoader } from "./components/Episodes";
import {
  EpisodeDetail,
  loader as episodeDetailLoader,
} from "./components/EpisodeDetail";
import { Locations, loader as locationsLoader } from "./components/Locations";
import {
  LocationDetail,
  loader as locationDetailLoader,
} from "./components/LocationDetail";

const router = createBrowserRouter([
  {
    path: "/",
    element: <App />,
    children: [
      { index: true, element: <h1>Home</h1> },
      { path: "characters", loader: charactersLoader, element: <Characters /> },
      {
        path: "characters/:characterId",
        loader: characterDetailLoader,
        element: <CharacterDetail />,
        children: [
          {
            index: true,
            loader: characterCommentsLoader,
            action: characterCommentsAction,
            element: <CharacterComments />,
          },
        ],
      },
      {
        path: "episodes",
        loader: episodesLoader,
        element: <Episodes />,
        children: [
          { index: true, element: <h1>Choose an episode</h1> },
          {
            path: ":episodeId",
            loader: episodeDetailLoader,
            element: <EpisodeDetail />,
          },
        ],
      },

      {
        path: "locations",
        loader: locationsLoader,
        element: <Locations />,
        children: [
          { index: true, element: <h1>Choose a location</h1> },
          {
            path: ":locationId",
            loader: locationDetailLoader,
            element: <LocationDetail />,
          },
        ],
      },
    ],
  },
]);

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(
  <React.StrictMode>
    <RouterProvider router={router} />
  </React.StrictMode>
);
