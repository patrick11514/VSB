import { useEffect } from 'react';
import { useState } from 'react';

// TODO: check this component - is the color changing based on the props it receives?
function Numbers(props) {
    // TODO: is this component the owner of the color state?
    const [color, setColor] = useState(props.color);

    useEffect(() => {
        setColor(props.color);
    }, [props.color]);

    return <h2 style={{ color: color }}>{JSON.stringify(props.numbers)}</h2>;
}

// No need to change this component
function WinningNumbers({ numbers }) {
    const [color, setColor] = useState('lightcoral');

    return (
        <div>
            <h1>Today&apos;s winning numbers:</h1>
            <p>
                Pick a color:{' '}
                <select
                    value={color}
                    onChange={(e) => {
                        setColor(e.target.value);
                    }}
                >
                    <option value="lightcoral">lightcoral</option>
                    <option value="midnightblue">midnightblue</option>
                    <option value="rebeccapurple">rebeccapurple</option>
                </select>
            </p>
            <Numbers color={color} numbers={numbers} />
        </div>
    );
}

export { WinningNumbers };
