"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.Server = void 0;
const node_dgram_1 = __importDefault(require("node:dgram"));
const node_stream_1 = require("node:stream");
class Server {
    port;
    server;
    ee;
    constructor(port) {
        this.server = node_dgram_1.default.createSocket('udp4');
        this.port = port;
        this.ee = new node_stream_1.EventEmitter();
    }
    async start() {
        return new Promise((resolve, reject) => {
            this.server.on('listening', () => {
                const address = this.server.address();
                console.log(`Server is listening on: ${address.address}:${this.port}`);
            });
            this.server.on('message', (msg, rinfo) => {
                this.ee.emit('message', msg, rinfo);
            });
            this.server.bind(this.port);
        });
    }
    getEmitter() {
        return this.ee;
    }
}
exports.Server = Server;
