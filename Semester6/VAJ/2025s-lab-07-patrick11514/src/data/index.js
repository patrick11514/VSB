const todos = ["Finish the Lab on React Router"];

export async function getTodos() {
  await fakeNetwork();
  return todos;
}

export async function addTodo(todo) {
  await fakeNetwork();
  todos.push(todo);
  return todo;
}

export function randomName() {
  const names = ["Alice", "Bob", "Charlie", "Dave", "Eve"];
  return names[Math.floor(Math.random() * names.length)];
}

async function fakeNetwork() {
  return new Promise((res) => {
    setTimeout(res, Math.random() * 1600);
  });
}
