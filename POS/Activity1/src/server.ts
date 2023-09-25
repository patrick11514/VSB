import dgram from 'node:dgram'
import { EventEmitter } from 'node:stream'

export class Server {
    port: number
    server: dgram.Socket
    ee: EventEmitter

    constructor(port: number) {
        this.server = dgram.createSocket('udp4')
        this.port = port
        this.ee = new EventEmitter()
    }

    async start() {
        return new Promise<void>((resolve, reject) => {
            this.server.on('listening', () => {
                const address = this.server.address()

                console.log(`Server is listening on: ${address.address}:${this.port}`)
            })

            this.server.on('message', (msg, rinfo) => {
                this.ee.emit('message', msg, rinfo)
            })

            this.server.bind(this.port)
        })
    }

    getEmitter() {
        return this.ee
    }
}
