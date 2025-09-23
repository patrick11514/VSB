import React from "react";
import { Outlet, useLoaderData } from "react-router-dom";

export function loader() {
  return fetch("https://rickandmortyapi.com/api/episode").then((res) =>
    res.json()
  );
}

export function Episodes() {
  const characters = useLoaderData();

  return (
    <div>
      <h1>Episodes</h1>
      <pre>{JSON.stringify(characters, null, 2)}</pre>
      <Outlet />
    </div>
  );
}
