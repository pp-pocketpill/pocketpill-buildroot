#!/bin/sh

# Launch the process in background, record the PID into a file, wait
# for the process to terminate and erase the recorded PID
env SDL_NOMOUSE=1 /usr/games/sdlgnuboy --source /usr/games/configs/gnuboy.rc "$1"&
pid record $!
wait $!
pid erase
