#!/bin/bash

DEFAULT_VOLUME_DECREASE=5
VOLUME_DECREASE=$DEFAULT_VOLUME_DECREASE

if [ "$1" != "" ]; then
  VOLUME_DECREASE=$1
fi

CURRENT_VOLUME=$(amixer get PCM | awk '$0~/%/{print $4}' | tr -d '[]%')

# Round volume to nearest 5 to deal with ugly volume values
CURRENT_VOLUME=$(((5/2+$CURRENT_VOLUME)/5*5))

set_volume $(($CURRENT_VOLUME-$VOLUME_DECREASE))%
