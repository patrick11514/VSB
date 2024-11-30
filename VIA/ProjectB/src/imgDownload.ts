const fs = require('fs');
const Path = require('path');

const images: Record<string, string> = {
    Hero1: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-APR-Promo-Hero-%-v3.png',
    Hero2: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-Model-Y-%-US-v2.png',
    Hero3: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-Model-3-%-US.png',
    Hero4: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-Model-X-%-US.png',
    Hero5: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-Model-S-%-US.png',
    Hero6: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-Cybertruck-%-v3.png',
    Hero7: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-SolarPanels-01-%',
    Hero8: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-SolarRoof-%-US.png',
    Hero9: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-Powerwall-%-US.png',
    Hero10: 'https://digitalassets.tesla.com/tesla-contents/image/upload/f_auto,q_auto/Homepage-Accessories-%-US.png'
};

const DEST = './src/assets/images';
const TYPES = ['Mobile', 'Desktop'] as const;

const download = (name: string, path: string) => {
    const currTypes = [...TYPES, ...(name === 'Hero1' ? ['Tablet'] : [])];

    const promiseArray = [];

    for (const type of currTypes) {
        promiseArray.push(
            (async () => {
                const URL = path.replace('%', type);
                const response = await fetch(URL);
                const data = await response.arrayBuffer();
                fs.writeFileSync(Path.join(DEST, `${name}-${type}.png`), Buffer.from(data));
            })()
        );
    }

    return promiseArray;
};

const promises = Object.entries(images)
    .map(([name, path]) => download(name, path))
    .reduce((a, b) => [...a, ...b], []);
Promise.all(promises);
