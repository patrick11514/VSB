import { app } from './src/server.js'
const port = 3000

app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
})