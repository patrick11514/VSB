[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/oTvD4rc6)
## LAB Excercise

This excersise is focused on express web server and ORM library prisma.

Download Postman so it's easier to send requests to the server. Postman https://www.postman.com/downloads/
(or if you know any other tool, you can use that)

As with cloning any JS project, first run `npm install` to install all the dependencies listed in `package.json` file -- look into the file to see what dependencies are being installed.

You can start the project by running `npm run dev` in the terminal. Notice what is specified in `package.json` under dev script. The command `--watch` will automatically restart the server when you make changes to the code.

Notice the Hello World on when you viist http://localhost:3000 or when you make GET request with Postman to http://localhost:3000.

---

### Tasks - Prisma ORM

We will follow quick start for prisma tutorial but with combination of express, if you run into any prolems you can follow the guide here:
https://www.prisma.io/docs/getting-started/quickstart 


Steps:
1. Install the prisma in this project
 `npm install prisma --save-dev`

2. Init the sqlite `npx prisma init --datasource-provider sqlite`

3. Find the file `prisma/schema.prisma` 
and add into it:
```javascript
model User {
  id    Int     @id @default(autoincrement())
  email String  @unique
  name  String?
  posts Post[]
}

model Post {
  id        Int     @id @default(autoincrement())
  title     String
  content   String?
  published Boolean @default(false)
  author    User    @relation(fields: [authorId], references: [id])
  authorId  Int
}
```
Models in the Prisma schema have two main purposes:

Represent the tables in the underlying database
Serve as foundation for the generated Prisma Client API

4. Run the migration `npx prisma migrate dev --name init`
 
    This command did two things:
    It creates a new SQL migration file for this migration in the prisma/migrations directory.
    It runs the SQL migration file against the database.
    Because the SQLite database file didn't exist before, the command also created it inside the prisma directory with the name dev.db as defined via the environment variable in the .env file.

----------------
Now your DB and prisma is setup. 

Let's create a first user running `node script.js` it should print you the newly created user.

You can run `npx prisma studio` to see the database and the data in it. (You can also use any other tool to see the SQL Lite database)

We can also use the prisma studio to add dummy data to work on our endpoints.

Let's work on tasks in server.js
### Tasks:
Create REST API endpoints - based on the rest standards.
Add proper error handling to the endpoints.

#### Listing of user / posts
1. List all users
2. List specific user
3. List a specific post from a user
4. List all posts from user. Should have optional query parameter to filter only published posts

#### CRUD on user
1. Create new user
2. Update user
3. Delete user

#### Posts
1. Get all posts on the platform. (Add optional query paramter to filter only publishe posts)

#### Create new post
1. Create new post, user ID is mandatory


#### Example of good handling of the request:

```javascript
app.get('/users/:id', async (req, res) => {
    // We could add more validation of the incoming request params if we wanted here
    // and return status code 400 (bad request) with some error in case of incorrect input
    // FYI: You can do it manully or better on real project use library like zod / yoi / express-validator

    try{
        const user = await prisma.user.findUnique({
            where: {
                id: parseInt(req.params.id)
            }
        })
    
        if (user) {
            // If everything is OK, we return status 200 (default) and user object
            res.status(200).json(user)
        } else {
            // If we don't have user with that ID, we should return 404 (not found) based on the API design.
            res.status(404).json({ error: "We don't have user with that ID" })
        }
    }
    catch (e) {
        // It's crucial to console.error every error, so we can debug it later
        // Also usually don't want to send to the user stack trace (security reasons)
        console.error(e)
        // If we catched any other problem, we return status 500 (internal server error)
        res.status(500).json({ error: "There was server error getting user." })
    }
})
```

- How to send a Post request in Postman
  ![image](https://user-images.githubusercontent.com/8086995/223738127-fc28a41b-9f7c-45bf-8e05-045c072f7ae3.png)





