import React from "react";
import { useLoaderData } from "react-router-dom";

export function loader() {
  return fetch("https://rickandmortyapi.com/api/character").then((res) =>
    res.json()
  );
}

export function Characters() {
  const characters = useLoaderData();

  return (
    <div>
      <h1>Characters</h1>
      <pre>{JSON.stringify(characters, null, 2)}</pre>
    </div>
  );
}
