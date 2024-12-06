const search = document.querySelector('#search')!;
const username = search.querySelector<HTMLInputElement>('#username')!;
const tag = search.querySelector<HTMLInputElement>('#tag')!;
const region = search.querySelector<HTMLSelectElement>('#region')!;
const lastUpdate = document.querySelector('#lastUpdate')!;
const matchList = document.querySelector('#matches')!;

//fill options
if (region) {
    for (const reg of REGIONS) {
        const option = document.createElement('option');
        option.textContent = reg;
        option.value = reg;
        if (reg == 'EUN1') option.selected = true;
        region.appendChild(option);
    }
}

const button = document.querySelector<HTMLButtonElement>('button#search')!;

type UserData = {
    region: Region;
    level: number;
    lastUpdate: number;
    pfp: number;
    username: string;
    puuid: string;
    tag: string;
    title?: string;
    item: string[];
    ranks: Record<
        RankedQueue,
        | {
            lp: number;
            rank: Rank;
            tier: Tier;
            wins: number;
            loses: number;
            wr: number;
        }
        | undefined
    >;
    matches: Match[];
};

let userData: UserData | null = null;

const checkResponse = <$Type>(item: RiotResponse<$Type> | undefined): item is $Type => {
    if (!item) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to contact RiotAPI, please try again later.'
        });
        return false;
    }

    if ('status' in (item as any)) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to find your account.'
        });

        search.classList.add('border-red-500');
        search.classList.remove('border-white');
        return false;
    }

    return true;
};

const lookup = async () => {
    let err = false;

    userData = null;
    render();

    if (username.value == '') {
        SwalAlert({
            icon: 'error',
            title: 'Please enter username'
        });
        err = true;
    }

    if (tag.value == '') {
        SwalAlert({
            icon: 'error',
            title: 'Please enter tag'
        });
        err = true;
    }

    if (err) {
        search.classList.add('border-red-500');
        search.classList.remove('border-white');

        return;
    }

    search.classList.remove('border-red-500');
    search.classList.add('border-white');

    button.disabled = true;

    const reg = region.value as Region;

    const mainData = await RiotAPI.getUser(username.value, tag.value, reg);
    if (!checkResponse(mainData)) return;

    const profileData = await RiotAPI.getSummoner(mainData.puuid, reg);
    if (!checkResponse(profileData)) return;

    const challenges = await RiotAPI.getChallenges(mainData.puuid, reg);
    if (!checkResponse(challenges)) return;

    const rankedData = await RiotAPI.getRankedData(profileData.id, reg);
    if (!checkResponse(rankedData)) return;

    const ranks: UserData['ranks'] = {
        RANKED_SOLO_5x5: undefined,
        RANKED_FLEX_SR: undefined
    };

    for (const queue of rankedData) {
        if (queue.queueType == 'RANKED_SOLO_5x5' || queue.queueType == 'RANKED_FLEX_SR') {
            ranks[queue.queueType] = {
                lp: queue.leaguePoints,
                wins: queue.wins,
                loses: queue.losses,
                rank: queue.rank,
                tier: queue.tier,
                wr: (queue.wins / (queue.wins + queue.losses)) * 100 //in percents
            };
        }
    }

    const matches = await RiotAPI.getMatches(profileData.puuid, reg);
    if (!checkResponse(matches)) return;
    const filtered = matches.filter((match) => checkResponse(match));

    userData = {
        region: reg,
        username: mainData.gameName,
        puuid: profileData.puuid,
        tag: mainData.tagLine,
        level: profileData.summonerLevel,
        pfp: profileData.profileIconId,
        title: challenges.title,
        item: challenges.challenges,
        lastUpdate: parseInt(profileData.revisionDate),
        ranks,
        matches: filtered
    };

    render();

    button.disabled = false;
};

button.addEventListener('click', lookup);
username.addEventListener('keydown', (ev) => {
    if (ev.key == 'Enter') lookup();
});
tag.addEventListener('keydown', (ev) => {
    if (ev.key == 'Enter') lookup();
});

const profile = document.querySelector('section#profile')!;
const icon = profile.querySelector('#icon')!;
const userTag = profile.querySelector('#username')!;

const content = document.querySelector('section#content')!;

const putRank = (
    root: HTMLDivElement,
    data: UserData['ranks'][keyof UserData['ranks']]
) => {
    console.log(root);
    const img = root.querySelector('img')!;
    const rankInfo = root.querySelector('div')!;
    const tier = rankInfo.querySelector('h2')!;
    const lp = rankInfo.querySelector('h4')!;

    const wrInfo = root.querySelector('div:last-child')!;
    const WL = wrInfo.querySelector('h4:first-child')!;
    const WR = wrInfo.querySelector('h4:last-child')!;

    if (data == undefined) {
        img.src = '/assets/images/Unranked.png';
        tier.textContent = 'Unranked';
        lp.textContent = 'N/A LP';
        WL.textContent = '0W 0L';
        WR.textContent = 'N/A% Win Rate';
    } else {
        const tierFirstUpper = firstUpper(data.tier);

        img.src = `/assets/images/${tierFirstUpper}.png`;
        tier.textContent = `${tierFirstUpper} ${data.rank}`;
        lp.textContent = `${data.lp} LP`;
        WL.textContent = `${data.wins}W ${data.loses}L`;
        WR.textContent = `${data.wr.toFixed(2)}% Win Rate`;
    }
};

const QueueToIdMap = {
    RANKED_SOLO_5x5: 'solo-rank',
    RANKED_FLEX_SR: 'flex-rank'
} satisfies Record<RankedQueue, string>;

const getPlayerData = (
    participant: Participant,
    myRiotId: [string, string]
): {
    champId: number;
    champName: string;
    lvl: number;
    kills: number;
    deaths: number;
    asists: number;
    items: number[];
    trinket: number;
    name: string;
    me: boolean;
    win: boolean;
    summoners: number[];
    runes: Participant['perks'];
    cs: number;
    vision: number;
    damage: number;
    golds: number;
} => {
    type Nums = 0 | 1 | 2 | 3 | 4;

    return {
        champId: participant.championId,
        champName: participant.championName,
        lvl: participant.champLevel,
        kills: participant.kills,
        deaths: participant.deaths,
        asists: participant.assists,
        items: Array.from({ length: 6 }).map(
            (_, i) => participant[`item${i}` as `item${Nums}`]
        ),
        trinket: participant.item6,
        name: participant.summonerName,
        me:
            participant.riotIdGameName === myRiotId[0] &&
            participant.riotIdTagline === myRiotId[1],
        win: participant.win,
        summoners: [participant.summoner1Id, participant.summoner2Id],
        runes: participant.perks,
        cs: participant.totalMinionsKilled,
        vision: participant.visionScore,
        damage: participant.totalDamageDealtToChampions,
        golds: participant.goldEarned
    };
};

const elementWithText = (element: keyof HTMLElementTagNameMap, text: string) => {
    const el = document.createElement(element);
    el.textContent = text;
    return el;
};

const elementWithAttr = <
    $Element extends keyof HTMLElementTagNameMap,
    $Attr extends keyof HTMLElementTagNameMap[$Element]
>(
    element: $Element,
    attr: $Attr,
    value: HTMLElementTagNameMap[$Element][$Attr]
) => {
    const el = document.createElement(element);
    el[attr] = value;
    return el;
};

const moreMatchData = async (match: Match, moreStatsDiv: HTMLDivElement) => {
    if (!userData) return;

    //prepare all elements and then put them at the end, just to replace loading
    const teamInfo = elementWithText('h1', 'Teams info');
    const { info } = match;
    const teams: {
        win: boolean;
        players: ReturnType<typeof getPlayerData>[];
    }[] = [
            {
                win: false,
                players: []
            },
            {
                win: false,
                players: []
            }
        ];
    for (const participant of info.participants) {
        if (participant.teamId == 100) {
            teams[0].win = participant.win;
            teams[0].players.push(
                getPlayerData(participant, [userData.username, userData.tag])
            );
        } else {
            teams[1].win = participant.win;
            teams[1].players.push(
                getPlayerData(participant, [userData.username, userData.tag])
            );
        }
    }

    //all elements which will be added at the end
    const elems: HTMLElement[] = [];

    let maxDamage = 0;
    for (const team of teams)
        for (const player of team.players) maxDamage = Math.max(maxDamage, player.damage);

    for (const team of teams) {
        const text = elementWithText('h2', team.win ? 'Winners' : 'Losers');
        const winLoss = team.win ? 'win' : 'loss';
        text.classList.add(winLoss);
        elems.push(text);

        const playerList = document.createElement('div');
        playerList.classList.add(winLoss);

        for (const player of team.players) {
            /*  Champion + level  */
            const champ = document.createElement('div');
            champ.appendChild(
                elementWithAttr(
                    'img',
                    'src',
                    RiotAPI.getAsset('champion', player.champName)
                )
            );
            champ.appendChild(elementWithText('div', player.lvl.toString()));
            playerList.append(champ);

            /*  Summoner spells*/
            const summs = document.createElement('div');
            for (const summonerSpell of player.summoners) {
                const data = await RiotAPI.getSummonerSpell(summonerSpell);
                summs.appendChild(
                    elementWithAttr('img', 'src', RiotAPI.getAsset('summoner', data.id))
                );
            }
            playerList.appendChild(summs);

            /*  Runes  */
            const runeList = await RiotAPI.extractRunes(player.runes);
            const runes = document.createElement('div');
            let i = 0;
            for (const rune of runeList.main) {
                runes.appendChild(
                    elementWithAttr(
                        'img',
                        'src',
                        RiotAPI.getAsset(
                            'runeFromFunc',
                            i == 0 ? rune.runes[0] : rune.category
                        )
                    )
                );
                ++i;
            }
            playerList.appendChild(runes);

            /*  Summoner Name  */
            const name = elementWithText('div', player.name);
            if (player.me) {
                name.classList.add('me');
            }
            playerList.appendChild(name);

            const scoreDiv = document.createElement('div');
            /* KDA */
            const kda = document.createElement('h1');
            kda.appendChild(elementWithText('span', player.kills.toString()));
            kda.appendChild(elementWithText('span', '/'));
            kda.appendChild(elementWithText('span', player.deaths.toString()));
            kda.appendChild(elementWithText('span', '/'));
            kda.appendChild(elementWithText('span', player.asists.toString()));
            scoreDiv.appendChild(kda);

            /*  KDA num  */
            const KDA = document.createElement('h2');
            const kdaValue = document.createElement('span');
            const kdaNum = (player.kills + player.asists) / player.deaths;
            kdaValue.textContent = kdaNum.toFixed(2);
            if (kdaNum >= 5) {
                kdaValue.classList.add('excellent');
            } else if (kdaNum >= 3) {
                kdaValue.classList.add('perfect');
            }
            KDA.appendChild(kdaValue);
            KDA.appendChild(elementWithText('span', ' KDA'));
            scoreDiv.appendChild(KDA);
            playerList.appendChild(scoreDiv);

            /*  Items  */
            const items = document.createElement('div');

            /*  Regular items  */
            for (const item of [...player.items, player.trinket]) {
                if (item == 0) {
                    items.appendChild(elementWithText('div', ''));
                } else {
                    items.appendChild(
                        elementWithAttr('img', 'src', RiotAPI.getAsset('item', item))
                    );
                }
            }

            playerList.appendChild(items);

            /*  Stats  */
            const stats = document.createElement('div');

            /*  Damage  */
            const damage = document.createElement('div');
            damage.appendChild(elementWithText('span', `${player.damage} Damage`));
            const graph = document.createElement('div');
            const line = document.createElement('div');
            line.style.width = (100 * (player.damage / maxDamage)).toString() + '%';
            graph.appendChild(line);
            damage.appendChild(graph);
            stats.appendChild(damage);

            /*  CS  */
            stats.appendChild(elementWithText('div', `${player.cs} CS`));
            /*  Golds  */
            stats.appendChild(elementWithText('div', `${player.golds} Golds`));
            /*  Wards  */
            stats.appendChild(elementWithText('div', `${player.vision} Vision`));

            playerList.appendChild(stats);
        }

        elems.push(playerList);
    }

    /*  Get timestamp end extract data from it  */
    const timeline = await RiotAPI.getTimeline(match.metadata.matchId, userData.region);

    if (!timeline || 'status' in timeline) {
        SwalAlert({
            icon: 'error',
            title: 'Unable to load game data'
        });
        return;
    }

    //getId in timeline of me
    const id = timeline.info.participants.find(
        (part) => part.puuid === userData!.puuid
    )!.participantId;

    const extractedFrames: [
        number,
        (typeof timeline)['info']['frames'][number]['participantFrames'][ParticipantIds]
    ][] = [];

    for (const frame of timeline.info.frames) {
        extractedFrames.push([frame.timestamp, frame.participantFrames[id]]);
    }

    const graphs: (keyof (typeof extractedFrames)[number][1])[] = [
        'xp',
        'totalGold',
        'damageStats',
        'minionsKilled'
    ];
    const names = ['Experience', 'Golds', 'Damage', 'Minions'];
    const colors = ['#00b330', '#ffdd00', '#ff0000', '#0088ff'];

    elems.push(elementWithText('h1', 'Your stats through game'));

    /*  Graph Config  */

    const graphDiv = document.createElement('div');
    const buttonsDiv = document.createElement('div');
    const canvasDiv = document.createElement('div');

    const graphConfig: ChartOptions = {
        scales: {
            x: {
                grid: {
                    color: '#b5b5b5'
                },
                ticks: {
                    color: '#b5b5b5'
                }
            },
            y: {
                grid: {
                    color: '#b5b5b5'
                },
                ticks: {
                    color: '#b5b5b5'
                }
            }
        },
        plugins: {
            legend: {
                labels: {
                    color: '#b5b5b5'
                }
            }
        }
    };

    const canvasList: HTMLCanvasElement[] = [];
    const buttonList: HTMLButtonElement[] = [];

    let i = 0;
    /*  Creating graphs  */
    for (const graph of graphs) {
        const canvas = document.createElement('canvas');
        if (i != 0) canvas.classList.add('!hidden');

        canvasList.push(canvas);

        const button = document.createElement('button');
        if (i == 0) button.classList.add('active');
        buttonList.push(button);

        button.textContent = names[i];
        button.addEventListener('click', () => {
            canvasList.forEach((c) => c.classList.add('!hidden'));
            canvas.classList.remove('!hidden');
            buttonList.forEach((c) => c.classList.remove('active'));
            button.classList.add('active');
        });

        canvasDiv.appendChild(canvas);
        buttonsDiv.appendChild(button);

        const frameTimestamp: [number, number][] = extractedFrames.map((frame) => [
            frame[0],
            graph === 'damageStats'
                ? frame[1][graph]['totalDamageDoneToChampions']
                : frame[1][graph]
        ]);

        new Chart(canvasList[i], {
            type: 'line',
            data: {
                labels: frameTimestamp.map(
                    (frame) => Math.floor(frame[0] / 1000 / 60) + ' min'
                ),
                datasets: [
                    {
                        label: names[i],
                        data: frameTimestamp.map((frame) => frame[1]),
                        backgroundColor: colors[i],
                        borderColor: colors[i],
                        borderWidth: 4
                    }
                ]
            },
            options: graphConfig
        });

        ++i;
    }

    graphDiv.appendChild(buttonsDiv);
    graphDiv.appendChild(canvasDiv);
    elems.push(graphDiv);

    //now we can remove loading + add other elements
    moreStatsDiv.removeChild(moreStatsDiv.childNodes[0]);

    //add other elements
    moreStatsDiv.appendChild(teamInfo);
    for (const el of elems) {
        moreStatsDiv.appendChild(el);
    }

    moreStatsDiv.dataset.loaded = 'true';
};

const addMatch = async (match: Match) => {
    if (!userData) return;

    const { info } = match;
    const rootDiv = document.createElement('div');

    rootDiv.dataset.matchId = match.metadata.matchId;

    const mainDiv = document.createElement('div');
    const players: ReturnType<typeof getPlayerData>[] = [];

    for (const participant of info.participants) {
        players.push(getPlayerData(participant, [userData.username, userData.tag]));
    }

    const me = players.find((player) => player.me)!;
    rootDiv.classList.add(me.win ? 'win' : 'lose');

    /*  BASIC MATCH INFO  */
    const matchInfo = document.createElement('div');
    const gameType = document.createElement('h1');
    let gameName = QUEUES[info.queueId as keyof typeof QUEUES] ?? 'Unknown';
    if (info.queueId >= 830 && info.queueId <= 850) {
        gameName += `(${COOP_TITLES[(info.queueId - 830) / 10]})`;
    }
    gameType.textContent = gameName;
    matchInfo.appendChild(gameType);

    matchInfo.appendChild(elementWithText('h3', timeDiff(info.gameEndTimestamp)));

    const result = document.createElement('h2');
    result.textContent = me.win ? 'WIN ' : 'LOSE ';
    const gameTime = document.createElement('span');

    const minutes = Math.floor(info.gameDuration / 60);
    const seconds = info.gameDuration - minutes * 60;

    gameTime.textContent = `${minutes}:${seconds}`;
    result.appendChild(gameTime);
    matchInfo.appendChild(result);

    mainDiv.appendChild(matchInfo);

    /*  CHAMPION + LEVEL  */
    const champLvl = document.createElement('div');
    champLvl.appendChild(
        elementWithAttr('img', 'src', RiotAPI.getAsset('champion', me.champName))
    );
    champLvl.appendChild(elementWithText('div', me.lvl.toString()));
    mainDiv.appendChild(champLvl);

    /*  SUMMONERS  */
    const summoner = document.createElement('div');
    for (const summonerSpell of me.summoners) {
        const data = await RiotAPI.getSummonerSpell(summonerSpell);
        summoner.appendChild(
            elementWithAttr('img', 'src', RiotAPI.getAsset('summoner', data.id))
        );
    }
    mainDiv.appendChild(summoner);

    /*  Runes  */
    const runeList = await RiotAPI.extractRunes(me.runes);
    const runes = document.createElement('div');
    let i = 0;
    for (const rune of runeList.main) {
        runes.appendChild(
            elementWithAttr(
                'img',
                'src',
                RiotAPI.getAsset('runeFromFunc', i == 0 ? rune.runes[0] : rune.category)
            )
        );
        ++i;
    }
    mainDiv.appendChild(runes);

    /* Player score */
    const score = document.createElement('div');

    /* KDA */
    const kda = document.createElement('h1');
    kda.appendChild(elementWithText('span', me.kills.toString()));
    kda.appendChild(elementWithText('span', '/'));
    kda.appendChild(elementWithText('span', me.deaths.toString()));
    kda.appendChild(elementWithText('span', '/'));
    kda.appendChild(elementWithText('span', me.asists.toString()));
    score.appendChild(kda);

    /*  KDA num  */
    const KDA = document.createElement('h2');
    const kdaValue = document.createElement('span');
    const kdaNum = (me.kills + me.asists) / me.deaths;
    kdaValue.textContent = kdaNum.toFixed(2);
    if (kdaNum >= 5) {
        kdaValue.classList.add('excellent');
    } else if (kdaNum >= 3) {
        kdaValue.classList.add('perfect');
    }
    KDA.appendChild(kdaValue);
    KDA.appendChild(elementWithText('span', ' KDA'));
    score.appendChild(KDA);

    /*  CS  */
    score.appendChild(elementWithText('h2', `${me.cs} CS`));
    /*  Vision  */
    score.appendChild(elementWithText('h2', `${me.vision} vision`));
    mainDiv.appendChild(score);

    /*  Items  */
    const items = document.createElement('div');

    /*  Regular items  */
    for (const item of [...me.items, me.trinket]) {
        if (item == 0) {
            items.appendChild(elementWithText('div', ''));
        } else {
            items.appendChild(
                elementWithAttr('img', 'src', RiotAPI.getAsset('item', item))
            );
        }
    }

    mainDiv.appendChild(items);

    /*  Players  */
    const playerList = document.createElement('div');
    for (const player of players) {
        const playerEl = document.createElement('div');

        if (player.me) {
            playerEl.classList.add('me');
        }

        playerEl.appendChild(
            elementWithAttr('img', 'src', RiotAPI.getAsset('champion', player.champName))
        );
        playerEl.appendChild(elementWithText('span', player.name));
        playerList.appendChild(playerEl);
    }
    mainDiv.appendChild(playerList);

    /*  Open Button  */
    const open = document.createElement('div');
    const button = document.createElement('i');
    button.classList.value = 'bi bi-caret-down-fill text-2xl';
    open.appendChild(button);
    mainDiv.appendChild(open);

    rootDiv.appendChild(mainDiv);

    /* More stats div */
    const moreStats = document.createElement('div');
    moreStats.classList.add('!hidden');
    moreStats.appendChild(
        elementWithAttr(
            'img',
            'src',
            'https://media.tenor.com/_62bXB8gnzoAAAAj/loading.gif'
        )
    );

    button.addEventListener('click', () => {
        if (moreStats.classList.contains('!hidden')) {
            moreStats.classList.remove('!hidden');
            button.classList.remove('bi-caret-down-fill');
            button.classList.add('bi-caret-up-fill');

            if (!moreStats.dataset.loaded) {
                moreMatchData(match, moreStats);
            }
            return;
        }
        moreStats.classList.add('!hidden');
        button.classList.add('bi-caret-down-fill');
        button.classList.remove('bi-caret-up-fill');
    });

    rootDiv.appendChild(moreStats);

    matchList.appendChild(rootDiv);
};

const render = async () => {
    if (!userData) {
        profile.classList.add('invisible');
        content.classList.add('invisible');
        lastUpdate.classList.add('invisible');
        return;
    }

    lastUpdate.textContent = 'Last Update: ' + toDate(userData.lastUpdate as any);
    icon.querySelector<HTMLImageElement>('img')!.src = RiotAPI.getAsset(
        'profileImage',
        userData.pfp
    );
    userTag.childNodes[0].textContent = userData.username + ' '; //update textNode
    userTag.children[0].textContent = '#' + userData.tag; //update the span with tag
    icon.querySelector('span')!.textContent = userData.level.toString();

    //update title
    profile.querySelector('#title')!.textContent = userData.title ?? '';
    const item = profile.querySelector('#challenges')!;
    for (const child of Array.from(item.children)) child.remove();

    for (const challenge of userData.item) {
        item.appendChild(
            elementWithAttr('img', 'src', RiotAPI.getAsset('challenges', challenge))
        );
    }

    //ranks
    for (const [queue, data] of Object.entries(userData.ranks) as Entries<
        UserData['ranks']
    >) {
        putRank(content.querySelector<HTMLDivElement>('#' + QueueToIdMap[queue])!, data);
    }

    //match list
    for (const match of userData.matches) {
        await addMatch(match);
    }

    profile.classList.remove('invisible');
    content.classList.remove('invisible');
    lastUpdate.classList.remove('invisible');
};
