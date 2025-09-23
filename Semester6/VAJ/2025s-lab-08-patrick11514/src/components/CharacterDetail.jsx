import React from "react";
import { useLoaderData, Outlet } from "react-router-dom";

export function loader({ params }) {
  return fetch(
    `https://rickandmortyapi.com/api/character/${params.characterId}`
  ).then((res) => res.json());
}

export function CharacterDetail() {
  const characters = useLoaderData();

  return (
    <div>
      <h1>CharacterDetail</h1>
      <pre>{JSON.stringify(characters, null, 2)}</pre>
      <Outlet />
    </div>
  );
}
