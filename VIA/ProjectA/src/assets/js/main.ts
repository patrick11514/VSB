const search = document.querySelector('#search')!;
const username = search.querySelector<HTMLInputElement>('#username')!;
const tag = search.querySelector<HTMLInputElement>('#tag')!;
const region = search.querySelector<HTMLSelectElement>('#region')!;
const lastUpdate = document.querySelector('#lastUpdate')!;

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
    level: number;
    lastUpdate: number;
    pfp: number;
    username: string;
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
                wr: (queue.wins / queue.losses) * 100 //in percents
            };
        }
    }

    const matches = await RiotAPI.getMatches(profileData.puuid, reg);
    if (!checkResponse(matches)) return;
    const filtered = matches.filter((match) => checkResponse(match));

    userData = {
        username: mainData.gameName,
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

const render = () => {
    if (!userData) {
        profile.classList.add('invisible');
        content.classList.add('invisible');
        lastUpdate.classList.add('invisible');
        return;
    }

    console.log(userData);

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
        const img = document.createElement('img');
        img.src = RiotAPI.getAsset('challenges', challenge);
        item.appendChild(img);
    }

    for (const [queue, data] of Object.entries(userData.ranks) as Entries<
        UserData['ranks']
    >) {
        putRank(content.querySelector<HTMLDivElement>('#' + QueueToIdMap[queue])!, data);
    }

    profile.classList.remove('invisible');
    content.classList.remove('invisible');
    lastUpdate.classList.remove('invisible');
};
