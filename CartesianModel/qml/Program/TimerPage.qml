import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: TimerPage.qml
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
    id: timerPage
    width: parent.width
    height: parent.height
    property var pageModel: teachingViewModel.pageModel[3]
    property bool pageActive: false
    property var dataList: pageModel.dataList
    property int currentHelpIndex: 0
    property int currentPageIndex: 0

    function prevPage(){
        if (currentPageIndex > 0){
            currentPageIndex--
        }else if(currentPageIndex == 0){
            currentPageIndex = 11
        }
    }

    function nextPage(){
        if ((currentPageIndex + 1) * 9 < 100){
            currentPageIndex++
        }else if(currentPageIndex == 11){
            currentPageIndex = 0
        }
    }

    QtObject{
        id: local
        readonly property int timerGroupId: 13
        property color startStateColor: "black"
        property color upStateColor: "black"
        property color selectedTabColor: "black"
        property color pageColor: "black"
        readonly property bool jogDialSwOpe: (commonViewModel.topPopupDialog === Enums.PopupDialogStatusNone)
    }

    Component.onCompleted: {
        local.startStateColor = resourceManager.dataColor(local.timerGroupId, 1)
        local.upStateColor = resourceManager.dataColor(local.timerGroupId, 2)
        local.selectedTabColor = resourceManager.dataColor(local.timerGroupId, 3)
        local.pageColor = resourceManager.dataColor(local.timerGroupId, 4)
    }

    Component.onDestruction: pageModel.deactivate()

    onPageActiveChanged: {
        if (pageActive) {
//            timer_tab.currentIndex = timer_tab1.TabBar.index
            commonViewModel.pageChange(Enums.TeachingTimer)
            pageModel.activate()
        } else {
            pageModel.deactivate()
        }
    }


    Item {
        id: stack_timer
        width: parent.width
        height: 450
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: -3
        anchors.topMargin: 52
        Rectangle{
             id : timerbg
             anchors.left: parent.left
             anchors.leftMargin: 10
             anchors.top: parent.top
             anchors.topMargin: - 30
             width: parent.width - 40
             height: parent.height - 90
             radius: 10
             color: "#eaeaea"
             border.color: "#cccccc"
        }
        Item {
            Item {
                id: stack_timer_item
                width: parent.width - 100
                height: 450
                anchors.top: parent.top
                anchors.left: parent.left

                Grid {
                    x: 30
                    y: -10
                    rows: 3
                    columns: 3
                    columnSpacing: 10
                    rowSpacing: 10

                    Repeater {
                        model: 9
                        Rectangle {
                            id: timer_item
                            property int dataIndex: currentPageIndex * 9 + index
                            property int labelIndex: 7 + 2 * dataIndex
                            property var timerData: timerPage.dataList[dataIndex]

                            visible: dataIndex < 100
                            width: 300
                            height: 100
                            color: "#666666"

                            // 상단 텍스트
                            Text {
                                width: 230
                                height: 40
                                x: 10
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.WordWrap
                                font.pixelSize: 19
                                font.bold: true
                                color: "#ffffff"
                                text: dataList[dataIndex].msglabel.name
                            }

                            // 도움 아이콘 (MouseArea 제거)
                            Image {
                                id: helpIcon
                                anchors.top: parent.top
                                anchors.right: parent.right
                                anchors.topMargin: 6
                                anchors.rightMargin: 14
                                source: (currentHelpIndex === dataIndex)
                                        ? resourceManager.imagePath + "/10_MODE/a100_06.png"
                                        : resourceManager.imagePath + "/10_MODE/a100_05.png"
                            }

                            // 하단 값/에딧 영역
                            Rectangle {
                                id: valueArea
                                width: 300
                                height: 60
                                anchors.bottom: parent.bottom
                                border.color: "#999999"
                                border.width: 1
                                color: "#cccccc"

                                Text {
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.topMargin: 8
                                    anchors.leftMargin: 8
                                    font.pixelSize: 24
                                    color: (timerData.startState || timerData.upState)
                                           ? local.startStateColor
                                           : resourceManager.rgb(38, labelIndex + 1)
                                    text: dataList[dataIndex].marklabel.name
                                }

                                LimitNumericEditBox {
                                    width: 130
                                    height: 40
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    anchors.rightMargin: 8
                                    fontPixelSize: 22
                                    color: opened ? "#cc3333" : "#ffffff"
                                    textColor: timerData.upState ? local.upStateColor : "black"
                                    keyLayoutType: 4
                                    min: timerData.min
                                    max: timerData.max
                                    rectRadius: 0
                                    borderWidth: 1
                                    value: pageModel.formatSpeed(dataIndex, timerData.time)
                                    unit: pageModel.getUnit(dataIndex)
                                    onReady: {
                                        const col = dataIndex % 3
                                        dialogDisplayY = 164

                                        if (col === 0) {
                                            dialogDisplayX = 374   // 1열(좌)
                                        } else if (col === 1) {
                                            dialogDisplayX = 180   // 2열(중)
                                        } else {
                                            dialogDisplayX = 400   // 3열(우)
                                        }
                                    }
                                    onEnter: {
                                        pageModel.setTime(dataIndex, parseFloat(newValue))
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                    }
                                }
                            }

                            // ▶ 상단(텍스트+아이콘) 영역 전체를 덮는 클릭 영역
                            MouseArea {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                anchors.bottom: valueArea.top   // 하단 편집 영역은 가리지 않음
                                onClicked: {
                                    currentHelpIndex = dataIndex
                                    phyKeyManager.ctrlBuzzer()
                                }
                            }
                        }
                    }
                }

            }

//            ImageScrollBar {
//                x:675
//                y:10
//                height: 440
//                topButtonPressImage: resourceManager.imagePath + "/11_TIMER/d110_05.png"
//                topButtonReleaseImage: resourceManager.imagePath + "/11_TIMER/d110_03.png"
//                botomButtonPressImage: resourceManager.imagePath + "/11_TIMER/d110_06.png"
//                botomButtonReleaseImage: resourceManager.imagePath + "/11_TIMER/d110_04.png"
//                pageLabal: currentPageIndex + 1 + "/" + Math.ceil(100 / 9)
//                pageLabelColor: local.pageColor
//                onScrollUpClicked: prevPage()
//                onScrollDownClicked: nextPage()
//            }
            Item {
                id: horizontalScrollBar
                width: 440
                height: 60
                x:268
                y:350


                Rectangle {
                    x:-8
                    y:-8
                    width: parent.width + 20
                    height: parent.height + 15
                    radius: 40
                    color: "#eaeaea"
                    border.color: "#cccccc"
                }

                Image {
                    id: leftButton
                    anchors.verticalCenter: parent.verticalCenter
                    source: mouseLeft.pressed
                            ? resourceManager.imagePath + "/11_TIMER/a110_03.png"
                            : resourceManager.imagePath + "/11_TIMER/a110_01.png"
                    width: 58
                    height: 58
                    MouseArea {
                        id: mouseLeft
                        anchors.fill: parent
                        onClicked: prevPage()
                    }
                }

                Image {
                    id: rightButton
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    source: mouseRight.pressed
                            ? resourceManager.imagePath + "/11_TIMER/a110_04.png"
                            : resourceManager.imagePath + "/11_TIMER/a110_02.png"
                    width: 58
                    height: 58
                    MouseArea {
                        id: mouseRight
                        anchors.fill: parent
                        onClicked: nextPage()
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: (currentPageIndex + 1) + " / " + Math.ceil(100 / 9)
                    font.pixelSize: 20
                    color: local.pageColor
                }
            }
        }
    }

    Rectangle {
        width: 900
        height: 80
        border.color: "#999999"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 20
        anchors.leftMargin: 37
        color: "#cccccc"
        radius: 10

        Rectangle {
            width: 120
            height: parent.height
            color: "transparent"
            Rectangle{
                width: 20
                height: 20
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 15
                color: "transparent"
                Image {
                    anchors.fill: parent
                    source: resourceManager.imagePath + "/11_TIMER/a110_05.png"
                }
            }

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                leftPadding: 4
                font.pixelSize: 20
                font.bold: true
                text: resourceManager.label(38, 6, resourceManager.language)
                color: resourceManager.rgb(38, 6)
            }
            Rectangle{
                width: 2
                height: parent.height -2
                anchors.right: parent.right
                y: 1
                color: "#999999"
            }

        }

        Label {
            width: 740
            height: 60
            x:140
            y:10
            background: Rectangle {
                anchors.fill: parent
                color: "transparent"
            }

            color: resourceManager.rgb(38, 207 + currentHelpIndex)
            font.pixelSize: 20
            font.bold: true
            wrapMode: Text.WordWrap
            lineHeight: 1.5
            text: resourceManager.label(38, 207 + currentHelpIndex, resourceManager.language)
        }
    }
}
