import fs from 'node:fs'

const file = fs.readFileSync('fb-pages-politician.csv', 'utf-8').trim()
const lines = file.split('\n')

enum Status {
    Visited,
    Unvisited,
}

const hashmap: Record<
    string,
    {
        status: Status
        neighbors: string[]
    }
> = {}

//remove csv header
lines.shift()

lines.forEach((line) => {
    const [left, right] = line.trim().split(';')

    if (!hashmap[left]) {
        hashmap[left] = {
            status: Status.Unvisited,
            neighbors: [],
        }
    }

    if (!hashmap[right]) {
        hashmap[right] = {
            status: Status.Unvisited,
            neighbors: [],
        }
    }

    if (!hashmap[left].neighbors.includes(right)) {
        hashmap[left].neighbors.push(right)
    }

    if (!hashmap[right].neighbors.includes(left)) {
        hashmap[right].neighbors.push(left)
    }
})

//get components

const components: {
    nodes: string[]
    size: number
}[] = []

Object.keys(hashmap).forEach((node) => {
    if (hashmap[node].status == Status.Visited) return

    const stack = []
    stack.push(node)

    const component = []
    let size = 0

    while (stack.length !== 0) {
        const element = stack.pop() as string

        if (hashmap[element].status == Status.Visited) continue

        hashmap[element].status = Status.Visited
        component.push(element)
        size++

        hashmap[element].neighbors.forEach((neighbor) => {
            if (hashmap[neighbor].status == Status.Visited) return

            stack.push(neighbor)
        })
    }

    components.push({
        nodes: component,
        size,
    })
})

console.log('Number of components: ' + components.length)

components.forEach((component, index) => {
    let file = ''

    const pairs: string[] = []

    component.nodes
        .sort((a, b) => {
            return parseInt(a) - parseInt(b)
        })
        .forEach((node) => {
            hashmap[node].neighbors.forEach((neighbor) => {
                if (pairs.includes(`${neighbor};${node}`)) return
                pairs.push(`${node};${neighbor}`)
                file += `${node};${neighbor}\n`
            })
        })

    fs.writeFileSync(`fb-component-${index + 1}.csv`, file.trim())
})
