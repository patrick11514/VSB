export type UserData = {};

export type UserState =
    | {
          logged: false;
      }
    | {
          logged: true;
          data: UserData;
      };
