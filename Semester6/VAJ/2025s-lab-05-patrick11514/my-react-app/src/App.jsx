import './App.css';
import BurgerList from './BurgerList';
import Greeting from './Greeting';
import NumberDisplay from './NumberDisplay';

function App() {
    const name = 'Patrik Mintěl';

    const someNumber = 9;

    const burgerName = 'Big mac';
    const count = 5;
    return (
        <div>
            <h1>Hello, React!</h1>
            <p>My name is {name}</p>
            <Greeting />
            <NumberDisplay number={someNumber} />
            <BurgerList name={burgerName} count={count} />
        </div>
    );
}

export default App;
