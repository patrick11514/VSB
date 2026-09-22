export type Message =
	| {
			type: 'chatMessage';
			from: string;
			message: string;
			timestamp: number;
			me: boolean;
	  }
	| {
			type: 'statusUpdate';
			username: string;
			online: boolean;
			timestamp: number;
	  };
