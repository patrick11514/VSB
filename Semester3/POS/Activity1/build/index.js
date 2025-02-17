"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const client_1 = require("./client");
const server_1 = require("./server");
const ip = '158.196.135.85';
const serverPort = 8000;
const clientPort = 8010;
(async () => {
    const client = new client_1.Client(ip, serverPort);
    await client.connect();
    client.send('Random zprava');
    client.destroy();
})();
const getTime = (date) => {
    const hours = date.getHours();
    const minutes = date.getMinutes();
    const seconds = date.getSeconds();
    const strHours = hours > 9 ? hours : `0${hours}`;
    const strMinutes = minutes > 9 ? minutes : `0${minutes}`;
    const strSeconds = seconds > 9 ? seconds : `0${seconds}`;
    return `${strHours}:${strMinutes}:${strSeconds}`;
};
(async () => {
    const server = new server_1.Server(clientPort);
    const emitter = server.getEmitter();
    emitter.on('message', (message, rInfo) => {
        const text = message.subarray(9, message.length - 1).toString();
        let getIp = '';
        for (let i = 0; i < 4; i++) {
            getIp += `${message[i]}.`;
        }
        getIp = getIp.slice(0, -1);
        let time = message.subarray(4, 9).readInt32BE();
        const date = new Date(time * 1000);
        console.log(`[${getTime(date)}] Message from ${getIp}:${rInfo.port}: ${text}`);
        let clc = message[0];
        for (let i = 1; i < message.length - 1; i++) {
            clc = clc ^ message[i];
        }
        let clcFromMessage = message[message.length - 1];
        console.log(`Clc is ${clc} == ${clcFromMessage} => ${clc == clcFromMessage}`);
    });
    await server.start();
})();
