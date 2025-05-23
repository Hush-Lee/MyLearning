#!/usr/bin/env bash

# Add this script to your wm startup file.

DIR="$HOME/.config/polybar/forest"

# Terminate already running bar instances
killall -q polybar

# Wait until the processes have been shut down
while pgrep -u $UID -x polybar >/dev/null; do sleep 1; done

# Launch the bar
for m in $(polybar --list-monitors | cut -d":" -f1); do
    MONITOR=$m polybar -q top -c "$DIR"/config.ini &
    MONITOR=$m polybar -q bottom -c "$DIR"/config.ini &
    ... and so on
done
#polybar -q top -c "$DIR"/config.ini &
#polybar -q bottom -c "$DIR"/config.ini &
