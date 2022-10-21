git clone git://git.yoctoproject.org/poky
cd poky
git checkout -b dunfell origin/dunfell
cd ..
git clone git://git.openembedded.org/meta-openembedded -b dunfell
git clone git://git.yoctoproject.org/meta-raspberrypi -b dunfell
source poky/oe-init-build-env project-build-rpi
bitbake-layers add-layer ../meta-raspberrypi
bitbake-layers add-layer ../meta-openembedded/meta-oe
bitbake-layers add-layer ../meta-openembedded/meta-python
bitbake-layers add-layer ../meta-openembedded/meta-networking
bitbake-layers add-layer ../meta-openembedded/meta-multimedia
bitbake-layers show-layers
bitbake-layers create-layer ../meta-hcsr04
bitbake-layers add-layer ../meta-hcsr04
cd ..
cp -r recipes-ultrasonic meta-hcsr04
cp -r hcsr04-mod meta-raspberrypi/recipes-kernel
cd meta-raspberrypi/conf/machine
echo 'KERNEL_MODULE_AUTOLOAD += "g_serial hcsr04"' >> raspberrypi3.conf
cd ../../../
pwd
cp local.conf project-build-rpi/conf
source poky/oe-init-build-env project-build-rpi
sudo sysctl -n -w fs.inotify.max_user_watches=128000
bitbake -c clean rpi-test-image
bitbake rpi-test-image
echo "The image has been successfully built. You can now flash it into your SD card"
