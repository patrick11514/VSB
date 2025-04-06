#!/bin/bash

build() {
    echo "Generating..."
    cd src/lib/Generated && antlr4 -Dlanguage=TypeScript -visitor *.g4
    cd ../../../
    echo "Done!"
}

inotifywait -m -r -e modify --exclude '\.git/' --format "%w%f" src/lib/Generated | \
    grep --line-buffered -E '\.g4$' | \
    while read FILE; do
        echo "File $FILE change detected, rebuilding..."
        touch .trigger
    done &
PID=$(pgrep -f "inotifywait -m -r -e modify")

# Wait for ctrl+c
trap 'echo "Stopping watch..."; kill $PID; echo "Killed $PID!"' EXIT

while true; do
    sleep 1
    if [[ -f .trigger ]]; then
        rm .trigger
        build
    fi
done
