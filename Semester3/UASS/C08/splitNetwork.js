const fs = require('fs')

const file = fs.readFileSync('fb-pages-politician_timestamp.csv', 'utf-8')

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

    const [from, to, s, e] = line.trim().split(';')

    if (!start) {
        start = new Date(s)
    }

    if (!end) {
        end = new Date(e)
    }

    if (new Date(s).getTime() < start.getTime()) {
        start = new Date(s)
    }

    if (new Date(e).getTime() > end.getTime()) {
        end = new Date(e)
    }
})

const segments = 5
const segmentLength = (end.getTime() - start.getTime()) / segments

const segmentsArray = []

for (let i = 0; i < segments; i++) {
    segmentsArray.push({
        start: new Date(start.getTime() + i * segmentLength),
        end: new Date(start.getTime() + (i + 1) * segmentLength)
    })
}

const fileName = 'fb-%number%.csv'

console.log('Full start time: ', start, ' and end time: ', end, ' with ', segments, ' segments')

//create csv file and save it
segmentsArray.forEach((segment, index) => {
    let fileContent = ''
    console.log('Segment from: ', segment.start, ' to: ', segment.end)
    lines.forEach((line) => {
        if (line.trim() == '') return

        const [from, to, s, e] = line.trim().split(';')

        const startTime = new Date(s)
        const endTime = new Date(e)

        if (startTime.getTime() >= segment.start.getTime() || endTime.getTime() <= segment.end.getTime()) {
            fileContent += `${from};${to}\n`
        }
    })

    fs.writeFileSync(fileName.replace('%number%', index), fileContent)
})
