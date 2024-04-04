#!/usr/bin/bash

OPTIONS="Shutdown\nLock\nReboot\nSuspend"
CHOSEN=`echo -e "$OPTIONS" | dmenu -p 'Exit options' -i`

case "$CHOSEN" in
	'Shutdown') 
	    loginctl poweroff 
	    ;;
    'Lock')
        slock
        ;;
	'Reboot') 
	    loginctl reboot
	    ;;
	'Suspend') 
        loginctl suspend 
        ;;
	*) 
        exit 1 
        ;;
esac

exit 0
