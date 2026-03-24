import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
import "../Widget"
import "../Components/RemindDialog"
/*************************************************************************************
* Title: UI/UX of AEK
* File: PassPage.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.04
* Worker: SH.HONG
**************************************************************************************/
Item {
    id: passPage
    width: parent.width
    height: parent.height
    property var pageModel: teachingViewModel.pageModel[5]
    property bool pageActive: false

    Component.onDestruction: pageModel.deactivate()

    onPageActiveChanged: {
        if (pageActive) {
            pageModel.startPageChanging()
            commonViewModel.pageChange(Enums.TeachingPass)
            pageModel.activate()
        } else {
            pageModel.deactivate()
        }
    }

    QtObject {
        id: local
        readonly property int passDataMagnification: 100
        property bool dataLoaded: pageModel.dataLoaded
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "15"
        readonly property string pageNumColor: resourceManager.dataColor(groupNo, 1)
        readonly property string settingValueSelectionColor: resourceManager.dataColor(groupNo, 2)
    }

    property var modepageModel: teachingViewModel.pageModel[0]
        //현재위치 배경 shhong
    Rectangle{
            id : counterbg
            anchors.left: parent.left
            anchors.leftMargin: 60
            anchors.top: parent.top
            anchors.topMargin: 37
            width: parent.width - 155
            height: parent.height - 92
            radius: 10
            color: "#eaeaea"
            border.color: "#cccccc"
        }

    // item1보다 살짝 위에 공통 설명 헤더(예: Extract 페이지 안내 문구)
    Rectangle {
        id: passInfoHeader
        width: 500
        height: 60
        x: 80
        y: 49
        color: "#999999"
        border.color: "#999999"

        Text {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 22
            font.bold: true
            color: "white"
            text: resourceManager.label(40, 12, resourceManager.language)
        }
    }

    Item {
        id: offsetContainer
        x: 70
        y: 110
        width: parent.width
        height: parent.height

    StackLayout{
        id:stack_pass
        width: parent.width
        height: 506
        currentIndex: pass_tab.currentIndex
        //第一页

        Item{
            Rectangle{
                id: item1
                readonly property int index: 0
                width: 400
                height: 200
                x:10
                y:10
                color: "#ffffff"
                border.color: "#999999"
                Rectangle{
                    width: 400
                    height: 60
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        font.bold:true
                        color: "white"
                        text: resourceManager.label(40,1, resourceManager.language)
                    }
                }
                Image {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 90
                    anchors.bottomMargin: 24
                    source: resourceManager.imagePath + "/17_pass/d170_00.png"
                }//图示
                Column{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 15
                    spacing: 6
                    Text{
                        width: 120
                        height: 30
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        color: resourceManager.rgb(40, 7)
                        text: resourceManager.label(40,7, resourceManager.language)
                    }
                    LimitNumericEditBox{
                        width: 125
                        height: 40
                        fontPixelSize: 20
                        color: "#ffffff"
                        textColor: "black"
                        borderColor: "#999999"
                        keyLayoutType: 4
                        value: pageModel.formatData(0, pageModel.distanceList[item1.index])
//                        clicklimit: !modepageModel.mode31State
                        onEnter: {
                            pageModel.setDistance(item1.index, parseInt(newValue * local.passDataMagnification))
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                        onReady: {
                            max = pageModel.formatPointMaxValue(0)
                            min = pageModel.formatPointMinValue(0)
                            dialogDisplayX = 540
                            dialogDisplayY = 280
                        }
                        onOpen: textColor = penSet.settingValueSelectionColor
                        onClose: textColor = "black"
                        onClickfail: pass_remind_dialog.open()
                    }
                    Rectangle{
                        width: 125
                        height: 40
                        color: "#cccccc"
                        border.color: "#cccccc"
                        Text{
                            anchors.centerIn: parent
                            visible: local.dataLoaded
                            font.pixelSize: 20
                            text: pageModel.formatData(1, pageModel.timeList[item1.index])
                            color: "black"
                        }
                    }
                }//下降距离
            }//正臂取出下降

            Rectangle{
                id: item2
                readonly property int index: 1
                width: 400
                height: 200
                x:416
                y:10
                color: "#ffffff"
                border.color: "#999999"
                border.width: 1
                Rectangle{
                    width: 400
                    height: 60
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        font.bold: true
                        color: "white"
                        text: resourceManager.label(40,2, resourceManager.language)
                    }
                }
                Image {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 90
                    anchors.bottomMargin: 24
                    source: resourceManager.imagePath + "/17_pass/d170_01.png"
                }//图示
                Column{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 15
                    spacing: 6
                    Text{
                        width: 120
                        height: 30
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        color: resourceManager.rgb(40, 9)
                        text: resourceManager.label(40,9, resourceManager.language)
                    }
                    LimitNumericEditBox{
                        width: 125
                        height: 40
                        fontPixelSize: 20
                        color: "#ffffff"
                        textColor: "black"
                        borderColor: "#999999"
                        keyLayoutType: 4
                        value: pageModel.formatData(5, pageModel.distanceList[item2.index])
//                        clicklimit: !modepageModel.mode31State
                        onEnter: {
                            pageModel.setDistance(item2.index, parseInt(newValue * local.passDataMagnification))
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                        onReady: {
                            max = pageModel.formatPointMaxValue(3)
                            min = pageModel.formatPointMinValue(3)
                            dialogDisplayX = 410
                            dialogDisplayY = 280
                        }
                        onOpen: textColor = penSet.settingValueSelectionColor
                        onClose: textColor = "black"
                        onClickfail: pass_remind_dialog.open()
                    }
                    Rectangle{
                        width: 125
                        height: 40
                        color: "#cccccc"
                        border.color: "#cccccc"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            visible: local.dataLoaded
                            font.pixelSize: 22
                            text: pageModel.formatData(6, pageModel.timeList[item2.index])
                            color: "black"
                        }
                    }
                }//上升距离
            }//正臂取出上升

            Rectangle{
                id: item3
                readonly property int index: 2
                width: 400
                height: 200
                x:10
                y:215
                color: "#ffffff"
                border.color: "#999999"
                Rectangle{
                    width: 400
                    height: 60
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        font.bold: true
                        color: "white"
                        text: resourceManager.label(40,3, resourceManager.language)
                    }
                }
                Image {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 90
                    anchors.bottomMargin: 24
                    source: resourceManager.imagePath + "/17_pass/d170_02.png"
                }//图示
                Column{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 15
                    spacing: 6
                    Text{
                        width: 125
                        height: 40
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        color: resourceManager.rgb(40, 7)
                        text: resourceManager.label(40,7, resourceManager.language)
                    }
                    LimitNumericEditBox{
                        width: 125
                        height: 40
                        fontPixelSize: 20
                        color: "#ffffff"
                        textColor: "black"
                        borderColor: "#999999"
                        keyLayoutType: 4
                        value: pageModel.formatData(2, pageModel.distanceList[item3.index])
//                        clicklimit: !modepageModel.mode31State
                        onEnter: {
                            pageModel.setDistance(item3.index, parseInt(newValue * local.passDataMagnification))
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                        onReady: {
                            max = pageModel.formatPointMaxValue(1)
                            min = pageModel.formatPointMinValue(1)
                            dialogDisplayX = 540
                            dialogDisplayY = 280
                        }
                        onOpen: textColor = penSet.settingValueSelectionColor
                        onClose: textColor = "black"
                        onClickfail: pass_remind_dialog.open()
                    }
                    Rectangle{
                        width: 125
                        height: 40
                        color: "#cccccc"
                        border.color: "#cccccc"
                        Text{
                            anchors.centerIn: parent
                            visible: local.dataLoaded
                            font.pixelSize: 22
                            text: pageModel.formatData(3, pageModel.timeList[item3.index])
                            color: "black"
                        }
                    }
                }//下降距离
            }//副臂取出下降

            Rectangle{
                id: item4
                readonly property int index: 3
                width: 400
                height: 200
                x:416
                y:215
                color: "#ffffff"
                border.color: "#999999"
                Rectangle{
                    width: 400
                    height: 60
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        font.bold: true
                        color: "white"
                        text: resourceManager.label(40,4, resourceManager.language)
                    }
                }
                Image {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 90
                    anchors.bottomMargin: 24
                    source: resourceManager.imagePath + "/17_pass/d170_03.png"
                }//图示
                Column{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 20
                    anchors.bottomMargin: 15
                    spacing: 6
                    Text{
                        width: 120
                        height: 30
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        color: resourceManager.rgb(40, 9)
                        text: resourceManager.label(40,9, resourceManager.language)
                    }
                    LimitNumericEditBox{
                        width: 125
                        height: 40
                        fontPixelSize: 20
                        color: "#ffffff"
                        textColor: "black"
                        borderColor: "#999999"
                        keyLayoutType: 4
                        value: pageModel.formatData(7, pageModel.distanceList[item4.index])
//                        clicklimit: !modepageModel.mode31State
                        onEnter: {
                            pageModel.setDistance(item4.index, parseInt(newValue * local.passDataMagnification))
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                        onReady: {
                            max = pageModel.formatPointMaxValue(4)
                            min = pageModel.formatPointMinValue(4)
                            dialogDisplayX = 410
                            dialogDisplayY = 280
                        }
                        onOpen: textColor = penSet.settingValueSelectionColor
                        onClose: textColor = "black"
                        onClickfail: pass_remind_dialog.open()
                    }
                    Rectangle{
                        width: 125
                        height: 40
                        color: "#cccccc"
                        border.color: "#cccccc"
                        Text{
                            anchors.centerIn: parent
                            visible: local.dataLoaded
                            font.pixelSize: 22
                            text: pageModel.formatData(8, pageModel.timeList[item4.index])
                            color: "black"
                        }
                    }
                }//上升距离
            }//副臂取出上升
        }
        //第二页
        Item{
            Rectangle{
                id: item5
                readonly property int index: 4
                width: 800
                height: 200
                x:10
                y:10
                color: "#ffffff"
                border.color: "#999999"
                border.width: 1
                Rectangle{
                    width: 800
                    height: 60
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        font.bold: true
                        color: "white"
                        text: resourceManager.label(40,5, resourceManager.language)
                    }
                }
                Image {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 330
                    anchors.bottomMargin: 24
                    source: resourceManager.imagePath + "/17_pass/d170_04.png"
                }//图示
                Column{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 30
                    anchors.bottomMargin: 30
                    spacing: 6
                    Text{
                        width: 120
                        height: 40
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        color: resourceManager.rgb(40, 7)
                        text: resourceManager.label(40,7, resourceManager.language)
                    }
                    LimitNumericEditBox{
                        width: 120
                        height: 40
                        fontPixelSize: 20
                        color: "#ffffff"
                        textColor: "black"
                        borderColor: "#999999"
                        keyLayoutType: 4
                        value: pageModel.formatData(4, pageModel.distanceList[item5.index])
//                        clicklimit: !modepageModel.mode31State
                        onEnter: {
                            pageModel.setDistance(item5.index, parseInt(newValue * local.passDataMagnification))
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                        onReady: {
                            max = pageModel.formatPointMaxValue(2)
                            min = pageModel.formatPointMinValue(2)
                            dialogDisplayX = 410
                            dialogDisplayY = 280
                        }
                        onOpen: textColor = penSet.settingValueSelectionColor
                        onClose: textColor = "black"
                        onClickfail: pass_remind_dialog.open()
                    }
                }//下降距离
            }//置物侧下降

            Rectangle{
                id: item6
                readonly property int index: 5
                width: 800
                height: 200
                x:10
                y:215
                color: "#ffffff"
                border.color: "#999999"
                Rectangle{
                    width: 800
                    height: 60
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        font.bold: true
                        color: "white"
                        text: resourceManager.label(40,6, resourceManager.language)
                    }
                }
                Image {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 330
                    anchors.bottomMargin: 24
                    source: resourceManager.imagePath + "/17_pass/d170_05.png"
                }//图示
                Column{
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 30
                    anchors.bottomMargin: 30
                    spacing: 6
                    Text{
                        width: 120
                        height: 30
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        color: resourceManager.rgb(40, 9)
                        text: resourceManager.label(40,9, resourceManager.language)
                    }
                    LimitNumericEditBox{
                        width: 120
                        height: 40
                        fontPixelSize: 20
                        color: "#ffffff"
                        textColor: "black"
                        borderColor: "#999999"
                        keyLayoutType: 4
                        value: pageModel.formatData(9, pageModel.distanceList[item6.index])
//                        clicklimit: !modepageModel.mode31State
                        onEnter: {
                            pageModel.setDistance(item6.index, parseInt(newValue * local.passDataMagnification))
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                        onReady: {
                            max = pageModel.formatPointMaxValue(5)
                            min = pageModel.formatPointMinValue(5)
                            dialogDisplayX = 410
                            dialogDisplayY = 280
                        }
                        onOpen: textColor = penSet.settingValueSelectionColor
                        onClose: textColor = "black"
                        onClickfail: pass_remind_dialog.open()
                    }
                }//上升距离
            }//置物侧上升
        }
    }
TabBar{
    id: pass_tab
    width: 250
    height: 45
    x : 560
    anchors.top: parent.top     // 위쪽 정렬
    anchors.topMargin: -37       // 위쪽 여백
    spacing: 10                 // 버튼 간 간격을 10으로 설정
    background: Rectangle{
        color: "transparent"
    }

    // 첫 번째 탭 버튼 (Rectangle로 변경)
    TabButton {
        width: 120
        height: 45
        background: Rectangle {
            width: parent.width
            height: parent.height
            color: pass_tab.currentIndex == 0 || pass_tab1.pressed ? "#cc3333" : "#cccccc"  // 눌렀을 때 색상 변경
            border.color: "#ffffff"
        }

        MouseArea {
            id: pass_tab1
            anchors.fill: parent
            onClicked: {
                pass_tab.currentIndex = 0
                phyKeyManager.ctrlBuzzer()
            }
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: pass_tab1.pressed ? 19 : 20
            font.bold: true
            color: pass_tab.currentIndex == 0 || pass_tab1.pressed ? "white" : "#666666"
            // text: resourceManager.label(36,118,resourceManager.language)  // 다국어 텍스트
            text: "Extract"
        }
    }

    // 두 번째 탭 버튼 (Rectangle로 변경)
    TabButton {

        width: 120
        height: 45
        background: Rectangle {
            width: parent.width
            height: parent.height
            color: pass_tab.currentIndex == 1 || pass_tab2.pressed ? "#cc3333" : "#cccccc"  // 눌렀을 때 색상 변경
            border.color: "#ffffff"
        }

        MouseArea {
        id: pass_tab2
            anchors.fill: parent
            onClicked: {
                pass_tab.currentIndex = 1
                phyKeyManager.ctrlBuzzer()
            }
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: pass_tab2.pressed ? 19 : 20
            font.bold: true
            color: pass_tab.currentIndex == 1 || pass_tab2.pressed ? "white" : "#666666"
            // text: resourceManager.label(36,119,resourceManager.language)  // 다국어 텍스트
            text: "Release"
        }
    }
}
    // TabBar{
    //     id:pass_tab
    //     width: 200
    //     anchors.right: parent.right
    //     anchors.top: parent.top
    //     anchors.topMargin: -67
    //     anchors.rightMargin: 184
    //     spacing: 20
    //     background: Rectangle{
    //         color: "transparent"
    //     }
    //     TabButton{
    //         id:pass_tab1
    //         background: Image {
    //             source: pass_tab1.pressed ? resourceManager.imagePath + "/17_pass/d170_08.png" : resourceManager.imagePath + "/17_pass/d170_06.png"
    //         }
    //         onClicked:{
    //             pass_tab.currentIndex = 0
    //             phyKeyManager.ctrlBuzzer()
    //         }
    //     }
    //     TabButton{
    //         id:pass_tab2
    //         background: Image {
    //             source: pass_tab2.pressed ? resourceManager.imagePath + "/17_pass/d170_09.png" : resourceManager.imagePath + "/17_pass/d170_07.png"
    //         }
    //         onClicked: {
    //             pass_tab.currentIndex = 1
    //             phyKeyManager.ctrlBuzzer()
    //         }
    //     }
    // }


    Remind_Forbid_Dialog{
        id:pass_remind_dialog
        x:60
        y:120
        message: resourceManager.label(40,11,resourceManager.language)
        modal: true
    }//非手动状态时模式切换禁止窗口
    }
/***********************************************************************/
//    Connections {
//        target: phyKeyManager
//        enabled: passPage.pageActive
//        onJogScrollUpChanged: pass_tab.currentIndex = 0
//        onJogScrollDownChanged: pass_tab.currentIndex = 1
//    }

//    MouseArea {
//        anchors.fill: parent
//        propagateComposedEvents: true
//        onPressed: mouse.accepted = false
//        onReleased: mouse.accepted = false
//        onClicked: mouse.accepted = false
//        onWheel: {
//            if (!REAL_TARGET) {
//                if(wheel.angleDelta.y > 0) {
//                    phyKeyManager.decrementEmulatedJogCount()
//                } else if(wheel.angleDelta.y < 0) {
//                    phyKeyManager.incrementEmulatedJogCount()
//                }
//            }
//        }
//    }
}
