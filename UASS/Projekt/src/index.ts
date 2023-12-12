import fs from 'node:fs'

const file = fs
    .readFileSync('soc-sign-bitcoinalpha.csv', 'utf-8')
    .split('\n')
    .map((l) => l.trim())

fs.writeFileSync('network2/network.csv', `Source;Target;From;To\n`)

file.forEach((line) => {
    const [a, b, time] = line.split(';').map((s) => s.trim())

    fs.appendFileSync('network2/network.csv', `${a};${b};${time};${time}\n`)
})
