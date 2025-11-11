const wrapper = document.querySelector("#page-content");

const getInputs = () => Array.from(wrapper.querySelectorAll("input")).filter(i => i.type != "hidden")

const getSelects = () => Array.from(wrapper.querySelectorAll("select"))


const save = () => {
    const inputs = getInputs();
	const pairs = inputs.map((input, idx) => [idx, input.value])

    const selects = getSelects();
    const selectPairs = selects.map((select, idx) => [idx, select.value])

    const data = {
        "inputs": pairs,
        "selects": selectPairs
    };

    localStorage.setItem("backup-data", JSON.stringify(data));
}

const load = () => {
    const data = JSON.parse(localStorage.getItem("backup-data"));
    if (!data) return;

    const inputs = getInputs();
    data.inputs.forEach(([idx, value]) => {
        if (inputs[idx]) {
            inputs[idx].value = value;
        }
    });

    const selects = getSelects();
    data.selects.forEach(([idx, value]) => {
        if (selects[idx]) {
            selects[idx].value = value;
        }
    });
}