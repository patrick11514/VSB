const fs = require('fs')

const file = fs.readFileSync('network2/network.csv', 'utf-8')

const lines = file.split('\n')

//remove first line
lines.shift()

//get max frame
/**
 * @type Date
 */
let start
/**
 * @type Date
 */
let end

lines.forEach((line) => {
    if (line.trim() == '') return

    let [from, to, time] = line.trim().split(';')

    time = time * 1000

    if (!start) {
        start = new Date(time)
    }

    if (time > start.getTime()) {
        if (!end || time > end.getTime()) {
            end = new Date(time)
        }
    }
})

const segments = 8
const segmentLength = (end.getTime() - start.getTime()) / segments

const segmentsArray = []

for (let i = 0; i < segments; i++) {
    segmentsArray.push({
        start: new Date(start.getTime() + i * segmentLength),
        end: new Date(start.getTime() + (i + 1) * segmentLength),
    })
}

const fileName = 'network2/splited/network-%number%.csv'

console.log('Full start time: ', start, ' and end time: ', end, ' with ', segments, ' segments')

//create csv file and save it
segmentsArray.forEach((segment, index) => {
    let fileContent = ''
    console.log('Segment from: ', segment.start, ' to: ', segment.end)
    lines.forEach((line) => {
        if (line.trim() == '') return

        const [from, to, time] = line.trim().split(';')

        const currentTime = new Date(time * 1000)

        if (currentTime.getTime() >= segment.start.getTime() && segment.end.getTime() <= currentTime.getTime()) {
            fileContent += `${from};${to}\n`
        }
    })

    fs.writeFileSync(fileName.replace('%number%', index), fileContent)
})
