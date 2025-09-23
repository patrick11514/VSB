import express from 'express'

const app = express()

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res) => {
  res.send('Hello World!')
})

// See tasks in README.md


// Get all users
app.get('/users/', async (req, res) => {
  
})

// Get specific user
app.get('/users/:id', async (req, res) => {

})

// Implement the rest of the end points




export { app };