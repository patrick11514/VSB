[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/qCiVs_GA)
# Lab 05 - Basics of React

## Tasks

### Task 1: Create and run basic app

1. Init a new react application. The best way to create a simple React App right now is [Vite](https://vite.dev/guide/)

   - We can create a simple vite React app by running

   ```
   npm create vite@latest my-react-app -- --template react
   ```

1. Verify that the application is correctly installed by running the commands

   - `cd my-react-app` enter the correct folder
   - `npm install` install the dependencies
   - `npm run dev` which will start a local **development** server which does automatic rebuilds of your app when you change files

   You should see your sample application running on `http://localhost:5173/` in your browser.

### Task 2: Clean up and practice JSX

1. Open the `src/App.jsx` file and clean up the existing code to start with a clean slate. Remove all the existing JSX inside the `return` statement.

1. Practice JSX by creating a new heading and a paragraph.

   - Define a variable `const name = "Your Name"` at the top of the `App` function.
   - Inside the `return` statement, add a `<h1>` element with the text "Hello, React!".
   - Add a `<p>` element that uses JSX interpolation to display "My name is {name}".

   Your `App.jsx` file should initially look like this after cleaning up:

   ```javascript
   import './App.css'

   function App() {
     // Define your variables here

     return (
       // Add your JSX here
     )
   }

   export default App
   ```

   - Save the file and check your browser to see the changes.

### Task 3: Create a custom component

1. Create a new file named `Greeting.jsx` in the `src` directory.

1. In `Greeting.jsx`, define a functional component that returns a simple greeting message.

   - Start by creating a function named `Greeting`.
   - Inside the function, return a JSX element, such as a `<p>` tag, with a greeting message.

1. Import and render the `Greeting` component in `App.jsx`.

   - At the top of `App.jsx`, import the `Greeting` component:

     ```javascript
     import Greeting from "./Greeting";
     ```

   - Inside the `return` statement of the `App` component, render the `Greeting` component:

     ```javascript
     return (
       <div>
         {/* Existing JSX elements */}
         <Greeting />
       </div>
     );
     ```

   - Save the files and check your browser to see the changes.

### Task 4: Practice Passing Props and Conditional Rendering

1. Create a New Component for Conditional Rendering:

   - Create a new file named `NumberDisplay.jsx` in the `src` directory.

1. Define the `NumberDisplay` Component:

   - In `NumberDisplay.jsx`, define a functional component that accepts a `number` prop.
   - Use this prop to conditionally render a message based on its value.

   - Hint: Use a simple `if` statement or a ternary operator to decide which message to display.

1. Pass Props from `App.jsx`:

   - In `App.jsx`, import the `NumberDisplay` component.
   - Pass a `number` prop to the `NumberDisplay` component.

   - Hint: Define a constant number in `App.jsx` and pass it as a prop to `NumberDisplay`.

1. Test Your Changes:

   - Save the files and check your browser to see the changes.
   - Ensure that the message displayed by `NumberDisplay` changes based on the value of the `number` prop.

### Task 5: Practice Rendering Loops

1. Create a New Component for Rendering Loops:

   - Create a new file named `BurgerList.jsx` in the `src` directory.

1. Define the `BurgerList` Component:

   - In `BurgerList.jsx`, define a functional component that accepts `count` and `name` props.
   - Use these props to render a list of items.

   - Hint: Use a loop or the `Array.from()` method to generate an array of the specified size (`count`). [docs](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/from#using_arrow_functions_and_array.from)
   - Each list item should display the text "{name} is a great burger!".

1. Pass Props from `App.jsx`:

   - In `App.jsx`, import the `BurgerList` component.
   - Pass `count` and `name` props to the `BurgerList` component.

   - Hint: Define constants for `count` and `name` in `App.jsx` and pass them as props to `BurgerList`.

1. Test Your Changes:

   - Save the files and check your browser to see the changes.
   - Ensure that the list displays the correct number of items with the specified message.
