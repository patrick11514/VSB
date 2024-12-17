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

export type DePromise<$Type> = $Type extends Promise<infer $Inner> ? $Inner : $Type;
