<script lang="ts">
    export let title: string;
    export let value: string | number | undefined = undefined;
    export let type: 'text' | 'number' | 'email' | 'password' = 'text';

    let ID = Math.random().toString(36).substr(2, 9);
</script>

<div class="form-group">
    <label for={ID}>{title}</label>
    <input {type} {value} on:input={(ev) => (value = ev.currentTarget.value)} id={ID} class="form-control" />
</div>
