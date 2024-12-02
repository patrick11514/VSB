const PROXY = 'http://localhost:3000/' as const;
const ROOT = `${PROXY}https://%REGION%.api.riotgames.com` as const;
const DDRAGON_VERSION = '14.23.1' as const;

const ROUTING = ['AMERICAS', 'EUROPE', 'ASIA', 'SEA'] as const;
type Routing = (typeof ROUTING)[number];

const REGIONS = [
    'BR1',
    'EUN1',
    'EUW1',
    'JP1',
    'KR',
    'LA1',
    'LA2',
    'ME1',
    'NA1',
    'OC1',
    'PH2',
    'RU',
    'SG2',
    'TH2',
    'TR1',
    'TW2',
    'VN2'
] as const;

type Region = (typeof REGIONS)[number];

const MAPPINGS = {
    AMERICAS: ['NA1', 'BR1', 'LA1', 'LA2'],
    ASIA: ['KR', 'JP1'],
    EUROPE: ['EUN1', 'EUW1', 'TR1', 'RU'],
    SEA: ['OC1', 'PH2', 'SG2', 'TH2', 'TW2', 'VN2']
} as const satisfies Record<Routing, Region[]>;

type RiotResponse<$Data> =
    | {
        status: {
            status_code: number;
            message: string;
        };
    }
    | $Data;

const getData = async <$Return>(
    route: string,
    regionOrRouting: Region | Routing
): Promise<$Return | undefined> => {
    try {
        const response = await fetch(ROOT.replace('%REGION%', regionOrRouting) + route, {
            headers: {
                'X-Riot-Token': env.API_KEY
            }
        });

        const json = await response.json();

        return json;
    } catch (err) {
        console.error(err);
        return undefined;
    }
};

type Entries<T> = {
    [K in keyof T]: [K, T[K]];
}[keyof T][];

const AssetMap = {
    challenges: 'img/challeges/images',
    profileImage: `${DDRAGON_VERSION}/img/profileicon`
} as const satisfies Record<string, string>;

class RiotAPI {
    private static getRoutingByRegion(region: Region): Routing {
        for (const [routing, regions] of Object.entries(MAPPINGS) as Entries<
            typeof MAPPINGS
        >) {
            if (region in regions) return routing;
        }

        return 'EUROPE'; //DEFAULT
    }

    static getAsset(asset: keyof typeof AssetMap, key: string | number) {
        return `/assets/ddragon/${AssetMap[asset]}/${key}.png`;
    }

    static async getUser(username: string, tag: string, region: Region) {
        const data = await getData<{
            puuid: string;
            gameName: string;
            tagLine: string;
        }>(
            `/riot/account/v1/accounts/by-riot-id/${username}/${tag}`,
            this.getRoutingByRegion(region)
        );

        return data;
    }

    static async getSummoner(puuid: string, region: Region) {
        const data = await getData<{
            accountId: string;
            id: string;
            profileIconId: number;
            puuid: string;
            revisionDate: string;
            summonerLevel: number;
        }>(`/lol/summoner/v4/summoners/by-puuid/${puuid}`, region);

        return data;
    }
}
