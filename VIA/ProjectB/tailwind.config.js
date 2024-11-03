/** @type {import('tailwindcss').Config} */
module.exports = {
    content: ["./src/**/*.{html,js,css}"],
    theme: {
        extend: {
            colors: {
                text: "#171A20"
            },
            fontFamily: {
                "universal-sans": "UniversalSans, Arial, sans-serif"
            },
            height: {
                hero: "calc(100vh - 44px)"
            }
        },
    },
    plugins: [],
}
