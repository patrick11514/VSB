export type LocationData = {
    lastUpdate: number;
    lat: number;
    lon: number;
    precision: number;
    heading: number | null;
    speed: number | null;
    altitude:
        | {
              value: number;
              precision: number;
          }
        | undefined;
};
