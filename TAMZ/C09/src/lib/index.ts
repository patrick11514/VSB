export const formatTime = (date: Date) => {
	const days = date.getDate() > 9 ? date.getDate() : `0${date.getDate()}`;
	const months = date.getMonth() + 1 > 9 ? date.getMonth() + 1 : `0${date.getMonth() + 1}`;
	const hours = date.getHours() > 9 ? date.getHours() : `0${date.getHours()}`;
	const minutes = date.getMinutes() > 9 ? date.getMinutes() : `0${date.getMinutes()}`;
	const seconds = date.getSeconds() > 9 ? date.getSeconds() : `0${date.getSeconds()}`;

	return `${days}.${months}. ${hours}:${minutes}:${seconds}`;
};
