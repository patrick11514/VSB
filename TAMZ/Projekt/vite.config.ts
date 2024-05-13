import { sveltekit } from '@sveltejs/kit/vite';
import { SvelteKitPWA } from '@vite-pwa/sveltekit';
import { defineConfig } from 'vite';

export default defineConfig({
    plugins: [
        sveltekit(),
        SvelteKitPWA({
            manifest: {
                name: 'Bankovní Aplikace',
                short_name: 'bank_app',
                start_url: '/',
                display: 'standalone',
                background_color: '#ffffff',
                lang: 'cs',
                scope: '/'
            }
        })
    ]
});
