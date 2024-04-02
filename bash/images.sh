#!/usr/bin/bash

CHOSEN=`ls "$HOME"/images | dmenu -p 'Directorys' -i`

if [ -z "$CHOSEN" ]; then
    exit 1
fi

nsxiv "$HOME"/images/"$CHOSEN"

exit 0
