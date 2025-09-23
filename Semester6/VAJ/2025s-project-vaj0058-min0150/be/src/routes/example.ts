import { Route } from '$/types/route';
import { Router } from 'express';

const router = Router();

router.get('/', (req, res) => {
    res.send('Here will be all users');
});

router.get('/:id', (req, res) => {
    const { id } = req.params;
    res.send(`Here will be user with id ${id}`);
});

export default {
    router,
    mount: '/users'
} satisfies Route;
