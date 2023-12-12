import fs from 'node:fs'

const file = fs
    .readFileSync('soc-sign-bitcoinalpha.csv', 'utf-8')
    .trim()
    .split('\n')
    .map((l) => l.trim())

fs.writeFileSync('network2/network.csv', `Source;Target;Time\n`)

file.forEach((line) => {
    const [a, b, _, time] = line.split(',').map((s) => s.trim())

    const dateTime = new Date(parseInt(time) * 1000)
    const str = dateTime.toISOString().slice(0, -1)

    fs.appendFileSync('network2/network.csv', `${a};${b};${str}\n`)
})

console.log('done')
