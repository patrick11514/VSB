import { Counter } from './components/Counter';
import { Lottery } from './components/Lottery';
import { WindowSizer } from './components/WindowSizer';
import { Stopwatch } from './components/Stopwatch';
import { StarWars } from './components/StarWars';

function App() {
    return (
        <div className="App">
            <Counter />
            <Lottery />
            <WindowSizer />
            <Stopwatch />
            <StarWars />
        </div>
    );
}

export default App;
