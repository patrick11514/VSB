import { APICreate } from '@patrick115/sveltekitapi';
import type { Context } from './context';

export const api = new APICreate<Context>();

export const router = api.router;
export const procedure = api.procedure;
