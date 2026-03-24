import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
import "../.."
import "../../Manual"
import "../../Widget"   // ✅ ImageButton 타입 위해 추가
import "../../Program/PosSet/PosSetUtils.js" as PosSetUtils  // ✅ manualSpeed 계산

Item {
    id: posSetPage
    property var pageModel: teachingViewModel.pageModel[1]
    property bool pageActive: false

    // ✅ 표시용(선택 상태) 스피드: 기본값
    property int uiManualSpeed: Enums.ManualSpeed01

    // ✅ commonViewModel 기반 현재 스피드 계산 (참고용)
    property int manualSpeed: PosSetUtils.getManualSpeedValue(
                                  commonViewModel.manualSpeedStatus,
                                  commonViewModel.manualSpeedSetting
                              )

    // ✅ 현재 commonViewModel 값으로 uiManualSpeed 동기화
    function syncManualSpeed() {
        uiManualSpeed = PosSetUtils.getManualSpeedValue(
                    commonViewModel.manualSpeedStatus,
                    commonViewModel.manualSpeedSetting
                )
    }

    // ✅ 부팅/진입 직후 값이 늦게 들어오는 경우 대비: 짧게 여러 번 재시도
    Timer {
        id: speedSyncRetry
        interval: 80
        repeat: true
        property int tries: 0
        onTriggered: {
            posSetPage.syncManualSpeed()
            tries++
            if (tries >= 10) { // 약 0.8초
                stop()
            }
        }
    }

    // ✅ 다른 화면에서 속도/설정이 바뀌면(상태값 변경) Grid 선택도 따라가게
    Connections {
        target: commonViewModel

        function onManualSpeedStatusChanged() {
            posSetPage.syncManualSpeed()
        }
        function onManualSpeedSettingChanged() {
            posSetPage.syncManualSpeed()
        }
    }

    function changePage(page) {
        switch(page) {
        case 0:
            pageModel.startPageChanging()
            commonViewModel.pageChange(Enums.TeachingPosSet)
            break
        case 1:
            pageModel.startPageChanging()
            commonViewModel.pageChange(Enums.TeachingPosFix)
            break
        default:
            break
        }
    }

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdTeachingPosSet
        property int language: 0
    }

    ComponentLoader {
        id: componentLoader
        guard: !posSetPage.pageActive
        loadComponentList: [ pos_adjust_page ]
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    onPageActiveChanged: {
        let tabIndex = 1

        if (pageActive) {
            pageModel.activate()

            // ✅ 페이지 진입 시 즉시 1회 + 재시도(부팅 직후 대비)
            posSetPage.syncManualSpeed()
            speedSyncRetry.tries = 0
            speedSyncRetry.start()

            let tab2Visible = false
            pos_tab2.visible = tab2Visible

            let tab1Visible = false
            pos_tab1.visible = tab1Visible

            componentLoader.loadNextComponent()

            changePage(tabIndex)
            pos_tab.currentIndex = tabIndex
        } else {
            pos_tab1.visible = false
            pos_tab2.visible = false
            pageModel.deactivate()
        }
    }

    TabBar{
        id:pos_tab
        width:240
        height: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 2
        anchors.leftMargin: 6
        z:1
        spacing: 14
        background: Rectangle{ color: "transparent" }

        TabButton{
            id:pos_tab1
            anchors.top: parent.top
            anchors.topMargin: 5
            z: (pos_tab.currentIndex  == 1)
            width: visible ? implicitWidth : -pos_tab.spacing
            background: Image {
                source: (pos_tab.currentIndex == 0 || mouse1.pressed)
                        ? resourceManager.imagePath + "/13_POINT_set/d130_01.png"
                        : resourceManager.imagePath + "/13_POINT_set/d130_02.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: resourceManager.label(36,1,local.language)
                font.pixelSize: mouse1.pressed ? 23 : 22
                color: "white"
            }
            MouseArea{
                id:mouse1
                anchors.fill: parent
                onClicked: {
                    if (pos_tab.currentIndex !== 0) {
                        pos_tab.currentIndex = 0
                        posSetPage.changePage(0)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }

        TabButton{
            id:pos_tab2
            anchors.top: parent.top
            anchors.topMargin: 5
            z: (pos_tab.currentIndex  == 1)
            width: visible ? implicitWidth : -pos_tab.spacing
            background: Image {
                source: (pos_tab.currentIndex == 1 || mouse2.pressed)
                        ? resourceManager.imagePath + "/13_POINT_set/d130_01.png"
                        : resourceManager.imagePath + "/13_POINT_set/d130_02.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: resourceManager.label(36,2,local.language)
                font.pixelSize: mouse2.pressed ? 23 : 22
                color: "white"
            }
            MouseArea{
                id:mouse2
                anchors.fill: parent
                onClicked: {
                    if (pos_tab.currentIndex !== 1) {
                        pos_tab.currentIndex = 1
                        posSetPage.changePage(1)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }
    }

    // ✅ posnowaxis (여기에 좌상단 버튼 추가)
    Rectangle{
        id : posnowaxis
        x: 20
        y: 197
        width: 857
        height: 384
        radius: 10
        color: "#eaeaea"
        border.color: "#cccccc"
    }

    // ✅ 좌상단 5개 버튼
    Grid{
        id: manualSpeedMini
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 30
        anchors.topMargin: 25
        z :10
        columns: 5
        rows: 1
        columnSpacing: 2
        rowSpacing: 2

        function clickSpeed(newSpeed){
            posSetPage.uiManualSpeed = newSpeed      // ✅ 즉시 표시
            commonViewModel.modbusSpeedControl(newSpeed)
            phyKeyManager.ctrlBuzzer()
        }

        // 0.1
        Row{
            spacing: 9.5
            Image{
                width: 47; height: 47
                fillMode: Image.PreserveAspectFit
                source: (posSetPage.uiManualSpeed === Enums.ManualSpeed01)
                        ? resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                        : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                MouseArea{ anchors.fill: parent; onClicked: manualSpeedMini.clickSpeed(Enums.ManualSpeed01) }
            }
            Text{
                width: 45; height: 42
                font.pixelSize: 18; font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: -6
                topPadding: 2
                color: resourceManager.rgb(56,4)
                text: resourceManager.label(56,4,resourceManager.language)
            }
        }

        // 1
        Row{
            spacing: 9.5
            Image{
                width: 47; height: 47
                fillMode: Image.PreserveAspectFit
                source: (posSetPage.uiManualSpeed === Enums.ManualSpeed1)
                        ? resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                        : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                MouseArea{ anchors.fill: parent; onClicked: manualSpeedMini.clickSpeed(Enums.ManualSpeed1) }
            }
            Text{
                width: 45; height: 42
                font.pixelSize: 20; font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: -6
                topPadding: 2
                color: resourceManager.rgb(56,5)
                text: resourceManager.label(56,5,resourceManager.language)
            }
        }

        // 저속
        Row{
            spacing: 9.5
            Image{
                width: 47; height: 47
                fillMode: Image.PreserveAspectFit
                source: (posSetPage.uiManualSpeed === Enums.ManualSpeedSlow)
                        ? resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                        : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                MouseArea{ anchors.fill: parent; onClicked: manualSpeedMini.clickSpeed(Enums.ManualSpeedSlow) }
            }
            Text{
                width: 45; height: 42
                font.pixelSize: 20; font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: -6
                topPadding: 2
                color: resourceManager.rgb(56,6)
                text: resourceManager.label(56,6,resourceManager.language)
            }
        }

        // 중속
        Row{
            spacing: 9.5
            Image{
                width: 47; height: 47
                fillMode: Image.PreserveAspectFit
                source: (posSetPage.uiManualSpeed === Enums.ManualSpeedMedium)
                        ? resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                        : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                MouseArea{ anchors.fill: parent; onClicked: manualSpeedMini.clickSpeed(Enums.ManualSpeedMedium) }
            }
            Text{
                width: 45; height: 42
                font.pixelSize: 20; font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: -6
                topPadding: 2
                color: resourceManager.rgb(56,7)
                text: resourceManager.label(56,7,resourceManager.language)
            }
        }

        // 고속
        Row{
            spacing: 9.5
            Image{
                width: 47; height: 47
                fillMode: Image.PreserveAspectFit
                source: (posSetPage.uiManualSpeed === Enums.ManualSpeedHigh)
                        ? resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                        : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                MouseArea{ anchors.fill: parent; onClicked: manualSpeedMini.clickSpeed(Enums.ManualSpeedHigh) }
            }
            Text{
                width: 45; height: 42
                font.pixelSize: 20; font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: -6
                topPadding: 2
                color: resourceManager.rgb(56,7)
                text: resourceManager.label(56,8,resourceManager.language)
            }
        }
    }

    StackLayout{
        id:stack_pos
        width: parent.width
        height: 520
        anchors.top: parent.top
        anchors.left: parent.left
        currentIndex: pos_tab.currentIndex

        Pos_Set{
            id: pos_set_page
            pageModel: posSetPage.pageModel
            pageActive: posSetPage.pageActive && (pos_tab.currentIndex === 0)
        }

        Loader {
            id: pos_adjust_page
            property bool isActive: posSetPage.pageActive && (pos_tab.currentIndex === 1)
            sourceComponent: pos_adjust_component
            active: false
            asynchronous: !isActive
            onIsActiveChanged: if (isActive) active = true

            Component {
                id: pos_adjust_component
                Pos_Adjust{
                    pageModel: posSetPage.pageModel
                    pageActive: pos_adjust_page.isActive
                }
            }
        }
    }
}
