import Icon from '$/components/Icon';
import { Link } from 'react-router-dom';

export default function Home() {
    const launches: {
        name: string;
        description: string;
        date: Date;
    }[] = [
        {
            name: 'Launch 1',
            description:
                'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.',
            date: new Date('2023-10-01')
        },
        {
            name: 'Launch 2',
            description:
                'Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.',
            date: new Date('2024-08-01')
        },
        {
            name: 'Launch 3',
            description:
                'Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.',
            date: new Date('2025-01-01')
        }
    ];

    return (
        <>
            <div className="via-background to-background h-main font-poppins flex w-full flex-col items-center justify-center bg-gradient-to-t from-blue-500/80">
                <h1 className="text-center text-4xl font-bold lg:text-5xl">
                    Launch YOUR satelites to undiscovered worlds!
                </h1>
                <h2 className="text-center text-2xl font-bold lg:text-3xl">
                    Explore the universe with our launch services
                </h2>
                <Link
                    to="/shop"
                    className="border-primary bg-transaprent hover:bg-accent/70 text-primary mt-4 rounded-md border-2 px-4 py-2 font-bold transition-colors duration-200"
                >
                    Launch Now
                </Link>
            </div>
            <hr className="border-text w-full border-2" />
            <div className="h-main flex w-full flex-col justify-center bg-blue-500/80 bg-gradient-to-b p-4 text-center">
                <h1 className="font-poppins mt-8 mb-2 text-4xl font-bold underline lg:text-5xl">
                    Featured Launches
                </h1>
                <div className="flex flex-row flex-wrap justify-center gap-4">
                    {launches.map((launch, id) => (
                        <div
                            className="border-text bg-background/20 flex max-w-64 cursor-pointer flex-col rounded-md border-2 p-4 transition-transform duration-200 hover:scale-105"
                            key={id}
                        >
                            <h2 className="mb-2 text-2xl font-bold lg:text-3xl">
                                {launch.name}
                            </h2>
                            <p className="flex-1 text-lg lg:text-xl">
                                {launch.description}
                            </p>
                            <span className="font-bold">
                                <Icon name="bi-rocket-fill" />{' '}
                                {launch.date.toDateString()}
                            </span>
                        </div>
                    ))}
                </div>
            </div>
            <hr className="border-text w-full border-2" />
            <div className="h-main to-background flex w-full flex-col justify-center bg-gradient-to-b from-blue-500/80 p-4 text-center">
                <h1 className="font-poppins mt-8 mb-2 text-4xl font-bold underline lg:text-5xl">
                    Our top models
                </h1>
                <div className="flex flex-row flex-wrap justify-center gap-4">
                    {launches.map((launch, id) => (
                        <div
                            className="border-text bg-background/20 flex max-w-64 cursor-pointer flex-col rounded-md border-2 p-4 transition-transform duration-200 hover:scale-105"
                            key={id}
                        >
                            <h2 className="mb-2 text-2xl font-bold lg:text-3xl">
                                {launch.name}
                            </h2>
                            <p className="flex-1 text-lg lg:text-xl">
                                {launch.description}
                            </p>
                            <span className="font-bold">
                                <Icon name="bi-rocket-fill" />{' '}
                                {launch.date.toDateString()}
                            </span>
                        </div>
                    ))}
                </div>
            </div>
        </>
    );
}
