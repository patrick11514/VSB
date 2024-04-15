const { Marp } = require('@marp-team/marp-core');
const Highlight = require('highlight.js');
const hljsSvelte = require('highlightjs-svelte');

const hljs = Highlight.default;
hljsSvelte(hljs);

module.exports = (opts) => {
    const marp = new Marp(opts);

    marp.highlighter = (code, lang) =>
        hljs.highlight(code, {
            language: lang,
        }).value;

    return marp;
};
