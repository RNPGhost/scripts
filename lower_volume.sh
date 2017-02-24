#!/bin/bash

# defaults
VOLUME_DECREASE=5

# include parameter
if [ "$1" != "" ]; then
  VOLUME_DECREASE=$1
fi

VOLUME=$(~/scripts/rounded_volume.sh 5)

~/scripts/set_volume.sh $(($VOLUME-$VOLUME_DECREASE))%
