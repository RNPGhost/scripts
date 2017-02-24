#!/bin/bash

# config
DEVICE_NAME=Master

# defaults
ROUND_SIZE=1

if [ "$1" != "" ]; then
  ROUND_SIZE=$1
fi

VOLUME=$(amixer get $DEVICE_NAME | egrep -o --max-count=1 "[0-9]+%" | tr -d '%')
echo $((( ( ($ROUND_SIZE/2)+$VOLUME) /$ROUND_SIZE) *$ROUND_SIZE))
