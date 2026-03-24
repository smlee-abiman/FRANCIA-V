import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: File_3.qml
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
    id:setupChgImportUsb
    property int page1: 1
    property int page2: 1
    property bool pageActive: false
    property bool jogScrollOn:false
    property int listCnt: externalMemoryViewModel.usbDirFileCnt
    property var extMemoryFileList:externalMemoryViewModel.extUsbMemoryFileList
    property int childDirListCnt: externalMemoryViewModel.usbChildDirFileCnt
    property var extMemoryChildDirFileList:externalMemoryViewModel.extUsbMemoryChildDirFileList
    property bool listSelect:false
    property bool rightListSelect:false
    property bool listAreaSelect:false
    property bool listAreaPos:false         // true:left false:right
    property bool jogDialFirstClick:false
    property bool leftFolderSelect:false
    property string usbReadConfirm:resourceManager.label(51,36,resourceManager.language)
    property string usbReadProcessingMsg:resourceManager.label(51,41,resourceManager.language)
    property string usbReadProcessFailMsg:resourceManager.label(51,43,resourceManager.language)
    property string usbReadProcessSuccessMsg:resourceManager.label(51,39,resourceManager.language)
    property string usbReadFolderConfirm:resourceManager.label(51,47,resourceManager.language)
    property string callAttentionPowerRestartMsg:resourceManager.label(51,51,resourceManager.language)
    property int hierarchyCnt:0
    property bool isUsbConnected:externalMemoryViewModel.isUSBConnected
    property bool isAlreadyUsbFileList:externalMemoryViewModel.isAlreadyUsbFileList
    property bool isAlreadyUsbChildFileList:externalMemoryViewModel.isAlreadyUsbChildFileList
    property int leftTotalPage:(setupChgImportUsb.listCnt / 7 === 0) ? 1 :
                                (setupChgImportUsb.listCnt % 7 === 0) ? setupChgImportUsb.listCnt / 7 : (setupChgImportUsb.listCnt / 7) + 1
    property int rightTotalPage:(setupChgImportUsb.childDirListCnt / 7 === 0) ? 1 :
                                (setupChgImportUsb.childDirListCnt % 7 === 0) ? setupChgImportUsb.childDirListCnt / 7 : (setupChgImportUsb.childDirListCnt / 7) + 1
    property bool usbStautsPopupDisp:false
    property string usbStatusMsg:resourceManager.label(51,44,resourceManager.language)
    property int ftpStatus:ftpManager.ftpStatus
    property bool isAlreadyFtpAccess:true
    property bool isReadOnly:false

    onPageActiveChanged: {
        console.debug("File_3 onPageActiveChanged pageActive:"+pageActive)
        if (pageActive) {
            externalMemoryViewModel.curTabIndex = 2;
            externalMemoryViewModel.activate()
            externalMemoryViewModel.startModbusMonitoring()
            console.debug("File_3 onPageActiveChanged isAlreadyUsbFileList:"+setupChgImportUsb.isAlreadyUsbFileList, "isUsbConnected:"+setupChgImportUsb.isUsbConnected)
            if (setupChgImportUsb.isUsbConnected === true){
                if (setupChgImportUsb.isAlreadyUsbFileList === false){
                    externalMemoryViewModel.getUsbDirFileList()
                    externalMemoryViewModel.getUsbDirFileListCount()
                }
            }
            if (setupChgImportUsb.ftpStatus <= 0 || setupChgImportUsb.ftpStatus === 5){
                setupChgImportUsb.isAlreadyFtpAccess = false
////                externalMemoryViewModel.startFtpCommunication();
            }
            //externalMemoryViewModel.activate()
        } else {
            setupChgImportUsb.initScreenInfo()
            externalMemoryViewModel.deactivate()
            externalMemoryViewModel.stopModbusMonitoring()
        }
    }
    function initScreenInfo(){
        setupChgImportUsb.initSelectScreenStatus()
        setupChgImportUsb.page1 = 1
        setupChgImportUsb.page2 = 1
    }
    function initSelectScreenStatus(){
        setupChgImportUsb.listSelect = false
        setupChgImportUsb.rightListSelect = false
        setupChgImportUsb.listAreaPos = false
    }
    function clearLeftListSelect(){
        setupChgImportUsb.listSelect = false
        setupChgImportUsb.leftFolderSelect = false
    }
    function clearRightListSelect(){
        setupChgImportUsb.rightListSelect = false
    }

    function confirmpPopupProcess(){
        var msgStr
        let leftListIndex = 0
        let rightListIndex = -1
        var isDir
        if (setupChgImportUsb.listSelect === true){
            if (setupChgImportUsb.rightListSelect === true){
                if (setupChgImportUsb.listAreaPos === true){
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadConfirm).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryFileList[leftListIndex].fileName);
                } else {
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    rightListIndex = (setupChgImportUsb.page2 - 1) * 7 + exportMemRightList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadConfirm).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryChildDirFileList[rightListIndex].fileName);
                }
            } else {
                if (setupChgImportUsb.listAreaPos === true){
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    isDir = (leftListIndex >= 0) ? setupChgImportUsb.extMemoryFileList[leftListIndex].fileUrlDir : false
                    if (isDir === true){
                        msgStr = (setupChgImportUsb.usbReadFolderConfirm).toString()
                            .replace("[folder]", setupChgImportUsb.extMemoryFileList[leftListIndex].fileName);
                    } else {
                        msgStr = (setupChgImportUsb.usbReadConfirm).toString()
                            .replace("[file]", setupChgImportUsb.extMemoryFileList[leftListIndex].fileName);
                    }
                } else {
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    rightListIndex = (setupChgImportUsb.page2 - 1) * 7 + exportMemRightList.currentIndex
                    isDir = (leftListIndex >= 0) ? setupChgImportUsb.extMemoryFileList[leftListIndex].fileUrlDir : false
                    if (isDir === true){
                        msgStr = (setupChgImportUsb.usbReadFolderConfirm).toString()
                            .replace("[folder]", setupChgImportUsb.extMemoryFileList[leftListIndex].fileName);
                    } else {
                        if (rightListIndex >= 0){
                            msgStr = (setupChgImportUsb.usbReadConfirm).toString()
                                .replace("[file]", setupChgImportUsb.extMemoryFileList[rightListIndex].fileName);
                        } else {
                            msgStr = ""
                        }
                    }
                }
            }
            confirmPopup.text = msgStr
            confirmPopup.listIndex = leftListIndex
            confirmPopup.subListIndex = rightListIndex
            confirmPopup.open();
        }
    }

    function popupProcess(mode){
        switch(mode){
        case 0:
            setupChgImportUsb.confirmpPopupProcess()
            break;
        default:
            break;
        }
    }
    function judgeFilePermission(){
        let leftListIndex = 0
        let rightListIndex = -1
        if (setupChgImportUsb.listSelect === true){
            if (setupChgImportUsb.rightListSelect === true){
                if (setupChgImportUsb.listAreaPos === true){
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    return externalMemoryViewModel.checkReadOnlyFile(setupChgImportUsb.extMemoryFileList[leftListIndex].fileName)
                } else {
                    rightListIndex = (setupChgImportUsb.page2 - 1) * 7 + exportMemRightList.currentIndex
                    return externalMemoryViewModel.checkReadOnlyFile(setupChgImportUsb.extMemoryChildDirFileList[rightListIndex].fileName)
                }
            } else {
                if (setupChgImportUsb.listAreaPos === true){
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    return externalMemoryViewModel.checkReadOnlyFile(setupChgImportUsb.extMemoryFileList[leftListIndex].fileName)
                } else {
                    rightListIndex = (setupChgImportUsb.page2 - 1) * 7 + exportMemRightList.currentIndex
                    if (rightListIndex >= 0){
                        return externalMemoryViewModel.checkReadOnlyFile(setupChgImportUsb.extMemoryFileList[rightListIndex].fileName)
                    } else {
                        return false
                    }
                }
            }
        } else {
            return false
        }
    }

    Component{
        id:listdelegate1
        Rectangle{
            width: 310
            height: 50
            color:  (exportMemLeftList.currentIndex >= 0 && setupChgImportUsb.listSelect === true) ?
                    ((exportMemLeftList.currentIndex === index) ? "#cc3333" : (index%2 == 0) ? "#ffffff": "#cccccc") :
                     (index%2 == 0) ? "#ffffff": "#cccccc"
            border.color: "#898989"
            Image {
                id:leftKindIcon
                x:20
                anchors.verticalCenter: parent.verticalCenter
                source:(setupChgImportUsb.isAlreadyUsbFileList === false) ? "" :
                        (((setupChgImportUsb.page1 - 1) * 7 + index) >= setupChgImportUsb.listCnt) ? "":
                            (setupChgImportUsb.extMemoryFileList[(setupChgImportUsb.page1 - 1) * 7 + index].fileUrlDir === true) ?
                                resourceManager.imagePath + "/28_USB/a280_13.png" : resourceManager.imagePath + "/28_USB/a280_14.png"
            }
            Text{
                id:leftFileNameStr
                anchors.left:leftKindIcon.right
                anchors.leftMargin:(setupChgImportUsb.isAlreadyUsbFileList === false) ? 10 :
                        (((setupChgImportUsb.page1 - 1) * 7 + index) >= setupChgImportUsb.listCnt) ? 10:
                            (setupChgImportUsb.extMemoryFileList[(setupChgImportUsb.page1 - 1) * 7 + index].fileUrlDir === true) ? 10 : 8
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                text: (setupChgImportUsb.isAlreadyUsbFileList === false) ? "" :
                    (((setupChgImportUsb.page1 - 1) * 7 + index) >= setupChgImportUsb.listCnt) ? "":
                        setupChgImportUsb.extMemoryFileList[(setupChgImportUsb.page1 - 1) * 7 + index].fileName
                color:(setupChgImportUsb.listSelect === true) ?
                    ((index === exportMemLeftList.currentIndex) ? externalMemoryViewModel.getSelectedFileFolderListDispColor() :
                        externalMemoryViewModel.getFileFolderListDispColor()) : externalMemoryViewModel.getFileFolderListDispColor()
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    setupChgImportUsb.page2 = 1
                    //console.log("File_3 listdelegate1 listCnt:"+setupChgImportUsb.listCnt, "index:"+index, "page1:"+setupChgImportUsb.page1)
                    if(((setupChgImportUsb.page1 - 1) * 7 + index) >= setupChgImportUsb.listCnt){
                        //NOP
                    } else {
                        setupChgImportUsb.listSelect = true
                        exportMemLeftList.currentIndex = index
                        setupChgImportUsb.listAreaPos = true
                        console.debug("listdelegate1 listCnt:"+setupChgImportUsb.listCnt, "index:"+index, "fileUrlDir:"+setupChgImportUsb.extMemoryFileList[index].fileUrlDir, "page1:"+setupChgImportUsb.page1)
                        let targetIndex = (setupChgImportUsb.page1 - 1) * 7 + index;
                        if (setupChgImportUsb.listCnt > 0 && setupChgImportUsb.extMemoryFileList[targetIndex].fileUrlDir === true){
                            setupChgImportUsb.leftFolderSelect = true
                            externalMemoryViewModel.getChildDirFileList(1, setupChgImportUsb.hierarchyCnt, targetIndex);
                        } else if (setupChgImportUsb.listCnt > 0 && setupChgImportUsb.extMemoryFileList[targetIndex].fileUrlDir === false){
                            setupChgImportUsb.leftFolderSelect = false
                            externalMemoryViewModel.getChildDirFileList(1, -1, targetIndex);
                        }
                        setupChgImportUsb.rightListSelect = false
                        setupChgImportUsb.isReadOnly = setupChgImportUsb.judgeFilePermission()
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
            color:  (exportMemRightList.currentIndex >= 0 && setupChgImportUsb.rightListSelect === true) ?
                        ((exportMemRightList.currentIndex === index) ? "#cc3333" : (index%2 == 0) ? "#ffffff": "#cccccc") :
                        (index%2 == 0) ? "#ffffff": "#cccccc"
            border.color: "#898989"
            Image {
                x:20
                anchors.verticalCenter: parent.verticalCenter
                source: resourceManager.imagePath + "/28_USB/a280_14.png"
            }
            Image {
                id:rightKindIcon
                x:20
                anchors.verticalCenter: parent.verticalCenter
                source:(setupChgImportUsb.isAlreadyUsbChildFileList === false) ? "" :
                        (((setupChgImportUsb.page2 - 1)* 7 + index) >= setupChgImportUsb.childDirListCnt) ? "":
                            (setupChgImportUsb.extMemoryChildDirFileList[(setupChgImportUsb.page2 - 1) * 7 + index].fileUrlDir === true) ? resourceManager.imagePath + "/28_USB/a280_13.png" : resourceManager.imagePath + "/28_USB/a280_14.png"
            }
            Text{
                id:rightFileNameStr
                anchors.left:rightKindIcon.right
                anchors.leftMargin:10
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                text:(setupChgImportUsb.isAlreadyUsbChildFileList === false) ? "" :
                        (((setupChgImportUsb.page2 - 1) * 7 + index) >= setupChgImportUsb.childDirListCnt) ? "":
                            setupChgImportUsb.extMemoryChildDirFileList[(setupChgImportUsb.page2 - 1) * 7 + index].fileName
                color:(setupChgImportUsb.listSelect === true) ?
                    ((index === exportMemRightList.currentIndex) ? externalMemoryViewModel.getSelectedFileFolderListDispColor() :
                        externalMemoryViewModel.getFileFolderListDispColor()) : externalMemoryViewModel.getFileFolderListDispColor()
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.debug("listdelegate2 page2:"+setupChgImportUsb.page2, "index:"+index, "childDirListCnt:"+setupChgImportUsb.childDirListCnt)
                    if(((setupChgImportUsb.page2 - 1) * 7 + index) >= setupChgImportUsb.childDirListCnt){
                        //NOP
                    } else {
                        setupChgImportUsb.rightListSelect = true
                        exportMemRightList.currentIndex = index
                        setupChgImportUsb.listAreaPos = false
                        setupChgImportUsb.isReadOnly = setupChgImportUsb.judgeFilePermission()
                    }
                    phyKeyManager.ctrlBuzzer()
                }
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
            border.color: (setupChgImportUsb.listAreaSelect === false) ? "#999999" : (setupChgImportUsb.listAreaPos === true) ? "#999999" : "#999999"
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
                model: 7
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
                id:leftScrollBar
                x:335
                y:43
                height: 398
                jogScrollOn: setupChgImportUsb.jogScrollOn
                topButtonPressImage: resourceManager.imagePath + "/28_USB/a280_09" + ".png"
                topButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_07" + ".png"
                botomButtonPressImage: resourceManager.imagePath + "/28_USB/a280_10" + ".png"
                botomButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_08" + ".png"
                pageLabal:(setupChgImportUsb.isAlreadyUsbFileList === false) ? "" : setupChgImportUsb.page1 + "/" + setupChgImportUsb.leftTotalPage
                onScrollUpClicked: {
                    setupChgImportUsb.clearLeftListSelect()
                    setupChgImportUsb.clearRightListSelect()
                    if (setupChgImportUsb.listAreaSelect === true){
                        if (setupChgImportUsb.listAreaPos === true){
                            if (setupChgImportUsb.page1 > 1) {
                                setupChgImportUsb.page1--
                                setupChgImportUsb.listSelect = false
                                setupChgImportUsb.leftFolderSelect = false
                                externalMemoryViewModel.getChildDirFileList(1, -1, 0)
                                setupChgImportUsb.page2 = 1;
                                setupChgImportUsb.isReadOnly = setupChgImportUsb.judgeFilePermission()
                            } else {
                                setupChgImportUsb.page1 = setupChgImportUsb.leftTotalPage
                            }
                        }
                    } else {
                        if (setupChgImportUsb.page1 > 1) {
                            setupChgImportUsb.page1--
                            setupChgImportUsb.listSelect = false
                            setupChgImportUsb.leftFolderSelect = false
                            externalMemoryViewModel.getChildDirFileList(1, -1, 0)
                            setupChgImportUsb.page2 = 1;
                            setupChgImportUsb.isReadOnly = setupChgImportUsb.judgeFilePermission()
                        } else {
                            setupChgImportUsb.page1 = setupChgImportUsb.leftTotalPage
                        }
                    }
                }
                onScrollDownClicked: {
                    setupChgImportUsb.clearLeftListSelect()
                    setupChgImportUsb.clearRightListSelect()
                    if (setupChgImportUsb.listAreaSelect === true){
                        if (setupChgImportUsb.listAreaPos === true){
                            if (setupChgImportUsb.page1 >= setupChgImportUsb.leftTotalPage) {
                                setupChgImportUsb.page1 = 1
                            } else {
                                setupChgImportUsb.page1++
                                setupChgImportUsb.listSelect = false
                                setupChgImportUsb.leftFolderSelect = false
                                externalMemoryViewModel.getChildDirFileList(1, -1, 0)
                                setupChgImportUsb.page2 = 1;
                                setupChgImportUsb.isReadOnly = setupChgImportUsb.judgeFilePermission()
                            }
                        }
                    } else {
                        if (setupChgImportUsb.page1 >= setupChgImportUsb.leftTotalPage) {
                            setupChgImportUsb.page1 = 1
                        } else {
                            setupChgImportUsb.page1++
                            setupChgImportUsb.listSelect = false
                            setupChgImportUsb.leftFolderSelect = false
                            externalMemoryViewModel.getChildDirFileList(1, -1, 0)
                            setupChgImportUsb.page2 = 1;
                            setupChgImportUsb.isReadOnly = setupChgImportUsb.judgeFilePermission()
                        }
                    }
                }
            }

        }
        Rectangle{
            width: 400
            height: 450
            color: "transparent"
            border.color: (setupChgImportUsb.listAreaSelect === false) ? "#999999" : (setupChgImportUsb.listAreaPos === false) ? "#999999" : "#999999"
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
                id:exportMemRightList
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
                    positionViewAtIndex((pg-1)*7,ListView.Beginning)
                }
                Component.onCompleted: {
                    positionViewAtIndex((pg-1)*7,ListView.Beginning)
                }
            }
            //换页按钮
            ImageScrollBar{
                id:rightScrollBar
                x:335
                y:43
                height: 398
                jogScrollOn: setupChgImportUsb.jogScrollOn
                topButtonPressImage: resourceManager.imagePath + "/28_USB/a280_09" + ".png"
                topButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_07" + ".png"
                botomButtonPressImage: resourceManager.imagePath + "/28_USB/a280_10" + ".png"
                botomButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_08" + ".png"
                pageLabal:(setupChgImportUsb.isAlreadyUsbChildFileList === false) ? "" : setupChgImportUsb.page2 + "/" + setupChgImportUsb.rightTotalPage
                onScrollUpClicked: {
                    setupChgImportUsb.clearRightListSelect()
                    if (setupChgImportUsb.listAreaSelect === true){
                        if (setupChgImportUsb.listAreaPos === false
                            && setupChgImportUsb.leftFolderSelect === true){
                            if (setupChgImportUsb.page2 > 1) {
                                setupChgImportUsb.page2--
                            } else {
                                setupChgImportUsb.page2 = setupChgImportUsb.rightTotalPage
                            }
                        }
                    } else {
                        if (setupChgImportUsb.leftFolderSelect === true){
                            if (setupChgImportUsb.page2 > 1) {
                                setupChgImportUsb.page2--
                            } else {
                                setupChgImportUsb.page2 = setupChgImportUsb.rightTotalPage
                            }
                        }
                    }
                }
                onScrollDownClicked: {
                    setupChgImportUsb.clearRightListSelect()
                    if (setupChgImportUsb.listAreaSelect === true){
                        if (setupChgImportUsb.listAreaPos === false
                            && setupChgImportUsb.leftFolderSelect === true){
                            if (setupChgImportUsb.page2 >= setupChgImportUsb.rightTotalPage) {
                                setupChgImportUsb.page2 = 1
                            } else {
                                setupChgImportUsb.page2++
                            }
                        }
                    } else {
                        if (setupChgImportUsb.leftFolderSelect === true){
                            if (setupChgImportUsb.page2 >= setupChgImportUsb.rightTotalPage) {
                                setupChgImportUsb.page2 = 1
                            } else {
                                setupChgImportUsb.page2++
                            }
                        }
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
                id:btnFromUsb
                visible:(!setupChgImportUsb.isReadOnly)
                anchors.horizontalCenter: parent.horizontalCenter
                y:40
                upImageSource: resourceManager.imagePath + "/28_USB/a280_11.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_12.png"
                fontPixelSize:18
                text:resourceManager.label(51,34, resourceManager.language)
                textColor: "#666666"
                enableBtnTextColorChg:true
                btnPressColor:externalMemoryViewModel.getCommandPressColor()
                onClicked:{
                    setupChgImportUsb.popupProcess(0)
                    phyKeyManager.ctrlBuzzer()
                }
            }
            Rectangle{
                id:invalidBtnFromUsb
                visible:(setupChgImportUsb.isReadOnly)
                width: 100
                height: 52
                color: "#cccccc"
                border.color: "#999999"
                border.width: 2
                x:37
                y:40
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    text: resourceManager.label(51,34, resourceManager.language)
                    color: resourceManager.rgb(51,34)
                }
            }
        }
    }

    FileConfirmPopup{
        id:confirmPopup
        viewModel:externalMemoryViewModel
        tabIndex:2
        onProcessingDialogOpen:{
            if (setupChgImportUsb.pageActive === true){
                var msgStr
                let leftListIndex = 0
                let rightListIndex = 0
                if (setupChgImportUsb.listAreaPos === true){
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadProcessingMsg).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryFileList[leftListIndex].fileName);
                } else {
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    rightListIndex = (setupChgImportUsb.page2 - 1) * 7 + exportMemRightList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadProcessingMsg).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryChildDirFileList[rightListIndex].fileName);
                }
                processingPopup.text = msgStr
                processingPopup.isBtnVisible = false
                processingPopup.buttonIndex = btnIndex
                processingPopup.open()
            }
        }
        onDialogClose:{
        }
        onCallAttentionDialogOpen:{
            if (setupChgImportUsb.pageActive === true){
                console.debug("onCallAttentionDialogOpen btnIndex:"+btnIndex)
                var msgStr = setupChgImportUsb.callAttentionPowerRestartMsg.toString()
                processingPopup.text = msgStr
                processingPopup.isBtnVisible = false
                processingPopup.buttonIndex = btnIndex
                processingPopup.open()
            }
        }
    }//点击导入确认弹窗
    FileConfirmPopup{
        id:processingPopup
        viewModel:externalMemoryViewModel
        tabIndex:2
        onEnactSuccessDialogOpen:{
            if (setupChgImportUsb.pageActive === true){
                var msgStr
                let leftListIndex = 0
                let rightListIndex = 0
                if (setupChgImportUsb.listAreaPos === true){
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadProcessSuccessMsg).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryFileList[leftListIndex].fileName);
                } else {
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    rightListIndex = (setupChgImportUsb.page2 - 1) * 7 + exportMemRightList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadProcessSuccessMsg).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryChildDirFileList[rightListIndex].fileName);
                }
                messagePopup.openProcessFinishPopup(msgStr, false, btnIndex)
            }
        }
        onEnactFailDialogOpen:{
            if (setupChgImportUsb.pageActive === true){
                var msgStr
                let leftListIndex = 0
                let rightListIndex = 0
                if (setupChgImportUsb.listAreaPos === true){
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadProcessFailMsg).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryFileList[leftListIndex].fileName);
                } else {
                    leftListIndex = (setupChgImportUsb.page1 - 1) * 7 + exportMemLeftList.currentIndex
                    rightListIndex = (setupChgImportUsb.page2 - 1) * 7 + exportMemRightList.currentIndex
                    msgStr = (setupChgImportUsb.usbReadProcessFailMsg).toString()
                        .replace("[file]", setupChgImportUsb.extMemoryChildDirFileList[rightListIndex].fileName);
                }
                messagePopup.openProcessFinishPopup(msgStr, false, btnIndex)
            }
        }
        onDialogClose:{
        }
    }//进程信息提示弹窗

    Item {
        id: messagePopup
        anchors.centerIn: parent
        function _openPopup(stateName, msgStr, btnVisible = true, btnIndex = -1) {
            close()
            state = stateName
            _inner_confirmPopup.text = msgStr
            _inner_confirmPopup.isBtnVisible = btnVisible
            _inner_confirmPopup.buttonIndex = btnIndex
            _inner_confirmPopup.open()
        }
        function openProcessFinishPopup(msgStr, btnVisible, btnIndex) {
            _openPopup("processFinishPopup", msgStr, btnVisible, btnIndex)
        }
        function openUsbStatusPopup(msgStr, btnVisible) {
            _openPopup("usbStatusPopup", msgStr, btnVisible)
        }
        function openCompliePopup(msgStr) {
            _openPopup("compliePopup", msgStr, false)
        }
        function close() {
            if (_inner_confirmPopup.opened) {
                // _inner_confirmPopup.dialogClose()    // todo: need to call ?
                _inner_confirmPopup.close()
            }
        }
        FileConfirmPopup {
            id: _inner_confirmPopup
            tabIndex: 2
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
                        // console.log("File_3 processFinishPopup onDialogClose()")     // for debug.
                        //setupChgImportUsb.listSelect = false
                    }
                }
            },
            State {
                name: "usbStatusPopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose:{
                        console.debug("File_3 usbStatusPopup onDialogClose()")         // for debug.
                        if (setupChgImportUsb.usbStautsPopupDisp === true){
                            setupChgImportUsb.usbStautsPopupDisp = false
                        }
                    }
                }
            },
            State {
                name: "compliePopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose:{
                        // console.log("File_3 compliePopup onDialogClose()")         // for debug.
                    }
                }
            }
        ]
    }

/***********************************************************************/
    Connections{
        target: externalMemoryViewModel
        onIsUSBConnectedChanged:{
            console.debug("File_3 setupChgImportUsb pageActive:"+setupChgImportUsb.pageActive,
                "isUSBConnected:"+isUSBConnected, "usbStautsPopupDisp:"+setupChgImportUsb.usbStautsPopupDisp, "isAlreadyUsbFileList:"+setupChgImportUsb.isAlreadyUsbFileList)
            if (setupChgImportUsb.pageActive === true){
                if (isUSBConnected === false){
                    if (setupChgImportUsb.usbStautsPopupDisp === false){
                        var msgStr = setupChgImportUsb.usbStatusMsg
                        messagePopup.openUsbStatusPopup(msgStr, false)
                        setupChgImportUsb.usbStautsPopupDisp = true
                    }
                    externalMemoryViewModel.getUsbDirFileList()
                    externalMemoryViewModel.getUsbDirFileListCount()
                    externalMemoryViewModel.clearUsbChildDirFileList(0)
                    setupChgImportUsb.clearLeftListSelect()
                    setupChgImportUsb.clearRightListSelect()
                    externalMemoryViewModel.firstUsbConnected = false
                } else {
                    if (setupChgImportUsb.usbStautsPopupDisp === true){
                        messagePopup.close()
                        setupChgImportUsb.usbStautsPopupDisp = false
                    }
                    if (setupChgImportUsb.isAlreadyUsbFileList === false){
                        externalMemoryViewModel.getUsbDirFileList()
                        externalMemoryViewModel.getUsbDirFileListCount()
                    }
                }
            }
        }
        onComplieStateChanged:{
            let msgStr = ""
            switch(externalMemoryViewModel.complieState){
            case 0:
                msgStr = resourceManager.label(51,56,resourceManager.language)
                messagePopup.openCompliePopup(msgStr)
                break;
            case 1:
                msgStr = resourceManager.label(51,54,resourceManager.language)
                messagePopup.openCompliePopup(msgStr)
                break;
            case 2:
                msgStr = resourceManager.label(51,55,resourceManager.language)
                messagePopup.openCompliePopup(msgStr)
                break;
            }
        }
        onIsRequestFtpFileInfoChanged:{
            //console.log("File_3 onIsRequestFtpFileInfoChanged pageActive:"+setupChgImportUsb.pageActive, "isRequestFtpFileInfo:"+externalMemoryViewModel.isRequestFtpFileInfo)
            if(setupChgImportUsb.pageActive === true && externalMemoryViewModel.isRequestFtpFileInfo === true){
                setupChangeViewModel.dandoriListUpdate = 3
            }
        }
    }
    Connections {
        target: phyKeyManager
        enabled: setupChgImportUsb.pageActive
        onOperableSwStatusChanged: {
            console.debug("File_3 onOperableSwStatusChanged:"+operableSwStatus)
            console.debug("File_3 onOperableSwStatusChanged:"+operableSwStatus, "pageActive:"+setupChgImportUsb.pageActive, "jogScrollOn:"+setupChgImportUsb.jogScrollOn)
            if (setupChgImportUsb.pageActive === true){
                if (setupChgImportUsb.jogDialFirstClick === true){
                    setupChgImportUsb.listAreaSelect = operableSwStatus;
                } else {
                    setupChgImportUsb.listAreaSelect = operableSwStatus;
                }
            }
        }
        onJogDialSwitchValidChanged: {
            console.debug("File_3 onJogDialSwitchValidChanged:"+jogDialSwitchValid)
            console.debug("File_3 onJogDialSwitchValidChanged:"+jogDialSwitchValid, "pageActive:"+setupChgImportUsb.pageActive, "jogScrollOn:"+setupChgImportUsb.jogScrollOn)
            if (setupChgImportUsb.pageActive === true){
                if (setupChgImportUsb.listAreaSelect === false){
                    setupChgImportUsb.listAreaSelect = true;
                    setupChgImportUsb.jogDialFirstClick = true
                }
                setupChgImportUsb.listAreaPos = !setupChgImportUsb.listAreaPos
            }
        }
    }
    Connections{
        target: ftpManager
        onFtpStatusChanged:{
            console.debug("File_3 ftpManager onFtpStatusChanged:"+ftpStatus, "pageActive:"+setupChgImportUsb.pageActive)
            if (setupChgImportUsb.pageActive === true && setupChgImportUsb.isAlreadyFtpAccess === false){
                if(REAL_TARGET){
                    switch(ftpStatus){
                    case 0:
////                        externalMemoryViewModel.startFtpCommunication();
                        break;
                    case 4:
                        setupChgImportUsb.isAlreadyFtpAccess = true
                        break;
                    default:
                        break;
                    }
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
