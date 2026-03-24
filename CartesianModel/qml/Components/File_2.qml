import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: File_2.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.23
* Worker: SH.HONG
**************************************************************************************/
Item {
    id:setupChgExportUsb
    property int page1: 1
    property int page2: 0
    property bool pageActive: false
    property bool jogScrollOn:false
    property int listCnt: externalMemoryViewModel.remoteDirFileCnt
    property var extFtpControllerFileList:externalMemoryViewModel.extFtpControllerFileList
    property bool listSelect:false
    property bool listAreaSelect:false
    property bool listAreaPos:false         // true:left false:right
    property bool jogDialFirstClick:false
    property string usbWriteConfirm:resourceManager.label(51,35,resourceManager.language)
    property string usbWriteProcessingMsg:resourceManager.label(51,40,resourceManager.language)
    property string usbWriteProcessFailMsg:resourceManager.label(51,42,resourceManager.language)
    property string usbWriteProcessSuccessMsg:resourceManager.label(51,38,resourceManager.language)
    property int leftTotalPage:(setupChgExportUsb.listCnt / 7 === 0) ? 1 :
                                (setupChgExportUsb.listCnt % 7 === 0) ? setupChgExportUsb.listCnt / 7 : (setupChgExportUsb.listCnt / 7) + 1
    property bool usbStautsPopupDisp:false
    property string usbStatusMsg:resourceManager.label(51,44,resourceManager.language)
    property bool isAlreadyFtpAccess:true
    property int ftpStatus:ftpManager.ftpStatus
    property bool isFtpListRetryOk:false
    property bool isStartupFtpStatus:ftpManager.isStartupFtpStatus
    property bool ignoreFirstUsbConnected:false
    property bool isFtpFileAcquired:externalMemoryViewModel.isFtpFileAcquired
    property string orderingMsg:resourceManager.label(22,11,local.language)
    property bool isModbusConnectStatus:externalMemoryViewModel.isModbusConnect

    QtObject {
        id: local
        property int language: 0
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    onPageActiveChanged: {
        console.debug("File_2 onPageActiveChanged pageActive:"+pageActive)
        if (pageActive) {
            externalMemoryViewModel.curTabIndex = 1;
            if(REAL_TARGET) {
                externalMemoryViewModel.startModbusMonitoring()
                if(setupChgExportUsb.isFtpFileAcquired === true && setupChgExportUsb.isModbusConnectStatus === true){
                    setupChgExportUsb.pageInitProcess()
                } else {
                    externalMemoryViewModel.clearFtpControllerViewFileList(0);
                }
            } else {
                setupChgExportUsb.pageInitProcess()
            }
        } else {
            setupChgExportUsb.initScreenInfo()
            externalMemoryViewModel.timerStop()
            externalMemoryViewModel.stopUsbMonitoring()
            externalMemoryViewModel.stopModbusMonitoring()
        }
    }
    function pageInitProcess(){
            //console.log("File_2 onPageActiveChanged dandoriListUpdate:"+setupChangeViewModel.dandoriListUpdate)
            if (setupChangeViewModel.dandoriListUpdate === 3){
                setupChangeViewModel.getFtpFileData()
            }
            let fileListCnt = externalMemoryViewModel.getFtpRemoteFileListCount();
            console.debug("File_2 onPageActiveChanged fileListCnt:"+fileListCnt)
            if (fileListCnt <= 0){
                console.debug("File_2 onPageActiveChanged ftpStatus:"+setupChgExportUsb.ftpStatus)
                if (setupChgExportUsb.ftpStatus <= 0 || setupChgExportUsb.ftpStatus === 5){
                    setupChgExportUsb.isAlreadyFtpAccess = false
////                    externalMemoryViewModel.startFtpCommunication();
                }
                for(let i = 0; i < 2; i++){
                    let retryListCnt = externalMemoryViewModel.getFtpRemoteFileListCount();
                    //console.log("File_2 onPageActiveChanged retryListCnt:"+retryListCnt)
                    if(retryListCnt > 0){
                        setupChgExportUsb.isFtpListRetryOk = true
                        break
                    }
                }
                console.debug("File_2 onPageActiveChanged isFtpListRetryOk:"+setupChgExportUsb.isFtpListRetryOk)
                if (setupChgExportUsb.isFtpListRetryOk === true){
                    externalMemoryViewModel.getFtpFileList();
                }
            } else {
                if (setupChgExportUsb.ftpStatus <= 4){
                    console.debug("File_2 onPageActiveChanged ftpStatus:"+setupChgExportUsb.ftpStatus, "isStartupFtpStatus:"+setupChgExportUsb.isStartupFtpStatus)
                    if (setupChgExportUsb.ftpStatus < 4){
////                        externalMemoryViewModel.startFtpCommunication();
                    } else {
                        if (setupChgExportUsb.isStartupFtpStatus === false){
                            externalMemoryViewModel.getFtpFileList();
                        }
                    }
                }
            }
            externalMemoryViewModel.startUsbMonitoring()
    }
    function initScreenInfo(){
        setupChgExportUsb.initSelectScreenStatus()
        setupChgExportUsb.page1 = 1
        setupChgExportUsb.page2 = 1
    }
    function initSelectScreenStatus(){
        setupChgExportUsb.listSelect = false
        setupChgExportUsb.listAreaPos = false
    }
    function clearLeftListSelect(){
        setupChgExportUsb.listSelect = false
    }

    function orderingPopupProcess(){
        var msgStr = (setupChgExportUsb.orderingMsg).toString()
        messagePopup.openOrderingPopup(msgStr, false, false)
    }

    Component{
        id:listdelegate1
        Rectangle{
            width: 310
            height: 50
            border.color: "#999999"
            color:  (exportMemLeftList.currentIndex >= 0 && setupChgExportUsb.listSelect === true) ?
                    ((exportMemLeftList.currentIndex === index) ? "#cc3333" : (index%2 == 0) ? "#ffffff": "#cccccc") :
                     (index%2 == 0) ? "#ffffff": "#cccccc"
            Image {
                id:leftKindIcon
                x:20
                anchors.verticalCenter: parent.verticalCenter
                source:(((setupChgExportUsb.page1 - 1) * 7 + index) >= setupChgExportUsb.listCnt) ? "":
                        (setupChgExportUsb.extFtpControllerFileList[(setupChgExportUsb.page1 - 1) * 7 + index].fileUrlDir === true) ?
                            resourceManager.imagePath + "/28_USB/a280_13.png" : resourceManager.imagePath + "/28_USB/a280_14.png"
            }
            Text{
                id:leftFileNameStr
                anchors.left:leftKindIcon.right
                anchors.leftMargin:10
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                text: (((setupChgExportUsb.page1 - 1) * 7 + index) >= setupChgExportUsb.listCnt) ? "":
                        setupChgExportUsb.extFtpControllerFileList[(setupChgExportUsb.page1 - 1) * 7 + index].fileName
                color:(setupChgExportUsb.listSelect === true) ?
                    ((index === exportMemLeftList.currentIndex) ? externalMemoryViewModel.getSelectedFileFolderListDispColor() :
                        externalMemoryViewModel.getFileFolderListDispColor()) : externalMemoryViewModel.getFileFolderListDispColor()
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.debug("File_2 listdelegate1 listCnt:"+setupChgExportUsb.listCnt, "index:"+index, "page1:"+setupChgExportUsb.page1)
                    if(((setupChgExportUsb.page1 - 1) * 7 + index) >= setupChgExportUsb.listCnt){
                        //NOP
                    } else {
                        setupChgExportUsb.listSelect = true
                        exportMemLeftList.currentIndex = index
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }

        }
    }
    Component{
        id:listdelegate2
        Rectangle{
            width: 310
            height: 50
            border.color: "#999999"
            color: "#ffffff"
            Image {
                x:20
                anchors.verticalCenter: parent.verticalCenter
                source: resourceManager.imagePath + "/28_USB/a280_14.png"
            }
        }
    }
    Row{
        x:5
        y:4
        spacing: 10
        Rectangle{
            width: 400
            height: 450
            color: "transparent"
            border.color: (setupChgExportUsb.listAreaSelect === false) ? "#999999" : (setupChgExportUsb.listAreaPos === true) ? "#999999" : "#999999"
            border.width: 2
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:6
                font.pixelSize: 26
                font.bold: true
                text: resourceManager.label(51,31,resourceManager.language)
                color: resourceManager.rgb(51,31)
            }
            ListView{
                id:exportMemLeftList
                property int pg: page1
                width: 310
                height: 398
                x:20
                y:43
                clip: true
                spacing: 8
                model:7
                delegate: listdelegate1
                interactive: false
                onPgChanged: {
                    positionViewAtIndex((pg-1)*7,ListView.Beginning)
                }
                Component.onCompleted: {
                    positionViewAtIndex((pg-1)*7,ListView.Beginning)
                }
            }
            //换页按钮
            ImageScrollBar{
                x:335
                y:43
                height: 398
                jogScrollOn: setupChgExportUsb.jogScrollOn
                topButtonPressImage: resourceManager.imagePath + "/28_USB/a280_09" + ".png"
                topButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_07" + ".png"
                botomButtonPressImage: resourceManager.imagePath + "/28_USB/a280_10" + ".png"
                botomButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_08" + ".png"
                pageLabal:setupChgExportUsb.page1 + "/" + setupChgExportUsb.leftTotalPage
                onScrollUpClicked: {
                    setupChgExportUsb.clearLeftListSelect()
                    if (setupChgExportUsb.listAreaSelect === true){
                        if (setupChgExportUsb.listAreaPos === true){
                            if (setupChgExportUsb.page1 > 1) {
                                setupChgExportUsb.page1--
                                setupChgExportUsb.listSelect = false
                            } else {
                                setupChgExportUsb.page1 = setupChgExportUsb.leftTotalPage
                            }
                        }
                    } else {
                        if (setupChgExportUsb.page1 > 1) {
                            setupChgExportUsb.page1--
                            setupChgExportUsb.listSelect = false
                        } else {
                            setupChgExportUsb.page1 = setupChgExportUsb.leftTotalPage
                        }
                    }
                }
                onScrollDownClicked: {
                    setupChgExportUsb.clearLeftListSelect()
                    if (setupChgExportUsb.listAreaSelect === true){
                        if (setupChgExportUsb.listAreaPos === true){
                            if (setupChgExportUsb.page1 >= setupChgExportUsb.leftTotalPage) {
                                setupChgExportUsb.page1 = 1
                            } else {
                                setupChgExportUsb.page1++
                                setupChgExportUsb.listSelect = false
                            }
                        }
                    } else {
                        if (setupChgExportUsb.page1 >= setupChgExportUsb.leftTotalPage) {
                            setupChgExportUsb.page1 = 1
                        } else {
                            setupChgExportUsb.page1++
                            setupChgExportUsb.listSelect = false
                        }
                    }
                }
            }

        }
        Rectangle{
            width: 400
            height: 450
            color: "transparent"
            border.color: (setupChgExportUsb.listAreaSelect === false) ? "#999999" : (setupChgExportUsb.listAreaPos === false) ? "#999999" : "#999999"
            border.width: 2
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                y:6
                font.pixelSize: 26
                font.bold: true
                text: resourceManager.label(51,32,resourceManager.language)
                color: resourceManager.rgb(51,32)
            }
            ListView{
                property int pg: page2
                width: 310
                height: 398
                x:20
                y:43
                clip: true
                spacing: 8
                model: 7
                delegate: listdelegate2
                interactive: false
                onPgChanged: {
                    positionViewAtIndex(pg*7,ListView.Beginning)
                }
                Component.onCompleted: {
                    positionViewAtIndex(pg*7,ListView.Beginning)
                }
            }
            //换页按钮
            ImageScrollBar{
                x:335
                y:43
                height: 398
                jogScrollOn: setupChgExportUsb.jogScrollOn
                topButtonPressImage: resourceManager.imagePath + "/28_USB/a280_09" + ".png"
                topButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_07" + ".png"
                botomButtonPressImage: resourceManager.imagePath + "/28_USB/a280_10" + ".png"
                botomButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_08" + ".png"
                //pageLabal: page2+1 + "/" + "3"
                pageLabal: ""
                onScrollUpClicked: {
                    // TODO:Folder does not exist on controller
                    if (setupChgExportUsb.listAreaSelect === true){
                        if (setupChgExportUsb.listAreaPos === false){
                            page2 = Math.max(page2 - 1, 0)
                        }
                    } else {
                        page2 = Math.max(page2 - 1, 0)
                    }
                }
                onScrollDownClicked: {
                    // TODO:Folder does not exist on controller
                    if (setupChgExportUsb.listAreaSelect === true){
                        if (setupChgExportUsb.listAreaPos === false){
                            page2 = Math.min(page2 + 1, 2)
                        }
                    } else {
                        page2 = Math.min(page2 + 1, 2)
                    }
                }
            }
        }
        Rectangle{
            width: 176
            height: 450
            color: "#eaeaea"
            border.color: "#999999"
            ImageButton{
                id:btnToUsb
                anchors.horizontalCenter: parent.horizontalCenter
                y:40
                upImageSource: resourceManager.imagePath + "/28_USB/a280_11.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_12.png"
                fontPixelSize:18
                text:resourceManager.label(51,33, resourceManager.language)
                textColor:"#666666"
                enableBtnTextColorChg:true
                btnPressColor:externalMemoryViewModel.getCommandPressColor()
                onClicked:{
                    if (setupChgExportUsb.listSelect === true){
                        confirmPopup.buttonIndex = 0
                        let selectListIndex = (setupChgExportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex;
                        var msgStr = (setupChgExportUsb.usbWriteConfirm).toString()
                            .replace("[file]", setupChgExportUsb.extFtpControllerFileList[selectListIndex].fileName);
                        confirmPopup.text = msgStr
                        confirmPopup.listIndex = selectListIndex
                        confirmPopup.subListIndex = 0
                        confirmPopup.open();
                    }
                }
            }
            ImageButton{
                id:btnAllToUsb
                anchors.horizontalCenter: parent.horizontalCenter
                y:120
                upImageSource: resourceManager.imagePath + "/28_USB/a280_11.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_12.png"
                fontPixelSize:18
                text:resourceManager.label(51,52, resourceManager.language)
                textColor: "#666666"
                enableBtnTextColorChg:true
                btnPressColor:externalMemoryViewModel.getCommandPressColor()
                onClicked:{
                    console.debug("isUSBConnected:"+externalMemoryViewModel.isUSBConnected, "firstUsbConnected:"+externalMemoryViewModel.firstUsbConnected)
                    if (externalMemoryViewModel.isUSBConnected || externalMemoryViewModel.firstUsbConnected){
                        confirmPopup.buttonIndex = 1
                        var msgStr = (setupChgExportUsb.usbWriteConfirm).toString()
                            .replace("[file]", resourceManager.label(51,52,resourceManager.language));
                        confirmPopup.text = msgStr
                        confirmPopup.open();
                    }
                }
            }
        }
    }
    FileConfirmPopup{
        id:confirmPopup
        viewModel:externalMemoryViewModel
        tabIndex:1
        onProcessingDialogOpen:{
            if (setupChgExportUsb.pageActive === true){
                var msgStr
                console.debug("onProcessingDialogOpen btnIndex:"+btnIndex)
                if (btnIndex === 0){
                    msgStr = (setupChgExportUsb.usbWriteProcessingMsg).toString()
                        .replace("[file]", setupChgExportUsb.extFtpControllerFileList[(setupChgExportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex].fileName);
                } else {
                    msgStr = (setupChgExportUsb.usbWriteProcessingMsg).toString()
                        .replace("[file]", resourceManager.label(51,52,resourceManager.language));
                }
                processingPopup.text = msgStr
                processingPopup.isBtnVisible = false
                processingPopup.buttonIndex = btnIndex
                processingPopup.open()
            }
        }
        onEnactFailDialogOpen:{
            if (setupChgExportUsb.pageActive === true){
                var msgStr = (setupChgExportUsb.usbWriteProcessFailMsg).toString()
                    .replace("[file]", setupChgExportUsb.extFtpControllerFileList[(setupChgExportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex].fileName);
                messagePopup.openProcessFinishPopup(msgStr, false, btnIndex)
            }
        }
        onDialogClose:{
        }
    }
    FileConfirmPopup{
        id:processingPopup
        viewModel:externalMemoryViewModel
        tabIndex:1
        onEnactSuccessDialogOpen:{
            if (setupChgExportUsb.pageActive === true){
                var msgStr
                console.debug("onEnactSuccessDialogOpen btnIndex:"+btnIndex)
                if (btnIndex === 0){
                    msgStr = (setupChgExportUsb.usbWriteProcessSuccessMsg).toString()
                        .replace("[file]", setupChgExportUsb.extFtpControllerFileList[(setupChgExportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex].fileName);
                } else {
                    msgStr = (setupChgExportUsb.usbWriteProcessSuccessMsg).toString()
                        .replace("[file]", resourceManager.label(51,52,resourceManager.language));
                }
                messagePopup.openProcessFinishPopup(msgStr, false, btnIndex)
            }
        }
        onEnactFailDialogOpen:{
            if (setupChgExportUsb.pageActive === true){
                var msgStr
                console.debug("onEnactFailDialogOpen btnIndex:"+btnIndex)
                if (btnIndex === 0){
                    msgStr = (setupChgExportUsb.usbWriteProcessFailMsg).toString()
                        .replace("[file]", setupChgExportUsb.extFtpControllerFileList[(setupChgExportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex].fileName);
                } else {
                    msgStr = (setupChgExportUsb.usbWriteProcessFailMsg).toString()
                        .replace("[file]", resourceManager.label(51,52,resourceManager.language));
                }
                messagePopup.openProcessFinishPopup(msgStr, false, btnIndex)
            }
        }
        onDialogClose:{
        }
    }

    Item {
        id: messagePopup
        anchors.centerIn: parent
        function _openPopup(stateName, msgStr, btnVisible = true, btnIndex = -1, closeBtnVisible = true) {
            close()
            state = stateName
            _inner_confirmPopup.text = msgStr
            _inner_confirmPopup.isBtnVisible = btnVisible
            _inner_confirmPopup.buttonIndex = btnIndex
            _inner_confirmPopup.isCloseBtnVisible = closeBtnVisible
            _inner_confirmPopup.open()
        }
        function openProcessFinishPopup(msgStr, btnVisible, btnIndex) {
            _openPopup("processFinishPopup", msgStr, btnVisible, btnIndex)
        }
        function openUsbStatusPopup(msgStr, btnVisible) {
            _openPopup("usbStatusPopup", msgStr, btnVisible)
        }
        function openOrderingPopup(msgStr, btnVisible, closeBtnVisible) {
            _openPopup("orderingPopup", msgStr, btnVisible, -1, closeBtnVisible)
        }
        function close() {
            if (_inner_confirmPopup.opened) {
                // _inner_confirmPopup.dialogClose()    // todo: need to call ?
                _inner_confirmPopup.close()
            }
        }
        FileConfirmPopup {
            id: _inner_confirmPopup
            tabIndex: 1
            viewModel: externalMemoryViewModel
            onDialogClose: {}
            // note: The behavior of FileConfirmPopup is set by messageDialog.states.
        }
        states: [
            State {
                name: "processFinishPopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose:{
                        // console.log("File_2 processFinishPopup onDialogClose()")     // for debug.
                        //setupChgExportUsb.listSelect = false
                    }
                }
            },
            State {
                name: "usbStatusPopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose:{
                        console.debug("File_2 usbStatusPopup onDialogClose()")         // for debug.
                        if (setupChgExportUsb.usbStautsPopupDisp === true){
                            setupChgExportUsb.usbStautsPopupDisp = false
                        }
                    }
                }
            },
            State {
                name: "orderingPopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose:{
                        //console.log("File_2 orderingPopup onDialogClose pageActive:"+setupChgExportUsb.pageActive)
                    }
                }
            }
        ]
    }
/***********************************************************************/
    Connections{
        target: ftpManager
        onFtpStatusChanged:{
            console.debug("File_2 ftpManager onFtpStatusChanged:"+ftpStatus, "pageActive:"+setupChgExportUsb.pageActive)
            if (setupChgExportUsb.pageActive === true && setupChgExportUsb.isAlreadyFtpAccess === false){
                if(REAL_TARGET){
                    switch(ftpStatus){
                    case 0:
////                        externalMemoryViewModel.startFtpCommunication();
                        break;
                    case 4:
                        externalMemoryViewModel.getFtpFileList();
                        break;
                    case 6:
                        let listCnt = externalMemoryViewModel.getFtpRemoteFileListCount();
                        if (listCnt <= 0){
                            setupChgExportUsb.isAlreadyFtpAccess = true
                            console.log("File_2 !!! FtpList listCnt:"+listCnt)
                        } else {
                            setupChgExportUsb.isAlreadyFtpAccess = true
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    Connections {
        target: phyKeyManager
        enabled: setupChgExportUsb.pageActive
        onOperableSwStatusChanged: {
            console.debug("File_2 onOperableSwStatusChanged:"+operableSwStatus)
            console.debug("File_2 onOperableSwStatusChanged:"+operableSwStatus, "pageActive:"+setupChgExportUsb.pageActive)
            if (setupChgExportUsb.pageActive === true){
                if (setupChgExportUsb.jogDialFirstClick === true){
                    setupChgExportUsb.listAreaSelect = operableSwStatus;
                } else {
                    setupChgExportUsb.listAreaSelect = operableSwStatus;
                }
            }
        }
        onJogDialSwitchValidChanged: {
            console.debug("File_2 onJogDialSwitchValidChanged:"+jogDialSwitchValid)
            console.debug("File_2 onJogDialSwitchValidChanged:"+jogDialSwitchValid, "pageActive:"+setupChgExportUsb.pageActive)
            if (setupChgExportUsb.pageActive === true){
                if (setupChgExportUsb.listAreaSelect === false){
                    setupChgExportUsb.listAreaSelect = true;
                    setupChgExportUsb.jogDialFirstClick = true
                }
                setupChgExportUsb.listAreaPos = !setupChgExportUsb.listAreaPos
            }
        }
    }
    Connections{
        target: externalMemoryViewModel
        onIsUSBConnectedChanged:{
            console.debug("File_2 setupChgExportUsb pageActive:"+setupChgExportUsb.pageActive, "isUSBConnected:"+isUSBConnected, "usbStautsPopupDisp:"+setupChgExportUsb.usbStautsPopupDisp, "firstUsbConnected:"+externalMemoryViewModel.firstUsbConnected)
            if (setupChgExportUsb.pageActive === true){
                if (isUSBConnected === false){
                    if (setupChgExportUsb.usbStautsPopupDisp === false){
                        var msgStr = setupChgExportUsb.usbStatusMsg
                        messagePopup.openUsbStatusPopup(msgStr, false)
                        setupChgExportUsb.usbStautsPopupDisp = true
                    }
                    externalMemoryViewModel.firstUsbConnected = false
                } else {
                    if (setupChgExportUsb.usbStautsPopupDisp === true){
                        messagePopup.close()
                        setupChgExportUsb.usbStautsPopupDisp = false
                    }
                    if (externalMemoryViewModel.firstUsbConnected == false){
                        //externalMemoryViewModel.createBackupFolder()      // USB挿抜検出ではフォルダ生成しない
                        setupChgExportUsb.ignoreFirstUsbConnected = true
                        externalMemoryViewModel.firstUsbConnected = true
                    }
                }
            }
        }
        onFirstUsbConnectedChanged:{
            console.debug("File_2 setupChgExportUsb pageActive:"+setupChgExportUsb.pageActive, "isUSBConnected:"+firstUsbConnected)
            if (setupChgExportUsb.pageActive === true){
                if (firstUsbConnected === false){
                    if (setupChgExportUsb.usbStautsPopupDisp === false){
                        var msgStr = setupChgExportUsb.usbStatusMsg
                        messagePopup.openUsbStatusPopup(msgStr, false)
                        setupChgExportUsb.usbStautsPopupDisp = true
                    }
                } else {
                    if (setupChgExportUsb.usbStautsPopupDisp === true){
                        messagePopup.close()
                        setupChgExportUsb.usbStautsPopupDisp = false
                    }
                    if (setupChgExportUsb.ignoreFirstUsbConnected === false){
                        //externalMemoryViewModel.createBackupFolder();      // USB挿抜検出ではフォルダ生成しない
                    } else {
                        setupChgExportUsb.ignoreFirstUsbConnected = false;
                    }
                }
            }
        }
        onIsFtpFileAcquiredChanged:{
            console.debug("File_2 onIsFtpFileAcquiredChanged isFtpFileAcquired:"+isFtpFileAcquired, "pageActive:"+setupChgExportUsb.pageActive)
            if(setupChgExportUsb.pageActive === true){
                if (isFtpFileAcquired === true){
                    setupChgExportUsb.pageInitProcess()
                } else {
                    setupChgExportUsb.orderingPopupProcess()
                }
            }
        }
        onIsModbusConnectChanged:{
            console.debug("File_2 onIsModbusConnectChanged isModbusConnect:"+isModbusConnect, "pageActive:"+setupChgExportUsb.pageActive, "isModbusConnect:"+externalMemoryViewModel.isModbusConnect)
            if (setupChgExportUsb.pageActive === true){
                if(externalMemoryViewModel.isModbusConnect === true){
                    setupChgExportUsb.pageInitProcess()
                } else {
                    setupChgExportUsb.initScreenInfo()
                }
            }
        }
    }
//    MouseArea {
//        anchors.fill: parent
//        propagateComposedEvents: true
//        onPressed: mouse.accepted = false
//        onReleased: mouse.accepted = false
//        onClicked: mouse.accepted = false
//        onWheel: {
//            if(!REAL_TARGET) {
//                if(wheel.angleDelta.y > 0) {
//                    //console.log("mouse wheel up")
//                    phyKeyManager.decrementEmulatedJogCount()
//                } else if(wheel.angleDelta.y < 0) {
//                    //console.log("mouse wheel down")
//                    phyKeyManager.incrementEmulatedJogCount()
//                }
//            }
//        }
//    }
}
