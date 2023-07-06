#!/bin/sh

# Launch the process in background, record the PID into a file, wait
# for the process to terminate and erase the recorded PID
cd ${HOME}
cp /data/gba_bios.bin ./gba_bios.bin
env SDL_NOMOUSE=1 /usr/games/gpsp "$1"&
pid record $!
wait $!
pid erase

