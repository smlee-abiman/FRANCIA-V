include(gtest_dependency.pri)

QT -= gui
QT += serialbus network core

TEMPLATE = app
CONFIG += console c++14 file_copies
CONFIG -= app_bundle
CONFIG += thread

unix:CONFIG(debug, debug|release):DEFINES += CORELIB_UNIT_TEST
else:unix:CONFIG(release, debug|release):DEFINES += CORELIB_RELEASE_TEST

HEADERS += \
        $$PWD/../CoreLib/CoreLib_global.h \
        $$PWD/../CoreLib/corelibapplicationmanager.h \
        $$PWD/../CoreLib/corelib.h \
        $$PWD/../CoreLib/corelibdebugdatamanager.h \
        $$PWD/../CoreLib/corelibdisplaycontrol.h \
        $$PWD/../CoreLib/corelibdrivermanager.h \
        $$PWD/../CoreLib/corelibexternalmemorymanger.h \
        $$PWD/../CoreLib/corelibfiletransferprotocol.h \
        $$PWD/../CoreLib/corelibgpiocontrol.h \
        $$PWD/../CoreLib/corelibinputkey.h \
        $$PWD/../CoreLib/corelibjogcontrol.h \
        $$PWD/../CoreLib/corelibmemorydatamanager.h \
        $$PWD/../CoreLib/corelibmodbuscommunication.h \
        $$PWD/../CoreLib/corelibmodbusdatamanager.h \
        $$PWD/../CoreLib/corelibsdcontrol.h \
        $$PWD/../CoreLib/corelibsystemcontrol.h \
        $$PWD/../CoreLib/corelibtouchcontrol.h \
        $$PWD/../CoreLib/corelibusbcontrol.h \
        $$PWD/../CoreLib/import_src/libcsvdocument.h \
        $$PWD/../CoreLib/import_src/libcsvreader.h \
        $$PWD/../CoreLib/import_src/libdataformat.h \
        $$PWD/../CoreLib/import_src/libpacketdata.h \
        $$PWD/../CoreLib/import_src/libsymboltable.h \
        corelibfiletransferprotocoltest.h \
        corelibgpiocontroltest.h \
        corelibjogcontroltest.h \
        corelibmodbuscommunicationtest.h \
        corelibsdcontroltest.h \
        corelibsystemcontroltest.h \
        corelibsysteminformationtest.h \
        corelibunittestmockmethod.h

SOURCES += \
        $$PWD/../CoreLib/corelibapplicationmanager.cpp \
        $$PWD/../CoreLib/corelibdebugdatamanager.cpp \
        $$PWD/../CoreLib/corelibdisplaycontrol.cpp \
        $$PWD/../CoreLib/corelibdrivermanager.cpp \
        $$PWD/../CoreLib/corelibexternalmemorymanger.cpp \
        $$PWD/../CoreLib/corelibfiletransferprotocol.cpp \
        $$PWD/../CoreLib/corelibgpiocontrol.cpp \
        $$PWD/../CoreLib/corelibinputkey.cpp \
        $$PWD/../CoreLib/corelibjogcontrol.cpp \
        $$PWD/../CoreLib/corelibmemorydatamanager.cpp \
        $$PWD/../CoreLib/corelibmodbuscommunication.cpp \
        $$PWD/../CoreLib/corelibmodbusdatamanager.cpp \
        $$PWD/../CoreLib/corelibsdcontrol.cpp \
        $$PWD/../CoreLib/corelibsystemcontrol.cpp \
        $$PWD/../CoreLib/corelibtouchcontrol.cpp \
        $$PWD/../CoreLib/corelibusbcontrol.cpp \
        $$PWD/../CoreLib/import_src/libcsvdocument.cpp \
        $$PWD/../CoreLib/import_src/libcsvreader.cpp \
        $$PWD/../CoreLib/import_src/libdataformat.cpp \
        $$PWD/../CoreLib/import_src/libpacketdata.cpp \
        $$PWD/../CoreLib/import_src/libsymboltable.cpp \
        corelibfiletransferprotocoltest.cpp \
        corelibgpiocontroltest.cpp \
        corelibjogcontroltest.cpp \
        corelibmodbuscommunicationtest.cpp \
        corelibsdcontroltest.cpp \
        corelibsystemcontroltest.cpp \
        corelibsysteminformationtest.cpp \
        corelibunittestmockmethod.cpp \
        main.cpp

DESTDIR = ../../bin
LIBS += -L../../bin/ -lCoreLib -lQt5Ftp
INCLUDEPATH += $$PWD/../CoreLib
INCLUDEPATH += $$PWD/../ExternalModules/qtftp/include
DEPENDPATH += $$OUT_PWD/../ExternalModules/qtftp/include
