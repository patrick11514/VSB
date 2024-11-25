/** @type {import('tailwindcss').Config} */
module.exports = {
    content: ["./src/**/*.{html,js,css}"],
    theme: {
        extend: {
            colors: {
                text: "#171A20",
                blue: "#3e6ae1",
                "cyber-text": "#a2a3a5",
                "cyber-bg": "#111"
            },
            fontFamily: {
                "universal-sans": "UniversalSans, Arial, sans-serif"
            },
            height: {
                hero: "calc(100vh - 44px)"
            },
            fontSize: {
                cyber: ["0.7rem", {
                    "letterSpacing": "0.125rem"
                }]
            }
        },
    },
    plugins: [],
}
