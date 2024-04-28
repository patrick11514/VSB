<script lang="ts">
    export let name: string;
</script>

<li class="nav-item">
    <a class="nav-link dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown" aria-expanded="false">{name}</a>
    <ul class="dropdown-menu">
        <slot />
    </ul>
</li>
