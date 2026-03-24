import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: File_Item.qml
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
    id:file_item
    width: 1018
    height: 590
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "#666666"
        border.width: 10
        z: 9999          // 항상 최상단 테두리
        radius: 0        // 필요하면 둥글게
    }
    property bool pageActive: false
    readonly property bool jogDialSwOpe: (commonViewModel.topPopupDialog === Enums.PopupDialogStatusFile)
    property var viewModel: setupChangeViewModel
    property var tabUsbExportDispSetting:[]
    property var tabUsbImportDispSetting:[]
    property int ftpStatus:ftpManager.ftpStatus
    property bool isAlreadyTabDispSetting:false
    property int preMainPage:0
    property int preSubPage:0
    property bool isManual: commonViewModel.manipulateMode === 1
    property string commentText: ""

    signal itemClose()
    signal openVtk()

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdSetupChgExtMem
    }

    function openSaveDialog(){
        setupFileMng.commentText = file_item.commentText
        setupFileMng.openSaveDialog();
    }

    function changePage(page) {
        switch(page) {
        case 0:
            commonViewModel.pageChange(Enums.SetupChgFileMng)
            break
        case 1:
            setupUsbExport.active = true
            setupUsbImport.active = false
            externalMemoryViewModel.curTabIndex = 1;
            commonViewModel.pageChange(Enums.SetupChgExportUsb)
            break
        case 2:
            setupUsbImport.active = true
            setupUsbExport.active = false
            externalMemoryViewModel.curTabIndex = 2;
            commonViewModel.pageChange(Enums.SetupChgImportUsb)
            break
        default:
            // NOP
            break
        }
    }

    // アクティブな画面以外のマウスイベントを拾わないようにするため
    MouseArea{
        anchors.fill: parent
    }
// 파일관리부분 타이틀바
    Rectangle{
        width: parent.width
        height: parent.height
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 1018
            height: 65
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Row{
                anchors.centerIn: parent
                spacing: 10
                Image {
                    x:200
                    y:2
                    source: resourceManager.imagePath + "/28_USB/d280_01.png"
                }
                Text{
                    font.pixelSize: 26
                    color: resourceManager.rgb(51,0)
                    text: resourceManager.label(51,0,resourceManager.language)
                }
            }
            ImageButton{
                x:950
                scale: 1.5
                anchors.verticalCenter: parent.verticalCenter
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    commonViewModel.backPrePage(file_item.preMainPage, file_item.preSubPage)
                    setupFileMng.listSelect = false
                    file_item.pageActive = false
                    setupChangeViewModel.isSetupProcActive = false
                    file_item.itemClose()
                }
            }//关闭弹窗
        }//标题栏
    }


    //画面切换
    TabBar{
        id:file_tab
        width: 520
        height: 51
        x:16
        y:72
        spacing: 20
        background: Rectangle{
            color: "transparent"
        }
        TabButton{
            id:setupChgTabBtn
            width: visible ? implicitWidth : -file_tab.spacing
            background: Image {
                source:(file_tab.currentIndex === 0) ? resourceManager.imagePath + "/28_USB/a280_05.png" : resourceManager.imagePath + "/28_USB/a280_06.png"
            }
            Text {
                id:setupChgTab
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                font.pixelSize: 20
                color: (file_tab.currentIndex === 0) ? "white" : "white"
                text: resourceManager.label(51,1,resourceManager.language)
            }
            onClicked: {
                file_tab.currentIndex = 0
                file_item.changePage(0)
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id:setupChgUsbExportTabBtn
            width: visible ? implicitWidth : -file_tab.spacing
            background: Image {
                source:(file_tab.currentIndex == 1) ? resourceManager.imagePath + "/28_USB/a280_05.png" : resourceManager.imagePath + "/28_USB/a280_06.png"
            }
            Text {
                id:setupChgExportUsbTab
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 0.8
                font.pixelSize: 20
                color: (file_tab.currentIndex === 1) ? "white" : "white"
                text: resourceManager.label(51,2,resourceManager.language)
            }
            onClicked: {
                file_tab.currentIndex = 1
                file_item.changePage(1)
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id:setupChgUsbImportTabBtn
            width: visible ? implicitWidth : -file_tab.spacing
            background: Image {
                source:(file_tab.currentIndex == 2) ? resourceManager.imagePath + "/28_USB/a280_05.png" : resourceManager.imagePath + "/28_USB/a280_06.png"
            }
            Text {
                id:setupChgImportUsbTab
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 0.8
                font.pixelSize: 20
                color: (file_tab.currentIndex === 2) ? "white" : "white"
                text: resourceManager.label(51,3,resourceManager.language)
            }
            onClicked: {
                file_tab.currentIndex = 2
                file_item.changePage(2)
                phyKeyManager.ctrlBuzzer()
            }
        }
    }

    StackLayout{
        id:stack_file
        width: 1005
        height: 460
        anchors.horizontalCenter: parent.horizontalCenter
        y:125
        currentIndex: file_tab.currentIndex
        File_1{
            id:setupFileMng
            pageActive: file_item.pageActive && (stack_file.currentIndex == 0)
            jogScrollOn: file_item.pageActive && (stack_file.currentIndex == 0)
            onOpenVirtualKeyboard: {
                file_item.openVtk()
            }
        }
        Loader {
            id: setupUsbExport
            source: "qrc:/qml/Components/File_2.qml";
            active: false
            property bool isActive: (file_item.pageActive && stack_file.currentIndex == 1)
            onLoaded: {
                item.pageActive = isActive;
                item.jogScrollOn = isActive;
            }
            onIsActiveChanged: {
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                    item.jogScrollOn = isActive;
                }
            }
        }
        Loader {
            id: setupUsbImport
            source: "qrc:/qml/Components/File_3.qml";
            active: false
            property bool isActive: (file_item.pageActive && stack_file.currentIndex == 2)
            onLoaded: {
                item.pageActive = isActive;
                item.jogScrollOn = isActive;
            }
            onIsActiveChanged: {
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                    item.jogScrollOn = isActive;
                }
            }
        }
    }
/***********************************************************************/
//    Connections{
//        target:subMainView.item
//        onOnBottomClick: {
//            console.debug("Connections subMainView onOnBottomClick:"+file_item.pageActive)
//            if (file_item.pageActive === true){
//                commonViewModel.backPrePage(file_item.preMainPage, file_item.preSubPage)
//                setupFileMng.listSelect = false
//                file_item.pageActive = false
//                file_item.itemClose()
//            }
//        }
//    }

//    Connections{
//        target:phyKeyManager
//        enabled: pageActive && file_item.jogDialSwOpe
//        onJogDialSwitchInvalidChanged:{
//            var pageIndex = file_tab.currentIndex
//            switch (file_tab.currentIndex) {
//            case 0:
//                pageIndex = setupChgUsbExportTabBtn.visible ? 1 :
//                                                              setupChgUsbImportTabBtn.visible ? 2 : pageIndex
//                break;
//            case 1:
//                pageIndex = setupChgUsbImportTabBtn.visible ? 2 :
//                                                              setupChgTabBtn.visible          ? 0 : pageIndex
//                break;
//            case 2:
//                pageIndex = setupChgTabBtn.visible          ? 0 :
//                                                              setupChgUsbExportTabBtn.visible ? 1 : pageIndex
//                break;
//            }
//            file_tab.currentIndex = pageIndex
//            file_item.changePage(pageIndex)
//        }
//    }
    Connections {
        target:commonViewModel
        onUserLevelChanged:{
            //console.log("onUserLevelChanged userlevel:"+userLevel)
            // note: The visibility settings for the setup change screen are not used.
            // However, we will leave the code as it may be used in the future.
            // file_dialog.setTabDispSettings()
        }
    }
}
