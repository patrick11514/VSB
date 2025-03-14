export type BaseCite = {
    name: string;
    author?: string;
    title: string;
    date: string | Date;
    citeDate: string | Date;
    lang: 'english';
};

export type OnlineCite = {
    type: 'online';
    url: string;
} & BaseCite;

export type Cite = OnlineCite;
