import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import TeachingViewModel 1.0
import Enums 1.0            // Enums.
import "PosSet"
import "Stack"
import "../Widget"
import "Program"
import ".."
/*************************************************************************************
* Title: UI/UX of AEK
* File: ProgramPage.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.21
* Worker: SH.HONG
**************************************************************************************/
/**********自动画面中程序窗口**********/
Item {
    id: program_page
    width: 1018
    height: 590
    property bool pageActive:false
    property var viewModel: teachingViewModel
    property int usr_select: commonViewModel.userLevel
    property bool preLoadEnable: false

    QtObject {
        id: local
        property var stackPageModel: teachingViewModel.pageModel[2]
        property bool editFromProgEditScreen: stackPageModel.editFromProgEditScreen
        property var pageTeachnigModeOpeAuth:[]
        property var pageTeachnigPosSetOpeAuth:[]
        property var pageTeachnigPalletizeOpeAuth:[]
        property var pageTeachnigTimerOpeAuth:[]
        property var pageTeachnigCounterOpeAuth:[]
        property var pageTeachnigPassOpeAuth:[]
        property var pageTeachnigProgramOpeAuth:[]

        property int numOfVisibleTabs: 0
        property int prevPageTab: 0
        readonly property int emptyTab: pro_stack.count - 1
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        function updateTabButtonVisiblity(){
            let visible1 = local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingMode,      0).visibles[local.userLevel]
            let visible2 = local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingPosSet,    0).visibles[local.userLevel] ||
                           local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingPosSet,    1).visibles[local.userLevel]
            let visible3 = local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingPalletize, 0).visibles[local.userLevel] ||
                           local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingPalletize, 1).visibles[local.userLevel]
            let visible4 = local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingTimer,     0).visibles[local.userLevel]
            let visible5 = local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingCounter,   0).visibles[local.userLevel]
            let visible6 = local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingPass,      0).visibles[local.userLevel]
            let visible7 = local.userSettingView.getScreenSettings(Enums.MainScreenIdTeachingProgram,   0).visibles[local.userLevel]

            // update tab image visibility.
            modeTabImage.visible      = visible1
            posSetTabImage.visible    = visible2
            palletizeTabImage.visible = visible3
            timerTabImage.visible     = visible4
            counterTabImage.visible   = visible5
            passTabImage.visible      = visible6
            programTabImage.visible   = visible7

            // count visible tabs.
            local.numOfVisibleTabs = local.updateTabButtonVisibleIndex()
        }
        function updateTabButtonVisibleIndex(){
            let numOfVisibleTabs = 0;
            if (modeTabImage.visible)      { numOfVisibleTabs++ }
            if (posSetTabImage.visible)    { numOfVisibleTabs++ }
            if (palletizeTabImage.visible) { numOfVisibleTabs++ }
            if (timerTabImage.visible)     { numOfVisibleTabs++ }
            if (counterTabImage.visible)   { numOfVisibleTabs++ }
            if (passTabImage.visible)      { numOfVisibleTabs++ }
            if (programTabImage.visible)   { numOfVisibleTabs++ }
            return numOfVisibleTabs
        }
        function updateCurrrentTabInVisibleTabs(){
            viewModel.currentPageTab =
                    local.editFromProgEditScreen ? palletizeTabImage.index :
                    modeTabImage.visible      ? modeTabImage.index      :
                    posSetTabImage.visible    ? posSetTabImage.index    :
                    palletizeTabImage.visible ? palletizeTabImage.index :
                    timerTabImage.visible     ? timerTabImage.index     :
                    counterTabImage.visible   ? counterTabImage.index   :
                    passTabImage.visible      ? passTabImage.index      :
                    programTabImage.visible   ? programTabImage.index
                                              : local.emptyTab
            local.prevPageTab = viewModel.currentPageTab
        }
    }
    QtObject {
        id: penSet
        readonly property string groupNo: "9"
        readonly property string selectedTabColor: "black"
        // readonly property string selectedTabColor: resourceManager.dataColor(groupNo, 1)
    }

    Component.onCompleted: {
        for(let i = 0; i < 5; i++){
            local.pageTeachnigModeOpeAuth[i] = commonViewModel.getOperationAuthority(10, i)
            local.pageTeachnigPosSetOpeAuth[i] = commonViewModel.getOperationAuthority(11, i)
            local.pageTeachnigPalletizeOpeAuth[i] = commonViewModel.getOperationAuthority(12, i)
            local.pageTeachnigTimerOpeAuth[i] = commonViewModel.getOperationAuthority(13, i)
            local.pageTeachnigCounterOpeAuth[i] = commonViewModel.getOperationAuthority(14, i)
            local.pageTeachnigPassOpeAuth[i] = commonViewModel.getOperationAuthority(15, i)
            local.pageTeachnigProgramOpeAuth[i] = commonViewModel.getOperationAuthority(16, i)
        }
    }
    onPageActiveChanged: {
        if (pageActive) {
            local.updateTabButtonVisiblity()
            local.updateCurrrentTabInVisibleTabs()
            componentLoader.loadNextComponent()
        } else {
            viewModel.currentPageTab = local.emptyTab
            local.prevPageTab = local.emptyTab
        }
    }

    ComponentLoader {
        id: componentLoader
        guard: programLoader.isActive || (!program_page.preLoadEnable && !program_page.pageActive)
        onGuardChanged: {
            if (!guard) {
                loadNextComponent()
            }
        }
        loadComponentList: [
            programLoader,
            posSetPageLoader,
            stackPageLoader,
            timerPageLoader,
            counterPageLoader,
            passPageLoader
        ]
    }
//todo:전체크기조정, 배경 색깔
    Rectangle{
        width: 1024
        height: 768
        anchors.fill: parent.Center
        //radius: 6
        color: "#EAEAEA"
        //센터 내용표시 배경
        Rectangle{
            width:970
            height:600
            x: 30
            y: 89
            anchors.fill: parent.Center
            color: "#FFFFFF"
            border.color: "#cccccc"
        }
    }//배경

    Row{
        //탭 부분을 처음시작하는 위치 좌표
        x: 30
        y: 27
        spacing: 4
        visible:viewModel.currentPageTab === 6 ? false : true
        Image {
            id: modeTabImage
            property int index: 0
            property bool selected: viewModel.currentPageTab === index
            source: (modeTabImage.selected || mouse_1.pressed) ? resourceManager.imagePath + "/16_program/a160_01.png" : resourceManager.imagePath + "/16_program/a160_02.png"
//            Image{
//                x:12
//                anchors.verticalCenter: parent.verticalCenter
//                source: (modeTabImage.selected || mouse_1.pressed) ? viewModel.pageModel[parent.index].activateIconPath : viewModel.pageModel[parent.index].deactivateIconPath
//            }
            Text{
                anchors.fill: parent
                width: 125
                height: 84
                x:46
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: mouse_1.pressed ? 25 : 24
                font.bold : true
                text: viewModel.pageModel[modeTabImage.index].pageLabel.name
                color: modeTabImage.selected ? "white" : "#666666"
            }
            MouseArea{
                id:mouse_1
                anchors.fill: parent
                enabled: !local.editFromProgEditScreen
                onClicked: {
                    if(local.pageTeachnigModeOpeAuth[commonViewModel.userLevel] !== 0){
                        local.prevPageTab = viewModel.currentPageTab
                        viewModel.currentPageTab = parent.index
                        resourceManager.titleString = resourceManager.label(20,31,resourceManager.language)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//Mode
        Image {
            id: posSetTabImage
            property int index: 1
            property bool selected: viewModel.currentPageTab === index
            source: (posSetTabImage.selected || mouse_2.pressed) ? resourceManager.imagePath + "/16_program/a160_01.png" : resourceManager.imagePath + "/16_program/a160_02.png"
//            Image{
//                x:12
//                anchors.verticalCenter: parent.verticalCenter
//                source: (posSetTabImage.selected || mouse_2.pressed) ? viewModel.pageModel[parent.index].activateIconPath : viewModel.pageModel[parent.index].deactivateIconPath
//            }
            Text{
                width: 125
                height: 84
                x:460
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: mouse_2.pressed ? 25 : 24
                font.bold : true
                text: viewModel.pageModel[posSetTabImage.index].pageLabel.name
                color: posSetTabImage.selected ? "white" : "#666666"
            }
            MouseArea{
                id:mouse_2
                anchors.fill: parent
                enabled: !local.editFromProgEditScreen
                onClicked: {
                    if(local.pageTeachnigPosSetOpeAuth[commonViewModel.userLevel] !== 0){
                        local.prevPageTab = viewModel.currentPageTab
                        viewModel.currentPageTab = parent.index
                        resourceManager.titleString = resourceManager.label(20,32,resourceManager.language)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//Position
        Image {
            id: palletizeTabImage
            property int index: 2
            property bool selected: viewModel.currentPageTab === index
            source: (palletizeTabImage.selected || mouse_3.pressed) ? resourceManager.imagePath + "/16_program/a160_01.png" : resourceManager.imagePath + "/16_program/a160_02.png"
//            Image{
//                x:12
//                anchors.verticalCenter: parent.verticalCenter
//                source: (palletizeTabImage.selected || mouse_3.pressed) ? viewModel.pageModel[parent.index].activateIconPath : viewModel.pageModel[parent.index].deactivateIconPath
//            }
            Text{
                width: 125
                height: 84
                x:46
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: mouse_3.pressed ? 25 : 24
                font.bold : true
                text: viewModel.pageModel[palletizeTabImage.index].pageLabel.name
                color: palletizeTabImage.selected ? "white" : "#666666"
            }
            MouseArea{
                id:mouse_3
                anchors.fill: parent
                enabled: !local.editFromProgEditScreen
                onClicked: {
                    if(local.pageTeachnigPalletizeOpeAuth[commonViewModel.userLevel] !== 0){
                        local.prevPageTab = viewModel.currentPageTab
                        viewModel.currentPageTab = parent.index
                        resourceManager.titleString = resourceManager.label(20,33,resourceManager.language)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//stack
        Image {
            id: timerTabImage
            property int index: 3
            property bool selected: viewModel.currentPageTab === index
            source: (timerTabImage.selected || mouse_4.pressed) ? resourceManager.imagePath + "/16_program/a160_01.png" : resourceManager.imagePath + "/16_program/a160_02.png"
//            Image{
//                x:12
//                anchors.verticalCenter: parent.verticalCenter
//                source: (timerTabImage.selected || mouse_4.pressed) ? viewModel.pageModel[parent.index].activateIconPath : viewModel.pageModel[parent.index].deactivateIconPath
//            }
            Text{
                width: 125
                height: 84
                x:46
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: mouse_4.pressed ? 25 : 24
                font.bold : true
                text: viewModel.pageModel[timerTabImage.index].pageLabel.name
                color: timerTabImage.selected ? "white" : "#666666"
            }
            MouseArea{
                id:mouse_4
                anchors.fill: parent
                enabled: !local.editFromProgEditScreen
                onClicked: {
                    if(local.pageTeachnigTimerOpeAuth[commonViewModel.userLevel] !== 0){
                        local.prevPageTab = viewModel.currentPageTab
                        viewModel.currentPageTab = parent.index
                        resourceManager.titleString = resourceManager.label(20,34,resourceManager.language)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//Timer
        Image {
            id: counterTabImage
            property int index: 4
            property bool selected: viewModel.currentPageTab === index
            source: (counterTabImage.selected || mouse_5.pressed) ? resourceManager.imagePath + "/16_program/a160_01.png" : resourceManager.imagePath + "/16_program/a160_02.png"
//            Image{
//                x:12
//                anchors.verticalCenter: parent.verticalCenter
//                source: (counterTabImage.selected || mouse_5.pressed) ? viewModel.pageModel[parent.index].activateIconPath : viewModel.pageModel[parent.index].deactivateIconPath
//            }
            Text{
                width: 125
                height: 84
                x:46
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: mouse_5.pressed ? 25 : 24
                font.bold : true
                text: viewModel.pageModel[counterTabImage.index].pageLabel.name
                color: counterTabImage.selected ? "white" : "#666666"
            }
            MouseArea{
                id:mouse_5
                anchors.fill: parent
                enabled: !local.editFromProgEditScreen
                onClicked: {
                    if(local.pageTeachnigCounterOpeAuth[commonViewModel.userLevel] !== 0){
                        local.prevPageTab = viewModel.currentPageTab
                        viewModel.currentPageTab = parent.index
                        resourceManager.titleString = resourceManager.label(20,35,resourceManager.language)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//Counter
        Image {
            id: passTabImage
            property int index: 5
            property bool selected: viewModel.currentPageTab === index
            source: (passTabImage.selected || mouse_6.pressed) ? resourceManager.imagePath + "/16_program/a160_01.png" : resourceManager.imagePath + "/16_program/a160_02.png"
//            Image{
//                x:12
//                anchors.verticalCenter: parent.verticalCenter
//                source: (passTabImage.selected || mouse_6.pressed) ? viewModel.pageModel[parent.index].activateIconPath : viewModel.pageModel[parent.index].deactivateIconPath
//            }
            Text{
                width: 125
                height: 84
                x:46
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: mouse_6.pressed ? 25 : 24
                font.bold : true
                text: viewModel.pageModel[passTabImage.index].pageLabel.name
                color: passTabImage.selected ? "white" : "#666666"
            }
            MouseArea{
                id:mouse_6
                anchors.fill: parent
                enabled: !local.editFromProgEditScreen
                onClicked: {
                    if(local.pageTeachnigPassOpeAuth[commonViewModel.userLevel] !== 0){
                        local.prevPageTab = viewModel.currentPageTab
                        viewModel.currentPageTab = parent.index
                        resourceManager.titleString = resourceManager.label(20,36,resourceManager.language)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//Pass
        Image {
            id: programTabImage
            property int index: 6
            property bool selected: viewModel.currentPageTab === index
            source: (programTabImage.selected || mouse_7.pressed) ? resourceManager.imagePath + "/16_program/a160_01.png" : resourceManager.imagePath + "/16_program/a160_02.png"
//            Image{
//                x:12
//                anchors.verticalCenter: parent.verticalCenter
//                source: (programTabImage.selected || mouse_7.pressed) ? viewModel.pageModel[parent.index].activateIconPath : viewModel.pageModel[parent.index].deactivateIconPath
//            }
            Text{
                width: 125
                height: 84
                x:46
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: mouse_7.pressed ? 25 : 24
                font.bold : true
                text: viewModel.pageModel[programTabImage.index].pageLabel.name
                color: programTabImage.selected ? "white" : "#666666"
            }
            MouseArea{
                id:mouse_7
                anchors.fill: parent
                enabled: !local.editFromProgEditScreen
                onClicked:{
                    if(local.pageTeachnigProgramOpeAuth[commonViewModel.userLevel] !== 0){
                        local.prevPageTab = viewModel.currentPageTab
                        viewModel.currentPageTab = parent.index
                        resourceManager.titleString = resourceManager.label(20,37,resourceManager.language)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//Program
    }//编辑菜单按钮栏

//rev by shhong
    //程序窗口
    StackLayout{
        // 편집페이지 제일 위 부모 요소
        id: pro_stack
        width: 1000
        height: 600
        x:28
        y:88
        currentIndex: viewModel.currentPageTab
        visible: viewModel.currentPageTab === 6 ? false : true
        ModePage{
            pageActive: program_page.pageActive && (parent.currentIndex == 0)
        }//ModePage


        Loader {
            Rectangle{
                id :posSetarea
                width: parent.width
                height: parent.height
                color: "transparent"
            }

            id: posSetPageLoader
            property bool isActive: program_page.pageActive && (parent.currentIndex == 1)
            active: false
            asynchronous: !isActive
            sourceComponent: PosSetPage {
                pageActive: posSetPageLoader.isActive
            }
            onLoaded: componentLoader.loadNextComponent()
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
            }
        }//PosSetPage
        Loader {
            id: stackPageLoader
            property bool isActive: program_page.pageActive && (parent.currentIndex == 2)
            active: false
            asynchronous: !isActive
            sourceComponent: StackPage {
                pageActive: stackPageLoader.isActive
                onEditFromProgEditScreenFinished: {
                    local.prevPageTab = viewModel.currentPageTab
                    viewModel.currentPageTab = 6
                }
            }
            onLoaded: componentLoader.loadNextComponent()
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
            }
        }//StackPage
        Loader {
            id: timerPageLoader
            source: "qrc:/qml/Program/TimerPage.qml";
            active: false
            property bool isActive: program_page.pageActive && (parent.currentIndex == 3)
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }//TimerPage
        Loader {
            id: counterPageLoader
            source: "qrc:/qml/Program/CounterPage.qml";
            active: false
            property bool isActive: program_page.pageActive && (parent.currentIndex == 4)
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }//CounterPage
        Loader {
            id: passPageLoader
            source: "qrc:/qml/Program/PassPage.qml";
            active: false
            property bool isActive: program_page.pageActive && (parent.currentIndex == 5)
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }//PassPage
    }
    Loader {
        id: programLoader
        property bool isActive: program_page.pageActive && (viewModel.currentPageTab === 6)
        active: false
        asynchronous: true
        visible: (status == Loader.Ready) && isActive
        onLoaded: componentLoader.loadNextComponent()
        sourceComponent: Program {
            pageActive: programLoader.isActive
            onProgramClose: {
                viewModel.currentPageTab =
                        (local.numOfVisibleTabs <= 1) ? local.emptyTab : local.prevPageTab
                switch (viewModel.currentPageTab) {
                case 0: resourceManager.titleString = resourceManager.label(20,31,resourceManager.language); break;
                case 1: resourceManager.titleString = resourceManager.label(20,32,resourceManager.language); break;
                case 2: resourceManager.titleString = resourceManager.label(20,33,resourceManager.language); break;
                case 3: resourceManager.titleString = resourceManager.label(20,34,resourceManager.language); break;
                case 4: resourceManager.titleString = resourceManager.label(20,35,resourceManager.language); break;
                case 5: resourceManager.titleString = resourceManager.label(20,36,resourceManager.language); break;
                case 6: resourceManager.titleString = resourceManager.label(20,37,resourceManager.language); break;
                default: // nop.
                }
            }
        }
        onIsActiveChanged: {
            if (isActive) {
                asynchronous = false
                active = true
            }
        }
    }//ProgramPage
}
