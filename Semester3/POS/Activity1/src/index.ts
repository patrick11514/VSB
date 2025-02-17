import { Client } from './client'
import { Server } from './server'
import dgram from 'node:dgram'
import prompt from 'prompt'

const ip = '158.196.135.85'
const serverPort = 8000
const clientPort = 8010

prompt.start()

const sendMessage = async () => {
    prompt.get(['message'], async (_, data) => {
        const client = new Client(ip, serverPort)

        await client.connect()

        let message = data.message as string
        client.send(message)
        client.destroy()
        await sendMessage()
    })
}

//client
sendMessage()

const getTime = (date: Date) => {
    const hours = date.getHours()
    const minutes = date.getMinutes()
    const seconds = date.getSeconds()

    const strHours = hours > 9 ? hours : `0${hours}`
    const strMinutes = minutes > 9 ? minutes : `0${minutes}`
    const strSeconds = seconds > 9 ? seconds : `0${seconds}`

    return `${strHours}:${strMinutes}:${strSeconds}`
}

//server
;(async () => {
    const server = new Server(clientPort)

    const emitter = server.getEmitter()

    emitter.on('message', (message: Buffer, rInfo: dgram.RemoteInfo) => {
        const text = message.subarray(9, message.length - 1).toString()

        //ip from packet
        let getIp = ''
        for (let i = 0; i < 4; i++) {
            getIp += `${message[i]}.`
        }
        //remove dot from end
        getIp = getIp.slice(0, -1)

        //get time
        let time = message.subarray(4, 9).readInt32BE()
        const date = new Date(time * 1000)
        console.log(`[${getTime(date)}] Message from ${getIp}:${rInfo.port}: ${text}`)

        let clc = message[0]
        for (let i = 1; i < message.length - 1; i++) {
            clc = clc ^ message[i]
        }

        let clcFromMessage = message[message.length - 1]
        console.log(`Clc is ${clc} == ${clcFromMessage} => ${clc == clcFromMessage}`)
    })

    await server.start()
})()
