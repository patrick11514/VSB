import Enquirer from 'enquirer';

//From Enquirer
interface Choice {
    name: string;
    message?: string;
    value?: unknown;
    hint?: string;
    role?: string;
    enabled?: boolean;
    disabled?: boolean | string;
}

type Prompt =
    | {
        name: string;
        message: string;
        type:
        | 'autocomplete'
        | 'editable'
        | 'form'
        | 'multiselect'
        | 'select'
        | 'survey'
        | 'list'
        | 'scale';
        choices: (string | Choice)[];
        maxChoices?: number;
        multiple?: boolean;
        initial?: number;
        delay?: number;
        separator?: boolean;
        sort?: boolean;
        linebreak?: boolean;
        edgeLength?: number;
        align?: 'left' | 'right';
        scroll?: boolean;
        hint?: string;
    }
    | {
        name: string;
        message: string;
        type: 'confirm';
        initial?: boolean;
    }
    | {
        name: string;
        message: string;
        type: 'input' | 'invisible' | 'list' | 'password' | 'text';
        initial?: string;
        multiline?: boolean;
        validate?: (input: string) => boolean | string;
        result?: (input: string) => string;
    }
    | {
        name: string;
        message: string;
        type: 'numeral';
        min?: number;
        max?: number;
        delay?: number;
        float?: boolean;
        round?: boolean;
        major?: number;
        minor?: number;
        initial?: number;
    }
    | {
        name: string;
        message: string;
        type: 'snippet';
        newline?: string;
        template?: string;
    }
    | {
        name: string;
        message: string;
        type: 'sort';
        hint?: string;
        drag?: boolean;
        numbered?: boolean;
    };

type PromptTypeValueType = {
    autocomplete: never;
    editable: never;
    form: never;
    multiselect: never;
    select: never;
    survey: never;
    list: never;
    scale: never;
    confirm: boolean;
    input: string;
    invisible: string;
    password: string;
    text: string;
    numeral: number;
    snippet: never;
    sort: never;
};

type Arrayable<T> = T | T[];

type GetFinalType<P extends Prompt> = P['type'] extends 'multiselect'
    ? GetType<P>[]
    : GetType<P>;

type GetReturnType<T extends Arrayable<Prompt>> =
    T extends Array<Prompt>
    ? {
        [K in keyof T as Key<T, K>]: K extends string
        ? T[K] extends Prompt
        ? GetFinalType<T[K]>
        : never
        : never;
    }
    : T extends Prompt
    ? { [K in GetKey<T>]: GetFinalType<T> }
    : never;

type GetType<P extends Prompt> = P extends { initial?: infer T }
    ? T
    : P extends { choices: infer C }
    ? C extends Array<infer T>
    ? T extends { name: string }
    ? T['name']
    : T extends string
    ? T
    : never
    : C extends { name: string }
    ? C['name']
    : C extends string
    ? C
    : never
    : PromptTypeValueType[P['type']];
type GetKey<P extends Prompt> = P extends { name: infer T }
    ? T extends string
    ? T
    : never
    : never;
type Key<A extends Prompt[], P extends keyof A> = P extends string
    ? A[P] extends Prompt
    ? GetKey<A[P]>
    : never
    : never;

export const prompt = async <T extends Arrayable<Prompt>>(
    input: T
): Promise<GetReturnType<T>> => {
    return Enquirer.prompt(input as any);
};
