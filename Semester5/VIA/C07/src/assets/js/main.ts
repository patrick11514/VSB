type Method = "GET" | "POST" | "PUT" | "PATCH" | "DELETE"

const fetchData = async <T>(path: string, method: Method = "GET", data?: object) => {
    try {
        const response = await fetch(path, {
            method,
            body: data ? JSON.stringify(data) : undefined
        })

        const json = await response.json();

        return json as T;
    } catch (e) {
        console.error(e);
        return undefined;
    }
}

type Data = {
    den: string
    denc: string
    banka: string
    kurzy: {
        jednotka: number
        dev_stred: number
        dev_nakup: number
        dev_prodej: number
    }[]
}

const select = document.querySelector("#currency")
let fetchedData: Data

const getData = async () => {
    const data = await fetchData<Data>("https://data.kurzy.cz/json/meny/b[1].json")

    if (!data) return

    fetchedData = data;

    for (const currency in fetchedData.kurzy) {
        const option = document.createElement("option");
        option.value = currency
        option.innerText = currency
        select?.appendChild(option)
    }
}

getData()

const output = document.querySelector("#output");

document.querySelector("#input")?.addEventListener("input", (ev) => {
    const target = ev.currentTarget as HTMLInputElement | null;

    if (!target) return;


})
