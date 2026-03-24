import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: Deceleration_page.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.18
* Worker: SH.HONG
**************************************************************************************/
Item {
    property bool pageActive: false
    width: parent.width
    height: parent.height
    property var viewModel: functionViewModel.accelerationView
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length

    property int axisNum1: (1 <= numAxis ? axisOrder[0] : 0)
    property int axisNum2: (2 <= numAxis ? axisOrder[1] : 0)
    property int axisNum3: (3 <= numAxis ? axisOrder[2] : 0)
    property int axisNum4: (4 <= numAxis ? axisOrder[3] : 0)
    property int axisNum5: (5 <= numAxis ? axisOrder[4] : 0)
    property int axisNum6: (6 <= numAxis ? axisOrder[5] : 0)
    property int axisNum7: (7 <= numAxis ? axisOrder[6] : 0)
    property int axisNum8: (8 <= numAxis ? axisOrder[7] : 0)
    property var axisNameIndex: [0, 1, 2, 3, 4, 5, 6, 7]
    property var coordinateX:[113,109,105,101,97,93,89,85,81,77,73,69,65,61,57,53,49,45,41,37,33,29,25,21,17,13]
    property var coordinateAllX:[113,102,91,80,69,58,47,36,25,14]
    property var data1: viewModel.getData(axisNum1)
    property var data2: viewModel.getData(axisNum2)
    property var data3: viewModel.getData(axisNum3)
    property var data4: viewModel.getData(axisNum4)
    property var data5: viewModel.getData(axisNum5)
    property var data6: viewModel.getData(axisNum6)
    property var data7: viewModel.getData(axisNum7)
    property var data8: viewModel.getData(axisNum8)

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

    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.FunctionDeceleration)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }

    Component.onDestruction: viewModel.deactivate()

    Rectangle{
        x:2
        y:40
        width: 1018
        height: 590
        anchors.fill: parent.Center
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"
    }//背景

    //Y轴
    Item{
        visible: (1 <= numAxis || !data1)
        Rectangle{
            width: 470
            height: 90
            x:20
            y:70
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum1],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum1])
            }
        }
        Image {
            x:210
            y:82
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased: {
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data1.acceleration
                    let ret = viewModel.writeAcceleration(data1.accelerationAddress, data1.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum1, currentValue, data1.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_Y
            width: 130
            height: 50
            x:270
            y:82
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2

            Canvas{
                property var acceleration: data1.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas1
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas1.width,canvas1.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data1.acceleration+1
                }
            }
        }
        Image {
            x:408
            y:82
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased: {
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = data1.acceleration
                    let ret = viewModel.writeAcceleration(data1.accelerationAddress, data1.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum1, currentValue, data1.acceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Text{
            anchors.top: deplay_Y.bottom
            anchors.horizontalCenter: deplay_Y.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data1.timer.timer, viewModel.allAcceleration)
        }
    }
    //MX轴
    Item{
        visible: (2 <= numAxis || !data2)
        Rectangle{
            width: 470
            height: 90
            x:20
            y:180
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum2],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum2])
            }
        }
        Image {
            x:210
            y:192
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data2.acceleration
                    let ret = viewModel.writeAcceleration(data2.accelerationAddress, data2.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum2, currentValue, data2.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_MX
            width: 130
            height: 50
            x:270
            y:192
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: data2.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas2
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas2.width,canvas2.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "#ffffff"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data2.acceleration+1
                }
            }
        }
        Image {
            x:408
            y:192
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased: {
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                    let currentValue = data2.acceleration
                    let ret = viewModel.writeAcceleration(data2.accelerationAddress, data2.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum2, currentValue, data2.acceleration+1)
                }
            }
        }
        Text{
            anchors.top: deplay_MX.bottom
            anchors.horizontalCenter: deplay_MX.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data2.timer.timer, viewModel.allAcceleration)
        }
    }
    //MZ轴
    Item{
        visible: (3 <= numAxis || !data3)
        Rectangle{
            width: 470
            height: 90
            x:20
            y:290
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum3],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum3])
            }
        }
        Image {
            x:210
            y:302
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data3.acceleration
                    let ret = viewModel.writeAcceleration(data3.accelerationAddress, data3.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum3, currentValue, data3.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_MZ
            width: 130
            height: 50
            x:270
            y:302
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: data3.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas3
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas3.width,canvas3.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data3.acceleration+1
                }
            }
        }
        Image {
            x:408
            y:302
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = data3.acceleration
                    let ret = viewModel.writeAcceleration(data3.accelerationAddress, data3.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum3, currentValue, data3.acceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Text{
            anchors.top: deplay_MZ.bottom
            anchors.horizontalCenter: deplay_MZ.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data3.timer.timer, viewModel.allAcceleration)
        }
    }
    //SX轴
    Item{
        visible: (4 <= numAxis || !data4)
        Rectangle{
            width: 470
            height: 90
            x:20
            y:400
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum4],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum4])
            }
        }
        Image {
            x:210
            y:412
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data4.acceleration
                    let ret = viewModel.writeAcceleration(data4.accelerationAddress, data4.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum4, currentValue, data4.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_SX
            width: 130
            height: 50
            x:270
            y:412
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: data4.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas4
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas4.width,canvas4.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data4.acceleration+1
                }
            }

        }
        Image {
            x:408
            y:412
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = data4.acceleration
                    let ret = viewModel.writeAcceleration(data4.accelerationAddress, data4.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum4, currentValue, data4.acceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Text{
            anchors.top: deplay_SX.bottom
            anchors.horizontalCenter: deplay_SX.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data4.timer.timer, viewModel.allAcceleration)
        }
    }
    //SZ轴
    Item{
        visible: (5 <= numAxis || !data5)
        Rectangle{
            width: 470
            height: 90
            x:20
            y:510
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum5],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum5])
            }
        }
        Image {
            x:210
            y:522
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data5.acceleration
                    let ret = viewModel.writeAcceleration(data5.accelerationAddress, data5.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum5, currentValue, data5.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_SZ
            width: 130
            height: 50
            x:270
            y:522
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: data5.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas5
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas5.width,canvas5.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data5.acceleration+1
                }
            }
        }
        Image {
            x:408
            y:522
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = data5.acceleration
                    let ret = viewModel.writeAcceleration(data5.accelerationAddress, data5.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum5, currentValue, data5.acceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Text{
            anchors.top: deplay_SZ.bottom
            anchors.horizontalCenter: deplay_SZ.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data5.timer.timer, viewModel.allAcceleration)
        }
    }

    //轴6
    Item{
        visible: (6 <= numAxis || !data6)
        Rectangle{
            width: 470
            height: 90
            x:550
            y:70
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum6],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum6])
            }
        }
        Image {
            x:740
            y:82
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data6.acceleration
                    let ret = viewModel.writeAcceleration(data6.accelerationAddress, data6.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum6, currentValue, data6.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_6
            width: 130
            height: 50
            x:800
            y:82
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: data6.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas6
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas6.width,canvas6.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data6.acceleration+1
                }
            }

        }
        Image {
            x:938
            y:82
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = data6.acceleration
                    let ret = viewModel.writeAcceleration(data6.accelerationAddress, data6.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum6, currentValue, data6.acceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Text{
            anchors.top: deplay_6.bottom
            anchors.horizontalCenter: deplay_6.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data6.timer.timer, viewModel.allAcceleration)
        }
    }
    //轴7
    Item{
        visible: (7 <= numAxis || !data7)
        Rectangle{
            width: 470
            height: 90
            x:550
            y:180
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum7],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum7])
            }
        }
        Image {
            x:740
            y:192
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data7.acceleration
                    let ret = viewModel.writeAcceleration(data7.accelerationAddress, data7.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum7, currentValue, data7.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_7
            width: 130
            height: 50
            x:800
            y:192
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: data7.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas7
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas7.width,canvas7.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data7.acceleration+1
                }
            }

        }
        Image {
            x:938
            y:192
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = data7.acceleration
                    let ret = viewModel.writeAcceleration(data7.accelerationAddress, data7.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum7, currentValue, data7.acceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Text{
            anchors.top: deplay_7.bottom
            anchors.horizontalCenter: deplay_7.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data7.timer.timer, viewModel.allAcceleration)
        }
    }
    //C轴
    Item{
        visible: (8 <= numAxis || !data8)
        Rectangle{
            width: 470
            height: 90
            x:550
            y:290
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(22, axisNameIndex[axisNum8],local.language)
                color: resourceManager.rgb(22, axisNameIndex[axisNum8])
            }
        }
        Image {
            x:740
            y:302
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = data8.acceleration
                    let ret = viewModel.writeAcceleration(data8.accelerationAddress, data8.acceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum8, currentValue, data8.acceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_8
            width: 130
            height: 50
            x:800
            y:302
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: data8.acceleration

                onAccelerationChanged: requestPaint();

                id:canvas8
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas8.width,canvas8.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: data8.acceleration+1
                }
            }

        }
        Image {
            x:938
            y:302
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = data8.acceleration
                    let ret = viewModel.writeAcceleration(data8.accelerationAddress, data8.acceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(axisNum8, currentValue, data8.acceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Text{
            anchors.top: deplay_8.bottom
            anchors.horizontalCenter: deplay_8.horizontalCenter
            font.pixelSize: 20
            color: "#999999"
            text: getShowTimer(data8.timer.timer, viewModel.allAcceleration)
        }
    }
    //全体
    Item{
        Rectangle{
            width: 470
            height: 90
            x:530
            y:400
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(48, 9,local.language)
                color: resourceManager.rgb(48, 9)
            }
        }
        Image {
            x:740
            y:412
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = viewModel.allAcceleration
                    let ret = viewModel.writeAllAcceleration(viewModel.allAcceleration-10);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(8, currentValue, viewModel.allAcceleration-10)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_all
            width: 130
            height: 50
            x:800
            y:412
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: viewModel.allAcceleration

                onAccelerationChanged: requestPaint();

                id:canvas9
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas9.width,canvas9.height);
                    draw_line_all(ctx,((acceleration/10)-1))
                }
            }
            Rectangle{
                width: 52
                height: 22
                x:77
                y:54
                color: "#eaeaea"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: viewModel.allAcceleration + "%"
                }
            }
        }
        Image {
            x:938
            y:412
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = viewModel.allAcceleration
                    let ret = viewModel.writeAllAcceleration(viewModel.allAcceleration+10);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(8, currentValue, viewModel.allAcceleration+10)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }

    }
    //插补
    Item{
        Rectangle{
            width: 470
            height: 90
            x:530
            y:510
            color: "#eaeaea"
            border.color: "#999999"
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 24
                font.bold: true
                leftPadding: 40
                text: resourceManager.label(48, 10,local.language)
                color: resourceManager.rgb(48, 10)
            }
        }
        Image {
            x:740
            y:522
            source: resourceManager.imagePath + "/18_accel/a180_00.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_01.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_00.png"
                }
                onClicked: {
                    let currentValue = viewModel.interpolationAcceleration
                    let ret = viewModel.writeInterpolationAcceleration(viewModel.interpolationAcceleration-1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(9, currentValue, viewModel.interpolationAcceleration-1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            id:deplay_interpolation
            width: 130
            height: 50
            x:800
            y:522
            color: "#ffffff"
            border.color: "#999999"
            border.width: 2
            Canvas{
                property var acceleration: viewModel.interpolationAcceleration

                onAccelerationChanged: requestPaint();

                id:canvas10
                width: 130
                height: 50
                contextType: "2d"
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0,0,canvas10.width,canvas10.height);
                    draw_line(ctx,acceleration)
                }
            }
            Rectangle{
                width: 32
                height: 22
                x:94
                y:24
                color: "white"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: viewModel.interpolationAcceleration+1
                }
            }

        }
        Image {
            x:938
            y:522
            source: resourceManager.imagePath + "/18_accel/a180_02.png"
            MouseArea{
                anchors.fill: parent
                onPressed: parent.source = resourceManager.imagePath + "/18_accel/a180_03.png"
                onReleased:{
                    parent.source = resourceManager.imagePath + "/18_accel/a180_02.png"
                }
                onClicked: {
                    let currentValue = viewModel.interpolationAcceleration
                    let ret = viewModel.writeInterpolationAcceleration(viewModel.interpolationAcceleration+1);
                    if (ret === 0)
                        operationRecord.recordSetAcceleration(9, currentValue, viewModel.interpolationAcceleration+1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }

    function draw_line(ctx,index){
        ctx.save()
        ctx.beginPath()
        ctx.lineWidth = 2
        ctx.fillStyle = "#cc3333"
        ctx.strokeStyle = "#cc3333";
        ctx.moveTo(0,50)
        ctx.lineTo(coordinateX[index],8)
        ctx.moveTo(coordinateX[index],8)
        ctx.lineTo(130,8)
        //描边
        ctx.stroke()
        ctx.restore();
    }
    function draw_line_all(ctx,index){
        ctx.save()
        ctx.beginPath()
        ctx.lineWidth = 2
        ctx.fillStyle = "#cc3333"
        ctx.strokeStyle = "#cc3333";
        ctx.moveTo(0,50)
        ctx.lineTo(coordinateAllX[index],8)
        ctx.moveTo(coordinateAllX[index],8)
        ctx.lineTo(130,8)
        //描边
        ctx.stroke()
        ctx.restore();
    }
    function getShowTimer(timer, allAcceleration){
        var value = timer * (100 / allAcceleration);
        var format = resourceManager.getPenSetIntValue("23", 1, 0);
        return resourceManager.formatData(value, format)
    }
}
