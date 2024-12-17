<script lang="ts">
    import Button from '$/components/Button.svelte';
    import FormItem from '$/components/FormItem.svelte';
    import Input from '$/components/Input.svelte';
    import { API } from '$/lib/api';

    type Entries = 'username' | 'email' | 'password' | 'password2';
    type Data = Record<
        Entries,
        {
            value: string;
            error?: string;
        }
    >;

    const data = $state<Data>(
        Object.fromEntries(
            ['username', 'email', 'password', 'password2'].map((item) => {
                return [item, { value: '', error: undefined }];
            })
        ) as Data
    );

    const register = async () => {
        Object.values(data).map((item) => (item.error = undefined)); // reset

        if (!data.username.value) {
            data.username.error = 'Zadej prosím uživatelské jméno';
        }

        if (!data.email.value) {
            data.email.error = 'Zadej prosím email';
        }

        if (!data.password.value) {
            data.password.error = 'Zadej prosím heslo';
        }

        if (!data.password2.value) {
            data.password2.error = 'Zadej prosím heslo znovu';
        }

        if (data.password.value != data.password2.value) {
            data.password2.error = 'Hesla se neshodují';
        }

        if (Object.values(data).some((item) => item.error)) {
            return;
        }

        console.log(await API.auth.register(Object.fromEntries(Object.entries(data).map(([key, value]) => [key, value.value])) as Record<Entries, string>));
    };
</script>

<section class="m-auto flex flex-col items-center justify-center rounded-md border-2 border-black p-4 text-center">
    <h1 class="mb-2 w-max border-b-2 border-b-black font-poppins text-2xl font-bold lg:text-3xl">Registrace</h1>
    <FormItem id="username" label="Uživatelské jméno" error={data.username.error}>
        <Input id="username" bind:value={data.username.value} bind:invalid={data.username.error} />
    </FormItem>
    <FormItem id="email" label="Email" error={data.email.error}>
        <Input id="email" type="email" bind:value={data.email.value} bind:invalid={data.email.error} />
    </FormItem>
    <FormItem id="password" label="Heslo" error={data.password.error}>
        <Input id="password" type="password" bind:value={data.password.value} bind:invalid={data.password.error} />
    </FormItem>
    <FormItem id="password2" label="Heslo (znova)" error={data.password2.error}>
        <Input id="password2" type="password" bind:value={data.password2.value} bind:invalid={data.password2.error} />
    </FormItem>
    <Button onclick={register}>Registrovat se</Button>
</section>
