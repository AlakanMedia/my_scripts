#!/usr/bin/bash

OPTIONS=`ls "$HOME"/images/`
USB_WALLPAPERS='/mnt/usb/wallpapers/'

if [ -d "$USB_WALLPAPERS" ]; then
    OPTIONS="$OPTIONS\nusb"
fi

CHOSEN=`echo -e "$OPTIONS" | dmenu -p 'Directories' -i`

if [ -z "$CHOSEN" ]; then
    exit 0
fi

if [ "$CHOSEN" = "usb" ]; then
    nsxiv "$USB_WALLPAPERS"
else
    nsxiv "$HOME"/images/"$CHOSEN"
fi

exit 0
