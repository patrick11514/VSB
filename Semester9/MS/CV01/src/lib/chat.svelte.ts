import { EventEmitter } from 'eventemitter3';
import MQTT from 'paho-mqtt';
import { SvelteMap } from 'svelte/reactivity';

type Events = {
	'user-status': (name: string, online: boolean) => void;
	'list-update': (list: Map<string, boolean>) => void;
	message: (from: string, timestamp: number, message: string) => void;
	dmMessage: (chat: string, from: string, timestamp: number, message: string) => void;
	changeState: (state: 'online' | 'offline') => void;
};

const SEPARATOR = ' ';

export class Chat extends EventEmitter<Events> {
	private MQTT = $state() as MQTT.Client;
	private userList = new SvelteMap<string, boolean>();
	private online = false;

	constructor(
		private username?: string,
		private address?: string,
		private mqttUsername?: string,
		private mqttPassword?: string
	) {
		super();

		const [host, portStr] = (address ?? 'pcfeib425t.vsb.cz').split(':');
		const port = portStr ? parseInt(portStr, 10) : 9999;

		this.MQTT = new MQTT.Client(host, port, this.username ?? 'anon');

		this.connect();
	}

	connect() {
		const lastMessage = new MQTT.Message('offline');
		lastMessage.retained = true;
		lastMessage.destinationName = '/mschat/status/' + (this.username ?? 'anon');
		lastMessage.qos = 0;

		const connectOptions: MQTT.ConnectionOptions = {
			onSuccess: this.connected.bind(this),
			onFailure: this.onLost.bind(this),
			willMessage: lastMessage,
			timeout: 5,
			cleanSession: false
		};

		const isAuthProvided =
			(this.mqttUsername !== undefined && this.mqttUsername.trim() !== '') ||
			(this.mqttPassword !== undefined && this.mqttPassword !== '');

		if (isAuthProvided) {
			connectOptions.userName = this.mqttUsername?.trim()
				? this.mqttUsername.trim()
				: (this.username ?? 'username');
			connectOptions.password = this.mqttPassword ?? '';
		}

		this.MQTT.connect(connectOptions);
	}

	disconnect() {
		if (this.username && this.online) this.MQTT.send('/mschat/status/' + this.username, 'offline', 0, true);
		if (!this.online) return;

		setTimeout(() => {
			this.MQTT.disconnect();
			this.online = false;
		}, 1000);
	}

	private parseMessage(text: string): { timestamp: number; message: string } {
		const [rawTS, ...messageParts] = text.includes(SEPARATOR)
			? text.split(SEPARATOR)
			: text.includes('\t')
				? text.split('\t')
				: text.includes('\n')
					? text.split('\n')
					: text.split(' ');
		let message: string;
		let timestamp: number;

		const checkTimestmap = Number(rawTS);
		if (isNaN(checkTimestmap)) {
			message = text;
			timestamp = Date.now();
		} else {
			message = messageParts.join(' ');
			timestamp = checkTimestmap * 1000;
		}

		return { timestamp, message };
	}

	connected() {
		this.emit('changeState', 'online');
		this.online = true;

		this.MQTT.subscribe('/mschat/#');

		if (this.username) {
			this.MQTT.send('/mschat/status/' + this.username, 'online', 0, true);
		}

		this.MQTT.onMessageArrived = (msg) => {
			const { destinationName: dst, payloadString: value } = msg;

			console.log('DST', msg.destinationName);
			console.log('PLD', msg.payloadString);

			if (dst.startsWith('/mschat/status/')) {
				const username = dst.replace('/mschat/status/', '');
				const online = value.toLowerCase() == 'online';

				if (this.userList.get(username) !== undefined) {
					this.emit('user-status', username, online);
				}

				this.userList.set(username, online);

				this.emit('list-update', this.userList);
			} else if (dst.startsWith('/mschat/all/')) {
				const username = dst.replace('/mschat/all/', '');
				const { timestamp, message } = this.parseMessage(value);
				this.emit('message', username, timestamp, message);
			} else if (dst.startsWith('/mschat/user/')) {
				const names = dst.replace('/mschat/user/', '').split('/');
				if (names.length != 2) return;

				const [to, from] = names;

				if (to !== this.username) return;

				const { timestamp, message } = this.parseMessage(value);

				this.emit('dmMessage', from, from, timestamp, message);
			}
		};

		this.MQTT.onConnectionLost = this.onLost.bind(this);
	}

	onLost() {
		this.emit('changeState', 'offline');
		this.online = false;

		setTimeout(() => {
			if (!this.online) {
				this.connect();
			}
		}, 10000);
	}

	sendMessage(message: string, timestamp?: number) {
		this.MQTT.send(
			'/mschat/all/' + (this.username ?? 'anon'),
			Math.round((timestamp ?? Date.now()) / 1000).toString() + SEPARATOR + message,
			0,
			false
		);
	}

	sendDM(to: string, message: string, timestamp?: number) {
		const msg = Math.round((timestamp ?? Date.now()) / 1000).toString() + SEPARATOR + message;

		this.MQTT.send('/mschat/user/' + to + '/' + (this.username ?? 'anon'), msg, 0, false);

		//feedback  message back
		this.emit('dmMessage', to, this.username ?? 'anon', Date.now(), message);
	}
}
