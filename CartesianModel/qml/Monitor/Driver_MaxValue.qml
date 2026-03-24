import QtQuick 2.12
import QtQuick.Controls 2.5
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Driver_MaxValue.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.02
* Worker: IJ.YI
**************************************************************************************/
Item {
    id:maxvalue
    width: 1012
    height: 520

    // Background color
    Rectangle {
        width: parent.width
        height: parent.height - 60
        anchors.top: parent.top
        anchors.topMargin: 60
        color: "#FFFFFF"
        border.width: 2
        border.color: "#CCCCCC"
    }

    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property int peakCurPage : 1
    property int peakTotalPages : (maxvalue.numAxis <= 5) ? 1 : maxvalue.numAxis / 5 + 1
    property var pageModel:monitorViewModel.drvMonitorView
    property var axisDataList: pageModel.axisInfo.axisDataList
    property int scrollCurPage: maxvalue.numAxis <= 5 ? 1 : parseInt(maxvalue.realMonCurPage, 10)
    property int scrollTotalPages: parseInt((maxvalue.numAxis <= 5 ? 1 : maxvalue.numAxis / 5 + 1), 10)

    // title name
    Rectangle {
        id : driver_title_area
        width: 850
        height: 70
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        color: "#999999"
        // title nae list
        Row{
            leftPadding: 100
            Repeater{
                model: 3
                Text {
                    width: 250
                    height: 70
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: "#000000"
                    text: resourceManager.label(33, 27 + index, resourceManager.language)
                }
            }
        }
    }
    Column {
        anchors.top: driver_title_area.bottom
        anchors.topMargin: 10
        anchors.left: driver_title_area.left
        spacing: 5
        Repeater {
            model: 5
            // data list box area
            Rectangle{
                id: axis_data_base
                width: 850
                height: 73
                color: "#CCCCCC"
                property int axisNum: maxvalue.axisOrder[(maxvalue.peakCurPage-1)*5+index]
                Rectangle{
                    width: 102
                    height: 73
                    color: "transparent"
                    border.color: "#FFFFFF"
                }
                // axis name
                Text{
                    x:10
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: "#000000"
                    text: resourceManager.label(22, axis_data_base.axisNum, resourceManager.language)
                }
                Row {
                    leftPadding: 102
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: -1
                    // speed
                    Rectangle{
                        width: 250
                        height: 73
                        color: "transparent"
                        border.color: "#FFFFFF"
                        Text {
                            width: 125
                            height: 73
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color: "#000000"
                            text: ((axisDataList[axis_data_base.axisNum].speedMin === "0") ? "" : "") + axisDataList[axis_data_base.axisNum].speedMin
                        }
                        // vertical line
                        Rectangle{
                            width: 1
                            height: 60
                            anchors.centerIn: parent
                            color: "#FFFFFF"
                        }
                        Text {
                            width: 125
                            height: 73
                            anchors.right: parent.right
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color: "#000000"
                            text: axisDataList[axis_data_base.axisNum].speedPeak
                        }
                    }
                    // Deviation
                    Rectangle{
                        width: 250
                        height: 73
                        color: "transparent"
                        border.color: "#FFFFFF"
                        Text {
                            width: 125
                            height: 73
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color: "#000000"
                            text: ((axisDataList[axis_data_base.axisNum].deviationMin === "0") ? "" : "") + axisDataList[axis_data_base.axisNum].deviationMin
                        }
                        // vertical line
                        Rectangle{
                            width: 1
                            height: 60
                            anchors.centerIn: parent
                            color: "#FFFFFF"
                        }
                        Text {
                            width: 125
                            height: 73
                            anchors.right: parent.right
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color: "#000000"
                            text: axisDataList[axis_data_base.axisNum].deviationPeak
                        }
                    }
                    // torque
                    Rectangle{
                        width: 250
                        height: 73
                        color: "transparent"
                        border.color: "#FFFFFF"
                        Text {
                            width: 125
                            height: 73
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color: "#000000"
                            text: ((axisDataList[axis_data_base.axisNum].torqueMin === "0") ? "" : "") +  axisDataList[axis_data_base.axisNum].torqueMin
                        }
                        // vertical line
                        Rectangle{
                            width: 1
                            height: 60
                            anchors.centerIn: parent
                            color: "#FFFFFF"
                        }
                        Text {
                            width: 125
                            height: 73
                            anchors.right: parent.right
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color: "#000000"
                            text: axisDataList[axis_data_base.axisNum].torquePeak
                        }
                    }
                }
            }
        }
    } // Column
    // right area
    Rectangle{
        width: 85
        height: 380
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 160
        color: "transparent"
        // reset Key
        Rectangle{
            width: 70
            height: 55
            radius: 10
            border.width: 2
            border.color: "#999999"
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            color: reset_mouse.pressed ? "#CC3333" : "#CCCCCC"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                font.bold: true
                color: reset_mouse.pressed ? "#FFFFFF" : "#666666"
                text: resourceManager.label(33, 32, resourceManager.language)
            }
            MouseArea {
                id : reset_mouse
                anchors.fill: parent
                onCanceled: {
                    pageModel.initAxisPeakData(false);
                    operationRecord.recordResetDrvMonitor()
                    operationRecord.recordOpeDrvMonitorReset()
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        // add axis cisible scrollbar
        Rectangle {
            width: 60
            height: 260
            radius: 30
            color: "#CCCCCC"
            visible: maxvalue.peakTotalPages > 1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            ImageScrollBar {
                height: 240
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                topButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_03.png"
                topButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_04.png"
                botomButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_05.png"
                botomButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_06.png"
                pageLabal: maxvalue.scrollCurPage + "/" + maxvalue.scrollTotalPages
                pageLabelColor: "black"
                visible: maxvalue.peakTotalPages > 1
                onScrollUpClicked: {
                    if (maxvalue.peakCurPage > 1) {
                        maxvalue.peakCurPage--
                    } else {
                        maxvalue.peakCurPage = maxvalue.peakTotalPages
                    }
                }
                onScrollDownClicked: {
                    if (maxvalue.peakCurPage >= maxvalue.peakTotalPages) {
                        maxvalue.peakCurPage = 1
                    } else {
                        maxvalue.peakCurPage++
                    }
                }
            }
        }
    }
} // item

