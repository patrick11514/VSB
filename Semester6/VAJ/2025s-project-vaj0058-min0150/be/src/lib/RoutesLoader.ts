import { Route } from '$/types/route';
import fs from 'node:fs/promises';
import path from 'node:path';
import Logger from './logger';
import { Express } from 'express';

const l = new Logger('RoutesLoader', 'blue');

export const loadRoutes = async (app: Express) => {
    l.start('Loading routes...');

    const dirs = [path.join(import.meta.dirname, '../routes')];
    const files: string[] = [];

    while (dirs.length) {
        const dir = dirs.pop()!;
        const content = await fs.readdir(dir, { withFileTypes: true });

        for (const file of content) {
            if (file.isDirectory()) {
                dirs.push(path.join(dir, file.name));
            } else if (
                file.isFile() &&
                (file.name.endsWith('.ts') || file.name.endsWith('.js'))
            ) {
                files.push(path.join(dir, file.name));
            }
        }
    }

    const routes = await Promise.all(
        files.map(async (file) => {
            const module = (await import(file)) as {
                default: Route;
            };
            if (!module.default) {
                throw new Error(`Route file ${file} does not export default`);
            }

            return [module.default.mount, module.default.router] as const;
        })
    );

    for (const [mount, router] of routes) {
        app.use(mount, router);
    }

    l.stop(`Loaded ${routes.length} routes`);
};
