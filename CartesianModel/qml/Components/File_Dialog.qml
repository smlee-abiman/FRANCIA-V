import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: File_Dialog.qml
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
Dialog {
    id:file_dialog
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

    width: 1017
    height: 591
    closePolicy: Popup.CloseOnEscape

    signal dialogClose()
    signal openVtk()

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdSetupChgExtMem
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
    function getTabDispSettings(){
        // note: The visibility settings for the setup change screen are not used.
        // However, we will leave the code as it may be used in the future.
        // for(let i = 0; i < 5; i++){
        //     file_dialog.tabUsbExportDispSetting[i] = viewModel.getTabDispSetting(0, i)
        //     file_dialog.tabUsbImportDispSetting[i] = viewModel.getTabDispSetting(1, i)
        // }
    }

    function setTabDispSettings(){
        // note: The visibility settings for the setup change screen are not used.
        // However, we will leave the code as it may be used in the future.
        // setupChgUsbExportTabBtn.visible = (file_dialog.tabUsbExportDispSetting[commonViewModel.userLevel] === 0) ? false : true
        // setupChgUsbImportTabBtn.visible = (file_dialog.tabUsbImportDispSetting[commonViewModel.userLevel] === 0) ? false : true
    }

    function openSaveDialog(){
        setupFileMng.commentText = file_dialog.commentText
        setupFileMng.openSaveDialog();
    }

    onPageActiveChanged: {
        // console.log("File_Dialog onPageActiveChanged pageActive:"+pageActive)
        if (pageActive) {
            //file_tab.currentIndex = 0
            //file_dialog.changePage(0)

            // note: The visibility settings for the setup change screen are not used.
            // However, we will leave the code as it may be used in the future.
            // if (file_dialog.isAlreadyTabDispSetting === false){
            //     file_dialog.getTabDispSettings()
            //     file_dialog.isAlreadyTabDispSetting = true
            // }
            // file_dialog.setTabDispSettings()

            // update tab visiblity.
            let visible1 = local.userSettingView.getScreenSettings(local.dp1, 0).visibles[local.userLevel]
            let visible2 = local.userSettingView.getScreenSettings(local.dp1, 1).visibles[local.userLevel] && isManual
            let visible3 = local.userSettingView.getScreenSettings(local.dp1, 2).visibles[local.userLevel] && isManual
            base.visible = visible1 | visible2 | visible3
            setupChgTabBtn.visible          = visible1
            setupChgUsbExportTabBtn.visible = visible2
            setupChgUsbImportTabBtn.visible = visible3
            // update current tab index.
            let tabIndex = visible1 ? 0 :
                           visible2 ? 1 :
                           visible3 ? 2 : -1
            file_tab.currentIndex = tabIndex
            file_dialog.changePage(file_tab.currentIndex)
        } else {
            // hide tab.
            setupChgTabBtn.visible          = false
            setupChgUsbExportTabBtn.visible = false
            setupChgUsbImportTabBtn.visible = false
            file_tab.currentIndex = -1
            viewModel.deactivate()
            externalMemoryViewModel.deactivate()
        }
    }


    Image {
        x:440
        y:5
        source: resourceManager.imagePath + "/28_USB/d280_01.png"
    }
    Text{
        x:490
        y:2
        font.pixelSize: 28
        color: resourceManager.rgb(51,0)
        text: resourceManager.label(51,0,resourceManager.language)
    }
    ImageButton{
        x:935
        y:3
        scale: 1.5
        upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        onClicked: {
            commonViewModel.backPrePage(file_dialog.preMainPage, file_dialog.preSubPage)
            setupFileMng.listSelect = false
            file_dialog.pageActive = false
            file_dialog.dialogClose()
            file_dialog.close()
        }
    }

    //画面切换
    TabBar{
        id:file_tab
        width: 500
        height: 55
        x:-10
        y:58
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
                anchors.centerIn: parent
                font.pixelSize: 24
                color: (file_tab.currentIndex === 0) ? viewModel.getSelectedTagColor() : resourceManager.rgb(51,1)
                text: resourceManager.label(51,1,resourceManager.language)
            }
            onClicked: {
                file_tab.currentIndex = 0
                file_dialog.changePage(0)
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
                anchors.centerIn: parent
                font.pixelSize: 24
                color: (file_tab.currentIndex === 1) ? viewModel.getSelectedTagColor() : resourceManager.rgb(51,2)
                text: resourceManager.label(51,2,resourceManager.language)
            }
            onClicked: {
                file_tab.currentIndex = 1
                file_dialog.changePage(1)
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
                anchors.centerIn: parent
                font.pixelSize: 24
                color: (file_tab.currentIndex === 2) ? viewModel.getSelectedTagColor() : resourceManager.rgb(51,3)
                text: resourceManager.label(51,3,resourceManager.language)
            }
            onClicked: {
                file_tab.currentIndex = 2
                file_dialog.changePage(2)
            }
        }
    }

    Image {
        id: base
        x:-12
        y:55
        StackLayout{
            id:stack_file
            width: parent.width
            height: parent.height
            currentIndex: file_tab.currentIndex
            File_1{
                id:setupFileMng
                pageActive: file_dialog.pageActive && (stack_file.currentIndex == 0)
                jogScrollOn: file_dialog.pageActive && (stack_file.currentIndex == 0)
                onOpenVirtualKeyboard: {
                    file_dialog.openVtk()
                }
            }
            Loader {
                id: setupUsbExport
                source: "qrc:/qml/Components/File_2.qml";
                active: false
                property bool isActive: (file_dialog.pageActive && stack_file.currentIndex == 1)
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
                property bool isActive: (file_dialog.pageActive && stack_file.currentIndex == 2)
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
    }

    Connections
    {
        target:subMainView.item
        onOnBottomClick: {
            //console.log("Connections subMainView onOnBottomClick:"+file_dialog.pageActive)
            if (file_dialog.pageActive === true){
                commonViewModel.backPrePage(file_dialog.preMainPage, file_dialog.preSubPage)
                setupFileMng.listSelect = false
                file_dialog.pageActive = false
                file_dialog.dialogClose()
                file_dialog.close()
            }
        }
    }

    Connections
    {
        target:phyKeyManager
        enabled: pageActive && file_dialog.jogDialSwOpe
        onJogDialSwitchInvalidChanged:{
            var pageIndex = file_tab.currentIndex
            switch (file_tab.currentIndex) {
            case 0:
                pageIndex = setupChgUsbExportTabBtn.visible ? 1 :
                                                              setupChgUsbImportTabBtn.visible ? 2 : pageIndex
                break;
            case 1:
                pageIndex = setupChgUsbImportTabBtn.visible ? 2 :
                                                              setupChgTabBtn.visible          ? 0 : pageIndex
                break;
            case 2:
                pageIndex = setupChgTabBtn.visible          ? 0 :
                                                              setupChgUsbExportTabBtn.visible ? 1 : pageIndex
                break;
            }
            file_tab.currentIndex = pageIndex
            file_dialog.changePage(pageIndex)
        }
    }
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
