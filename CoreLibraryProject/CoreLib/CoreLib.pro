QT -= gui
QT += serialbus network core quick

TEMPLATE = lib
DEFINES += CORELIB_LIBRARY

CONFIG += c++14 file_copies

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    corelibapplicationmanager.cpp \
    corelibdebugdatamanager.cpp \
    corelibdisplaycontrol.cpp \
    corelibdrivermanager.cpp \
    corelibexternalmemorymanger.cpp \
    corelibfiletransferprotocol.cpp \
    corelibgpiocontrol.cpp \
    corelibinputkey.cpp \
    corelibjogcontrol.cpp \
    corelibmemorydatamanager.cpp \
    corelibmodbuscommunication.cpp \
    corelibmodbusdatamanager.cpp \
    corelibsdcontrol.cpp \
    corelibsystemcontrol.cpp \
    corelibsysteminformation.cpp \
    corelibtouchcontrol.cpp \
    corelibusbcontrol.cpp \
    import_src/libcsvdocument.cpp \
    import_src/libcsvreader.cpp \
    import_src/libdataformat.cpp \
    import_src/libpacketdata.cpp \
    import_src/libsymboltable.cpp

HEADERS += \
    CoreLib_global.h \
    corelibapplicationmanager.h \
    corelib.h \
    corelibdebugdatamanager.h \
    corelibdisplaycontrol.h \
    corelibdrivermanager.h \
    corelibexternalmemorymanger.h \
    corelibfiletransferprotocol.h \
    corelibgpiocontrol.h \
    corelibinputkey.h \
    corelibjogcontrol.h \
    corelibmemorydatamanager.h \
    corelibmodbuscommunication.h \
    corelibmodbusdatamanager.h \
    corelibsdcontrol.h \
    corelibsystemcontrol.h \
    corelibsysteminformation.h \
    corelibtouchcontrol.h \
    corelibusbcontrol.h \
    import_src/libcsvdocument.h \
    import_src/libcsvreader.h \
    import_src/libdataformat.h \
    import_src/libpacketdata.h \
    import_src/libsymboltable.h

# Default rules for deployment.
unix {
    contains(QT_ARCH, arm.*) {
		DEFINES += REAL_TARGET
        target.path = /opt/SystemSettingApp/CoreLib
    } else {
        target.path = /usr/lib
    }
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml_corelib.qrc

DESTDIR = ../../bin
LIBS += -L../../bin/ -lQt5Ftp
INCLUDEPATH += $$PWD/../ExternalModules/qtftp/include
DEPENDPATH += $$OUT_PWD/../ExternalModules/qtftp/include
