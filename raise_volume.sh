#!/bin/bash

# config
DEVICE_NAME=Master

# defaults
VOLUME_INCREASE=5

# include parameter
if [ "$1" != "" ]; then
  VOLUME_INCREASE=$1
fi

VOLUME=$(~/scripts/rounded_volume.sh 5)

~/scripts/set_volume.sh $(($VOLUME+$VOLUME_INCREASE))%
