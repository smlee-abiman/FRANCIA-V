#!/bin/sh

set -e
set -x

systemctl stop itz3000-gui.service
TSLIB_CALIBFILE=/etc/pointercal ts_calibrate

rm -f /etc/localtime
ln -s /usr/share/zoneinfo/Asia/Tokyo /etc/localtime

systemctl set-default graphical.target
