import fs from 'node:fs'

const file = fs
    .readFileSync('edit-enwikibooks.csv', 'utf-8')
    .split('\n')
    .map((l) => l.trim())

file.forEach((line) => {
    const [a, b, _, time] = line.split(';').map((s) => s.trim())

    fs.appendFileSync('network2/network.csv', `${a};${b};${time}\n`)
})
