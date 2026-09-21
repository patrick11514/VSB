import { EventEmitter } from 'eventemitter3';
import MQTT from 'paho-mqtt';
import { SvelteMap } from 'svelte/reactivity';

type Events = {
	'user-status': (name: string, online: boolean) => void;
	'list-update': (list: Map<string, boolean>) => void;
	message: (from: string, timestamp: number, message: string) => void;
};

export class Chat extends EventEmitter<Events> {
	private MQTT = $state() as MQTT.Client;
	private userList = new SvelteMap<string, boolean>();

	constructor(private username?: string) {
		super();

		this.MQTT = new MQTT.Client('pcfeib425t.vsb.cz', 9999, this.username ?? 'anon');
		this.MQTT.connect({
			onSuccess: this.connected.bind(this)
		});
	}

	disconnect() {
		if (this.username) this.MQTT.send('/mschat/status/' + this.username, 'offline', 0, true);

		setTimeout(() => {
			this.MQTT.disconnect();
		}, 1000);
	}

	connected() {
		this.MQTT.subscribe('/mschat/#');

		if (this.username) this.MQTT.send('/mschat/status/' + this.username, 'online', 0, true);

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
				const [rawTS, ...messageParts] = value.split(' ');
				let message: string;
				let timestamp: number;

				const checkTimestmap = Number(rawTS);
				if (isNaN(checkTimestmap)) {
					message = value;
					timestamp = Date.now();
				} else {
					message = messageParts.join(' ');
					timestamp = checkTimestmap * 1000;
				}

				this.emit('message', username, Number(timestamp), message);
			}
		};
	}

	sendMessage(message: string) {
		this.MQTT.send(
			'/mschat/all/' + (this.username ?? 'anon'),
			Math.round(Date.now() / 1000).toString() + ' ' + message,
			0,
			false
		);
	}
}
