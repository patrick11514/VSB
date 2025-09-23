[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/lYIA5XyV)
# Assignment - React Hooks

In this assignment, you’ll practice using React Hooks (`useState` and `useEffect`) to build interactive components. Follow the tasks below to enhance your understanding of state management and side effects in React.

## Task 1: useState - Counter Component

1. **Import and Render the Counter Component**

   - Import the `Counter` component from `components/Counter.js` into `App.js`.
   - Render the `Counter` component inside the main `<div>` in `App.js`.

2. **Implement State Management**

   - In the `Counter` component, add the `useState` hook to keep track of an internal count state.

3. **Add Buttons for Incrementing and Doubling**
   - Add two buttons to the `Counter` component:
     - **Increment Button**: On click, increment the count by 1.
     - **Double Button**: On click, double the current count value.
   - Tip: Make sure to use the function notation for the state setter to avoid using stale state values.

## Task 2: useState and Conditional Rendering - Lottery Component

1. **Import and Render the Lottery Component**

   - Import the `Lottery` component from `components/Lottery.js` into `App.js`.
   - Render the `Lottery` component inside the main `<div>` in `App.js`.

2. **Manage Lottery Numbers with State**

   - In the `Lottery` component, use the `useState` hook to keep track of an array of lottery numbers.

3. **Add Numbers to the Lottery**

   - Implement functionality so that clicking a button adds the next number to the lottery numbers array.

4. **Implement Conditional Rendering**
   - Import the `WinningNumbers` component from `components/WinningNumbers.js` into `components/Lottery.js`.
   - Modify the `Lottery` component to:
     - Display the UI for rolling the next number (`<div>` with `className="TodaysNumbers"`) if there are fewer than 10 numbers.
     - Display the `WinningNumbers` component (passing the current numbers as props) when there are exactly 10 numbers.

_Note: The color change functionality will not work yet, this is a bonus task._

## Task 3: useEffect - WindowSizer Component

1. **Import and Render the WindowSizer Component**

   - Import the `WindowSizer` component from `components/WindowSizer.js` into `App.js`.
   - Render the `WindowSizer` component inside the main `<div>` in `App.js`.

2. **Update Dimensions on Resize**
   - In the `WindowSizer` component, use the `useEffect` hook to update the window dimensions whenever the window is resized.
   - Use `window.addEventListener('resize', ...)` to listen for resize events and update state accordingly.
   - Ensure you clean up the event listener in the cleanup function using `window.removeEventListener`.

## Task 4: useEffect - Stopwatch Component

1. **Import and Render the Stopwatch Component**

   - Import the `Stopwatch` component from `components/Stopwatch.js` into `App.js`.
   - Render the `Stopwatch` component inside the main `<div>` in `App.js`.

2. **Control the Timer with State**

   - Modify the `useEffect` hook in the `Stopwatch` component so that it only increments the time when the `running` state variable is true.
   - Ensure all dependencies are correctly specified in the `useEffect` hook.

3. **Implement Reset Functionality**
   - Ensure that clicking the Reset button stops the timer and resets the time to 0.

## Task 5: useEffect - StarWars Component

1. **Import and Render the StarWars Component**

   - Import the `StarWars` component from `components/StarWars.js` into `App.js`.
   - Render the `StarWars` component inside the main `<div>` in `App.js`.

2. **Fetch Movie Data with Side Effects**

   - In the `StarWars` component, use the `useEffect` hook to fetch data about a selected movie from the Star Wars API.
   - Ensure all dependencies are correctly specified in the `useEffect` hook.
   - Avoid race conditions and handle stale results appropriately (see Synchronizing with Effects).

## Bonus Task: Additional Features

1. **Fixing Color Change in WinningNumbers Component**

   - Investigate the `WinningNumbers` and `Numbers` components in `components/WinningNumbers.js`.
   - Resolve the color change issue so that the `Numbers` component changes colors based on the selected option in `WinningNumbers`.

2. **Populate Movie List Dynamically**

   - Update the `<select>` element in the `StarWars` component to use dynamic data fetched from the Star Wars API.
   - Use the `fetchMovieList` helper function to retrieve the list of movies.

Good luck! Make sure to test each component thoroughly to ensure all functionalities work as expected.
