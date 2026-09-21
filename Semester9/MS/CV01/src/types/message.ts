export type Message =
	| {
			type: 'chatMessage';
			from: string;
			message: string;
			timestamp: number;
	  }
	| {
			type: 'statusUpdate';
			username: string;
			online: boolean;
			timestamp: number;
	  };
