#!/usr/bin/bash

CURRENT_KEYMAP=`setxkbmap -query | grep layout | cut -d ':' -f2 | xargs`

if [ "$CURRENT_KEYMAP" = "us" ]; then
    setxkbmap latam
elif [ "$CURRENT_KEYMAP" = "latam" ]; then
    setxkbmap us
else
    exit 1
fi

exit 0
