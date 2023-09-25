import net from 'net'

export class Client {
    ip: string
    port: number
    client: net.Socket

    constructor(ip: string, port: number) {
        this.client = new net.Socket()
        this.ip = ip
        this.port = port
    }

    async connect() {
        return new Promise<void>((resolve, reject) => {
            this.client.on('error', () => {
                reject()
            })

            this.client.connect(this.port, this.ip, () => {
                console.log(`Connected to: ${this.ip}:${this.port}`)
                resolve()
            })
        })
    }

    send(message: string) {
        console.log(`Message ${message} sent to ${this.ip}:${this.port}`)
        this.client.write(message)
    }

    destroy() {
        this.client.destroy()
    }
}
