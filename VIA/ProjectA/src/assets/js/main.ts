const search = document.querySelector('#search')!;
const username = search.querySelector<HTMLInputElement>('#username')!;
const tag = search.querySelector<HTMLInputElement>('#tag')!;
const region = search.querySelector<HTMLSelectElement>('#region')!;

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
    pfp: number;
    username: string;
    tag: string;
    title?: string;
    challenges: string[]
};

let userData: UserData | null = null;

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

    if (!mainData) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to contact RiotAPI, please try again later.'
        });
        return;
    }

    if ('status' in mainData) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to find your account.'
        });

        search.classList.add('border-red-500');
        search.classList.remove('border-white');
        return;
    }

    const profileData = await RiotAPI.getSummoner(mainData.puuid, reg);

    if (!profileData) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to contact RiotAPI, please try again later.'
        });
        return;
    }

    if ('status' in profileData) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to find your account.'
        });

        search.classList.add('border-red-500');
        search.classList.remove('border-white');
        return;
    }

    const challenges = await RiotAPI.getChallenges(mainData.puuid, reg);
    if (!challenges) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to contact RiotAPI, please try again later.'
        });
        return;
    }

    if ('status' in challenges) {
        button.disabled = false;

        SwalAlert({
            icon: 'error',
            title: 'Unable to find your account.'
        });

        search.classList.add('border-red-500');
        search.classList.remove('border-white');
        return;
    }

    userData = {
        username: mainData.gameName,
        tag: mainData.tagLine,
        level: profileData.summonerLevel,
        pfp: profileData.profileIconId,
        title: challenges.title,
        challenges: challenges.challenges
    };


    render();

    button.disabled = false;
}

button.addEventListener('click', lookup);
username.addEventListener("keydown", (ev) => {
    if (ev.key == "Enter") lookup();
})
tag.addEventListener("keydown", (ev) => {
    if (ev.key == "Enter") lookup();
})

const profile = document.querySelector('section#profile')!;
const icon = profile.querySelector('#icon')!;
const userTag = profile.querySelector('#username')!;

const content = document.querySelector('section#content')!;

const render = () => {
    if (!userData) {
        profile.classList.add('invisible');
        content.classList.add('invisible');
        return;
    }

    icon.querySelector<HTMLImageElement>('img')!.src = RiotAPI.getAsset(
        'profileImage',
        userData.pfp
    );
    userTag.childNodes[0].textContent = userData.username + ' '; //update textNode
    userTag.children[0].textContent = '#' + userData.tag; //update the span with tag
    icon.querySelector('span')!.textContent = userData.level.toString();

    //update title
    profile.querySelector("#title")!.textContent = userData.title ?? "";
    const challenges = profile.querySelector("#challenges")!;
    for (const child of Array.from(challenges.children)) child.remove();

    for (const challenge of userData.challenges) {
        const img = document.createElement("img");
        img.src = RiotAPI.getAsset("challenges", challenge);
        challenges.appendChild(img);
    }

    profile.classList.remove('invisible');
    content.classList.remove('invisible');
};

