#!/bin/sh

#########################################################################################
# usage:
#   sudo sh update_gpu_driver.sh [-d <destination root directory>]
# options:
#   -d <destination root directory> ... Updates the gpu driver file to the specified destination directory.
#                                       Default directory is ( "/" )

#########################################################################################
# user settings.
UPDATE_ROOT_DIR="/"                         # default destination directory.

#########################################################################################
# script settings.
readonly ECHO="/bin/echo"
readonly TAR="/bin/tar"
readonly CP="/bin/cp"
readonly RM="/bin/rm"
readonly SCRIPT_NAME=$(basename $0)
readonly SCRIPT_DIR=$(cd $(dirname $0); pwd)
readonly GPU_DRIVER_ROOT_DIR=$(cd ${SCRIPT_DIR}/../driver/gpu_driver; pwd)
readonly GPU_DRIVER_DIR=${GPU_DRIVER_ROOT_DIR}/ti335x
readonly GPU_ARCHIVE_FILEPATH="${GPU_DRIVER_ROOT_DIR}/am335x_gpu_images.tar.gz"

#########################################################################################
# main.
$ECHO "${SCRIPT_NAME}: start."

# parse command line options.
ARG_DIR=""
for OPT in "$@"
do
    case $OPT in
        -d)
            ARG_DIR="$2"
            if [ -z ${ARG_DIR} ]; then
                $ECHO "${SCRIPT_NAME}: The destination directory is not specified. Does not update gpu driver."
                exit 0
            fi
            UPDATE_ROOT_DIR=$(cd $ARG_DIR; pwd)
            break
            ;;
    esac
    shift
done

# prepare.
if [ ! -e ${UPDATE_ROOT_DIR} ]; then
    $ECHO "${SCRIPT_NAME}: not Found ${UPDATE_ROOT_DIR}. Does not update gpu driver."
    exit 0
fi

if [ ! -e ${GPU_ARCHIVE_FILEPATH} ]; then
    $ECHO "${SCRIPT_NAME}: not Found ${UPDATE_ROOT_DIR}. Does not update gpu driver."
    exit 0
fi

# expand.
COMMAND="$TAR zxf ${GPU_ARCHIVE_FILEPATH} -C ${GPU_DRIVER_ROOT_DIR}"
$ECHO "${SCRIPT_NAME}: exexute ... $COMMAND"
ret=$($COMMAND)
$ECHO "${SCRIPT_NAME}: result ... status:${?}, ret:${ret}"

# update.
COMMAND="$CP -rf ${GPU_DRIVER_DIR}/* ${UPDATE_ROOT_DIR}"
$ECHO "${SCRIPT_NAME}: exexute ... $COMMAND"
ret=$($COMMAND)
$ECHO "${SCRIPT_NAME}: result ... status:${?}, ret:${ret}"

# post.
sync; sync; sync;

# cleanup.
COMMAND="$RM -rf ${GPU_DRIVER_DIR}"
$ECHO "${SCRIPT_NAME}: exexute ... $COMMAND"
ret=$($COMMAND)
$ECHO "${SCRIPT_NAME}: result ... status:${?}, ret:${ret}"

$ECHO "${SCRIPT_NAME}: end."

exit 0
