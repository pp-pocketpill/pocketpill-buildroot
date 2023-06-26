# PocketPill Linux (Buildroot)

This Repo is based on [the impressive work of unframework](https://github.com/unframework/licheepi-nano-buildroot).
The base is basically just a slightly modified version of the licheepi-nano base, with addition for anything required to
run the pocketpill.

## Features

- [x] Boot from SD
- [x] USB-Gadget Serial
- [ ] Keys as input device
- [ ] Display support

## Building the Image

The easiest way is using Docker (on Windows/MacOS/Linux).

Run the image build command:

```sh
mkdir dest # (if not already there)
docker build --output type=tar,dest=- . | tar x -C dist
```

The full build may take up to an hour, depending on your host machine.

The built image will be available in `dist/sdcard.img` - you can write this to your bootable micro SD card (see below).

## Iterating on the Base Image

```sh
mkdir dest # (if not already there)
docker build -f Dockerfile.dev --output type=tar,dest=- . | tar x -C dist
```

Here is how the base image is generated (these commands are just for the repo maintainer):

```sh
docker build -f Dockerfile --target main -t olel/pocketpill-buildroot:latest
```
