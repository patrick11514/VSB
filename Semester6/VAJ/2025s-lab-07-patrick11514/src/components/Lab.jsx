import { useLoaderData, Form, useActionData, Outlet } from "react-router-dom";
import { addTodo, getTodos } from "../data/index";

export function todoLoader() {
    if (Math.random() < 0.5) {
        throw new Error("Test error");
    }
    return getTodos();
}

export async function todoCreate({ request }) {
    const data = await request.formData();
    if (!data.has("todo")) {
        return { todo: "Todo is required" };
    }
    const todo = data.get("todo");
    if (!todo) {
        return { todo: "Todo is required" };
    }
    await addTodo(todo);
}

export default function Lab() {
    const todos = useLoaderData();
    const errors = useActionData();

    return (
        <div>
            <h1>Lab Page</h1>
            <h2>Todos:</h2>
            <ul>
                {todos.map((todo, idx) => (
                    <li key={idx}>{todo}</li>
                ))}
            </ul>
            <h2>Add a new Todo:</h2>
            <Form method="post">
                <div style={{ display: "flex", flexDirection: "column" }}>
                    <input type="text" name="todo" placeholder="Enter a new todo" />
                    {errors && errors.todo && (
                        <span style={{ color: "red" }}>{errors.todo}</span>
                    )}
                </div>
                <button type="submit">Add Todo</button>
            </Form>

            <Outlet />
        </div>
    );
}
