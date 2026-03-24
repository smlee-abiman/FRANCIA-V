include(gtest_dependency.pri)

TARGET = pendant-test
TEMPLATE = app

QT += testlib
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated
DEFINES += QT_DEPRECATED_WARNINGS

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

CONFIG += c++14

SOURCES += \
    ../src/bitarraylistmodel.cpp \
    ../src/csvdocument.cpp \
    ../src/csvreader.cpp \
    ../src/dataformat.cpp \
    ../src/symboltable.cpp \
    bitarraylistmodel_test.cpp \
    csvreader_test.cpp \
    dataformat_test.cpp \
    main.cpp \
    symboltable_test.cpp

HEADERS += \
    ../src/bitarraylistmodel.h \
    ../src/csvdocument.h \
    ../src/csvreader.h \
    ../src/dataformat.h \
    ../src/symboltable.h \
    testhelpers.h

INCLUDEPATH += ../src
