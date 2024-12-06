/** @type {import('tailwindcss').Config} */
module.exports = {
    content: ["./src/**/*.{html,ts,js,css}", "!./src/assets/ddragon/**/*"],
    theme: {
        extend: {
            screens: {
                sxl: "1200px"
            },
            colors: {
                'text': '#dae3fa',
                'background': '#040c21',
                'primary': '#88acf0',
                'secondary': '#321189',
                'accent': '#8a2be4',
                "gold": "rgb(255, 155, 0)"
            },
            fontFamily: {
                "roboto": "Roboto"
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
            fontSize: {
                "ssm": ["0.75rem", {
                    lineHeight: "1rem"
                }]
            }
        },
    },
    plugins: [],
}
