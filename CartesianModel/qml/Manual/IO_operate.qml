import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Shapes 1.12
import Enums 1.0
import QtGraphicalEffects 1.0
import "../Widget"

/*************************************************************************************
* Title: UI/UX of AEK
* File: IO_operate.qml
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
Dialog {
    id:io_operate
    width:1024
    height: 698
    modal: false
    closePolicy: Popup.CloseOnEscape
    property int in_page: manualViewModel.ioView.inPageNum
    property int in_page_max: manualViewModel.ioView.inPageMaxNum
    property int out_page: manualViewModel.ioView.outPageNum
    property int out_page_max: manualViewModel.ioView.outPageMaxNum


    background: Rectangle{
        color: "#FFFFFF"
        ImageButton{
            id:error_dialog_close
            anchors.right: parent.right
            anchors.rightMargin: 15
            y:8
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
            downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
            onClicked: {
                io_operate.close()
            }
        }//关闭弹窗
    }
///----------------------------------------------------------------------------
    onOpened: {
        commonViewModel.pageChange(Enums.ManualIO)
        manualViewModel.ioView.activate()
    }
    onClosed: {
        manualViewModel.ioView.deactivate()
        commonViewModel.backPrePage()
    }


///----------------------------------------------------------------------------
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
///----------------------------------------------------------------------------
    Rectangle{
        x: -4

        width: 150
        height: 40
        color: "#cc3333"
        border.color: "#999999"
        Text {
            anchors.centerIn: parent
            font.pixelSize: 25
            font.bold: true
            text: resourceManager.label(31,4, local.language)
            color: "white"
        }
    }

    Rectangle{
        id:io_input
        x:-4
        y:40
        width: 500
        height: 640
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"

        Grid{
            x:8
            y:24
            columns: 2
            rows: 9
            rowSpacing: 10
            columnSpacing: 4

            Repeater{
                model: manualViewModel.ioView.getInDataList(in_page)
                Row{
                    spacing: 2
                    visible: modelData.index !== -1

                    Rectangle{
                        width: 240
                        height: 50
                        color: "#eaeaea"
                        border.color: "#999999"
                        radius: 5

                        Row{
                            anchors.fill: parent
                            anchors.leftMargin: 10     // 램프가 좌측에서 5px 떨어짐
                            anchors.rightMargin: 5
                            spacing: 8

                            Image {
                                width: 22
                                height: 22
                                anchors.verticalCenter: parent.verticalCenter
                                fillMode: Image.PreserveAspectFit
                                source: modelData.value === false
                                        ? resourceManager.imagePath + "/08_Monitor/d080_12.png"
                                        : resourceManager.imagePath + "/08_Monitor/d080_11.png"
                            }

                            Text{
                                height: parent.height
                                // 남는 폭 자동으로 쓰고 싶으면 anchors로 처리
                                anchors.verticalCenter: parent.verticalCenter
                                verticalAlignment: Text.AlignVCenter   // 배경(50) 중앙에 글자 세로 정렬
                                horizontalAlignment: Text.AlignLeft
                                leftPadding: 2
                                font.pixelSize: 20
                                font.bold: true
                                wrapMode: Text.WordWrap
                                text: resourceManager.symbolAt(modelData.index) + " "
                                      + resourceManager.getMessageStringValue("1", modelData.index, local.language)
                                color: "black"
                            }
                        }
                    }
                }

            }
        }

        Rectangle{
            anchors.bottom: parent.bottom
            width: 500
            height: 65
            color: "transparent"
            border.color: "#999999"

            Text {
                anchors.centerIn: parent
                font.pixelSize: 24
                text: (in_page + 1) + "/" + in_page_max
            }

            TabBar{
                id: input_tab
                width: 460
                height: 48
                anchors.centerIn: parent
                spacing: 359
                background: Rectangle{ color: "transparent" }

                TabButton{
                    id: input_tab1
                    width: 46
                    height: 46
                    padding: 0

                    background: Item {}   // 배경은 비워두기

                    contentItem: Image {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -6   // ← 값 +면 아래로, -면 위로 (2~4 정도부터)
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: input_tab1.pressed
                                ? resourceManager.imagePath + "/06_MANU/a060_01.png"
                                : resourceManager.imagePath + "/06_MANU/a060_02.png"
                    }

                    onClicked: {
                        if (manualViewModel.ioView.inPageNum <= 0)
                            manualViewModel.ioView.inPageNum = (in_page_max - 1)
                        else
                            manualViewModel.ioView.inPageNum--
                        phyKeyManager.ctrlBuzzer()
                    }
                } // left button

                TabButton{
                    id: input_tab2
                    width: 46
                    height: 46
                    padding: 0

                    background: Item {}

                    contentItem: Image {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -6   // ← 값 +면 아래로, -면 위로 (2~4 정도부터)
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: input_tab2.pressed
                                ? resourceManager.imagePath + "/06_MANU/a060_03.png"
                                : resourceManager.imagePath + "/06_MANU/a060_04.png"
                    }

                    onClicked: {
                        if (manualViewModel.ioView.inPageNum >= (in_page_max - 1))
                            manualViewModel.ioView.inPageNum = 0
                        else
                            manualViewModel.ioView.inPageNum++
                        phyKeyManager.ctrlBuzzer()
                    }
                } // right button
            }
        } // 堆叠窗口切换

    }
///----------------------------------------------------------------------------
    Rectangle{
        width: 150
        height: 40
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 79
        color: "#cc3333"
        border.color: "#999999"
        Text {
            anchors.centerIn: parent
            font.pixelSize: 25
            font.bold: true
            color: "white"
            text: resourceManager.label(31,5, local.language)
        }
    }
    Rectangle{
        id:io_output
        x:504
        y:40
        width: 500
        height: 640
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"


        Rectangle{
            id:output_signal_page
            width: 200
            height: 576
            color: "transparent"
            Column{
                y:45
                spacing: 38
                Repeater{
                    model: manualViewModel.ioView.getOutDataSignalList(out_page)
                    Rectangle{
                        x:7
                        width: 200
                        height: 60
                        color: "#eaeaea"
                        border.color: "#999999"
                        radius: 5
                        visible: modelData.di !== -1
                        Image {
                            width: 22
                            height: 22
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            source: modelData.value
                                    ? resourceManager.imagePath + "/08_Monitor/d080_11.png"
                                    : resourceManager.imagePath + "/08_Monitor/d080_12.png"
                        }
                        Text {
                            anchors.left: parent.left
                            wrapMode: Text.WordWrap
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 38
                            text: (modelData.di !== -1)
                                  ? resourceManager.symbolAt(modelData.di) + " " +
                                    resourceManager.getMessageStringValue("1", modelData.di, local.language)
                                  : ""
                            font.pixelSize: 20
                            font.bold: true
                            color: modelData.di ? resourceManager.dataColor(6, 1) : resourceManager.dataColor(6, 2)
                        }
                    }
                }
            }
        }//指示灯窗口

        Rectangle{
            id:output_page
            x:202
            width: 301
            height: 576
            color: "transparent"
            border.width: 0
            Grid{
                anchors.horizontalCenter: parent.horizontalCenter
                y:45
                columns: 2
                rows: 5
                rowSpacing: 38
                columnSpacing: 6
                flow: Grid.TopToBottom
                Repeater{
                    id: out_item
                    model: manualViewModel.ioView.getOutDataList(out_page)

                    Rectangle {
                        width: 135
                        height: 60
                        color: "#eaeaea"
                        border.color: "#999999"
                        radius: 5
                        visible: modelData.keyNo ? true : false

                        Text {
                            anchors.fill: parent
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 20
                            font.bold: true
                            color: "black"
                            text: resourceManager.label(
                                      31,
                                      54 + ((out_page * 10) + index),
                                      local.language
                                  )
                        }

                        MouseArea{
                            anchors.fill: parent

                            onPressed: {
                                parent.color = "#ffffff"   // 눌림 효과
                                commonViewModel.modbusMemblemSwControl(true, modelData.keyNo)
                            }

                            onReleased: {
                                parent.color = "#eaeaea"
                                commonViewModel.modbusMemblemSwControl(false, modelData.keyNo)
                            }

                            onClicked: phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }
        }//输出堆叠窗口

        Rectangle{
            anchors.bottom: parent.bottom
            width: 500
            height: 65
            color: "transparent"
            border.color: "#999999"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 24
                text: (out_page + 1) + "/" + out_page_max
            }
            TabBar{
                id: output_tab
                width: 460
                height: 48
                anchors.centerIn: parent
                spacing: 359
                background: Rectangle{ color: "transparent" }

                TabButton{
                    id: output_tab1
                    width: 46
                    height: 46
                    padding: 0
                    background: Item {}

                    contentItem: Image {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -6   // 필요하면 2~6 사이로 조절
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: output_tab1.pressed
                                ? resourceManager.imagePath + "/06_MANU/a060_01.png"
                                : resourceManager.imagePath + "/06_MANU/a060_02.png"
                    }

                    onClicked: {
                        if (manualViewModel.ioView.outPageNum <= 0) {
                            manualViewModel.ioView.outPageNum = (out_page_max - 1)
                            commonViewModel.changeScreenId(6, 1)
                        } else {
                            manualViewModel.ioView.outPageNum--
                            if (manualViewModel.ioView.outPageNum < 6) {
                                commonViewModel.changeScreenId(6, 0)
                            }
                        }
                        phyKeyManager.ctrlBuzzer()
                    }
                } // left button

                TabButton{
                    id: output_tab2
                    width: 46
                    height: 46
                    padding: 0
                    background: Item {}

                    contentItem: Image {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -6   // 필요하면 2~6 사이로 조절
                        width: parent.width
                        height: parent.height
                        fillMode: Image.PreserveAspectFit
                        source: output_tab2.pressed
                                ? resourceManager.imagePath + "/06_MANU/a060_03.png"
                                : resourceManager.imagePath + "/06_MANU/a060_04.png"
                    }

                    onClicked: {
                        if (manualViewModel.ioView.outPageNum >= (out_page_max - 1)) {
                            manualViewModel.ioView.outPageNum = 0
                            commonViewModel.changeScreenId(6, 0)
                        } else {
                            manualViewModel.ioView.outPageNum++
                            if (manualViewModel.ioView.outPageNum > 5) {
                                commonViewModel.changeScreenId(6, 1)
                            }
                        }
                        phyKeyManager.ctrlBuzzer()
                    }
                } // right button
            }

        }//堆叠窗口切换
    }
}
