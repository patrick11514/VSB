/** @type {import('tailwindcss').Config} */
export default {
    content: ['./src/**/*.{html,js,svelte,ts}'],
    theme: {
        extend: {
            fontFamily: {
                kanit: ['Kanit', 'sans-serif'],
                roboto: ['Roboto', 'sans-serif'],
                ubuntu: ['Ubuntu', 'sans-serif']
            }
        }
    },
    plugins: []
};
