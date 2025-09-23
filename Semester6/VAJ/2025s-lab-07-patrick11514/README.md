[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Y06mAgIC)
# LAB 07 - React Router

## React Router

### Task 1 - Initial Setup

1. Install React Router in your project.
2. In `main.jsx`, create a browser router with a root route and two child routes.
3. Render the RouterProvider instead of the App component.
4. Set up the root route to render the `App.jsx` component.
5. Configure one child route as an index route, rendering the `Home.jsx` component.
6. Set up the other child route at the `/lab` path to render the `Lab.jsx` component.
7. In `App.jsx`, add navigation links for switching between the Home and Lab pages.

### Task 2 - Add a Loader to the Lab Page

1. Create a loader function in `Lab.jsx` that loads todo data from `src/data/index.js` (mock library).
2. Use this loader in the route definition for the Lab page.
3. Display the todo list in the Lab component, using `<ul>` and `<li>` elements.

### Task 3 - Add a Form to Create a New Todo

1. Define an action in `Lab.jsx` to add a new todo to the list using the mock library.
2. Use this action in the Lab route definition.
3. Create a form in the Lab component, designed to trigger the action when submitted.

### Task 4 - Add Child Routes and Implement useNavigation

1. Create two new components, `Bouncer.jsx` and `Trigger.jsx`, in the `src/components` folder.
2. Add child routes to the Lab page:
   • Trigger as an index route.
   • Bouncer at `/bouncer/:name`.
3. Use the Outlet component in the Lab component to render child routes.
4. In Trigger, generate a random name using `randomName()` from the mock library and render a link navigating to `bouncer/${name}`.
5. In Bouncer, display the name received from the route parameters.
6. Use `useNavigation` and `useEffect` hooks in Bouncer to navigate back to the Lab page after 2 seconds.

### Task 5 - Error Boundary Setup

1. Create an `ErrorBoundary.jsx` component in `src/components` to handle errors in the application.
2. Use this component as the `ErrorBoundary` prop for the `Lab` router configuration.
3. In the loader function of the Lab page, add a simple code to throw an error 50% of the time, to test the error boundary.

```js
if (Math.random() < 0.5) {
  throw new Error("Test error");
}
```

### Bonus Task - Add a Not Found Page

1. Create a `NotFound.jsx` component in `src/components` to serve as a 404 page.
2. Update your router configuration to render this component for any unrecognized path.
3. Add a link in the `NotFound` page to navigate back to the Home page.
