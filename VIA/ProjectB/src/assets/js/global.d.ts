declare global {
    interface Window {
        navLastScroll: number | null;
    }

    interface DocumentEventMap {
        included: undefined;
    }
}

export { };
