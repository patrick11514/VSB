// Map contract error messages to user-friendly messages
function formatContractError(errorMsg: string): string {
	const lower = errorMsg.toLowerCase();

	// Contract access control errors
	if (lower.includes('only owner')) {
		return 'Only the auction owner can perform this action.';
	}
	if (lower.includes('only winner')) {
		return 'Only the auction winner can perform this action.';
	}

	// State and timing errors
	if (lower.includes('invalid state')) {
		return 'The auction is not in the correct state for this action.';
	}
	if (lower.includes('auction not ended')) {
		return 'The auction has not ended yet. Wait until the end time.';
	}
	if (lower.includes('bidding ended')) {
		return 'Bidding has ended. No more bids are accepted.';
	}

	// Bidding errors
	if (lower.includes('cannot bid 0')) {
		return 'You cannot bid zero. Please enter a valid bid amount.';
	}
	if (lower.includes('total bid not high enough')) {
		return 'Your total bid is not high enough. Please increase your bid.';
	}

	// Passphrase errors
	if (lower.includes('invalid passphrase')) {
		return 'The passphrase is incorrect. Please verify and try again.';
	}

	// Refund errors
	if (lower.includes('winner cannot refund')) {
		return 'The auction winner cannot request a refund. Only other bidders can.';
	}
	if (lower.includes('no funds to refund')) {
		return 'You have no funds to refund. You may not have placed any bids.';
	}

	// Transaction failures
	if (lower.includes('payout to winner failed') || lower.includes('refund failed')) {
		return 'The transaction failed. Please try again.';
	}

	// Generic revert
	if (lower.includes('revert') || lower.includes('error')) {
		return errorMsg;
	}

	return errorMsg;
}

export default function formatError(err: unknown, ctx?: { minBidEth?: string }) {
	// string
	if (typeof err === 'string') {
		const s = String(err).toLowerCase();
		// MetaMask rejection
		if (s.includes('rejected') || s.includes('user rejected')) {
			return 'Transaction rejected by user.';
		}
		// Invalid passphrase (decryption failure)
		if (s.includes('unsupported state') || s.includes('unable to authenticate')) {
			return "You've entered an invalid passphrase. Please check and try again.";
		}
		// Try contract error formatting
		return formatContractError(err);
	}

	// Ethers.js common shapes - treat as unknown record
	const e = err as unknown as Record<string, unknown> | null;

	// direct reason field
	if (e && typeof e['reason'] === 'string') {
		const r = String(e['reason']);
		const rLower = r.toLowerCase();
		// MetaMask rejection
		if (rLower.includes('rejected') || rLower.includes('user rejected')) {
			return 'Transaction rejected by user.';
		}
		// Invalid passphrase
		if (rLower.includes('unsupported state') || rLower.includes('unable to authenticate')) {
			return "You've entered an invalid passphrase. Please check and try again.";
		}
		if (r === 'Bid too low' && ctx?.minBidEth) {
			return `Your bid is too low — current highest is ${ctx.minBidEth} ETH; please bid higher.`;
		}
		return formatContractError(r);
	}

	// revert.args from some ethers shapes
	type RevertShape = { args?: unknown[] };
	if (
		e &&
		e['revert'] &&
		Array.isArray((e['revert'] as RevertShape).args) &&
		typeof ((e['revert'] as RevertShape).args as unknown[])[0] === 'string'
	) {
		const msg = String(((e['revert'] as RevertShape).args as unknown[])[0]);
		if (msg === 'Bid too low' && ctx?.minBidEth) {
			return `Your bid is too low — current highest is ${ctx.minBidEth} ETH; please bid higher.`;
		}
		return formatContractError(msg);
	}

	// try parse message containing execution reverted
	if (e && typeof e['message'] === 'string') {
		const m = String(e['message']);
		const mLower = m.toLowerCase();
		// MetaMask rejection
		if (mLower.includes('rejected') || mLower.includes('user rejected')) {
			return 'Transaction rejected by user.';
		}
		// Invalid passphrase
		if (mLower.includes('unsupported state') || mLower.includes('unable to authenticate')) {
			return "You've entered an invalid passphrase. Please check and try again.";
		}
		// pattern: execution reverted: "Reason"
		const m1 = m.match(/execution reverted: "?([^"\n]+)"?/i);
		if (m1 && m1[1]) {
			const reason = m1[1];
			if (reason === 'Bid too low' && ctx?.minBidEth) {
				return `Your bid is too low — current highest is ${ctx.minBidEth} ETH; please bid higher.`;
			}
			return formatContractError(reason);
		}

		// fallback to trimmed message
		return formatContractError(m.split('\n')[0]);
	}

	// last-resort
	try {
		return JSON.stringify(err);
	} catch {
		return String(err);
	}
}
