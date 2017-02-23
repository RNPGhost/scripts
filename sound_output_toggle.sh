#!/usr/bin/bash

# Configuration file.
CONFIG_FILE=~/.asoundrc

# Fetch the current default.
get_current() {
  readlink "$CONFIG_FILE" | grep -Po '(?<=\.asoundrc\.).*'
}

CURRENT=$(get_current)
if [[ "$CURRENT" == headphones ]]; then
  echo "Switching to speakers."
  ln -sf .asoundrc.speakers "$CONFIG_FILE"
else
  echo "Switching to headphones."
  ln -sf .asoundrc.headphones "$CONFIG_FILE"
fi
