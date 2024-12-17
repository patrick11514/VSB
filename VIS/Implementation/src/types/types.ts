export type UserData = {
    id: number;
    username: string;
    email: string;
};

export type UserState =
    | {
        logged: false;
    }
    | {
        logged: true;
        data: UserData;
    };
