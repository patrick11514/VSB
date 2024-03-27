<script lang="ts">
    import { SwalAlert } from "$lib"
    import IonButton from "../components/Ion/ionButton.svelte"
    import IonInput from "../components/Ion/ionInput.svelte"

    let url = "https://homel.vsb.cz/~mor03/TAMZ/TAMZ22.php"
    let login = ""
    let getDisabled = false
    let code = ""
    let sendDisabled = true
    let resetDisabled = true

    const getCode = async () => {
        if (login.length === 0) {
            return
        }
        getDisabled = true

        const params = new URLSearchParams()
        params.append("user", login)
        params.append("timestamp", Date.now().toString())

        const response = await fetch(`${url}?${params.toString()}`)
        const data = await response.text()

        if (!response.ok) {
            getDisabled = false
            SwalAlert({
                icon: "error",
                title: data,
            })
            return
        }

        code = data
        sendDisabled = false
    }

    const authenticate = async () => {
        if (code.length === 0) {
            return
        }

        sendDisabled = true

        const response = await fetch(url, {
            headers: {
                Authorization: "Bearer " + code,
            },
        })

        const data = await response.text()

        if (!response.ok) {
            SwalAlert({
                icon: "error",
                title: data,
            })
            sendDisabled = false
            return
        }

        SwalAlert({
            icon: "success",
            toast: false,
            timer: 0,
            position: "center",
            title: data,
        })
        resetDisabled = false
    }

    const reset = () => {
        resetDisabled = true
        getDisabled = false
        sendDisabled = true
        code = ""
        login = ""
    }
</script>

<ion-item>
    <IonInput
        type="text"
        label="Script URL"
        labelPlacement="stacked"
        bind:value={url}
    />
</ion-item>
<ion-item>
    <IonInput
        type="text"
        label="Login"
        labelPlacement="stacked"
        bind:value={login}
    />
</ion-item>
<ion-item>
    <IonInput
        type="text"
        label="Secret code"
        labelPlacement="stacked"
        bind:value={code}
        readonly={true}
    />
</ion-item>

<IonButton disabled={getDisabled} on:click={getCode}>Get secret code</IonButton>
<IonButton disabled={sendDisabled} on:click={authenticate}>
    Authenticate with code
</IonButton>
<IonButton disabled={resetDisabled} on:click={reset} color="danger">
    Reset
</IonButton>
