/** @type {import('tailwindcss').Config} */
module.exports = {
    content: ["./src/**/*.{html,ts,js,css}"],
    theme: {
        extend: {
            colors: {
                text: "#171A20",
                blue: "#3e6ae1",
                "cyber-text": "#a2a3a5",
                "cyber-bg": "#111",
                "black-tansparent": "rgba(0,0,0,0.05)",
                "text-gray": "rgb(92, 94, 98)"
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
            },
            boxShadow: {
                footer: "0 -8px 8px 0 rgba(0,0,0,.1)",
                list: "0 calc(3px) 0 -1px currentcolor",
                "list-empty": "rgba(0, 0, 0, 0) 0px 0px 0px 0px"
            },
        },
    },
    plugins: [],
}
