#!/usr/bin/bash

pactl get-sink-volume @DEFAULT_SINK@ | head -n 1 | cut -d '/' -f2 | xargs

exit 0
