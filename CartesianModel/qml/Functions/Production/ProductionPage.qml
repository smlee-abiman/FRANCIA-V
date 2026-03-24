import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Shapes 1.12
import Enums 1.0
import LinesCanvas 1.0
import "../../Widget"
import "../../Components/RemindDialog"
/*************************************************************************************
* Title: UI/UX of AEK
* File: ProductionPage.qml
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
Item {
    property bool pageActive: false
    width: parent.width
    height: parent.height
    property var viewModel: functionViewModel.productionView
    property int dayYMax: 500
    property int monthYMax: 2000

    onPageActiveChanged: {
        if (pageActive) {
            viewModel.startPageChanging();
            commonViewModel.pageChange(Enums.FunctionProductionMng)
            viewModel.activate()
            dispHour();
            dispDay();
        } else {
            viewModel.deactivate()
        }
    }

    Component.onDestruction: viewModel.deactivate()

    property int productExpress: 0 // 0:day 1:month

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

    Rectangle{
        x:3
        y:30
        width: 1018
        height: 600
        anchors.fill: parent.Center
        radius: 6
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"
    }//背景

    Rectangle{
        width: 1006
        height: 130
        x:6
        y:36
        radius: 12
        color: "#cccccc"
        Rectangle{
            width: 1006
            height: 10
            anchors.bottom: parent.bottom
            color: "#cccccc"
        }
        //分隔线
        Rectangle{
            width: 1
            height: 126
            x:300
            y:2
        }
        Rectangle{
            width: 1
            height: 126
            x:550
            y:2
        }
        Rectangle{
            width: 702
            height: 1
            x:302
            anchors.verticalCenter: parent.verticalCenter
        }

        Row{
            width: 300
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            spacing: 6
            Text {
                id: textProgNum
                width: 170
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.pixelSize: 20
                font.bold: true
                color: "black"
                text:resourceManager.label(46,2,local.language)
            }
            LimitNumericEditBox{
                width: 120
                height: 50
                textVisible: viewModel.dataLoaded
                color: opened ? "#cc3333" : "white"
                textColor: "black"
                borderColor: "#999999"
                borderWidth: 2
                fontPixelSize: 20
                keyLayoutType: 0
                max: "9999999"
                min: "0"
                value: resourceManager.formatData(viewModel.progNum, resourceManager.getPenSetIntValue("21", 6, 0))
                onOpen: {
                    textProgNum.color = resourceManager.dataColor(21, 1);
                }
                onClose: {
                    textProgNum.color = resourceManager.rgb(46,2);
                }
                onEnter: {
                    viewModel.writeProgNum(newValue)
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }//预定生产数
        Row{
            width: 248
            height: 64
            x:300
            spacing: 6
            Text {
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                font.pixelSize: 20
                color: "black"
                text: resourceManager.label(46,3,local.language)
            }
            Rectangle{
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
                border.color: "#999999"
                border.width: 2
                Text {
                    visible: viewModel.dataLoaded
                    anchors.centerIn: parent
                    z: 2
                    font.pixelSize: 20
                    color: resourceManager.dataColor(21, 2)
                    text: resourceManager.formatData(viewModel.prodNum, resourceManager.getPenSetIntValue("21", 6, 1))
                }
            }
        }//当前生产数
        Row{
            width: 248
            height: 64
            x:300
            y:65
            spacing: 6
            Text {
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                color: "black"
                font.pixelSize: 20
                text: resourceManager.label(46,5,local.language)
            }
            Rectangle{
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
                border.color: "#999999"
                Text {
                    visible: viewModel.dataLoaded
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    z: 2
                    font.pixelSize: 20
                    color: resourceManager.dataColor(21, 2)
                    text: resourceManager.formatData(viewModel.badCount, resourceManager.getPenSetIntValue("21", 6, 2))
                }
            }
        }//当前不良数
        Row{
            width: 454
            height: 64
            x:552
            spacing: 6
            Text {
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                color: "black"
                font.pixelSize: 20
                text: resourceManager.label(46,4,local.language)
            }
            ProgressBar{
                //visible: viewModel.dataLoaded
                id:pro1
                width: 180
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                background: Rectangle{
                    width: parent.width
                    height: parent.height
                    color: "white"
                }
                contentItem: Item {
                    Rectangle{
                        implicitWidth: pro1.visualPosition * parent.width
                        height: parent.height
                        color: "#00d656"
                        Text {
                            id: text1
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            anchors.verticalCenter: parent.verticalCenter
                            z: 2
                            font.pixelSize: 17
                            font.bold: true
                            text: resourceManager.formatData(viewModel.achievementRate, resourceManager.getPenSetIntValue("21", 6, 3))
                        }
                    }
                }
                value: viewModel.achievementRate / 100
            }
            Rectangle{
                width: 85
                height: 46
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
                border.color: "#999999"
                Text {
                    visible: viewModel.dataLoaded
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    z: 2
                    font.pixelSize: 20
                    color: resourceManager.dataColor(21, 2)
                    text: resourceManager.formatData(viewModel.achievementRate, resourceManager.getPenSetIntValue("21", 6, 3))
                }
            }
        }//生产达成率
        Row{
            width: 454
            height: 64
            x:552
            y:65
            spacing: 6
            Text {
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                color: "black"
                font.pixelSize: 20
                text: resourceManager.label(46,6,local.language)
            }
            ProgressBar{
                //visible: viewModel.dataLoaded
                id:pro2
                width: 180
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                background: Rectangle{
                    width: parent.width
                    height: parent.height
                    color: "white"
                }
                contentItem: Item {
                    Rectangle{
                        implicitWidth: pro2.visualPosition * parent.width
                        height: parent.height
                        color: "#00d656"
                        Text {
                            id: text2
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            anchors.verticalCenter: parent.verticalCenter
                            z: 2
                            font.pixelSize: 17
                            font.bold: true
                            text: resourceManager.formatData(viewModel.badRate, resourceManager.getPenSetIntValue("21", 6, 4))
                        }
                    }
                }
                value: viewModel.badRate / 100
            }
            Rectangle{
                width: 85
                height: 46
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
                border.color: "#999999"
                Text {
                    visible: viewModel.dataLoaded
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    z: 2
                    font.pixelSize: 20
                    color: resourceManager.dataColor(21, 2)
                    text: resourceManager.formatData(viewModel.badRate, resourceManager.getPenSetIntValue("21", 6, 4))
                }
            }
        }//不良品率
    }//1行

    Rectangle{
        width: 1006
        height: 130
        x:5
        y:174
        color: "#cccccc"
        //分隔线
        Rectangle{
            width: 1002
            height: 1
            x:2
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle{
            width: 1
            height: 126
            x:300
            y:2
        }

        Row{
            width: 300
            height: 64
            spacing: 6
            Text {
                id: textOneFetchNum
                width: 170
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                color: "black"
                font.pixelSize: 20
                text: resourceManager.label(46,7,local.language)
            }
            LimitNumericEditBox{
                textVisible: viewModel.dataLoaded
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                color: opened ? "#cc3333" : "white"
                textColor: resourceManager.dataColor(21, 2)
                borderColor: "#999999"
                fontPixelSize: 20
                keyLayoutType: 0
                max: "255"
                min: "1"
                value: resourceManager.formatData(viewModel.oneFetchNum, resourceManager.getPenSetIntValue("21", 6, 5))
                onOpen: {
                    textOneFetchNum.color = resourceManager.dataColor(21, 1);
                }
                onClose: {
                    textOneFetchNum.color = resourceManager.rgb(46,7);
                }
                onEnter: {
                    viewModel.writeOneFetchNum(newValue)
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }//一次取出数
        Row{
            width: 300
            height: 64
            y:65
            spacing: 6
            Text {
                id:textNotificationTime
                width: 170
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                font.pixelSize: 20
                color: "black"
                text: resourceManager.label(46,10,local.language)
            }
            LimitNumericEditBox{
                textVisible: viewModel.dataLoaded
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                color: opened ? "#cc3333" : "white"
                textColor: resourceManager.dataColor(21, 2)
                borderColor: "#999999"
                fontPixelSize: 20
                keyLayoutType: 0
                max: "99"
                min: "1"
                value: resourceManager.formatData(viewModel.notificationTime, resourceManager.getPenSetIntValue("21", 6, 7))
                onOpen: {
                    textNotificationTime.color = resourceManager.dataColor(21, 1);
                }
                onClose: {
                    textNotificationTime.color = resourceManager.rgb(46,7);
                }
                onEnter: {
                    viewModel.writeNotificationTime(newValue)
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }//提前告知时间
        Row{
            width: 248
            height: 64
            x:302
            spacing: 6
            Text {
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                color: "black"
                font.pixelSize: 20
                text: resourceManager.label(46,8,local.language)
            }
            Rectangle{
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
                border.color: "#999999"
                Text {
                    visible: viewModel.dataLoaded
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    z: 2
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.dataColor(21, 2)
                    text: resourceManager.formatData(viewModel.fetchNum, resourceManager.getPenSetIntValue("21", 6, 6))
                }
            }
        }//取出次数
        Row{
            width: 366
            height: 64
            x:302
            y:65
            spacing: 6
            Text {
                width: 120
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
                font.bold: true
                color: "black"
                font.pixelSize: 20
                text: resourceManager.label(46,11,local.language)
            }
            Rectangle{
                width: 240
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
                border.color: "#999999"
                Text {
                    visible: viewModel.dataLoaded
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    z: 2
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.dataColor(21, 2)
                    text: viewModel.prodEnd
                }
            }
        }//预计结束时刻
        Rectangle {
            id: buttonRect
            x: 860
            y: 67
            width: 130
            height: 60
            color: "#ffffff"
            border.color: "#999999"

            property bool pressed: false

            Text {
                anchors.centerIn: parent
                font.pixelSize: 23
                color: pressed ? "white" : "white"
                text: resourceManager.label(46, 13, local.language)
            }

            MouseArea {
                anchors.fill: parent
                onPressed: buttonRect.pressed = true
                onReleased: buttonRect.pressed = false
                onClicked: {
                    remind_dialog.open()
                    phyKeyManager.ctrlBuzzer()
                }
            }

            // 배경색 동적 변경
            onPressedChanged: {
                buttonRect.color = pressed ? "#cc3333" : "#ffffff"
            }
        }
    }//2行

    Rectangle{
        width: 1012
        height: 310
        x:6
        y:308
        color: "#ffffff"
        Rectangle{
            width: 1012
            height: 10
            anchors.top: parent.top
            color: "#ffffff"
        }
        Rectangle{
            anchors.centerIn: parent
            width: 1000
            height: 308
            color: "Transparent"
            Text {
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                color: resourceManager.rgb(46,12)
                font.pixelSize: 28
                text: resourceManager.label(46,12,local.language)
            }

            Rectangle{
                visible: productExpress == 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 40
                width: 900
                height: 200
                color: "#FFFFFF"
                border.width: 2
                border.color: "#999999"
                clip: true

                Shape{
                    id: gridShapeDay
                    width: parent.width - (parent.border.width * 2)
                    height: parent.height - (parent.border.width * 2)
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: parent.border.width
                    anchors.leftMargin: parent.border.width
                    ShapePath{
                        strokeColor: "gray"
                        strokeWidth: 1
                        dashPattern: [7, 7]
                        fillColor: "transparent"
                        startX: 0
                        startY: 0

                        PathMove { x: 0;    y: (gridShapeDay.height / 5) * 1 }
                        PathLine { x: gridShapeDay.width;  y: (gridShapeDay.height / 5) * 1 }
                        PathMove { x: 0;    y: (gridShapeDay.height / 5) * 2 }
                        PathLine { x: gridShapeDay.width;  y: (gridShapeDay.height / 5) * 2 }
                        PathMove { x: 0;    y: (gridShapeDay.height / 5) * 3 }
                        PathLine { x: gridShapeDay.width;  y: (gridShapeDay.height / 5) * 3 }
                        PathMove { x: 0;    y: (gridShapeDay.height / 5) * 4 }
                        PathLine { x: gridShapeDay.width;  y: (gridShapeDay.height / 5) * 4 }
                        PathMove { x: 0;    y: (gridShapeDay.height / 5) * 5 }
                        PathLine { x: gridShapeDay.width;  y: (gridShapeDay.height / 5) * 5 }

                        PathMove { x: (gridShapeDay.width / 23) * 1; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 1; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 2; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 2; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 3; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 3; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 4; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 4; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 5; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 5; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 6; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 6; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 7; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 7; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 8; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 8; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 9; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 9; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 10; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 10; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 11; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 11; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 12; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 12; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 13; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 13; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 14; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 14; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 15; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 15; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 16; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 16; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 17; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 17; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 18; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 18; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 19; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 19; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 20; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 20; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 21; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 21; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 22; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 22; y: gridShapeDay.height }
                        PathMove { x: (gridShapeDay.width / 23) * 23; y: 0 }
                        PathLine { x: (gridShapeDay.width / 23) * 23; y: gridShapeDay.height }
                    }
                }

                //グラフ表示領域
                LinesCanvas{
                    visible: viewModel.dataLoaded
                    id: graphCanvasDay
                    width: parent.width - (parent.border.width * 2)
                    height: parent.height - (parent.border.width * 2)
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: parent.border.width
                    anchors.leftMargin: parent.border.width
                    lineValues: viewModel.lineValuesDay
                    maxY: getMaxY(viewModel.lineValuesDayMax)
                    xDivisions: 24
                }
            }
            Rectangle{
                visible: productExpress == 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: 40
                height: 220
                color: "Transparent"
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((dayYMax/5)*0).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 39
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((dayYMax/5)*1).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 80
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((dayYMax/5)*2).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 120
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((dayYMax/5)*3).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 159
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((dayYMax/5)*4).toString()
                    }
                }
            }
            Rectangle{
                visible: productExpress == 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                width: 950
                height: 30
                color: "Transparent"
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 55
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour11
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 133
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour10
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 211
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour9
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 289
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour8
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 367
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour7
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 445
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour6
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 523
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour5
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 601
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour4
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 679
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour3
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 757
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour2
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 835
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour1
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 913
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textHour0
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
            }
            Rectangle{
                visible: productExpress == 1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 40
                width: 900
                height: 200
                color: "#FFFFFF"
                border.color: "#999999"
                clip: true

                Shape{
                    id: gridShapeMonth
                    width: parent.width - (parent.border.width * 2)
                    height: parent.height - (parent.border.width * 2)
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: parent.border.width
                    anchors.leftMargin: parent.border.width
                    ShapePath{
                        strokeColor: "gray"
                        strokeWidth: 1
                        dashPattern: [7, 7]
                        fillColor: "transparent"
                        startX: 0
                        startY: 0

                        PathMove { x: 0;    y: (gridShapeMonth.height / 5) * 1 }
                        PathLine { x: gridShapeMonth.width;  y: (gridShapeMonth.height / 5) * 1 }
                        PathMove { x: 0;    y: (gridShapeMonth.height / 5) * 2 }
                        PathLine { x: gridShapeMonth.width;  y: (gridShapeMonth.height / 5) * 2 }
                        PathMove { x: 0;    y: (gridShapeMonth.height / 5) * 3 }
                        PathLine { x: gridShapeMonth.width;  y: (gridShapeMonth.height / 5) * 3 }
                        PathMove { x: 0;    y: (gridShapeMonth.height / 5) * 4 }
                        PathLine { x: gridShapeMonth.width;  y: (gridShapeMonth.height / 5) * 4 }
                        PathMove { x: 0;    y: (gridShapeMonth.height / 5) * 5 }
                        PathLine { x: gridShapeMonth.width;  y: (gridShapeMonth.height / 5) * 5 }

                        PathMove { x: (gridShapeMonth.width / 31) * 1; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 1; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 2; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 2; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 3; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 3; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 4; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 4; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 5; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 5; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 6; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 6; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 7; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 7; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 8; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 8; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 9; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 9; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 10; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 10; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 11; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 11; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 12; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 12; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 13; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 13; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 14; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 14; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 15; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 15; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 16; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 16; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 17; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 17; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 18; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 18; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 19; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 19; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 20; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 20; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 21; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 21; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 22; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 22; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 23; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 23; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 24; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 24; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 25; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 25; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 26; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 26; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 27; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 27; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 28; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 28; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 29; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 29; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 30; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 30; y: gridShapeMonth.height }
                        PathMove { x: (gridShapeMonth.width / 31) * 31; y: 0 }
                        PathLine { x: (gridShapeMonth.width / 31) * 31; y: gridShapeMonth.height }
                    }
                }

                //グラフ表示領域
                LinesCanvas{
                    visible: viewModel.dataLoaded
                    id: graphCanvasMonth
                    width: parent.width - (parent.border.width * 2)
                    height: parent.height - (parent.border.width * 2)
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.topMargin: parent.border.width
                    anchors.leftMargin: parent.border.width
                    lineValues: viewModel.lineValuesMonth
                    maxY: getMaxY(viewModel.lineValuesMonthMax)
                    xDivisions: 32
                }
            }
            Rectangle{
                visible: productExpress == 1
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: 40
                height: 220
                color: "Transparent"
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((monthYMax/5)*0).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 39
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((monthYMax/5)*1).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 80
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((monthYMax/5)*2).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 120
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((monthYMax/5)*3).toString()
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 159
                    width: 40
                    height: 20
                    color: "Transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: ((monthYMax/5)*4).toString()
                    }
                }
            }
            Rectangle{
                visible: productExpress == 1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                width: 950
                height: 30
                color: "Transparent"
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay31
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 46
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay30
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 75
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay29
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 103
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay28
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 133
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay27
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 162
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay26
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 191
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay25
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 220
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay24
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 249
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay23
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 277
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay22
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 306
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay21
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 335
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay20
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 364
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay19
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 393
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay18
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 422
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay17
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 451
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay16
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 480
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay15
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 509
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay14
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 538
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay13
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 567
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay12
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 596
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay11
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 625
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay10
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 654
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay9
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 683
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay8
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 712
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay7
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 741
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay6
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 770
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay5
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 799
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay4
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 828
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay3
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 857
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay2
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 886
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay1
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
                Rectangle{
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 915
                    width: 20
                    height: 20
                    color: "Transparent"
                    Text {
                        id: textDay0
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                    }
                }
            }
        }
        Rectangle{
            width: 124
            height: 40
            x:740
            y:12
            border.color: "#999999"
            border.width: 2
            color: (productExpress == 0) ? "#cc3333" : "#cccccc"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    dispHour();
                    productExpress = 0;
                    phyKeyManager.ctrlBuzzer()
                }
            }
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 21
                text: resourceManager.label(46,14,local.language)
                color: (productExpress == 0) ? "#ffffff" : "black"
            }
        }
        Rectangle{
            width: 124
            height: 40
            x:876
            y:12
            border.color: "#999999"
            color: (productExpress == 1) ? "#cc3333" : "#cccccc"
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    dispDay();
                    productExpress = 1;
                    phyKeyManager.ctrlBuzzer()
                }
            }
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 21
                text: resourceManager.label(46,15,local.language)
                color: (productExpress == 1) ? "#ffffff" : "black"
            }
        }
    }//生产管理图

    Remind_Dialog{
        id:remind_dialog
        message: resourceManager.label(46,16,local.language)

        onPressed: {
            commonViewModel.modbusMemblemSwControl(true, 76)
        }
        onReleased: {
            commonViewModel.modbusMemblemSwControl(false, 76)
        }
    }

    function dispHour(){
        viewModel.readDateTime();
        viewModel.readDayData();

        textHour0.text = viewModel.getHourStr(0);
        textHour1.text = viewModel.getHourStr(2);
        textHour2.text = viewModel.getHourStr(4);
        textHour3.text = viewModel.getHourStr(6);
        textHour4.text = viewModel.getHourStr(8);
        textHour5.text = viewModel.getHourStr(10);
        textHour6.text = viewModel.getHourStr(12);
        textHour7.text = viewModel.getHourStr(14);
        textHour8.text = viewModel.getHourStr(16);
        textHour9.text = viewModel.getHourStr(18);
        textHour10.text = viewModel.getHourStr(20);
        textHour11.text = viewModel.getHourStr(22);
    }

    function dispDay(){
        viewModel.readDateTime();
        viewModel.readMonthData();

        textDay0.text = viewModel.getDayStr(0);
        textDay1.text = viewModel.getDayStr(1);
        textDay2.text = viewModel.getDayStr(2);
        textDay3.text = viewModel.getDayStr(3);
        textDay4.text = viewModel.getDayStr(4);
        textDay5.text = viewModel.getDayStr(5);
        textDay6.text = viewModel.getDayStr(6);
        textDay7.text = viewModel.getDayStr(7);
        textDay8.text = viewModel.getDayStr(8);
        textDay9.text = viewModel.getDayStr(9);
        textDay10.text = viewModel.getDayStr(10);
        textDay11.text = viewModel.getDayStr(11);
        textDay12.text = viewModel.getDayStr(12);
        textDay13.text = viewModel.getDayStr(13);
        textDay14.text = viewModel.getDayStr(14);
        textDay15.text = viewModel.getDayStr(15);
        textDay16.text = viewModel.getDayStr(16);
        textDay17.text = viewModel.getDayStr(17);
        textDay18.text = viewModel.getDayStr(18);
        textDay19.text = viewModel.getDayStr(19);
        textDay20.text = viewModel.getDayStr(20);
        textDay21.text = viewModel.getDayStr(21);
        textDay22.text = viewModel.getDayStr(22);
        textDay23.text = viewModel.getDayStr(23);
        textDay24.text = viewModel.getDayStr(24);
        textDay25.text = viewModel.getDayStr(25);
        textDay26.text = viewModel.getDayStr(26);
        textDay27.text = viewModel.getDayStr(27);
        textDay28.text = viewModel.getDayStr(28);
        textDay29.text = viewModel.getDayStr(29);
        textDay30.text = viewModel.getDayStr(30);
        textDay31.text = viewModel.getDayStr(31);
    }

    function getMaxY(dataMax){
        var maxY = 400;

        while(maxY < dataMax){
            maxY *= 2;
        }

        return maxY;
    }
}
