const fs = require('fs')

const file = fs.readFileSync('data.json', 'utf-8')

const json = JSON.parse(file)

let csv = 'Source;Target;X;Y\n'

const nodes = {}

json.nodes.forEach((node) => {
    nodes[node.key] = {
        x: node.attributes.x,
        y: node.attributes.y
    }
})

json.edges.forEach((edge) => {
    const startNode = nodes[edge.source]
    const endNode = nodes[edge.target]
    csv += `${edge.source};${edge.target};${startNode.x};${startNode.y}\n`
})

fs.writeFileSync('data.csv', csv)
