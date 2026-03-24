#!/bin/sh

ret=`systemctl get-default`
#echo "${ret}"

#green led power on
#/bin/sh -c "echo 255 > /sys/class/leds/green/brightness"
#/bin/sh -c "echo 0 > /sys/class/leds/green/brightness"
if [ "${ret}" = "graphical.target" ]; then
echo "not calib"
else
set -e
set -x

systemctl stop itz3000-gui.service
export TSLIB_TSDEVICE=/dev/input/touchscreen0
TSLIB_CALIBFILE=/etc/pointercal ts_calibrate

rm -f /etc/localtime
ln -s /usr/share/zoneinfo/Asia/Tokyo /etc/localtime

systemctl set-default graphical.target
#green led power off
#/bin/sh -c "echo 0 > /sys/class/leds/green/brightness"
sync; sync; sync
reboot
fi

