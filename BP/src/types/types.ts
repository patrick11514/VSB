export type BaseCite = {
    name: string;
    author?: string;
    title: string;
    lang: 'english';
};

export type OnlineCite = {
    type: 'online';
    url: string;
    citeDate: string | Date;
    date: string | Date;
} & BaseCite;

export type BookCite = {
    type: 'book';
    year: string;
    publisher: string;
    isbn?: string;
} & BaseCite;

export type Cite = OnlineCite | BookCite;
