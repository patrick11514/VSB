import express from 'express';
import { env } from './types/env';
import { loadRoutes } from './lib/RoutesLoader';
import Logger from './lib/logger';

const app = express();
const l = new Logger('API', 'yellow');
l.start('Starting server...');

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res) => {
    res.send('Hello World!');
});

loadRoutes(app);

app.listen(env.PORT, () => {
    l.stop(`Server started on port ${env.PORT}`);
});
