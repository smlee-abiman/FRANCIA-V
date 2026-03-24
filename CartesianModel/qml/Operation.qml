import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import Enums 1.0
import "Origin"
import "Manual"
import "Auto"
import "Monitor"
import "Program"
import "Program/Program"

/*************************************************************************************
* Title: UI/UX of AEK
* File: Operation.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.06.18
* Worker: IJ.YI
**************************************************************************************/

/****************手动操作、原点复归、自动运行三个页面切换及执行操作(初始化）*********************/
/**CurrentIndex=0(原点页面) CurrentIndex=1(手动页面) CurrentIndex=2(自动页面)
 CurrentIndex=3(监控页面) CurrentIndex=4(教导页面) CurrentIndex=5(系统设定页面)
 CurrentIndex=6(机械参数页面) CurrentIndex=7(区域设定页面) CurrentIndex=8(维护页面)
 CurrentIndex=9(生产管理页面) CurrentIndex=10(ABS设定页面) CurrentIndex=11(加减速设定页面)
 CurrentIndex=12(版本页面) CurrentIndex=13(履历页面)
**/

StackView {
    id: stackview
    width: parent.width
    height: parent.height
    property int currentIndex: 0
    property bool bootCompleted: commonViewModel.bootCompleted
    property int topPopupDialog: commonViewModel.topPopupDialog

    // operation background color
    Rectangle{
        width: parent.width
        height: parent.height
        color: "#EAEAEA"
    }

///-------------------------------------------------------------------------------------------
    Component.onCompleted: {
        for(let i = 0; i < 5; i++){
            local.pageOriginOpeAuth[i] = commonViewModel.getOperationAuthority(5, i)
            local.pageManualOpeAuth[i] = commonViewModel.getOperationAuthority(6, i)
            local.pageAutoOpeAuth[i] = commonViewModel.getOperationAuthority(7, i)
            local.pageMonitorOpeAuth[i] = commonViewModel.getOperationAuthority(8, i)
            local.pageTeachnigOpeAuth[i] = commonViewModel.getOperationAuthority(9, i)
            local.pageFunctionSysSetOpeAuth[i] = commonViewModel.getOperationAuthority(17, i)
            local.pageFunctionMechanicalParmOpeAuth[i] = commonViewModel.getOperationAuthority(18, i)
            local.pageFunctionAreaSetOpeAuth[i] = commonViewModel.getOperationAuthority(19, i)
            local.pageFunctionMaintenanceOpeAuth[i] = commonViewModel.getOperationAuthority(20, i)
            local.pageFunctionProductionOpeAuth[i] = commonViewModel.getOperationAuthority(21, i)
            local.pageFunctionAbsOpeAuth[i] = commonViewModel.getOperationAuthority(22, i)
            local.pageFunctionDecelerationOpeAuth[i] = commonViewModel.getOperationAuthority(23, i)
            local.pageFunctionVersionOpeAuth[i] = commonViewModel.getOperationAuthority(24, i)
            local.pageHistoryOpeAuth[i] = commonViewModel.getOperationAuthority(25, i)
        }
        let startupSelectSwStatus = phyKeyManager.selectSwMode
        if (startupSelectSwStatus <= 0) {
            startupSelectSwStatus = 2   // Manual
        }
        updateSelectMode(startupSelectSwStatus)
    }

///-------------------------------------------------------------------------------------------
    QtObject {
        id: local
        property var pageOriginOpeAuth:[]
        property var pageManualOpeAuth:[]
        property var pageAutoOpeAuth:[]
        property var pageMonitorOpeAuth:[]
        property var pageTeachnigOpeAuth:[]
        property var pageFunctionSysSetOpeAuth:[]
        property var pageFunctionMechanicalParmOpeAuth:[]
        property var pageFunctionAreaSetOpeAuth:[]
        property var pageFunctionMaintenanceOpeAuth:[]
        property var pageFunctionProductionOpeAuth:[]
        property var pageFunctionAbsOpeAuth:[]
        property var pageFunctionDecelerationOpeAuth:[]
        property var pageFunctionVersionOpeAuth:[]
        property var pageHistoryOpeAuth:[]
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property bool visibleOfsystemSettings: (userSettingView.systemSettingsDisplayLevel <= (userLevel + 1))
        readonly property var teachingviewModel: teachingViewModel.pageModel[6]

        property int localSwNo: -1 ///本地三段开关
    }

///-------------------------------------------------------------------------------------------
    function backDefaultMenu(){
        if (local.localSwNo - 1 === currentIndex) {
            return
        }
        replace(ooperatePage, StackView.Immediate)
        ooperatePage.active = true
        currentIndex = local.localSwNo - 1

        switch(local.localSwNo - 1) {
        case 0:
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_42.png";
            resourceManager.topName = resourceManager.label(20,1,resourceManager.language);
            resourceManager.titleString = resourceManager.label(20,6,resourceManager.language);
            resourceManager.screenId = 1002;
            break
        case 1:
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_41.png"
            resourceManager.topName = resourceManager.label(20,1,resourceManager.language)
            commonViewModel.pageChange(Enums.ManualIO)
            resourceManager.screenId = 1001
            resourceManager.titleString = resourceManager.label(20,5,resourceManager.language)
            break
        case 2:
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_40.png"
            resourceManager.topName = resourceManager.label(20,0,resourceManager.language)
            resourceManager.screenId = 1000
            resourceManager.titleString = resourceManager.label(20,4,resourceManager.language)
            break

        }
    }

    function setIndex(index) {
        if (index === currentIndex) {
            return
        }
        switch(index){
        case 0:
            if((currentIndex !== 0) && (currentIndex !== 1) && (currentIndex !== 2)){
                replace(ooperatePage, StackView.Immediate)
                ooperatePage.active = true
                currentIndex = 0
            }
            break;
        case 1:
            if((currentIndex !== 0) && (currentIndex !== 1) && (currentIndex !== 2)){
                replace(ooperatePage, StackView.Immediate)
                ooperatePage.active = true
                currentIndex = 1
            }
            break;
        case 2:
            if((currentIndex !== 0) && (currentIndex !== 1) && (currentIndex !== 2)){
                replace(ooperatePage, StackView.Immediate)
                ooperatePage.active = true
                currentIndex = 2
            }
            break;
        case 3:
            replace(monitorPage, StackView.Immediate)
            monitorPage.active = true
            currentIndex = 3
            break;
        case 4:
            replace(programPage, StackView.Immediate)
            programPage.active = true
            currentIndex = 4
            break;
        case 5:
            if (controllerSettingPage.status == Loader.Ready) {
                replace(controllerSettingPage, StackView.Immediate)
            } else {
                controllerSettingPage.active = true
                controllerSettingPage.pendingScreenChange = true
            }
            currentIndex = 5
            break;
        case 6:
            replace(mechanicalconfigPage, StackView.Immediate)
            mechanicalconfigPage.active = true
            currentIndex = 6
            break;
        case 7:
            replace(areasetPage, StackView.Immediate)
            areasetPage.active = true
            currentIndex = 7
            break;
        case 8:
            replace(maintainPage, StackView.Immediate)
            maintainPage.active = true
            currentIndex = 8
            break;
        case 9:
            replace(productionPage, StackView.Immediate)
            productionPage.active = true
            currentIndex = 9
            break;
        case 10:
            replace(abssetPage, StackView.Immediate)
            abssetPage.active = true
            currentIndex = 10
            break;
        case 11:
            replace(decelerationPage, StackView.Immediate)
            decelerationPage.active = true
            currentIndex = 11
            break;
        case 12:
            replace(editionPage, StackView.Immediate)
            editionPage.active = true
            currentIndex = 12
            break;
        case 13:
            replace(recordPage, StackView.Immediate)
            recordPage.active = true
            currentIndex = 13
            break;
        }

        doSwitchScreen(index)
    }

    function doSwitchScreen(menuIndex) {
        if (menuIndex === -1) {
            return
        }
        switch(menuIndex) {
        case 0:
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_40.png"
            resourceManager.topName = resourceManager.label(20,0,resourceManager.language)
            resourceManager.screenId = 1000
            resourceManager.titleString = resourceManager.label(20,4,resourceManager.language)
            commonViewModel.modbusOriginPressed()
            break
        case 1:
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_41.png"
            resourceManager.topName = resourceManager.label(20,1,resourceManager.language)
            commonViewModel.pageChange(Enums.ManualIO)
            resourceManager.screenId = 1001
            resourceManager.titleString = resourceManager.label(20,5,resourceManager.language)
            commonViewModel.modbusManualPressed()
            break
        case 2:
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_42.png";
            resourceManager.topName = resourceManager.label(20,1,resourceManager.language);
            resourceManager.titleString = resourceManager.label(20,6,resourceManager.language);
            resourceManager.screenId = 1002;
            commonViewModel.modbusAutoPressed();
            break
        case 3:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1003
            resourceManager.titleString = resourceManager.label(20,7,resourceManager.language)
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_43.png"
            break
        case 4:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1004
            resourceManager.titleString = resourceManager.label(20,31,resourceManager.language)
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_44.png"
            break
        case 5:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1006
            resourceManager.titleString = resourceManager.label(20,27,resourceManager.language)
            break
        case 6:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1007
            resourceManager.titleString = resourceManager.label(20,11,resourceManager.language)
            break
        case 7:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1008
            resourceManager.titleString = resourceManager.label(20,12,resourceManager.language)
            break
        case 8:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1009
            resourceManager.titleString = resourceManager.label(20,13,resourceManager.language)
            break
        case 9:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1010
            resourceManager.titleString = resourceManager.label(20,14,resourceManager.language)
            break
        case 10:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1011
            resourceManager.titleString = resourceManager.label(20,15,resourceManager.language)
            break
        case 11:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1012
            resourceManager.titleString = resourceManager.label(20,16,resourceManager.language)
            break
        case 12:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1013
            resourceManager.titleString = resourceManager.label(20,17,resourceManager.language)
            break
        case 13:
            resourceManager.topName = resourceManager.label(20,4,resourceManager.language)
            resourceManager.screenId = 1014
            resourceManager.titleString = resourceManager.label(20,28,resourceManager.language)
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_46.png"
            break
        case 100:   // function button.
            dialog.open()
            //resourceManager.titleString = resourceManager.label(20,6,resourceManager.language)
            resourceManager.topIcon = resourceManager.imagePath + "/00_COMMON/d000_45.png"
            resourceManager.screenId = 1005
            break
        }
    }

    function updateSelectMode(selectSwMode){
        if (selectSwMode > 0) {
            stackview.checkFtpCommunication()
            selectSw(selectSwMode);
        } else {
            if (!REAL_TARGET) {
                selectSw(selectSwMode);
            }
        }

        local.localSwNo = selectSwMode
    }

    function selectSw(selectSwMode){
        if (selectSwMode === 1) {
            setIndex(2)
        } else if (selectSwMode === 2){
            setIndex(1)
        } else if (selectSwMode === 3){
            setIndex(0)
        } else {
            setIndex(1)
        }
    }

    function checkFtpCommunication(){
        //console.log("checkFtpCommunication ftpStatus:"+ftpManager.ftpStatus, "isDeterNoopCmd:"+ftpManager.isDeterNoopCmd)
        if (ftpManager.ftpStatus >= 4 && ftpManager.ftpStatus !== 5){
            if (ftpManager.isDeterNoopCmd === false){
                ftpManager.execFtpCommand(Enums.FtpCommandNOOP, null, "");
            }
        }
    }
///-------------------------------------------------------------------------------------------
    initialItem: {
        switch(resourceManager.screenId){            
        case 1000:
            ooperatePage;
            ooperatePage.active = true
            break;
        case 1001:
            ooperatePage;
            ooperatePage.active = true
            break;
        case 1002:
            ooperatePage;
            ooperatePage.active = true
            break;
        case 1003:
            monitorPage;
            monitorPage.active = true
            break;
        case 1004:
            programPage;
            programPage.active = true
            break;
        case 1006:
            controllerSettingPage;
            controllerSettingPage.active = true
            break;
        case 1007:
            mechanicalconfigPage;
            mechanicalconfigPage.active = true
            break;
        case 1008:
            areasetPage;
            areasetPage.active = true
            break;
        case 1009:
            maintainPage;
            maintainPage.active = true
            break;
        case 1010:
            productionPage;
            productionPage.active = true
            break;
        case 1011:
            abssetPage;
            abssetPage.active = true
            break;
        case 1012:
            decelerationPage;
            decelerationPage.active = true
            break;
        case 1013:
            editionPage;
            editionPage.active = true
            break;
        case 1014:
            recordPage;
            recordPage.active = true
            break;
        }
    }

    Loader{
        id: ooperatePage
        source: "qrc:/qml/OperateModePage.qml"
        property bool isActive: (stackview.currentIndex === 0) || (stackview.currentIndex === 1) || (stackview.currentIndex === 2)
        visible: isActive
        onLoaded: {
            if(ooperatePage.item != null){
                ooperatePage.item.pageActive = ooperatePage.isActive;
            }
        }
        onIsActiveChanged: {
            if(ooperatePage.item != null){
                ooperatePage.item.pageActive = ooperatePage.isActive;
            }
        }
    }//原点&手动&自动画面

    Loader{
        id: monitorPage
        source: "qrc:/qml/Monitor/MonitorPage.qml"
        property bool isActive: (stackview.currentIndex === 3)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(monitorPage.item != null){
                monitorPage.item.pageActive = monitorPage.isActive;
            }
            componentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(monitorPage.item != null){
                monitorPage.item.pageActive = monitorPage.isActive;
            }
        }
    }//监控画面

    Loader{
        id: programPage
        property bool isActive: (stackview.currentIndex === 4)
        active: false
        visible: isActive
        onLoaded: componentLoader.loadNextComponent()
        sourceComponent: ProgramPage {
            pageActive: programPage.isActive
            preLoadEnable: componentLoader.loadCompleted
        }
    }//教导画面

    Loader{
        id: controllerSettingPage
        source: "qrc:/qml/Functions/ControllerSettings/ControllerSettingPage.qml"
        property bool isActive: (stackview.currentIndex === 5)
        property bool pendingScreenChange: false
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(controllerSettingPage.item != null){
                controllerSettingPage.item.pageActive = controllerSettingPage.isActive;
            }
            if (pendingScreenChange && isActive) {
                replace(controllerSettingPage, StackView.Immediate)
            }
            pendingScreenChange = false
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(controllerSettingPage.item != null){
                controllerSettingPage.item.pageActive = controllerSettingPage.isActive;
            }
        }
    }//系统设定画面

    Loader{
        id: mechanicalconfigPage
        source: "qrc:/qml/Functions/MechanicalConfig/Mechanical_Config.qml"
        property bool isActive: (stackview.currentIndex === 6)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(mechanicalconfigPage.item != null){
                mechanicalconfigPage.item.pageActive = mechanicalconfigPage.isActive;
            }
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(mechanicalconfigPage.item != null){
                mechanicalconfigPage.item.pageActive = mechanicalconfigPage.isActive;
            }
        }
    }//机械参数画面

    Loader{
        id: areasetPage
        source: "qrc:/qml/Functions/AreaSet/Area_Set.qml"
        property bool isActive: (stackview.currentIndex === 7)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(areasetPage.item != null){
                areasetPage.item.pageActive = areasetPage.isActive;
            }
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(areasetPage.item != null){
                areasetPage.item.pageActive = areasetPage.isActive;
            }
        }
    }//区域设定画面

    Loader{
        id: maintainPage
        source: "qrc:/qml/Functions/Maintain/MaintainPage.qml"
        property bool isActive: (stackview.currentIndex === 8)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(maintainPage.item != null){
                maintainPage.item.pageActive = maintainPage.isActive;
            }
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(maintainPage.item != null){
                maintainPage.item.pageActive = maintainPage.isActive;
            }
        }
    }//维护画面

    Loader{
        id: productionPage
        source: "qrc:/qml/Functions/Production/ProductionPage.qml"
        property bool isActive: (stackview.currentIndex === 9)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(productionPage.item != null){
                productionPage.item.pageActive = productionPage.isActive;
            }
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(productionPage.item != null){
                productionPage.item.pageActive = productionPage.isActive;
            }
        }
    }//生产管理画面

    Loader{
        id: abssetPage
        source: "qrc:/qml/Functions/ABS/Abs_Set.qml"
        property bool isActive: (stackview.currentIndex === 10)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(abssetPage.item != null){
                abssetPage.item.pageActive = abssetPage.isActive;
            }
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(abssetPage.item != null){
                abssetPage.item.pageActive = abssetPage.isActive;
            }
        }
    }//ABS设定画面

    Loader{
        id: decelerationPage
        source: "qrc:/qml/Functions/Deceleration/Deceleration_page.qml"
        property bool isActive: (stackview.currentIndex === 11)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(decelerationPage.item != null){
                decelerationPage.item.pageActive = decelerationPage.isActive;
            }
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(decelerationPage.item != null){
                decelerationPage.item.pageActive = decelerationPage.isActive;
            }
        }
    }//加减速画面

    Loader{
        id: editionPage
        source: "qrc:/qml/Functions/EditionPage.qml"
        property bool isActive: (stackview.currentIndex === 12)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(editionPage.item != null){
                editionPage.item.pageActive = editionPage.isActive;
            }
            functionMenuComponentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(editionPage.item != null){
                editionPage.item.pageActive = editionPage.isActive;
            }
        }
    }//版本画面

    Loader{
        id: recordPage
        source: "qrc:/qml/Record/RecordPage.qml"
        property bool isActive: (stackview.currentIndex === 13)
        active: false
        asynchronous: !isActive
        visible: isActive
        onLoaded: {
            if(recordPage.item != null){
                recordPage.item.pageActive = recordPage.isActive;
            }
            componentLoader.loadNextComponent()
        }
        onIsActiveChanged: {
            if(recordPage.item != null){
                recordPage.item.pageActive = recordPage.isActive;
            }
        }
    }//履历画面

///-------------------------------------------------------------------------------------------
    Connections{
        target: ooperatePage.item
        onScreenChange:{
            if(id === 1){
                setIndex(3)
            }else if(id === 2){
                setIndex(4)
            }else if(id === 4){
                setIndex(13)
            }else {
                setIndex(id)
            }
        }
    }

    Connections {
        target:phyKeyManager
        onSelectSwModeChanged: {
            updateSelectMode(selectSwMode)
        }
    }


///-------------------------------------------------------------------------------------------
    ComponentLoader {
        id: componentLoader
        guard: !bootCompleted || programPage.isActive || (topPopupDialog === Enums.PopupDialogStatusFunction)
        loadComponentList: [
            programPage,
            monitorPage,
            recordPage,
            compileItems
        ]
        onGuardChanged: {
            if (!guard) {
                loadNextComponent()
            }
        }
    }

    ComponentLoader {
        id: functionMenuComponentLoader
        guard: (topPopupDialog !== Enums.PopupDialogStatusFunction)
        loadComponentList: [
            controllerSettingPage,
            areasetPage,
            productionPage,
            mechanicalconfigPage,
            maintainPage,
            editionPage,
            decelerationPage,
            abssetPage
        ]
        onGuardChanged: {
            if (!guard) {
                loadNextComponent()
            }
        }
    }

    Loader {
        id: compileItems
        source: "qrc:/qml/CompileItems.qml"
        active: false
        asynchronous: true
        onLoaded: componentLoader.loadNextComponent()
    }
///-------------------------------------------------------------------------------------------

}
