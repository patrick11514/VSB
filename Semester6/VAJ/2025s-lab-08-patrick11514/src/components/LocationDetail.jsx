import React from "react";
import { useLoaderData } from "react-router-dom";

export function loader({ params }) {
  return fetch(
    `https://rickandmortyapi.com/api/location/${params.locationId}`
  ).then((res) => res.json());
}

export function LocationDetail() {
  const locations = useLoaderData();

  return (
    <div>
      <h1>LocationDetail</h1>
      <pre>{JSON.stringify(locations, null, 2)}</pre>
    </div>
  );
}
