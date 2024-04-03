#!/usr/bin/bash

OPT1='Full'
OPT2='Select'
OPT3='Window'
OPTIONS="$OPT1\n$OPT2\n$OPT3"
OUTPUT="$HOME/images/screenshots/%d-%m-%Y-%T-ss.png"

CHOSEN=`echo -e "$OPTIONS" | dmenu -p 'Take screenshot' -i`

case "$CHOSEN" in
    "$OPT1")
        scrot "$OUTPUT"
        ;;
    "$OPT2")
        scrot "$OUTPUT" --select --line mode=edge
        ;;
    "$OPT3")
        scrot "$OUTPUT" --focused --border
        ;;
    *)
        exit 1
        ;;
esac

exit 0
