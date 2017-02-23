#!/usr/bin/bash

# Get the current date and time in the form:
# YYYY-MM-DD Weekday HH:MM
current_date() {
    date +'%Y-%m-%d %A %H:%M'
}

# Generate the status text
generate_status() {
    echo "$(current_date)"
}


# Periodically refresh the status text.
# The sleep duration is a trade off between responsiveness and processing cost.
# Since this currently only shows the time, sleeping for a second is fine.
# Adding volume may look laggy.

while true; do
  xsetroot -name "$(generate_status)"
  sleep 1s
done
