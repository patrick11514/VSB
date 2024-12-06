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

const QUEUES = {
    0: 'Custom',
    400: 'Draft Pick',
    430: 'Blind Pick',
    420: 'Solo/Duo',
    440: 'Flex',
    450: 'ARAM',
    700: 'Clash',
    720: 'ARAM Clash',
    830: 'Co-op vs AI',
    840: 'Co-op vs AI',
    850: 'Co-op vs AI',
    900: 'URF',
    1020: 'One for All',
    1300: 'Snow ARAM',
    1400: 'Nexus Blitz',
    1700: 'Arény (Cherry)'
} as const;

const COOP_TITLES = ['Intro', 'Beginner', 'Intermediate'] as const;

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
): Promise<RiotResponse<$Return> | undefined> => {
    try {
        const RETRY_STATUSES = [429, 503];

        while (true) {
            const response = await fetch(
                ROOT.replace('%REGION%', regionOrRouting) + route,
                {
                    headers: {
                        'X-Riot-Token': env.API_KEY
                    }
                }
            );

            const json = await response.json();

            if ('status' in json && 'status_code' in json.status) {
                if (RETRY_STATUSES.includes(json.status.status_code)) {
                    //sleep end retry
                    await sleep(1 * 1000);
                    continue;
                }
            }

            return json;
        }
    } catch (err) {
        console.error(err);
        return undefined;
    }
};

type Entries<T> = {
    [K in keyof T]: [K, T[K]];
}[keyof T][];

const AssetMap = {
    challenges: 'img/challenges-images',
    profileImage: `${DDRAGON_VERSION}/img/profileicon`,
    champion: `${DDRAGON_VERSION}/img/champion`,
    item: `${DDRAGON_VERSION}/img/item`,
    summoner: `${DDRAGON_VERSION}/img/spell`,
    rune: `img/perk-images/Styles`,
    runeFromFunc: `img`
} as const satisfies Record<string, string>;

type Tier =
    | 'IRON'
    | 'BRONZE'
    | 'SILVER'
    | 'GOLD'
    | 'PLATINUM'
    | 'MASTER'
    | 'GRANDMASTER'
    | 'CHALLENGER';
type ChallengeCategory =
    | 'COLLECTION'
    | 'EXPERTISE'
    | 'IMAGINATION'
    | 'TEAMWORK'
    | 'VETERANCY';

type BaseChallenge = {
    level: Tier;
    percentile: number;
};

type ChallengesPlayerData = {
    totalPoints: {
        level: Tier | 'EMERALD';
        current: number;
        max: number;
        percentile: number;
    };
    categoryPoints: Record<
        ChallengeCategory,
        BaseChallenge & {
            current: number;
            max: number;
        }
    >;
    challenges: (BaseChallenge & {
        achievedTime: number;
        value: number;
        challengeId: number;
        playersInLevel?: number;
        position?: number;
    })[];
    preferences: {
        title: string;
        challengeIds: number[];
    };
};

type ChallengeConfig = {
    id: number;
    localizedNames: Record<
        string,
        {
            description: string;
            name: string;
            shortDescription: string;
        }
    >;
}[];

type Rank = 'I' | 'II' | 'III' | 'IV';
type RankedQueue = 'RANKED_SOLO_5x5' | 'RANKED_FLEX_SR';

type GameMode = 'CLASSIC';

type Participant = {
    championId: number;
    championName: string;
    assists: number;
    deaths: number;
    kills: number;
    item0: number;
    item1: number;
    item2: number;
    item3: number;
    item4: number;
    item5: number;
    item6: number;
    riotIdGameName: string;
    riotIdTagline: string;
    summoner1Id: number;
    summoner2Id: number;
    summonerName: string;
    teamId: number;
    win: boolean;
    goldEarned: number;
    totalDamageDealt: number;
    totalDamageDealtToChampions: number;
    largestMultiKill: number;
    champLevel: number;
    totalMinionsKilled: number;
    visionScore: number;
    perks: {
        statPerks: {
            deffense: number;
            flex: number;
            offense: number;
        };
        styles: {
            description: string;
            style: number;
            selections: {
                perk: number;
            }[];
        }[];
    };
};

type MatchInfo = {
    gameCreation: number;
    gameDuration: number;
    gameEndTimestamp: number;
    gameId: number;
    gameMode: GameMode;
    queueId: number;
    teams: {
        bans: {
            championId: number;
            pickTurn: number;
        }[];
        teamId: 100 | 200;
        win: boolean;
    };
    participants: Participant[];
};

type Match = {
    info: MatchInfo;
    metadata: {}; //
};

class RiotAPI {
    private static SummonerData:
        | {
            data: Record<
                string,
                {
                    id: string;
                    name: string;
                    key: `${number}`;
                }
            >;
        }
        | undefined = undefined;

    private static PerkData:
        | {
            id: number;
            key: string;
            icon: string;
            name: string;
            slots: {
                runes: {
                    id: number;
                    key: string;
                    icon: string;
                    name: string;
                }[];
            }[];
        }[]
        | undefined = undefined;

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

    static async getChallenges(puuid: string, region: Region) {
        const playerData = await getData<ChallengesPlayerData>(
            `/lol/challenges/v1/player-data/${puuid}`,
            region
        );

        if (!playerData || 'status' in playerData) return playerData;

        const config = await getData<ChallengeConfig>(
            `/lol/challenges/v1/challenges/config`,
            region
        );

        if (!config || 'status' in config) return config;

        const preferences = playerData.preferences;

        const title = preferences.title
            ? preferences.title.substring(0, preferences.title.length - 2)
            : undefined;

        return {
            title: title
                ? config.find((item) => item.id.toString() == title)?.localizedNames[
                    'en_US'
                ].name
                : undefined,
            challenges: preferences.challengeIds
                .map((challengeId) => {
                    const challenge = playerData.challenges.find(
                        (challenge) => challenge.challengeId == challengeId
                    );
                    if (!challenge) return undefined;
                    return `${challengeId}-${challenge.level}`;
                })
                .filter((c) => c) as string[]
        };
    }

    static async getRankedData(summonerId: string, region: Region) {
        return await getData<
            {
                queueType: RankedQueue;
                freshBlood: boolean;
                hotStreak: boolean;
                leagueId: string;
                leaguePoints: number;
                losses: number;
                wins: number;
                rank: Rank;
                tier: Tier;
                veteran: boolean;
            }[]
        >(`/lol/league/v4/entries/by-summoner/${summonerId}`, region);
    }

    static async getMatches(puuid: string, region: Region, start = 0, count = 20) {
        const routing = this.getRoutingByRegion(region);

        const ids = await getData<string[]>(
            `/lol/match/v5/matches/by-puuid/${puuid}/ids?start=${start}&count=${count}`,
            routing
        );

        if (!ids || 'status' in ids) return ids;

        const promises = ids.map((id) =>
            getData<Match>(`/lol/match/v5/matches/${id}`, routing)
        );
        return await Promise.all(promises);
    }

    static async getSummonerSpell(id: number) {
        if (this.SummonerData === undefined) {
            //get data
            const response = await fetch(
                `/assets/ddragon/${DDRAGON_VERSION}/data/en_US/summoner.json`
            );
            const data = await response.json();
            this.SummonerData = (data as typeof this.SummonerData)!;
        }

        console.log(this.SummonerData);

        const spell = Object.values(this.SummonerData.data).find(
            (summ) => summ.key == id.toString()
        )!;

        return spell;
    }

    private static getRuneIcon(style: number, subStyle: number) {
        if (!this.PerkData) return '';

        const primaryStyle = this.PerkData.find((perk) => perk.id === style);
        if (!primaryStyle) return '';

        const flattedSlots = primaryStyle.slots.map((slot) => slot.runes).flat();
        const SubStyle = flattedSlots.find((sub) => sub.id === subStyle);
        if (!SubStyle) return '';

        return SubStyle.icon.slice(0, -4);
    }

    static async extractRunes(perks: Participant['perks']) {
        if (this.PerkData === undefined) {
            //get data
            const response = await fetch(
                `/assets/ddragon/${DDRAGON_VERSION}/data/en_US/runesReforged.json`
            );
            const data = await response.json();
            this.PerkData = (data as typeof this.PerkData)!;
        }

        return {
            main: perks.styles.map((style) => {
                return {
                    category:
                        this.PerkData!.find(
                            (perk) => perk.id === style.style
                        )?.icon.slice(0, -4) ?? '',
                    runes: style.selections.map((sel) =>
                        this.getRuneIcon(style.style, sel.perk)
                    )
                };
            }),
            stats: []
        };
    }
}
