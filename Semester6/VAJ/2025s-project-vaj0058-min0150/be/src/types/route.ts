import { Router } from 'express';

export type Route = {
    router: Router;
    mount: string;
};
