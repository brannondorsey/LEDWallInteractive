#!/bin/bash

# launches installation in background if it is not already running, 
# then exists with 0. Does nothing if installation is running, exits with 1

BASENAME=$(dirname $0)

if [ !  "$(ps aux | grep LEDWallVideoPlayerDebug | wc -l)" -gt "1" ]; then
	echo "Installation NOT running. Launching LEDWallVideoPlayerDebug"
	"$BASENAME/LEDWallVideoPlayerDebug.app/Contents/MacOS/LEDWallVideoPlayerDebug" #>/dev/null 2>&1 &
	exit 0
else
	echo "Installation running."
	exit 1
fi