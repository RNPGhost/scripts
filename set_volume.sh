#!/bin/bash

if [ "$1" == "" ]; then
  echo Failed: Please provide a new volume value.
  exit
fi

DIRECTION=
VOLUME_CHANGE=$1

FIRST_CHAR=${VOLUME_CHANGE:0:1}
if [ "$FIRST_CHAR" == "-" ] || [ "$FIRST_CHAR" == "+" ]; then
  DIRECTION=$FIRST_CHAR
  VOLUME_CHANGE=${VOLUME_CHANGE:1}
fi

amixer -q sset PCM $VOLUME_CHANGE$DIRECTION

CURRENT_VOLUME=$(amixer get PCM | awk '$0~/%/{print $4}' | tr -d '[]%')

echo Volume set to: $CURRENT_VOLUME

# Round volume to nearest 5 to deal with ugly volume values
CURRENT_VOLUME=$(((5/2+$CURRENT_VOLUME)/5*5))

echo New rounded volume: $CURRENT_VOLUME
