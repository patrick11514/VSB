<script lang="ts">
	import { formatTime } from '$/lib';
	import { onMount } from 'svelte';

	const size = 850;
	const xRects = 3;
	const yRects = 3;

	const colors = ['#4287f5', '#7bf542', '#eb1519', '#f242f5', '#0a0a0a', '#ff9100'];
	let hourColor = 2;
	let minuteColor = 1;
	let secondColor = 0;

	let canvas: HTMLCanvasElement;
	let context: CanvasRenderingContext2D | null;
	let baseDate = new Date();

	const showCenter = true;
	let showButtons = false;
	let showClock = true;

	type Point = {
		x: number;
		y: number;
	};

	const makeCircle = (
		ctx: CanvasRenderingContext2D,
		radius: number,
		startAngle: number,
		endAngle: number,
		clockwise = false
	) => {
		ctx.arc(
			size / 2,
			size / 2,
			radius,
			startAngle - (1 / 2) * Math.PI,
			endAngle - (1 / 2) * Math.PI,
			clockwise
		);
	};

	const makeRect = (ctx: CanvasRenderingContext2D, topLeft: Point, widthHeight: Point) => {
		ctx.rect(size / 2 + topLeft.x, size / 2 + topLeft.y, widthHeight.x, widthHeight.y);
	};

	let currentTime = '--.--. --:--:--';
	let audio: HTMLAudioElement | undefined = undefined;
	let minuteAudio: HTMLAudioElement | undefined = undefined;
	let hourAudio: HTMLAudioElement | undefined = undefined;
	let muted = false;

	const renderTick = () => {
		if (!context) {
			return;
		}

		if (!audio) return;

		const current = new Date();

		let time = new Date(baseDate);

		time.setHours(current.getHours());
		time.setMinutes(current.getMinutes());
		time.setSeconds(current.getSeconds());

		currentTime = formatTime(time);

		const hours = time.getHours();
		const minutes = time.getMinutes();
		const seconds = time.getSeconds();

		if (!muted) {
			if (minutes == 0) {
				hourAudio?.play();
			} else if (seconds == 0) {
				minuteAudio?.play();
			} else {
				//NEMAME VAS RADI XD
				//let aa = new Audio('/tick-tock.mp3');
				//aa.play();
				//let bb = new Audio('/gong.mp3');
				//bb.play();
				//let cc = new Audio('/tada.mp3');
				//cc.play();
				//let dd = new Audio(
				//	'https://cdn.discordapp.com/attachments/1195446134831849482/1232378488225533972/1195830298873692283.ogg?ex=66293d68&is=6627ebe8&hm=2c088bbaec8d242bd1f5da996ea42abebe592b9a3e2c8a977600506c975213c3&'
				//);
				//dd.play();
				//let ee = new Audio(
				//	'https://cdn.discordapp.com/attachments/748248027919286332/1232379047225462816/uwu.mp3?ex=66293ded&is=6627ec6d&hm=89b2f8547ee07f4686d0bb24b7fa1f4f424459848fdbda377e74155174772840&'
				//);
				//ee.volume = 1;
				//ee.play();
				//let ff = new Audio('/aaaaaaaaa.mp3');
				//ff.play();

				//CRAZY RANDOM NĚCO
				//audio.play();
				//minuteAudio.play();
				//hourAudio?.play();
				//setTimeout(
				//	() => {
				//		minuteAudio?.pause();
				//		audio?.pause();
				//		hourAudio?.pause();
				//	},
				//	(audio.duration / 2) * 1000
				//);

				audio.play();
				setTimeout(
					() => {
						audio?.pause();
					},
					(audio.duration / 2) * 1000
				);
			}
		}

		const day = time.getDate();
		const month = time.getMonth() + 1;

		let values = [seconds, minutes, hours];
		const maxValues = [60, 60, 24];
		const colorIndexes = [secondColor, minuteColor, hourColor];

		//reset
		context.reset();

		if (!showClock) {
			//dont't render clock
			return;
		}

		//main circle
		makeCircle(context, 200, 0, 2 * Math.PI);
		context.stroke();

		if (showCenter) {
			context.beginPath();
			context.lineWidth = 2;
			makeCircle(context, 2, 0, 2 * Math.PI);
			context.fill();
		}

		//inner circles
		for (const index in values) {
			context.strokeStyle = colors[colorIndexes[index]];
			context.lineWidth = 20;

			const i = parseInt(index);

			context.beginPath();
			// MAX .... 2PI
			// CURRENT .X
			//------------
			// X = 2PI * (Current/Max)
			makeCircle(context, 190 - i * 20, 0, 2 * Math.PI * (values[index] / maxValues[index]));
			context.stroke();
		}

		//binary day + month
		const bits = [5, 4];
		values = [day, month];
		const spacing = 20; //px
		//reset
		context.strokeStyle = '#000000';
		context.lineWidth = 1;

		for (const index in values) {
			const i = parseInt(index);

			const nOB = bits[index];

			let x = -((nOB * 20 + (nOB - 1) * 20) / 2);
			const y = Math.pow(-1, i + 1) * spacing * (2 - i);

			for (let i = nOB - 1; i >= 0; --i) {
				context.beginPath();
				makeRect(
					context,
					{
						x,
						y
					},
					{
						x: 20,
						y: 20
					}
				);

				x += spacing * 2;

				const value = values[index];

				if ((value >> i) & 0b1) {
					context.fill();
				}
				context.stroke();
			}
		}

		//render buttons
		if (showButtons) {
			for (let y = 0; y < yRects; ++y) {
				for (let x = 0; x < xRects; ++x) {
					context.beginPath();

					const width = size / xRects;
					const height = size / yRects;

					context.rect(x * width, y * height, width, height);
					context.font = '80px sans-serif';
					context.fillText(
						(y * xRects + x + 1).toString(),
						//x * width = get to correct corner
						// + width / 2 = to center by width
						// - 80 / 2 = center by width of number
						x * width + width / 2 - 80 / 2,
						//y * height = get to correct corner
						// + height / 2 = to center by height
						// + 80 / 2 = center by height of number
						y * height + height / 2 + 80 / 2
					);

					context.stroke();
				}
			}
		}
	};

	let timer: number | undefined = undefined;

	const sources = [
		{
			contentType: 'audio/ogg',
			source: '/tick-tock.ogg'
		},
		{
			contentType: 'audio/wav',
			source: '/tick-tock.wav'
		},
		{
			contentType: 'audio/mpeg',
			source: '/tick-tock.mp3'
		}
	] satisfies {
		contentType: `${string}/${string}`;
		source: string;
	}[];

	let currentSource: string = '';

	onMount(() => {
		if (!canvas) {
			return;
		}

		audio = new Audio();
		minuteAudio = new Audio('/gong.mp3');
		hourAudio = new Audio('/tada.mp3');

		for (const sourceIdx in sources) {
			const source = sources[sourceIdx];
			if (audio.canPlayType(source.contentType) != '') {
				currentSource = sourceIdx;
				audio.src = source.source;
				break;
			}
		}

		//audio.canPlayType;

		context = canvas.getContext('2d');

		renderTick();
		timer = setInterval(renderTick, 1000);

		document.addEventListener('keydown', handleKeyboard);

		return () => {
			if (timer) {
				clearInterval(timer);
			}

			document.removeEventListener('keydown', handleKeyboard);
		};
	});

	let date: string | undefined = undefined;
	$: {
		if (date != undefined) {
			baseDate = new Date(date);
		}
	}

	const toggleClock = () => {
		showClock = !showClock;
	};

	const changeHourColor = (offset: number) => {
		hourColor += offset;
		hourColor = hourColor % colors.length;
	};
	const changeMinuteColor = (offset: number) => {
		minuteColor += offset;
		minuteColor = minuteColor % colors.length;
	};
	const changeSecondColor = (offset: number) => {
		secondColor += offset;
		secondColor = secondColor % colors.length;
	};

	const formatDate = (date: Date) => {
		const day = date.getDate() > 9 ? date.getDate() : `0${date.getDate()}`;
		const month = date.getMonth() + 1 > 9 ? date.getMonth() + 1 : `0${date.getMonth() + 1}`;
		const year = date.getFullYear();
		return `${year}-${month}-${day}`;
	};

	const changeDate = (offset: number) => {
		if (!date) {
			date = formatDate(new Date());
		}

		const current = new Date(date);
		current.setDate(offset + current.getDate());

		date = formatDate(current);
	};

	const canvasClick = (
		ev: MouseEvent & {
			currentTarget: EventTarget & HTMLCanvasElement;
		}
	) => {
		const boundaries = ev.currentTarget.getBoundingClientRect();
		const x = ev.clientX - boundaries.x - 2; // 2  = border
		const y = ev.clientY - boundaries.y - 2;
		if (x < 0 || y < 0 || x > size || y > size) {
			//clicked on border
			return;
		}

		//calculate rectangle

		const xRect = Math.ceil(x / (size / xRects));
		const yRect = Math.ceil(y / (size / yRects));
		const rectId = (yRect - 1) * xRects + xRect;

		// Decrese day
		switch (rectId) {
			case 1:
				return changeHourColor(1);
			case 2:
				return changeMinuteColor(1);
			case 3:
				return changeSecondColor(1);
			case 4:
				return changeDate(-1);
			case 5:
				return toggleClock();
			case 6:
				return changeDate(1);
			case 7:
				return changeHourColor(-1);
			case 8:
				return changeMinuteColor(-1);
			case 9:
				return changeSecondColor(-1);
		}
	};

	const handleKeyboard = (ev: KeyboardEvent) => {
		const { key } = ev;

		if (isNaN(parseInt(key))) {
			return;
		}

		switch (parseInt(key)) {
			case 1:
				return changeHourColor(1);
			case 2:
				return changeMinuteColor(1);
			case 3:
				return changeSecondColor(1);
			case 4:
				return changeDate(-1);
			case 5:
				return toggleClock();
			case 6:
				return changeDate(1);
			case 7:
				return changeHourColor(-1);
			case 8:
				return changeMinuteColor(-1);
			case 9:
				return changeSecondColor(-1);
		}
	};

	const changeSource = (id: number) => {
		if (!audio) return;

		currentSource = id.toString();
		audio.src = sources[id].source;
	};
</script>

<main>
	<section>
		<canvas
			on:click={canvasClick}
			style="border:black 2px solid;"
			width={size}
			height={size}
			bind:this={canvas}
		/>
		<div class="flex">
			<h2>Info about Clock</h2>
			<group>
				<h3>Times</h3>
				<span>From inner: Hours, Minutes, Seconds</span>
			</group>
			<group>
				<h3>Binary date</h3>
				<span>Top: Days (max value 31 - 5bits), Bottom: Months (max value 12 - 4bits)</span>
			</group>
			<h3>Buttons</h3>
			{#if showButtons == false}
				<button on:click={() => (showButtons = true)}>Show buttons</button>
			{:else}
				<button on:click={() => (showButtons = false)}>Hide buttons</button>
			{/if}
			<table>
				<thead>
					<tr>
						<th>Button Number</th>
						<th>Action</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>1</td>
						<td>Change color of hours -> forward</td>
					</tr>
					<tr>
						<td>2</td>
						<td>Change color of minutes -> forward</td>
					</tr>
					<tr>
						<td>3</td>
						<td>Change color of seconds -> forward</td>
					</tr>
					<tr>
						<td>4</td>
						<td>Decrese day</td>
					</tr>
					<tr>
						<td>5</td>
						<td>Toggle clock show</td>
					</tr>
					<tr>
						<td>6</td>
						<td>Increse Day</td>
					</tr>
					<tr>
						<td>7</td>
						<td>Change color of hours -> backward</td>
					</tr>
					<tr>
						<td>8</td>
						<td>Change color of minutes -> backward</td>
					</tr>
					<tr>
						<td>9</td>
						<td>Change color of seconds -> backward</td>
					</tr>
				</tbody>
			</table>

			{#if muted === false}
				<button on:click={() => (muted = true)}>Mute sounds</button>
			{:else}
				<button on:click={() => (muted = false)}>Unmute sounds</button>
			{/if}
			<h2>Select source</h2>
			<div>
				{#each sources as source, i}
					<div>
						<input
							on:change={() => changeSource(i)}
							type="radio"
							id={i.toString()}
							checked={i.toString() == currentSource}
						/>
						<label for={i.toString()}>{source.contentType}</label>
					</div>
				{/each}
			</div>
		</div>
	</section>
	<div class="flex">
		<input type="date" bind:value={date} />
		{currentTime}
	</div>
</main>

<style>
	main {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
		width: 100%;
		height: 100%;
	}

	section {
		display: flex;
		flex-direction: row;
		align-items: center;
		justify-items: center;
		gap: 8px;
		width: 100%;
		height: 100%;
	}

	div.flex {
		display: flex;
		flex-direction: column;
		margin-right: auto;
		padding: 20px 0;
		gap: 8px;
	}

	table,
	th,
	td {
		border: black 1px solid;
		border-collapse: collapse;
		padding: 2px;
	}
</style>
