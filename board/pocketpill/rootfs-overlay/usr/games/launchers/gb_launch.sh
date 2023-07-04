#!/bin/sh

# Launch the process in background, record the PID into a file, wait
# for the process to terminate and erase the recorded PID
/usr/games/sdl2gnuboy --source /usr/games/configs/gnuboy.rc "$1"&
pid record $!
wait $!
pid erase
