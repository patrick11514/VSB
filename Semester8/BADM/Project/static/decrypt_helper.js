#!/usr/bin/env node
// Simple Node.js helper to decrypt payloads produced by the web UI
// Usage: node decrypt_helper.js <encrypted> <passphrase> > decrypted.bin

const fs = require('fs');
const crypto = require('crypto');

function usage() {
  console.error('Usage: node decrypt_helper.js <encrypted> <passphrase> > decrypted.bin');
  process.exit(1);
}

const [, , encPath, passphrase] = process.argv;
if (!encPath || !passphrase) usage();

const data = fs.readFileSync(encPath);
if (data.length < 28) {
  console.error('Encrypted file too small or corrupted');
  process.exit(2);
}

const salt = data.slice(0, 16);
const iv = data.slice(16, 28);
const cipher = data.slice(28);

// Derive key using PBKDF2 with 100000 iterations (SHA-256)
const key = crypto.pbkdf2Sync(Buffer.from(passphrase, 'utf8'), salt, 100000, 32, 'sha256');

// AES-GCM tag is appended to the ciphertext (last 16 bytes)
if (cipher.length < 16) {
  console.error('Ciphertext too short');
  process.exit(2);
}
const tag = cipher.slice(cipher.length - 16);
const ciphertext = cipher.slice(0, cipher.length - 16);

try {
  const decipher = crypto.createDecipheriv('aes-256-gcm', key, iv);
  decipher.setAuthTag(tag);
  const out = Buffer.concat([decipher.update(ciphertext), decipher.final()]);
  process.stdout.write(out);
} catch (err) {
  console.error('Decryption failed:', err.message);
  process.exit(3);
}
