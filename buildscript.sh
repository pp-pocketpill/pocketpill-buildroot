#! /bin/sh

echo "BR2_EXTERNAL=/root/pocketpill make pocketpill_defconfig"
BR2_EXTERNAL=/root/pocketpill make pocketpill_defconfig

#echo "cd output/build/linux-custom/ && rm .stamp_built .stamp_*_installed"
#cd output/build/linux-custom/ && rm .stamp_built .stamp_*_installed

#echo "cd output/build/linux-firmware-20221214/ && rm .stamp_built .stamp_*_installed"
#cd output/build/linux-firmware-20221214/ && rm .stamp_built .stamp_*_installed