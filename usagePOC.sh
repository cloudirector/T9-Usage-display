#!/bin/bash
set -e
commands=("echo cpu:$(top -bn1 | grep 'Cpu(s)' | awk '{printf "%.2f%%", $2 + $4}')"
          "free -m | awk '/Mem/ {printf \"%dMB/%dMB\", \$3, \$2}'"
          "uptime | cut -d ',' -f 1 | awk '{print \$1}'")
prev_output=""
while true; do
    for command in "${commands[@]}"; do
        for i in {1..10}; do
            output=$(eval "$command")
            if [ "$output" != "$prev_output" ]; then
                adb shell /usr/bin/guimgr_cli lcd_eng_mode x$output
                # echo "$output"
                prev_output="$output"
            fi
            sleep 1
        done
    done
done
