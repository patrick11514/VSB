import React from "react";
import { Form, useLoaderData } from "react-router-dom";
import { addComment, getComments } from "../api/comments";

export function loader({ params }) {
  return getComments(params.characterId);
}

export async function action({ request, params }) {
  const formData = await request.formData();
  const content = formData.get("content");
  return addComment(params.characterId, content);
}

export function CharacterComments() {
  const characters = useLoaderData();

  return (
    <div>
      <h1>CharacterComments</h1>
      <Form method="post">
        <input name="content" />
        <button type="submit">Add Comment</button>
      </Form>
      <pre>{JSON.stringify(characters, null, 2)}</pre>
    </div>
  );
}
