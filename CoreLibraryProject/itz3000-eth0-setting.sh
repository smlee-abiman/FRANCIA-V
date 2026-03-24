#!/bin/bash

#########################################################################################
# script settings.
readonly ENABLE_FILE="/opt/ChangeEthernetSpeed"
readonly ETH_TOOL="/usr/sbin/ethtool"
readonly ECHO="/bin/echo"
readonly CAT="/bin/cat"

#########################################################################################
# main.
$ECHO "itz3000-eth0-setting.sh: start."

# prepare.
if [ ! -e $ETH_TOOL ]; then
    $ECHO "itz3000-eth0-setting.sh: not Found ${ETH_TOOL}. Does not change the Ethernet setting."
    exit 0
fi

if [ ! -e ${ENABLE_FILE} ]; then
    $ECHO "itz3000-eth0-setting.sh: not Found ${ENABLE_FILE} file. Does not change the Ethernet setting."
    exit 0
fi

# read parameter, make command.
PARAMETER=$($CAT ${ENABLE_FILE})
COMMAND="${ETH_TOOL} -s eth0 ${PARAMETER}"

# execute.
$ECHO "itz3000-eth0-setting.sh: Found ${ENABLE_FILE} file. Change the Ethernet speed setting."
$ECHO "itz3000-eth0-setting.sh: execute ... ${COMMAND}"
ret=$($COMMAND)
$ECHO "itz3000-eth0-setting.sh: result ... status:${?}, ret:${ret}"

exit 0
