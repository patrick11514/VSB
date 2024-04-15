---
theme: gaia
_class: lead
paginate: true
backgroundColor: #fff
fontFamily: "Roboto"
marp: true
---

![bg left:40% 80%](https://marp.app/assets/marp.svg)

# **Marp**

Markdown Presentation Ecosystem

https://marp.app/

---

# How to write slides

Split pages by horizontal ruler (`---`). It's very simple! :satisfied:

```svelte
<script lang="ts">
    export let test: string = "ahoj"
</script>

{#each item as test}
    <h2>{item}</h2>
{/each}
```