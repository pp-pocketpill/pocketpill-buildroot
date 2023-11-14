#!/bin/sh

grep -q "GADGET_SERIAL" "${TARGET_DIR}/etc/inittab" \
	|| echo '/dev/ttyGS0::respawn:/sbin/getty -L  /dev/ttyGS0 0 vt100 # GADGET_SERIAL' >> "${TARGET_DIR}/etc/inittab"

grep -q "/dev/mmcblk0p3" "${TARGET_DIR}/etc/fstab" \
	|| echo '/dev/mmcblk0p3  /data           vfat    defaults         0       0' >> "${TARGET_DIR}/etc/fstab"

grep -q "/dev/mmcblk0p4" "${TARGET_DIR}/etc/fstab" \
	|| echo '/dev/mmcblk0p4  swap           swap    defaults         0       0' >> "${TARGET_DIR}/etc/fstab"
