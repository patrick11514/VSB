//@ts-ignore
import * as Express from 'express';

const app = Express();

app.get('*', async (req, res) => {
    const url = req.url.substring(1);

    if (!url.startsWith('https')) {
        res.send('FAIL');
        return;
    }

    console.log('Forwarding: ' + url);

    const response = await fetch(url, {
        headers: req.headers as HeadersInit
    });

    const data = await response.text();

    res.setHeader('Content-Type', 'application/json').status(response.status).send(data);
});

app.listen(3000, () => {
    console.log('Listening on 3000');
});
