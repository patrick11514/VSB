# Info

TODO

## Dev mode

```bash
npm run dev

# or start the server and open the app in a new browser tab

npm run dev -- --open
```

## Build

Building app:

```bash
npm run build
```

Show builded preview: \`npm run preview\`.
Start builded app using `npm run start` with [Node Adapter](https://kit.svelte.dev/docs/adapter-node) or config command in package.json using your own [Adapter](https://kit.svelte.dev/docs/adapters)

## Example ENV file (.env.example)

```YAML
HOST=0.0.0.0
PORT=5178
ORIGIN=http://localhost:5178
DATABASE_IP=10.10.10.223
DATABASE_PORT=3306
DATABASE_USER=superclovek
DATABASE_PASSWORD=tajnyheslo123456
DATABASE_NAME=db
DATABASE_URL=mysql://superclovek:tajnyheslo123456@10.10.10.223:3306/db
JWT_SECRET=text
#v sekundách (10 min =  10 * 60)
#expiruje pouze pokud uživatel danou dobu nic nedělá (neprochází stránky)
COOKIE_EXPIRE=1200
#v sekundách (5 minut = 5 * 60)
PUBLIC_CHECK_COOKIE_INTERVAL=300
```
