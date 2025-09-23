import React from "react";
import { Outlet, useLoaderData } from "react-router-dom";

export function loader() {
  return fetch("https://rickandmortyapi.com/api/location").then((res) =>
    res.json()
  );
}

export function Locations() {
  const characters = useLoaderData();

  return (
    <div>
      <h1>Locations</h1>
      <pre>{JSON.stringify(characters, null, 2)}</pre>
      <Outlet />
    </div>
  );
}
