ROOT=../..
CORELIBRARYPROJECT_ROOT=$${ROOT}/CoreLibraryProject

message(pendant root: $${ROOT})
message(CoreLibraryProject root: $${CORELIBRARYPROJECT_ROOT})

QT += testlib serialbus
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH+=../src \
             ../src/communication \
             ../src/resourcemanager \
             ../src/viewmodel \
             ../src/viewmodel/monitorview \
             ../src/viewmodel/teachingview \
             $${CORELIBRARYPROJECT_ROOT}/CoreLib \
             ./mock \
             .

DESTDIR = ../../bin
LIBS += -L../../bin/ -lCoreLib

SOURCES +=  \
    ../src/appenvironment.cpp \
    ../src/resourcemanager/csvdocument.cpp \
    ../src/resourcemanager/csvreader.cpp \
    ../src/resourcemanager/dataformat.cpp \
    ../src/resourcemanager/packetdata.cpp \
    ../src/resourcemanager/resourcemanager.cpp \
    ../src/resourcemanager/symboltable.cpp \
    ../src/viewmodel/monitorview/ioforceviewmodel.cpp \
    ../src/viewmodel/teachingview/label.cpp \
    ../src/viewmodel/teachingview/pagemodelbase.cpp \
    ../src/viewmodel/teachingview/teachingcounterpagemodel.cpp \
    ../src/viewmodel/teachingview/teachingtimerpagemodel.cpp \
    ../src/viewmodel/teachingview/timeraddress.cpp \
    ../src/viewmodel/teachingview/timerdata.cpp \
    ../src/viewmodel/viewmodelbase.cpp \
    mock/mock_communicationthread.cpp \
    mock/mock_modbusmanager.cpp \
    tst_ioforceviewmodel.cpp \
    tst_teachingcounterpagemodel.cpp \
    tst_teachingtimerpagemodel.cpp \
    main.cpp

HEADERS += \
    ../src/appenvironment.h \
    ../src/resourcemanager/csvdocument.h \
    ../src/resourcemanager/csvreader.h \
    ../src/resourcemanager/dataformat.h \
    ../src/resourcemanager/packetdata.h \
    ../src/resourcemanager/resourcemanager.h \
    ../src/resourcemanager/symboltable.h \
    ../src/viewmodel/monitorview/ioforceviewmodel.h \
    ../src/viewmodel/teachingview/label.h \
    ../src/viewmodel/teachingview/pagemodelbase.h \
    ../src/viewmodel/teachingview/teachingcounterpagemodel.h \
    ../src/viewmodel/teachingview/teachingtimerpagemodel.h \
    ../src/viewmodel/teachingview/teachingviewdef.h \
    ../src/viewmodel/teachingview/timeraddress.h \
    ../src/viewmodel/teachingview/timerdata.h \
    ../src/viewmodel/viewmodelbase.h \
    mock/mock_communicationthread.h \
    mock/mock_modbusmanager.h \
    tst_ioforceviewmodel.h \
    tst_teachingcounterpagemodel.h \
    tst_teachingtimerpagemodel.h
