import QtQml 2.12
import QtQuick 2.12
import QtQuick.Shapes 1.12
import GraphCanvas 1.0

Rectangle {
    id: root
    property var pageModel:monitorViewModel.drvMonitorView
    property int areaWidth:791
    property int areaHeight:528
    property color bgColor:"black"
    property int borderWidth:4
    property color borderColor:"#63638F"
    property int rectRadius:2
    property bool timerSetVisible:false
    property int timerSettingToggle:0
    property var timerValue:["0.2s", "0.5s", "1.0s", "2.0s"]
    property int verticalWidth200ms:32
    property double horizontalWidth200ms:32.0
    property int verticalWidth500ms:32
    property double horizontalWidth500ms:12.8
    property int verticalWidth1s:32
    property double horizontalWidth1s:12.8
    property int verticalWidth2s:32
    property double horizontalWidth2s:12.8
    //property int blueGraphDrawIndex:0
    //property int redGraphDrawIndex:0
    property var drawIndexMax:[21, 51, 101, 201]
    property int axisIndex:0
    property var speedPeakData:pageModel.axisSpeedPeak
    property var torquePeakData:pageModel.axisTorquePeak
    property bool graphMode:false

    width:  root.areaWidth
    height: root.areaHeight
    color: root.bgColor
    border.width: root.borderWidth
    border.color: root.borderColor
    radius:root.rectRadius
    clip: true

    property real minY: -100        //下限
    property real maxY: 100         //上限
    property real xDivisions: 300   //横の分割数
    property var redLineValues: new Array   //赤線の値(Array)
    property var blueLineValues: new Array  //青線の値(Array)

    property real xGridLineStep: 9  //Grid線の横分割数
    property real yGridLineStep: 7  //Grid線の縦分割数
    property int step: 4            //10000件データの読み飛ばすStep数

    property real speedMinY: -100        //速度下限
    property real speedMaxY: -100        //速度上限
    property real torqueMinY: -100        //トルク下限
    property real torqueMaxY: -100        //トルク上限

    Row{
        x:10
        y:6
        spacing: 4
        Rectangle {
            id:oneAxisSpeedRect
            width:40
            height:5
            anchors.verticalCenter: parent.verticalCenter
            color:"#0070C0"
            border.color:"#0070C0"
            clip: true
        }
        Rectangle {
            id:oneAxisSpeedPeakValue
            width:80
            height:20
            anchors.verticalCenter: parent.verticalCenter
            color:"#FFFFFF"
            border.color:"grey"
            clip: true
            Text {
                id:oneAxisSpeedPeakData
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                text:root.speedPeakData[0]
                color:"black"
            }
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 4
                font.pixelSize: 12
                text: "rpm"
            }
        }
    }
    Row{
        x:150
        y:6
        spacing: 4
        Rectangle {
            id:oneAxisTorqueRect
            width:40
            height:5
            anchors.verticalCenter: parent.verticalCenter
            color:"#D50000"
            border.color:"#D50000"
            clip: true
        }
        Rectangle {
            id:oneAxisTorquePeakValue
            width:80
            height:20
            anchors.verticalCenter: parent.verticalCenter
            color:"#FFFFFF"
            border.color:"grey"
            clip: true
            Text {
                id:oneAxisTorquePeakData
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                text:root.torquePeakData[0]
                color:"black"
            }
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.verticalCenterOffset: -2
                anchors.rightMargin: 4
                font.pixelSize: 12
                text: "%"
            }
        }
    }

    //中央の線
    Shape {
        id:gridCenterShape1
        visible: (root.timerSettingToggle === 0)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width

        ShapePath {
            strokeColor: "white"
            strokeWidth: 3
            strokeStyle: ShapePath.SolidLine
            startX: 0
            startY: (gridShape1.height / 2)
            PathLine {
                x: gridShape1.width
                y: (gridShape1.height / 2)
            }
        }
    }
    Shape {
        id:gridCenterShape2
        visible: (root.timerSettingToggle === 1)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width

        ShapePath {
            strokeColor: "white"
            strokeWidth: 2
            strokeStyle: ShapePath.SolidLine
            startX: 0
            startY: (gridShape2.height / 2)
            PathLine {
                x: gridShape2.width
                y: (gridShape2.height / 2)
            }
        }
    }
    Shape {
        id:gridCenterShape3
        visible: (root.timerSettingToggle === 2)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width

        ShapePath {
            strokeColor: "white"
            strokeWidth: 2
            strokeStyle: ShapePath.SolidLine
            startX: 0
            startY: (gridShape3.height / 2)
            PathLine {
                x: gridShape3.width
                y: (gridShape3.height / 2)
            }
        }
    }
    Shape {
        id:gridCenterShape4
        visible: (root.timerSettingToggle === 3)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width

        ShapePath {
            strokeColor: "white"
            strokeWidth: 2
            strokeStyle: ShapePath.SolidLine
            startX: 0
            startY: (gridShape4.height / 2)
            PathLine {
                x: gridShape4.width
                y: (gridShape4.height / 2)
            }
        }
    }
    Shape{
        id: gridShape1
        visible: (root.timerSettingToggle === 0)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width
        ShapePath{
            strokeColor: "gray"
            strokeWidth: 1
            strokeStyle: ShapePath.DashLine
            dashPattern: [7, 7]
            fillColor: "transparent"
            startX: 0
            startY: 32
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 0 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 1 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 1 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 2 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 2 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 3 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 3 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 4 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 4 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 5 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 5 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 6 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 6 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 7 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 7 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 8 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 8 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 9 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 9 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 10 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 10 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 11 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 11 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 12 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 12 }
            PathMove { x: 0;    y: 32 + root.verticalWidth200ms * 13 }
            PathLine { x: 783;  y: 32 + root.verticalWidth200ms * 13 }

            PathMove { x: root.horizontalWidth200ms * 1; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 1; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 2; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 2; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 3; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 3; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 4; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 4; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 5; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 5; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 6; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 6; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 7; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 7; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 8; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 8; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 9; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 9; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 10; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 10; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 11; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 11; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 12; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 12; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 13; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 13; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 14; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 14; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 15; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 15; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 16; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 16; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 17; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 17; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 18; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 18; y: 458 }
            PathMove { x: root.horizontalWidth200ms * 19; y: 0 }
            PathLine { x: root.horizontalWidth200ms * 19; y: 458 }
        }
    }
    Shape{
        id: gridShape2
        visible: (root.timerSettingToggle === 1)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width
        ShapePath{
            strokeColor: "gray"
            strokeWidth: 1
            strokeStyle: ShapePath.DashLine
            dashPattern: [7, 7]
            fillColor: "transparent"
            startX: 0
            startY: 32
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 0 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 1 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 1 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 2 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 2 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 3 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 3 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 4 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 4 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 5 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 5 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 6 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 6 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 7 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 7 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 8 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 8 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 9 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 9 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 10 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 10 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 11 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 11 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 12 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 12 }
            PathMove { x: 0;    y: 32 + root.verticalWidth500ms * 13 }
            PathLine { x: 783;  y: 32 + root.verticalWidth500ms * 13 }

            PathMove { x: root.horizontalWidth500ms * 1; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 1; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 2; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 2; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 3; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 3; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 4; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 4; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 5; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 5; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 6; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 6; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 7; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 7; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 8; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 8; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 9; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 9; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 10; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 10; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 11; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 11; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 12; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 12; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 13; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 13; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 14; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 14; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 15; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 15; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 16; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 16; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 17; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 17; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 18; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 18; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 19; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 19; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 20; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 20; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 21; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 21; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 22; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 22; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 23; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 23; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 24; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 24; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 25; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 25; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 26; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 26; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 27; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 27; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 28; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 28; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 29; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 29; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 30; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 30; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 31; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 31; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 32; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 32; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 33; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 33; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 34; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 34; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 35; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 35; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 36; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 36; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 37; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 37; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 38; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 38; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 39; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 39; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 40; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 40; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 41; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 41; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 42; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 42; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 43; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 43; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 44; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 44; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 45; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 45; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 46; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 46; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 47; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 47; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 48; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 48; y: 458 }
            PathMove { x: root.horizontalWidth500ms * 49; y: 0 }
            PathLine { x: root.horizontalWidth500ms * 49; y: 458 }
        }
    }
    Shape{
        id: gridShape3
        visible: (root.timerSettingToggle === 2)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width
        ShapePath{
            strokeColor: "gray"
            strokeWidth: 1
            strokeStyle: ShapePath.DashLine
            dashPattern: [7, 7]
            fillColor: "transparent"
            startX: 0
            startY: 32
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 0 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 1 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 1 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 2 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 2 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 3 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 3 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 4 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 4 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 5 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 5 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 6 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 6 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 7 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 7 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 8 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 8 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 9 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 9 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 10 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 10 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 11 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 11 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 12 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 12 }
            PathMove { x: 0;    y: 32 + root.verticalWidth1s * 13 }
            PathLine { x: 783;  y: 32 + root.verticalWidth1s * 13 }

            PathMove { x: root.horizontalWidth1s * 1; y: 0 }
            PathLine { x: root.horizontalWidth1s * 1; y: 458 }
            PathMove { x: root.horizontalWidth1s * 2; y: 0 }
            PathLine { x: root.horizontalWidth1s * 2; y: 458 }
            PathMove { x: root.horizontalWidth1s * 3; y: 0 }
            PathLine { x: root.horizontalWidth1s * 3; y: 458 }
            PathMove { x: root.horizontalWidth1s * 4; y: 0 }
            PathLine { x: root.horizontalWidth1s * 4; y: 458 }
            PathMove { x: root.horizontalWidth1s * 5; y: 0 }
            PathLine { x: root.horizontalWidth1s * 5; y: 458 }
            PathMove { x: root.horizontalWidth1s * 6; y: 0 }
            PathLine { x: root.horizontalWidth1s * 6; y: 458 }
            PathMove { x: root.horizontalWidth1s * 7; y: 0 }
            PathLine { x: root.horizontalWidth1s * 7; y: 458 }
            PathMove { x: root.horizontalWidth1s * 8; y: 0 }
            PathLine { x: root.horizontalWidth1s * 8; y: 458 }
            PathMove { x: root.horizontalWidth1s * 9; y: 0 }
            PathLine { x: root.horizontalWidth1s * 9; y: 458 }
            PathMove { x: root.horizontalWidth1s * 10; y: 0 }
            PathLine { x: root.horizontalWidth1s * 10; y: 458 }
            PathMove { x: root.horizontalWidth1s * 11; y: 0 }
            PathLine { x: root.horizontalWidth1s * 11; y: 458 }
            PathMove { x: root.horizontalWidth1s * 12; y: 0 }
            PathLine { x: root.horizontalWidth1s * 12; y: 458 }
            PathMove { x: root.horizontalWidth1s * 13; y: 0 }
            PathLine { x: root.horizontalWidth1s * 13; y: 458 }
            PathMove { x: root.horizontalWidth1s * 14; y: 0 }
            PathLine { x: root.horizontalWidth1s * 14; y: 458 }
            PathMove { x: root.horizontalWidth1s * 15; y: 0 }
            PathLine { x: root.horizontalWidth1s * 15; y: 458 }
            PathMove { x: root.horizontalWidth1s * 16; y: 0 }
            PathLine { x: root.horizontalWidth1s * 16; y: 458 }
            PathMove { x: root.horizontalWidth1s * 17; y: 0 }
            PathLine { x: root.horizontalWidth1s * 17; y: 458 }
            PathMove { x: root.horizontalWidth1s * 18; y: 0 }
            PathLine { x: root.horizontalWidth1s * 18; y: 458 }
            PathMove { x: root.horizontalWidth1s * 19; y: 0 }
            PathLine { x: root.horizontalWidth1s * 19; y: 458 }
            PathMove { x: root.horizontalWidth1s * 20; y: 0 }
            PathLine { x: root.horizontalWidth1s * 20; y: 458 }
            PathMove { x: root.horizontalWidth1s * 21; y: 0 }
            PathLine { x: root.horizontalWidth1s * 21; y: 458 }
            PathMove { x: root.horizontalWidth1s * 22; y: 0 }
            PathLine { x: root.horizontalWidth1s * 22; y: 458 }
            PathMove { x: root.horizontalWidth1s * 23; y: 0 }
            PathLine { x: root.horizontalWidth1s * 23; y: 458 }
            PathMove { x: root.horizontalWidth1s * 24; y: 0 }
            PathLine { x: root.horizontalWidth1s * 24; y: 458 }
            PathMove { x: root.horizontalWidth1s * 25; y: 0 }
            PathLine { x: root.horizontalWidth1s * 25; y: 458 }
            PathMove { x: root.horizontalWidth1s * 26; y: 0 }
            PathLine { x: root.horizontalWidth1s * 26; y: 458 }
            PathMove { x: root.horizontalWidth1s * 27; y: 0 }
            PathLine { x: root.horizontalWidth1s * 27; y: 458 }
            PathMove { x: root.horizontalWidth1s * 28; y: 0 }
            PathLine { x: root.horizontalWidth1s * 28; y: 458 }
            PathMove { x: root.horizontalWidth1s * 29; y: 0 }
            PathLine { x: root.horizontalWidth1s * 29; y: 458 }
            PathMove { x: root.horizontalWidth1s * 30; y: 0 }
            PathLine { x: root.horizontalWidth1s * 30; y: 458 }
            PathMove { x: root.horizontalWidth1s * 31; y: 0 }
            PathLine { x: root.horizontalWidth1s * 31; y: 458 }
            PathMove { x: root.horizontalWidth1s * 32; y: 0 }
            PathLine { x: root.horizontalWidth1s * 32; y: 458 }
            PathMove { x: root.horizontalWidth1s * 33; y: 0 }
            PathLine { x: root.horizontalWidth1s * 33; y: 458 }
            PathMove { x: root.horizontalWidth1s * 34; y: 0 }
            PathLine { x: root.horizontalWidth1s * 34; y: 458 }
            PathMove { x: root.horizontalWidth1s * 35; y: 0 }
            PathLine { x: root.horizontalWidth1s * 35; y: 458 }
            PathMove { x: root.horizontalWidth1s * 36; y: 0 }
            PathLine { x: root.horizontalWidth1s * 36; y: 458 }
            PathMove { x: root.horizontalWidth1s * 37; y: 0 }
            PathLine { x: root.horizontalWidth1s * 37; y: 458 }
            PathMove { x: root.horizontalWidth1s * 38; y: 0 }
            PathLine { x: root.horizontalWidth1s * 38; y: 458 }
            PathMove { x: root.horizontalWidth1s * 39; y: 0 }
            PathLine { x: root.horizontalWidth1s * 39; y: 458 }
            PathMove { x: root.horizontalWidth1s * 40; y: 0 }
            PathLine { x: root.horizontalWidth1s * 40; y: 458 }
            PathMove { x: root.horizontalWidth1s * 41; y: 0 }
            PathLine { x: root.horizontalWidth1s * 41; y: 458 }
            PathMove { x: root.horizontalWidth1s * 42; y: 0 }
            PathLine { x: root.horizontalWidth1s * 42; y: 458 }
            PathMove { x: root.horizontalWidth1s * 43; y: 0 }
            PathLine { x: root.horizontalWidth1s * 43; y: 458 }
            PathMove { x: root.horizontalWidth1s * 44; y: 0 }
            PathLine { x: root.horizontalWidth1s * 44; y: 458 }
            PathMove { x: root.horizontalWidth1s * 45; y: 0 }
            PathLine { x: root.horizontalWidth1s * 45; y: 458 }
            PathMove { x: root.horizontalWidth1s * 46; y: 0 }
            PathLine { x: root.horizontalWidth1s * 46; y: 458 }
            PathMove { x: root.horizontalWidth1s * 47; y: 0 }
            PathLine { x: root.horizontalWidth1s * 47; y: 458 }
            PathMove { x: root.horizontalWidth1s * 48; y: 0 }
            PathLine { x: root.horizontalWidth1s * 48; y: 458 }
            PathMove { x: root.horizontalWidth1s * 49; y: 0 }
            PathLine { x: root.horizontalWidth1s * 49; y: 458 }
        }
    }
    Shape{
        id: gridShape4
        visible: (root.timerSettingToggle === 3)
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width
        ShapePath{
            strokeColor: "gray"
            strokeWidth: 1
            strokeStyle: ShapePath.DashLine
            dashPattern: [7, 7]
            fillColor: "transparent"
            startX: 0
            startY: 32
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 0 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 1 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 1 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 2 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 2 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 3 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 3 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 4 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 4 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 5 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 5 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 6 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 6 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 7 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 7 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 8 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 8 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 9 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 9 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 10 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 10 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 11 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 11 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 12 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 12 }
            PathMove { x: 0;    y: 32 + root.verticalWidth2s * 13 }
            PathLine { x: 783;  y: 32 + root.verticalWidth2s * 13 }

            PathMove { x: root.horizontalWidth2s * 1; y: 0 }
            PathLine { x: root.horizontalWidth2s * 1; y: 458 }
            PathMove { x: root.horizontalWidth2s * 2; y: 0 }
            PathLine { x: root.horizontalWidth2s * 2; y: 458 }
            PathMove { x: root.horizontalWidth2s * 3; y: 0 }
            PathLine { x: root.horizontalWidth2s * 3; y: 458 }
            PathMove { x: root.horizontalWidth2s * 4; y: 0 }
            PathLine { x: root.horizontalWidth2s * 4; y: 458 }
            PathMove { x: root.horizontalWidth2s * 5; y: 0 }
            PathLine { x: root.horizontalWidth2s * 5; y: 458 }
            PathMove { x: root.horizontalWidth2s * 6; y: 0 }
            PathLine { x: root.horizontalWidth2s * 6; y: 458 }
            PathMove { x: root.horizontalWidth2s * 7; y: 0 }
            PathLine { x: root.horizontalWidth2s * 7; y: 458 }
            PathMove { x: root.horizontalWidth2s * 8; y: 0 }
            PathLine { x: root.horizontalWidth2s * 8; y: 458 }
            PathMove { x: root.horizontalWidth2s * 9; y: 0 }
            PathLine { x: root.horizontalWidth2s * 9; y: 458 }
            PathMove { x: root.horizontalWidth2s * 10; y: 0 }
            PathLine { x: root.horizontalWidth2s * 10; y: 458 }
            PathMove { x: root.horizontalWidth2s * 11; y: 0 }
            PathLine { x: root.horizontalWidth2s * 11; y: 458 }
            PathMove { x: root.horizontalWidth2s * 12; y: 0 }
            PathLine { x: root.horizontalWidth2s * 12; y: 458 }
            PathMove { x: root.horizontalWidth2s * 13; y: 0 }
            PathLine { x: root.horizontalWidth2s * 13; y: 458 }
            PathMove { x: root.horizontalWidth2s * 14; y: 0 }
            PathLine { x: root.horizontalWidth2s * 14; y: 458 }
            PathMove { x: root.horizontalWidth2s * 15; y: 0 }
            PathLine { x: root.horizontalWidth2s * 15; y: 458 }
            PathMove { x: root.horizontalWidth2s * 16; y: 0 }
            PathLine { x: root.horizontalWidth2s * 16; y: 458 }
            PathMove { x: root.horizontalWidth2s * 17; y: 0 }
            PathLine { x: root.horizontalWidth2s * 17; y: 458 }
            PathMove { x: root.horizontalWidth2s * 18; y: 0 }
            PathLine { x: root.horizontalWidth2s * 18; y: 458 }
            PathMove { x: root.horizontalWidth2s * 19; y: 0 }
            PathLine { x: root.horizontalWidth2s * 19; y: 458 }
            PathMove { x: root.horizontalWidth2s * 20; y: 0 }
            PathLine { x: root.horizontalWidth2s * 20; y: 458 }
            PathMove { x: root.horizontalWidth2s * 21; y: 0 }
            PathLine { x: root.horizontalWidth2s * 21; y: 458 }
            PathMove { x: root.horizontalWidth2s * 22; y: 0 }
            PathLine { x: root.horizontalWidth2s * 22; y: 458 }
            PathMove { x: root.horizontalWidth2s * 23; y: 0 }
            PathLine { x: root.horizontalWidth2s * 23; y: 458 }
            PathMove { x: root.horizontalWidth2s * 24; y: 0 }
            PathLine { x: root.horizontalWidth2s * 24; y: 458 }
            PathMove { x: root.horizontalWidth2s * 25; y: 0 }
            PathLine { x: root.horizontalWidth2s * 25; y: 458 }
            PathMove { x: root.horizontalWidth2s * 26; y: 0 }
            PathLine { x: root.horizontalWidth2s * 26; y: 458 }
            PathMove { x: root.horizontalWidth2s * 27; y: 0 }
            PathLine { x: root.horizontalWidth2s * 27; y: 458 }
            PathMove { x: root.horizontalWidth2s * 28; y: 0 }
            PathLine { x: root.horizontalWidth2s * 28; y: 458 }
            PathMove { x: root.horizontalWidth2s * 29; y: 0 }
            PathLine { x: root.horizontalWidth2s * 29; y: 458 }
            PathMove { x: root.horizontalWidth2s * 30; y: 0 }
            PathLine { x: root.horizontalWidth2s * 30; y: 458 }
            PathMove { x: root.horizontalWidth2s * 31; y: 0 }
            PathLine { x: root.horizontalWidth2s * 31; y: 458 }
            PathMove { x: root.horizontalWidth2s * 32; y: 0 }
            PathLine { x: root.horizontalWidth2s * 32; y: 458 }
            PathMove { x: root.horizontalWidth2s * 33; y: 0 }
            PathLine { x: root.horizontalWidth2s * 33; y: 458 }
            PathMove { x: root.horizontalWidth2s * 34; y: 0 }
            PathLine { x: root.horizontalWidth2s * 34; y: 458 }
            PathMove { x: root.horizontalWidth2s * 35; y: 0 }
            PathLine { x: root.horizontalWidth2s * 35; y: 458 }
            PathMove { x: root.horizontalWidth2s * 36; y: 0 }
            PathLine { x: root.horizontalWidth2s * 36; y: 458 }
            PathMove { x: root.horizontalWidth2s * 37; y: 0 }
            PathLine { x: root.horizontalWidth2s * 37; y: 458 }
            PathMove { x: root.horizontalWidth2s * 38; y: 0 }
            PathLine { x: root.horizontalWidth2s * 38; y: 458 }
            PathMove { x: root.horizontalWidth2s * 39; y: 0 }
            PathLine { x: root.horizontalWidth2s * 39; y: 458 }
            PathMove { x: root.horizontalWidth2s * 40; y: 0 }
            PathLine { x: root.horizontalWidth2s * 40; y: 458 }
            PathMove { x: root.horizontalWidth2s * 41; y: 0 }
            PathLine { x: root.horizontalWidth2s * 41; y: 458 }
            PathMove { x: root.horizontalWidth2s * 42; y: 0 }
            PathLine { x: root.horizontalWidth2s * 42; y: 458 }
            PathMove { x: root.horizontalWidth2s * 43; y: 0 }
            PathLine { x: root.horizontalWidth2s * 43; y: 458 }
            PathMove { x: root.horizontalWidth2s * 44; y: 0 }
            PathLine { x: root.horizontalWidth2s * 44; y: 458 }
            PathMove { x: root.horizontalWidth2s * 45; y: 0 }
            PathLine { x: root.horizontalWidth2s * 45; y: 458 }
            PathMove { x: root.horizontalWidth2s * 46; y: 0 }
            PathLine { x: root.horizontalWidth2s * 46; y: 458 }
            PathMove { x: root.horizontalWidth2s * 47; y: 0 }
            PathLine { x: root.horizontalWidth2s * 47; y: 458 }
            PathMove { x: root.horizontalWidth2s * 48; y: 0 }
            PathLine { x: root.horizontalWidth2s * 48; y: 458 }
            PathMove { x: root.horizontalWidth2s * 49; y: 0 }
            PathLine { x: root.horizontalWidth2s * 49; y: 458 }
        }
    }


    //グラフ表示領域
    GraphCanvas{
        id: graphCanvas
        width: root.width - (root.border.width * 2)
        height: root.height - (root.border.width * 2)
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: root.border.width
        anchors.leftMargin: root.border.width
        redLineValues: root.redLineValues
        blueLineValues: root.blueLineValues
        minY: root.minY
        maxY: root.maxY
        xDivisions: root.xDivisions
        step: root.step
        speedMinY: root.speedMinY
        speedMaxY: root.speedMaxY
        torqueMinY: root.torqueMinY
        torqueMaxY: root.torqueMaxY
        visible: true
        dirty: true
    }

    function requestPaint(startIndex){
        graphCanvas.startIndex = startIndex
        graphCanvas.redLineValues = root.redLineValues
        graphCanvas.blueLineValues = root.blueLineValues
        graphCanvas.dirty = true;   // Avoid redundant rendering.
    }
    Connections
    {
        target:pageModel
        onGraphSpeedDataChanged:{
            if (root.graphMode === false){
                root.blueLineValues = graphSpeedData;
                // requestPaint(root.blueGraphDrawIndex);
                requestPaint(0);
                // root.blueGraphDrawIndex++;
                // if (root.blueGraphDrawIndex >= root.drawIndexMax[root.timerSettingToggle]){
                //     root.blueGraphDrawIndex = 0
                // }
            }
        }
        onGraphTorqueDataChanged:{
            if (root.graphMode === false){
                //console.log("graphTorqueData:"+graphTorqueData)
                root.redLineValues = graphTorqueData;
                // requestPaint(root.redGraphDrawIndex);
                requestPaint(0);
                // root.redGraphDrawIndex++;
                // if (root.redGraphDrawIndex >= root.drawIndexMax[root.timerSettingToggle]){
                //     root.redGraphDrawIndex = 0
                // }
            }
        }
    }
}
