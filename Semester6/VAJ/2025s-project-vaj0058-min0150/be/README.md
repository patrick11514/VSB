# 🚀 VAJ - Backend

This is the **backend** of the **VAJ** project, built with **Express 5**, **TypeScript**, and uses **Prisma** for database access. It's designed to provide a robust and scalable API to support the frontend. 🛠️

---

## 📦 Installation

First, make sure you have **pnpm** or **npm** installed.

Install dependencies:

```bash
pnpm install
```

---

## ⚙️ Environment Setup

1. Copy the example environment variables file:

    ```bash
    cp .env.example .env
    ```

2. Modify `.env` as needed. Default values:
    ```env
    PORT=3000
    DATABASE_URL="file:./dev.db"
    ```

---

## 🛠️ Development

To start the backend in development mode:

```bash
pnpm dev
```

This runs with **tsx watch** for fast TypeScript reloading.

---

## 🏗️ Build & Start

To build the project for production:

```bash
pnpm build
```

Then start it:

```bash
pnpm start
```

---

## 📚 Tech Stack

- 🌐 **Express 5** – Web framework for building APIs
- 🗄 **Prisma** – Type-safe ORM for database access
- 🧪 **Zod** – Runtime schema validation
- 📦 **dotenv** – Environment variable management
- 🎨 **cli-color** – Colored CLI output
- 🧹 **Prettier** & **ESLint** – Code formatting and linting
- 💬 **TypeScript** – Strong typing and better dev experience

---

## 📜 Scripts

- `pnpm dev` – Run in development mode
- `pnpm build` – Compile TypeScript to JS
- `pnpm start` – Start the production build
- `pnpm check` – Type-check the code
- `pnpm format` – Format the codebase
- `pnpm lint` – Lint the code
- `pnpm migrate` – Run Prisma migrations
- `pnpm studio` – Open Prisma Studio (visual DB browser)

---

## 📂 Database

This project uses **SQLite** by default (`DATABASE_URL="file:./dev.db"`), but you can configure it for other databases in your `.env`.

Use **Prisma Migrate** to manage schema changes:

```bash
pnpm migrate
```

Or open **Prisma Studio** to explore your database:

```bash
pnpm studio
```
