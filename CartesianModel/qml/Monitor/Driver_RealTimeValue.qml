import QtQuick 2.12
import QtQuick.Controls 2.5
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Driver_RealTimeValue.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.01
* Worker: IJ.YI
**************************************************************************************/
Item {
    id : reltimevalue
    width: 1012
    height: 520

    // Background color
    Rectangle{
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
    property int realMonCurPage : 1
    property int realMonTotalPages : (reltimevalue.numAxis <= 5) ? 1 : reltimevalue.numAxis / 5 + 1
    property var pageModel:monitorViewModel.drvMonitorView
    property var axisDataList: pageModel.axisInfo.axisDataList
    property int scrollCurPage: reltimevalue.numAxis <= 5 ? 1 : parseInt(reltimevalue.realMonCurPage, 10)
    property int scrollTotalPages: parseInt((reltimevalue.numAxis <= 5 ? 1 : reltimevalue.numAxis / 5 + 1), 10)

    // title
    Rectangle {
        id : dirver_title_area
        width: 850
        height: 70
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        color: "#999999"
        Row {
            leftPadding: 100
            Repeater {
                model: 5
                Text {
                    width: 150
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
        anchors.top: dirver_title_area.bottom
        anchors.topMargin: 10
        anchors.left: dirver_title_area.left
        spacing: 5
        Repeater {
            model: 5
            // list box
            Rectangle{
                id : axis_data_base
                width: 850
                height: 73
                color: "#CCCCCC"
                property int axisNum: reltimevalue.axisOrder[(reltimevalue.realMonCurPage-1)*5+index]
                // axis name
                Rectangle{
                    width: 110
                    height: 73
                    color: "transparent"
                    border.color: "#FFFFFF"
                    Text {
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 22
                        font.bold: true
                        color: "#000000"
                        text: resourceManager.label(22, axis_data_base.axisNum, resourceManager.language)
                    }
                }
                Row{
                    x:110
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: -1
                    Repeater{
                        model: 5
                        Rectangle{
                            width: 150
                            height: 73
                            color: "transparent"
                            border.color: "#FFFFFF"
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                font.pixelSize: 20
                                visible:{
                                    if(DRV_MONITOR_GRAPH_10MS_CYCLE){
                                        return pageModel.dataLoaded
                                    } else {
                                        return true;
                                    }
                                }
                                color:"#000000"
                                text: {
                                    switch(index){
                                    case 0:
                                        axisDataList[axis_data_base.axisNum].speed
                                        break
                                    case 1:
                                        axisDataList[axis_data_base.axisNum].deviation
                                        break
                                    case 2:
                                        axisDataList[axis_data_base.axisNum].torque
                                        break
                                    case 3:
                                        axisDataList[axis_data_base.axisNum].loadRate
                                        break
                                    case 4:
                                        axisDataList[axis_data_base.axisNum].resurrectionResistorRate
                                        break
                                    default:
                                        break
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // add axis visible scrollbar
    Rectangle {
        width: 60
        height: 520
        radius: 30
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 100
        color: "#CCCCCC"
        visible: reltimevalue.realMonTotalPages > 1
        ImageScrollBar{
            height: 480
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            topButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_03.png"
            topButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_04.png"
            botomButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_05.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_06.png"
            pageLabal: reltimevalue.scrollCurPage + "/" + reltimevalue.scrollTotalPages
            pageLabelColor: "black"
            onScrollUpClicked: {
                if (reltimevalue.realMonCurPage > 1) {
                    reltimevalue.realMonCurPage--
                } else {
                    reltimevalue.realMonCurPage = reltimevalue.realMonTotalPages
                }
            }
            onScrollDownClicked: {
                if (reltimevalue.realMonCurPage >= reltimevalue.realMonTotalPages) {
                    reltimevalue.realMonCurPage = 1
                } else {
                    reltimevalue.realMonCurPage++
                }
            }
        }
    }
}

