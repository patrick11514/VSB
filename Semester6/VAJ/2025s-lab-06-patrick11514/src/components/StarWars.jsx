import { useEffect } from 'react';
import { useState } from 'react';

async function fetchMovieList(signal) {
    const response = await fetch(`https://swapi.dev/api/films/?format=json`, { signal });
    const data = await response.json();
    return data;
}

async function fetchMovieData(movieId, signal) {
    console.log(movieId);
    const response = await fetch(`https://swapi.dev/api/films/${movieId}/?format=json`, {
        signal
    });
    const data = await response.json();
    return data;
}

export function StarWars() {
    const [movieData, setMovieData] = useState(null);
    const [movieId, setMovieId] = useState('1');
    const [data, setData] = useState(null);

    useEffect(() => {
        const controller = new AbortController();
        const signal = controller.signal;

        fetchMovieList(signal).then(setMovieData);

        return () => {
            controller.abort();
        };
    }, []);

    // TODO: write an useEffect hook that will call the Star Wars API every time the movie ID changes
    // and updates the `data` state value with the result of the API call
    // You can use the `fetchMovieData` helper to call the API
    useEffect(() => {
        const controller = new AbortController();
        const signal = controller.signal;

        fetchMovieData(movieId, signal).then(setData);

        return () => controller.abort();
    }, [movieId]);

    return (
        <div>
            <h1>Star Wars API</h1>
            <select
                value={movieId}
                onChange={(e) => {
                    console.log(e, e.target.value);
                    setMovieId(e.target.value);
                }}
            >
                {movieData &&
                    movieData.results.map((movie, idx) => (
                        // idx + 1 because episodes the url /films/X takes index of episode, not their number
                        <option key={movie.episode_id} value={idx + 1}>
                            {movie.title}
                        </option>
                    ))}
            </select>
            <pre>{JSON.stringify(data, null, 2)}</pre>
        </div>
    );
}
