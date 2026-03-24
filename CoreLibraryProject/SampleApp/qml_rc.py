# Resource object code (Python 3)
# Created by: object code
# Created by: The Resource Compiler for Qt version 6.9.1
# WARNING! All changes made in this file will be lost!

from PySide6 import QtCore

qt_resource_data = b"\
\x00\x00\x00\xc6\
i\
mport QtQuick 2.\
12\x0aimport QtQuic\
k.Window 2.12\x0a\x0aW\
indow {\x0a    visi\
ble: true\x0a    wi\
dth: 640\x0a    hei\
ght: 480\x0a    tit\
le: qsTr(\x22Hello \
World\x22)\x0a\x0a    Sys\
temSettingPanel \
{\x0a\x0a    }\x0a\x0a    De\
bugWindow{\x0a\x0a    \
}\x0a}\x0a\x0a\
"

qt_resource_name = b"\
\x00\x08\
\x08\x01Z\x5c\
\x00m\
\x00a\x00i\x00n\x00.\x00q\x00m\x00l\
"

qt_resource_struct = b"\
\x00\x00\x00\x00\x00\x02\x00\x00\x00\x01\x00\x00\x00\x01\
\x00\x00\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\
\x00\x00\x01\x96\xe1\xef\xe3\x08\
"

def qInitResources():
    QtCore.qRegisterResourceData(0x03, qt_resource_struct, qt_resource_name, qt_resource_data)

def qCleanupResources():
    QtCore.qUnregisterResourceData(0x03, qt_resource_struct, qt_resource_name, qt_resource_data)

qInitResources()
