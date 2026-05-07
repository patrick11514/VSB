import crypto from 'crypto';
import fs from 'fs';
import path from 'path';
import solc from 'solc';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));

// Read the Solidity contract
const contractPath = path.join(__dirname, '../src/lib/contracts/Auction.sol');
const contractCode = fs.readFileSync(contractPath, 'utf8');

// Compile the contract
const input = {
	language: 'Solidity',
	sources: {
		'Auction.sol': {
			content: contractCode
		}
	},
	settings: {
		optimizer: {
			enabled: true,
			runs: 200
		},
		outputSelection: {
			'*': {
				'*': ['abi', 'evm']
			}
		}
	}
};

const output = JSON.parse(solc.compile(JSON.stringify(input)));

if (output.errors && output.errors.length > 0) {
	console.error('Compilation errors:', output.errors);
	process.exit(1);
}

const contract = output.contracts['Auction.sol']['Auction'];
const bytecode = contract.evm.bytecode.object;

// Create .env.local file with the bytecode
const envContent = `VITE_AUCTION_BYTECODE=${bytecode}\n`;
const envPath = path.join(__dirname, '../.env.local');

fs.writeFileSync(envPath, envContent);

console.log('Bytecode hash: ', crypto.createHash('sha256').update(bytecode).digest('hex'));

console.log('✓ Contract compiled successfully');
console.log('✓ Bytecode added to .env.local');
console.log(`✓ Bytecode length: ${bytecode.length} characters`);
