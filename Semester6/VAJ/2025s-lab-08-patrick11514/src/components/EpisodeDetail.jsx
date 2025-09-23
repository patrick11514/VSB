import React from "react";
import { useLoaderData } from "react-router-dom";

export function loader({ params }) {
  return fetch(
    `https://rickandmortyapi.com/api/episode/${params.episodeId}`
  ).then((res) => res.json());
}

export function EpisodeDetail() {
  const episodes = useLoaderData();

  return (
    <div>
      <h1>EpisodeDetail</h1>
      <pre>{JSON.stringify(episodes, null, 2)}</pre>
    </div>
  );
}
