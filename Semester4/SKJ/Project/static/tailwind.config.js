/** @type {import('tailwindcss').Config} */
module.exports = {
    content: ['../templates/AnimeFigures/*.html', 'assets/js/*.ts'],
    theme: {
        extend: {
            colors: {
                text: '#0c0f11',
                background: '#f8fafc',
                primary: '#5598c2',
                secondary: '#89c0e3',
                accent: '#56afe8'
            }
        },
        fontFamily: {
            roboto: ['Roboto', 'sans-serif'],
            poppins: ['Poppins', 'sans-serif']
        }
    },
    plugins: []
}
