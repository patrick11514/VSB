import { useState } from 'react';

function Counter() {
    const [count, setCount] = useState(0);

    return (
        <div>
            {/* Notice that we can add padding or other CSS styling to components */}
            <div style={{ padding: 5 }}>Counter is: {count}</div>
            <button
                onClick={() => {
                    setCount((count) => count + 1);
                }}
            >
                Click to increment
            </button>
            <button
                onClick={() => {
                    setCount((count) => count * 2);
                }}
            >
                Double the value
            </button>
        </div>
    );
}

export { Counter };
