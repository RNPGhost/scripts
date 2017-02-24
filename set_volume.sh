#!/bin/bash

if [ "$1" == "" ]; then 
  echo Failed: please provide set_volume with a parameter
  exit
fi

# config
DEVICE_NAME=Master

# defaults
DIRECTION=
VOLUME_CHANGE=$1

# get direction of volume change, if one is supplied
FIRST_CHAR=${VOLUME_CHANGE:0:1}
if [ "$FIRST_CHAR" == "-" ] || [ "$FIRST_CHAR" == "+" ]; then
  DIRECTION=$FIRST_CHAR
  VOLUME_CHANGE=${VOLUME_CHANGE:1}
fi

# set the volume
VOLUME=$(amixer set $DEVICE_NAME $VOLUME_CHANGE$DIRECTION | egrep -o --max-count=1 "[0-9]+%")
echo Volume set: $VOLUME

VOLUME=$(~/scripts/rounded_volume.sh 5)

echo Rounded volume: $VOLUME%
