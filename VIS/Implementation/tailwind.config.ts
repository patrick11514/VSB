import type { Config } from 'tailwindcss';

export default {
    content: ['./src/**/*.{html,js,svelte,ts}'],

    theme: {
        extend: {
            colors: {
                text: '#060d0d',
                background: '#f1fafa',
                primary: '#3fd6c8',
                secondary: '#83efe5',
                accent: '#43f6e5'
            },
            fontFamily: {
                poppins: ['Poppins', 'sans-serif'],
                roboto: ['Roboto', 'sans-serif']
            }
        }
    },

    plugins: []
} satisfies Config;
