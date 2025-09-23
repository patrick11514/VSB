import { useState } from 'react';
import { WinningNumbers } from './WinningNumbers';

function Lottery() {
    const [lotteryNumbers, setLotteryNumebers] = useState([]);

    return (
        <div className="Lottery">
            <div className="TodaysNumbers">
                {lotteryNumbers.length < 10 ? (
                    <>
                        <div style={{ padding: 5 }}>
                            Today&apos;s numbers are: {JSON.stringify(lotteryNumbers)}
                        </div>
                        <button
                            onClick={() => {
                                let nextNumber = Math.floor(Math.random() * 100);
                                console.log(`Next number is ${nextNumber}`);
                                setLotteryNumebers((lottNumbers) => [
                                    ...lottNumbers,
                                    nextNumber
                                ]);
                            }}
                        >
                            Roll next number
                        </button>
                    </>
                ) : (
                    <WinningNumbers numbers={lotteryNumbers} />
                )}
            </div>
        </div>
    );
}

export { Lottery };
