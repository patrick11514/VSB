<script lang="ts">
    import Button from '$/components/Button.svelte';
    import FormItem from '$/components/FormItem.svelte';
    import Input from '$/components/Input.svelte';
    import { SwalAlert } from '$/lib/functions';
    import { UserService } from '$/lib/service/userService';
    import type { UserState } from '$/types/types';
    import { goto } from '$app/navigation';
    import { getContext } from 'svelte';
    import type { Writable } from 'svelte/store';

    type Entries = 'username' | 'password';
    type Data = Record<
        Entries,
        {
            value: string;
            error?: string;
        }
    >;

    const data = $state<Data>(
        Object.fromEntries(
            ['username', 'password'].map((item) => {
                return [item, { value: '', error: undefined }];
            })
        ) as Data
    );

    let userState = getContext<Writable<UserState>>('userState');

    const login = async () => {
        Object.values(data).map((item) => (item.error = undefined)); // reset

        if (!data.username.value) {
            data.username.error = 'Zadej prosím uživatelské jméno';
        }

        if (!data.password.value) {
            data.password.error = 'Zadej prosím heslo';
        }

        if (Object.values(data).some((item) => item.error)) {
            return;
        }

        const service = new UserService();
        const response = await service.loginUser(data.username.value, data.password.value);
        if (!response.status) {
            SwalAlert({
                icon: 'error',
                title: response.message
            });
            return;
        }

        userState.set({
            logged: true,
            data: response.data
        });

        SwalAlert({
            icon: 'success',
            title: 'Byl jsi úspěšně přihlášen'
        });
        goto('/');
    };
</script>

<section class="m-auto flex flex-col items-center justify-center rounded-md border-2 border-black p-4 text-center">
    <h1 class="mb-2 w-max border-b-2 border-b-black font-poppins text-2xl font-bold lg:text-3xl">Přihlášení</h1>
    <FormItem id="username" label="Uživatelské jméno" error={data.username.error}>
        <Input id="username" bind:value={data.username.value} bind:invalid={data.username.error} />
    </FormItem>
    <FormItem id="password" label="Heslo" error={data.password.error}>
        <Input id="password" type="password" bind:value={data.password.value} bind:invalid={data.password.error} />
    </FormItem>
    <Button onclick={login}>Přihlásit se</Button>
</section>
