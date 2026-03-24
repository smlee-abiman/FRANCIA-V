#!/bin/bash
export LD_LIBRARY_PATH=/opt/SystemSettingApp/CoreLib:$LD_LIBRARY_PATH
#journalctl -u itz3000-gui.service -f
/opt/pendant-gui/bin/pendant-gui -L info &

