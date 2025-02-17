"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.Client = void 0;
const net_1 = __importDefault(require("net"));
class Client {
    ip;
    port;
    client;
    constructor(ip, port) {
        this.client = new net_1.default.Socket();
        this.ip = ip;
        this.port = port;
    }
    async connect() {
        return new Promise((resolve, reject) => {
            this.client.on('error', () => {
                reject();
            });
            this.client.connect(this.port, this.ip, () => {
                console.log(`Connected to: ${this.ip}:${this.port}`);
                resolve();
            });
        });
    }
    send(message) {
        console.log(`Message ${message} sent to ${this.ip}:${this.port}`);
        this.client.write(message);
    }
    destroy() {
        this.client.destroy();
    }
}
exports.Client = Client;
