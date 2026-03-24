QT += quick serialbus widgets charts network
QT += svg
CONFIG += c++14 file_copies
DESTDIR = bin

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# debug logging
# DEFINES += LOG_FILE_ENABLE
# DEFINES += LOG_MEMORY_ENABLE

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT

linux-oe-g++ {
    DEFINES += REAL_TARGET
    QTPLUGIN.platforms = qeglfs
    QTPLUGIN += qtslibplugin qtgraphicaleffects-qmlplugins qtvirtualkeyboardplugin qsvg
    # Minimize plugins to be linked statically
    QTPLUGIN.bearer = -
    QTPLUGIN.egldeviceintegrations = -
    QTPLUGIN.imageformats += qsvg
    QTPLUGIN.qmltooling = -
}

DEFINES += DRV_MONITOR_GRAPH_10MS_CYCLE
DEFINES += ENABLE_COMMUNICATION_DUAL_PORT
DEFINES += ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
#DEFINES += USE_FTP_COM_SECOND_PORT
#DEFINES += ENABLE_FTP_COM_ASYNC
DEFINES += ENABLE_ASYNC_COPY_TO_SD_FROM_USB

SOURCES += \
    src/appenvironment.cpp \
    src/axispointinfo.cpp \
    src/communication/ftpcommunication.cpp \
    src/communication/ftpcommunicationthread.cpp \
    src/communication/ftpmanager.cpp \
    src/communication/transferfilethread.cpp \
    src/communication/usbmanager.cpp \
    src/components/linescanvas.cpp \
    src/debugtool.cpp \
    src/fileitemmodel.cpp \
    src/fileitemobject.cpp \
    src/main.cpp \
    src/memorylogger.cpp \
    src/operationhistorylist.cpp \
    src/operationrecord.cpp \
    src/pendantapplication.cpp \
    src/platformhelpers.cpp \
    src/communication/programfilemanager.cpp \
    src/communication/modbuscommunication.cpp \
    src/communication/modbusmanager.cpp \
    src/communication/operationprogramlinedata.cpp \
    src/communication/communicationthread.cpp \
    src/communication/phykeymanager.cpp \
    src/resourcemanager/bitarraylistmodel.cpp \
    src/resourcemanager/memorymodel.cpp \
    src/resourcemanager/productionhistory.cpp \
    src/resourcemanager/resourcemanager.cpp \
    src/resourcemanager/packetdata.cpp \
    src/resourcemanager/csvreader.cpp \
    src/resourcemanager/dataformat.cpp \
    src/resourcemanager/csvdocument.cpp \
    src/resourcemanager/modbus.cpp \
    src/resourcemanager/symboltable.cpp \
    src/splashprogresscounter.cpp \
    src/splashprogresscounterworker.cpp \
    src/viewmodel/areasetpointinfo.cpp \
    src/viewmodel/areasetpointparser.cpp \
    src/viewmodel/autoview/autoaxisviewmodel.cpp \
    src/viewmodel/autoview/autocounterviewmodel.cpp \
    src/viewmodel/autoview/autoioviewmodel.cpp \
    src/viewmodel/autoview/autoprodviewmodel.cpp \
    src/viewmodel/autoview/autotimerviewmodel.cpp \
    src/viewmodel/autoview/autoviewmodel.cpp \
    src/viewmodel/axisdata.cpp \
    src/viewmodel/axisinfo.cpp \
    src/viewmodel/axisoperateviewmodel.cpp \
    src/viewmodel/commonviewmodel.cpp \
    src/viewmodel/externalmemoryviewmodel.cpp \
    src/viewmodel/functionview/abssetview/abssetviewmodel.cpp \
    src/viewmodel/functionview/accelerationview/accelerationviewmodel.cpp \
    src/viewmodel/functionview/areasetview/areasetdata.cpp \
    src/viewmodel/functionview/areasetview/areasetviewmodel.cpp \
    src/viewmodel/functionview/controllersettingsview/controllersettingsviewmodel.cpp \
    src/viewmodel/functionview/controllersettingsview/networksettingviewmodel.cpp \
    src/viewmodel/functionview/controllersettingsview/systemsettingviewmodel.cpp \
    src/viewmodel/functionview/controllersettingsview/usersettingviewmodel.cpp \
    src/viewmodel/functionview/functionviewmodel.cpp \
    src/viewmodel/functionview/maintainview/maintaindata.cpp \
    src/viewmodel/functionview/maintainview/maintainviewmodel.cpp \
    src/viewmodel/functionview/mechanicalparamview/driverparametersviewmodel.cpp \
    src/viewmodel/functionview/mechanicalparamview/drvparamviewmodel.cpp \
    src/viewmodel/functionview/mechanicalparamview/mechanicalparametersviewmodel.cpp \
    src/viewmodel/functionview/mechanicalparamview/mechanicalparamviewmodel.cpp \
    src/viewmodel/functionview/mechanicalparamview/modelselectviewmodel.cpp \
    src/viewmodel/functionview/mechanicalparamview/simplesetaxisinfo.cpp \
    src/viewmodel/functionview/mechanicalparamview/simplesetviewmodel.cpp \
    src/viewmodel/functionview/productionview/productionviewmodel.cpp \
    src/viewmodel/functionview/versionviewmodel.cpp \
    src/viewmodel/historyview/alarmhistorydata.cpp \
    src/viewmodel/historyview/alarmhistorydataparser.cpp \
    src/viewmodel/historyview/alarmhistoryviewmodel.cpp \
    src/viewmodel/historyview/historyview.cpp \
    src/viewmodel/historyview/iodata.cpp \
    src/viewmodel/historyview/iohistorydata.cpp \
    src/viewmodel/historyview/iohistorydataparser.cpp \
    src/viewmodel/historyview/iohistoryviewmodel.cpp \
    src/viewmodel/historyview/iostatus.cpp \
    src/viewmodel/historyview/operationhistorydata.cpp \
    src/viewmodel/historyview/operationhistorydataparser.cpp \
    src/viewmodel/historyview/operationhistorystring.cpp \
    src/viewmodel/historyview/operationhistoryviewmodel.cpp \
    src/viewmodel/manualview/manualioviewmodel.cpp \
    src/viewmodel/manualview/manualviewmodel.cpp \
    src/viewmodel/monitorview/drvmonitorviewmodel.cpp \
    src/viewmodel/monitorview/graphcanvas.cpp \
    src/viewmodel/monitorview/internalmonitorviewmodel.cpp \
    src/viewmodel/monitorview/ioforceviewmodel.cpp \
    src/viewmodel/monitorview/iomonitorviewmodel.cpp \
    src/viewmodel/monitorview/monitorviewmodel.cpp \
    src/viewmodel/originviewmodel.cpp \
    src/viewmodel/setupchangeviewmodel.cpp \
    src/viewmodel/teachingview/label.cpp \
    src/viewmodel/teachingview/pagemodelbase.cpp \
    src/viewmodel/teachingview/pointdata.cpp \
    src/viewmodel/teachingview/pointinfo.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandacc.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandalm.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandarc.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandarcc.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandaspd.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandbreak.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandcclr.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandcinc.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommanddec.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandend.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandgosub.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandgoto.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandhome.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandhomep.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandifgosub.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandifgoto.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandin.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandinc.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandjog.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandline.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandlinec.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandmove.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandmovep.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandnext.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandoff.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandon.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandout.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandoutp.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandpath.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandpause.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandprog.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandret.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandset.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandshift.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandsrv.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandunknown.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandwait.cpp \
    src/viewmodel/teachingview/programview/command/ncpcommandwaitpos.cpp \
    src/viewmodel/teachingview/programview/command/ncpvariable.cpp \
    src/viewmodel/teachingview/programview/ncparbitrarypoint.cpp \
    src/viewmodel/teachingview/programview/ncparbitrarypointstorage.cpp \
    src/viewmodel/teachingview/programview/ncpcommand.cpp \
    src/viewmodel/teachingview/programview/ncpcommandfactory.cpp \
    src/viewmodel/teachingview/programview/ncpcommandstring.cpp \
    src/viewmodel/teachingview/programview/ncpfile.cpp \
    src/viewmodel/teachingview/programview/ncplabel.cpp \
    src/viewmodel/teachingview/programview/ncplabelstorage.cpp \
    src/viewmodel/teachingview/programview/ncpoperate.cpp \
    src/viewmodel/teachingview/programview/ncpprogram.cpp \
    src/viewmodel/teachingview/programview/ncpstep.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_1item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_2item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_3.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_3item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_4.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_4item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_1item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_3.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_2item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_3.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_3item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_4.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_4item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_5.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_6.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_6item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_7.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu3.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu3_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu3_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_1item.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_3.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_4.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_3.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_4.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu5.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu5_1.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu5_2.cpp \
    src/viewmodel/teachingview/programview/programmenu/programmenu5_2item.cpp \
    src/viewmodel/teachingview/programview/teachingprogrampagepointinfo.cpp \
    src/viewmodel/teachingview/teachingcounterpagemodel.cpp \
    src/viewmodel/teachingview/teachingmodedata.cpp \
    src/viewmodel/teachingview/teachingmodepagemodel.cpp \
    src/viewmodel/teachingview/teachingpasspagemodel.cpp \
    src/viewmodel/teachingview/teachingpossetpagemodel.cpp \
    src/viewmodel/teachingview/teachingprogrampagemodel.cpp \
    src/viewmodel/teachingview/teachingstackpagemodel.cpp \
    src/viewmodel/teachingview/teachingtimerpagemodel.cpp \
    src/viewmodel/teachingview/teachingviewmodel.cpp \
    src/viewmodel/teachingview/timeraddress.cpp \
    src/viewmodel/teachingview/timerdata.cpp \
    src/viewmodel/variableviewmodel.cpp \
    src/viewmodel/viewmanagercolntroll.cpp \
    src/viewmodel/viewmodelbase.cpp

#    src/controllerviewmodel.cpp \
#    src/commonbarviewmodel.cpp \
#    src/autooperationviewmodel.cpp \
#    src/pointdataviewmodel.cpp \
#    src/editprogramviewmodel.cpp \
#    src/machineparamviewmodel.cpp \
#    src/programvariableviewmodel.cpp \
#    src/setupchangeoperationviewmodel.cpp \
#    src/ampmonitorviewmodel.cpp \
#    src/manualautocommonviewmodel.cpp \
#    src/manualoperationviewmodel.cpp \
#    src/filelist.cpp \


# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH =
QML_IMPORT_PATH = $$PWD/../CoreLibraryProject/CoreLib/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/appenvironment.h \
    src/axispointinfo.h \
    src/communication/ftpcommunication.h \
    src/communication/ftpcommunicationthread.h \
    src/communication/ftpmanager.h \
    src/communication/transferfilethread.h \
    src/communication/usbmanager.h \
    src/components/linescanvas.h \
    src/debugtool.h \
    src/enums.h \
    src/fileitemmodel.h \
    src/fileitemobject.h \
    src/identifiers.h \
    src/memorylogger.h \
    src/operationhistorylist.h \
    src/operationrecord.h \
    src/pendantapplication.h \
    src/platformhelpers.h \
    src/communication/programfilemanager.h \
    src/communication/communicationthread.h \
    src/communication/modbuscommandgenerator.h \
    src/communication/modbuscommunication.h \
    src/communication/modbusmanager.h \
    src/communication/operationprogramlinedata.h \
    src/communication/phykeymanager.h \
    src/resourcemanager/bitarraylistmodel.h \
    src/resourcemanager/csvreader.h \
    src/resourcemanager/packetdata.h \
    src/resourcemanager/resourcemanager.h \
    src/resourcemanager/dataformat.h \
    src/resourcemanager/memorymodel.h \
    src/resourcemanager/productionhistory.h \
    src/resourcemanager/csvdocument.h \
    src/resourcemanager/modbus.h \
    src/resourcemanager/symboltable.h \
    src/splashprogresscounter.h \
    src/splashprogresscounterworker.h \
    src/viewmodel/areasetpointinfo.h \
    src/viewmodel/areasetpointparser.h \
    src/viewmodel/autoview/autoaxisviewmodel.h \
    src/viewmodel/autoview/autocounterviewmodel.h \
    src/viewmodel/autoview/autoioviewmodel.h \
    src/viewmodel/autoview/autoprodviewmodel.h \
    src/viewmodel/autoview/autotimerviewmodel.h \
    src/viewmodel/autoview/autoviewmodel.h \
    src/viewmodel/axisdata.h \
    src/viewmodel/axisinfo.h \
    src/viewmodel/axisoperateviewmodel.h \
    src/viewmodel/commonviewmodel.h \
    src/viewmodel/externalmemoryviewmodel.h \
    src/viewmodel/functionview/abssetview/abssetviewmodel.h \
    src/viewmodel/functionview/accelerationview/accelerationviewmodel.h \
    src/viewmodel/functionview/areasetview/areasetdata.h \
    src/viewmodel/functionview/areasetview/areasetviewmodel.h \
    src/viewmodel/functionview/controllersettingsview/controllersettingsviewmodel.h \
    src/viewmodel/functionview/controllersettingsview/networksettingviewmodel.h \
    src/viewmodel/functionview/controllersettingsview/systemsettingviewmodel.h \
    src/viewmodel/functionview/controllersettingsview/usersettingviewmodel.h \
    src/viewmodel/functionview/functionviewmodel.h \
    src/viewmodel/functionview/maintainview/maintaindata.h \
    src/viewmodel/functionview/maintainview/maintainviewmodel.h \
    src/viewmodel/functionview/mechanicalparamview/driverparametersviewmodel.h \
    src/viewmodel/functionview/mechanicalparamview/drvparamviewmodel.h \
    src/viewmodel/functionview/mechanicalparamview/mechanicalparametersviewmodel.h \
    src/viewmodel/functionview/mechanicalparamview/mechanicalparamviewmodel.h \
    src/viewmodel/functionview/mechanicalparamview/modelselectviewmodel.h \
    src/viewmodel/functionview/mechanicalparamview/simplesetaxisinfo.h \
    src/viewmodel/functionview/mechanicalparamview/simplesetviewmodel.h \
    src/viewmodel/functionview/productionview/productionviewmodel.h \
    src/viewmodel/functionview/versionviewmodel.h \
    src/viewmodel/historyview/alarmhistorydata.h \
    src/viewmodel/historyview/alarmhistorydataparser.h \
    src/viewmodel/historyview/alarmhistoryviewmodel.h \
    src/viewmodel/historyview/historyview.h \
    src/viewmodel/historyview/iodata.h \
    src/viewmodel/historyview/iohistorydata.h \
    src/viewmodel/historyview/iohistorydataparser.h \
    src/viewmodel/historyview/iohistoryviewmodel.h \
    src/viewmodel/historyview/iostatus.h \
    src/viewmodel/historyview/operationhistorydata.h \
    src/viewmodel/historyview/operationhistorydataparser.h \
    src/viewmodel/historyview/operationhistorystring.h \
    src/viewmodel/historyview/operationhistoryviewmodel.h \
    src/viewmodel/manualview/manualioviewmodel.h \
    src/viewmodel/manualview/manualviewmodel.h \
    src/viewmodel/monitorview/drvmonitorviewmodel.h \
    src/viewmodel/monitorview/graphcanvas.h \
    src/viewmodel/monitorview/internalmonitorviewmodel.h \
    src/viewmodel/monitorview/ioforceviewmodel.h \
    src/viewmodel/monitorview/iomonitorviewmodel.h \
    src/viewmodel/monitorview/monitorviewmodel.h \
    src/viewmodel/originviewmodel.h \
    src/viewmodel/setupchangeviewmodel.h \
    src/viewmodel/teachingview/label.h \
    src/viewmodel/teachingview/pagemodelbase.h \
    src/viewmodel/teachingview/pointdata.h \
    src/viewmodel/teachingview/pointinfo.h \
    src/viewmodel/teachingview/programview/command/ncpcommandacc.h \
    src/viewmodel/teachingview/programview/command/ncpcommandalm.h \
    src/viewmodel/teachingview/programview/command/ncpcommandarc.h \
    src/viewmodel/teachingview/programview/command/ncpcommandarcc.h \
    src/viewmodel/teachingview/programview/command/ncpcommandaspd.h \
    src/viewmodel/teachingview/programview/command/ncpcommandbreak.h \
    src/viewmodel/teachingview/programview/command/ncpcommandcclr.h \
    src/viewmodel/teachingview/programview/command/ncpcommandcinc.h \
    src/viewmodel/teachingview/programview/command/ncpcommanddec.h \
    src/viewmodel/teachingview/programview/command/ncpcommandend.h \
    src/viewmodel/teachingview/programview/command/ncpcommandgosub.h \
    src/viewmodel/teachingview/programview/command/ncpcommandgoto.h \
    src/viewmodel/teachingview/programview/command/ncpcommandhome.h \
    src/viewmodel/teachingview/programview/command/ncpcommandhomep.h \
    src/viewmodel/teachingview/programview/command/ncpcommandifgosub.h \
    src/viewmodel/teachingview/programview/command/ncpcommandifgoto.h \
    src/viewmodel/teachingview/programview/command/ncpcommandin.h \
    src/viewmodel/teachingview/programview/command/ncpcommandinc.h \
    src/viewmodel/teachingview/programview/command/ncpcommandjog.h \
    src/viewmodel/teachingview/programview/command/ncpcommandline.h \
    src/viewmodel/teachingview/programview/command/ncpcommandlinec.h \
    src/viewmodel/teachingview/programview/command/ncpcommandmove.h \
    src/viewmodel/teachingview/programview/command/ncpcommandmovep.h \
    src/viewmodel/teachingview/programview/command/ncpcommandnext.h \
    src/viewmodel/teachingview/programview/command/ncpcommandoff.h \
    src/viewmodel/teachingview/programview/command/ncpcommandon.h \
    src/viewmodel/teachingview/programview/command/ncpcommandout.h \
    src/viewmodel/teachingview/programview/command/ncpcommandoutp.h \
    src/viewmodel/teachingview/programview/command/ncpcommandpath.h \
    src/viewmodel/teachingview/programview/command/ncpcommandpause.h \
    src/viewmodel/teachingview/programview/command/ncpcommandprog.h \
    src/viewmodel/teachingview/programview/command/ncpcommandret.h \
    src/viewmodel/teachingview/programview/command/ncpcommandset.h \
    src/viewmodel/teachingview/programview/command/ncpcommandshift.h \
    src/viewmodel/teachingview/programview/command/ncpcommandsrv.h \
    src/viewmodel/teachingview/programview/command/ncpcommandunknown.h \
    src/viewmodel/teachingview/programview/command/ncpcommandwait.h \
    src/viewmodel/teachingview/programview/command/ncpcommandwaitpos.h \
    src/viewmodel/teachingview/programview/command/ncpvariable.h \
    src/viewmodel/teachingview/programview/ncparbitrarypoint.h \
    src/viewmodel/teachingview/programview/ncparbitrarypointstorage.h \
    src/viewmodel/teachingview/programview/ncpcommand.h \
    src/viewmodel/teachingview/programview/ncpcommandfactory.h \
    src/viewmodel/teachingview/programview/ncpcommandstring.h \
    src/viewmodel/teachingview/programview/ncpfile.h \
    src/viewmodel/teachingview/programview/ncplabel.h \
    src/viewmodel/teachingview/programview/ncplabelstorage.h \
    src/viewmodel/teachingview/programview/ncpoperate.h \
    src/viewmodel/teachingview/programview/ncpprogram.h \
    src/viewmodel/teachingview/programview/ncpstep.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_1item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_2item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_3.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_3item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_4.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu1_4item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_1item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_3.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_2item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_3.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_3item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_4.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_4item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_5.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_6.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_6item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu2_4_7.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu3.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu3_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu3_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_1item.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_3.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_2_4.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_3.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu4_3_4.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu5.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu5_1.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu5_2.h \
    src/viewmodel/teachingview/programview/programmenu/programmenu5_2item.h \
    src/viewmodel/teachingview/programview/teachingprogrampagepointinfo.h \
    src/viewmodel/teachingview/teachingcounterpagemodel.h \
    src/viewmodel/teachingview/teachingmodedata.h \
    src/viewmodel/teachingview/teachingmodepagemodel.h \
    src/viewmodel/teachingview/teachingpasspagemodel.h \
    src/viewmodel/teachingview/teachingpossetpagemodel.h \
    src/viewmodel/teachingview/teachingprogrampagemodel.h \
    src/viewmodel/teachingview/teachingstackpagemodel.h \
    src/viewmodel/teachingview/teachingtimerpagemodel.h \
    src/viewmodel/teachingview/teachingviewdef.h \
    src/viewmodel/teachingview/teachingviewmodel.h \
    src/viewmodel/teachingview/timeraddress.h \
    src/viewmodel/teachingview/timerdata.h \
    src/viewmodel/variableviewmodel.h \
    src/viewmodel/viewmanagercolntroll.h \
    src/viewmodel/viewmodelbase.h

#    src/setupchangeoperationviewmodel.h \
#    src/pointdataviewmodel.h \
#    src/programvariableviewmodel.h \
#    src/machineparamviewmodel.h \
#    src/manualautocommonviewmodel.h \
#    src/manualoperationviewmodel.h \
#    src/editprogramviewmodel.h \
#    src/controllerviewmodel.h \
#    src/commonbarviewmodel.h \
#    src/autooperationviewmodel.h \
#    src/ampmonitorviewmodel.h \
#    src/filelist.h \

RESOURCES += qml \
    qml.qrc
qml.files = $$files(qml/**)
linux-oe-g++: qml.files -= qml/main.qml

# Internal assets which won't be replaced at run time
RESOURCES += assets
assets.files = $$files(assets/**)

# to teach systemd that the GUI is ready
linux {
    CONFIG += link_pkgconfig
    PKGCONFIG += libsystemd
}

COPIES += fonts images settings ini keybord_layout keybord_style ncp ftp

fonts.files = fonts/**
fonts.path = $$OUT_PWD/fonts

images.files = images/**
images.path = $$OUT_PWD/images

settings.files = settings/**
settings.path = $$OUT_PWD/settings

ini.files = ini/**
ini.path = $$OUT_PWD/ini

keybord_layout.files = keybord/layout/**
keybord_layout.path = $$OUT_PWD/keybord/layout

keybord_style.files = keybord/style/**
keybord_style.path = $$OUT_PWD/keybord/style/QtQuick/VirtualKeyboard/Styles/3GPendant

ncp.files = ncp/**
ncp.path = $$OUT_PWD/ncp

ftp.files = ftp/**
ftp.path = $$OUT_PWD/ftp

linux-oe-g++ {
    INSTALLS += target images_i settings_i ini_i keybord_layout_i keybord_style_i ncp_i ftp_i

    target.path = /opt/pendant-gui/bin

    images_i.files = images/**
    images_i.path = /opt/pendant-gui/images

    settings_i.files = settings/**
    settings_i.path = /opt/pendant-gui/settings

    ini_i.files = ini/**
    ini_i.path = /opt/pendant-gui/ini

    keybord_layout_i.files = keybord/layout/**
    keybord_layout_i.path = /opt/pendant-gui/keybord/layout

    keybord_style_i.files = keybord/style/**
    keybord_style_i.path = /opt/pendant-gui/keybord/style/QtQuick/VirtualKeyboard/Styles/3GPendant

    ncp_i.files = ncp/**
    ncp_i.path = /opt/pendant-gui/ncp

    ftp_i.files = ftp/**
    ftp_i.path = /opt/pendant-gui/ftp
}

#DESTDIR = ../bin
linux-oe-g++ {
    QT_IM_MODULE=qtvirtualkeyboard
    CONFIG += disable-desktop
}
LIBS += -L../bin/ -lCoreLib -lQt5Ftp

CONFIG(release, debug|release) {
    CONFIG += qtquickcompiler
}

INCLUDEPATH += $$PWD/../CoreLibraryProject/CoreLib
INCLUDEPATH += $$PWD/src/communication
INCLUDEPATH += $$PWD/src/resourcemanager
INCLUDEPATH += $$PWD/src/viewmodel
INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/../CoreLibraryProject/CoreLib
INCLUDEPATH += $$PWD/../CoreLibraryProject/ExternalModules/qtftp/include
DEPENDPATH += $$OUT_PWD/../CoreLibraryProject/ExternalModules/qtftp/include

DISTFILES += \
    ncp/DIR.NCP \
    qml/Auto/SubAxisPage.qml \
    qml/Auto/SubCounterPage.qml \
    qml/Auto/SubIoPage.qml \
    qml/Auto/SubProductionPage.qml \
    qml/Auto/SubTimerPage.qml \
    qml/CompileItems.qml \
    qml/ComponentLoader.qml \
    qml/Components/BackGroundModal.qml \
    qml/Components/ComparePopup.qml \
    qml/Components/Delete_Dia.qml \
    qml/Components/DummyFileDialogBackGround.qml \
    qml/Components/DummyFileDialogTitle.qml \
    qml/Components/FileConfirm.qml \
    qml/Components/FileConfirmPopup.qml \
    qml/Components/FileSave.qml \
    qml/Components/FileSavePopup.qml \
    qml/Components/File_1.qml \
    qml/Components/File_2.qml \
    qml/Components/File_3.qml \
    qml/Components/File_Dialog.qml \
    qml/Components/File_Item.qml \
    qml/Components/FunctionMenuDialog.qml \
    qml/Components/Language.qml \
    qml/Components/MainDialog/Alarm_Dialog.qml \
    qml/Components/MainDialog/Datetime_Dialog.qml \
    qml/Components/MainDialog/EmergencyStop_Dialog.qml \
    qml/Components/MainDialog/Error_Dialog.qml \
    qml/Components/MainDialog/Language_Dialog.qml \
    qml/Components/MainDialog/OperateCmd_Dialog.qml \
    qml/Components/MainDialog/Org_Dialog.qml \
    qml/Components/MainDialog/Speed_Dialog.qml \
    qml/Components/MainDialog/User_Dialog.qml \
    qml/Components/RemindDialog/Remind_Dialog.qml \
    qml/Components/RemindDialog/Remind_Forbid_Dialog.qml \
    qml/Components/RemindDialog/Remind_save.qml \
    qml/Components/Select_Variable_Dialog.qml \
    qml/Functions/AreaSet/AreaSetUtils.qml \
    qml/Functions/AreaSet/Area_Input.qml \
    qml/Functions/ControllerSettings/Axis_set.qml \
    qml/Functions/Maintain/Maintain_1.qml \
    qml/Functions/Maintain/Maintain_2.qml \
    qml/Functions/MechanicalConfig/DriveParameters/DrvParamSimpleData.qml \
    qml/Manual/SpeedControl.qml \
    qml/Manual/ManualUtils.js \
    qml/Meter/OriginUtils.js \
    qml/Monitor/DriveMonitorGraph.qml \
    qml/Monitor/DriveMonitorGraphLDraw.qml \
    qml/Monitor/DriveMonitorGraphMDraw.qml \
    qml/Monitor/Driver_MaxValue.qml \
    qml/Monitor/Driver_RealTimeValue.qml \
    qml/Monitor/IO_ForceSwapDialog.qml \
    qml/OperateModePage.qml \
    qml/Program/PosSet/PosSetUtils.js \
    qml/Program/Program/Label_Dia.qml \
    qml/Program/Program/MainMenu_1.qml \
    qml/Program/Program/MainMenu_1_1.qml \
    qml/Program/Program/MainMenu_1_2.qml \
    qml/Program/Program/MainMenu_1_3.qml \
    qml/Program/Program/MainMenu_1_4.qml \
    qml/Program/Program/MainMenu_2.qml \
    qml/Program/Program/MainMenu_2_1.qml \
    qml/Program/Program/MainMenu_2_2.qml \
    qml/Program/Program/MainMenu_2_3.qml \
    qml/Program/Program/MainMenu_2_4.qml \
    qml/Program/Program/MainMenu_2_4_1.qml \
    qml/Program/Program/MainMenu_2_4_2.qml \
    qml/Program/Program/MainMenu_2_4_3.qml \
    qml/Program/Program/MainMenu_2_4_4.qml \
    qml/Program/Program/MainMenu_2_4_5.qml \
    qml/Program/Program/MainMenu_2_4_6.qml \
    qml/Program/Program/MainMenu_2_4_7.qml \
    qml/Program/Program/MainMenu_3.qml \
    qml/Program/Program/MainMenu_3_1.qml \
    qml/Program/Program/MainMenu_3_2.qml \
    qml/Program/Program/MainMenu_4.qml \
    qml/Program/Program/MainMenu_4_1.qml \
    qml/Program/Program/MainMenu_4_2.qml \
    qml/Program/Program/MainMenu_4_2_1.qml \
    qml/Program/Program/MainMenu_4_2_2.qml \
    qml/Program/Program/MainMenu_4_2_3.qml \
    qml/Program/Program/MainMenu_4_2_4.qml \
    qml/Program/Program/MainMenu_4_3.qml \
    qml/Program/Program/MainMenu_4_3_1.qml \
    qml/Program/Program/MainMenu_4_3_2.qml \
    qml/Program/Program/MainMenu_4_3_3.qml \
    qml/Program/Program/MainMenu_4_3_4.qml \
    qml/Program/Program/MainMenu_5.qml \
    qml/Program/Program/MainMenu_5_1.qml \
    qml/Program/Program/MainMenu_5_2.qml \
    qml/Program/Program/Prog_Dia.qml \
    qml/Program/Program/Prog_select_Dia.qml \
    qml/Program/Program/Program.qml \
    qml/Program/Program/Variable_Dia.qml \
    qml/Program/Stack/Free_Stack.qml \
    qml/Program/Stack/StackPage.qml \
    qml/Program/Stack/Stack_animation.qml \
    qml/Program/Stack/Standard_Stack.qml \
    qml/Record/IoHistoryItem.qml \
    qml/Record/RecordDialog.qml \
    qml/Widget/AbnormalPopup.qml \
    qml/Widget/CircularProgressBar.qml \
    qml/Widget/ComboBox/StringComboBox_style1.qml \
    qml/Widget/ComboBox/StringComboBox_style2.qml \
    qml/Widget/ComboBox/StringComboBox_style3.qml \
    qml/Widget/ComboBox/StringComboBox_style4.qml \
    qml/Widget/ComboBox/StringComboBox_style5.qml \
    qml/Widget/ComboBox/Triangle.qml \
    qml/Widget/ConfirmPopup.qml \
    qml/Widget/HiddenButton.qml \
    qml/Widget/ModeButton.qml \
    qml/Widget/NumericKeyPad_adjust.qml \
    qml/Widget/PageText.qml \
    qml/Widget/ResourceStringComboBox1.qml \
    qml/Widget/ResourceStringComboBoxDefaultIndicator.qml


