import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Abs_Set.qml
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
    property var viewModel: functionViewModel.absSetView
    property int page: viewModel.pageNum
    property int page_max: viewModel.pageMaxNum
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length

    property var axisNameIndex: [0, 1, 2, 3, 4, 5, 6, 7]

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
            commonViewModel.pageChange(Enums.FunctionAbsSet)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }

    Component.onDestruction: viewModel.deactivate()

    Rectangle{
        x: 20
        y: 20
        width: 980
        height: 650
        anchors.fill: parent.Center
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"
    }//背景
    Rectangle {
        x: 70
        y: 70
        width: 870
        height: 46
        color: "#999999"
        Row{
            anchors.left: parent.left
            anchors.leftMargin: 172
            anchors.verticalCenter: parent.verticalCenter
            spacing: 4
            Text {
                width: 120
                height: 40
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                text: resourceManager.label(30,17,local.language)
                color: "black"
            }
            Text {
                width: 110
                height: 40
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                text: resourceManager.label(47,2,local.language)
                color: resourceManager.rgb(47,2)
            }
            Text {
                width: 110
                height: 40
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                text: resourceManager.label(47,3,local.language)
                color: resourceManager.rgb(47,3)
            }
            Text {
                width: 140
                height: 40
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                text: resourceManager.label(47,4,local.language)
                color: resourceManager.rgb(47,4)
            }
            Text {
                width: 140
                height: 40
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                text: resourceManager.label(47,5,local.language)
                color: resourceManager.rgb(47,5)
            }
        }//标题
    }
    Rectangle{
        width: 870
        height: 480
        x:70
        y:120
        border.color: "#999999"
        color: "#eaeaea"
        //分隔xian
        Rectangle{
            width: parent.width-4
            height: 4
            x:2
            y:parent.height/5

            color: "white"
        }
        Rectangle{
            width: parent.width-4
            height: 4
            x:2
            y:2*parent.height/5

            color: "white"
        }
        Rectangle{
            width: parent.width-4
            height: 4
            x:2
            y:3*parent.height/5

            color: "white"
        }
        Rectangle{
            width: parent.width-4
            height: 4
            x:2
            y:4*parent.height/5
            color: "white"
        }

        Repeater{
            model: viewModel.getDataList(page)
            Item{
                property int itemIndex: (page * 5) + index
                property int axisNum: (itemIndex <= numAxis ? axisOrder[itemIndex] : 0)
                y: (94 + 2) * index
                width: parent.width
                height: 94
                Text {
                    x:50
                    y:30
                    font.pixelSize: 26
                    text: resourceManager.label(22,axisNameIndex[axisNum],local.language)
                    color: resourceManager.rgb(22,axisNameIndex[axisNum])
                }
                Text {
                    width: 120
                    height: 94
                    x:172
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 26
                    color: "black"
                    font.bold: true
                    text: resourceManager.formatData(modelData.encoder, 64)
                }

                Rectangle{
                    width: 24
                    height: 24
                    x:338
                    y:35
                    radius: 12
                    border.width: 2
                    border.color: "#999999"
                    color: modelData.bm ? "#cc3333" : "#cccccc"
                }
                Rectangle{
                    width: 24
                    height: 24
                    x:448
                    y:35
                    radius: 12
                    border.width: 2
                    border.color: "#999999"
                    color: modelData.alert ? "#cc3333" : "#cccccc"
                }
                Rectangle{
                    width: 120
                    height: 55
                    x:532
                    y:20
                    radius: 6
                    border.color: "#999999"
                    border.width: 2
                    color: "#ffffff"
                    MouseArea{
                        anchors.fill: parent
                        onPressed: {
                            parent.color = "#cc3333"
                            resetText.color = resourceManager.dataColor(22, 1)
                            commonViewModel.modbusMemblemSwControl(true, modelData.resetKeyNo);
                        }
                        onReleased: {
                            parent.color = "#ffffff"
                            resetText.color = resourceManager.rgb(47,14)
                            commonViewModel.modbusMemblemSwControl(false, modelData.resetKeyNo);
                            operationRecord.recordKeyResetAbs(axisNum)
                        }
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                    Text {
                        id: resetText
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        text: resourceManager.label(47,14,local.language)
                        color: resourceManager.rgb(47,14)
                    }
                }
                Rectangle{
                    width: 120
                    height: 55
                    x:680
                    y:20
                    radius: 6
                    border.color: "#999999"
                    border.width: 2
                    color: "#ffffff"
                    MouseArea{
                        anchors.fill: parent
                        onPressed: {
                            parent.color = "#cc3333"
                            setText.color = resourceManager.dataColor(22, 2)
                            commonViewModel.modbusMemblemSwControl(true, modelData.setKeyNo);
                        }
                        onReleased: {
                            parent.color = "#ffffff"
                            setText.color = resourceManager.rgb(47,15)
                            commonViewModel.modbusMemblemSwControl(false, modelData.setKeyNo);
                            operationRecord.recordKeySetAbs(axisNum)
                        }
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                    Text {
                        id: setText
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        text: resourceManager.label(47,15,local.language)
                        color: resourceManager.rgb(47,15)
                    }
                }
            }
        }
    }

    ImageScrollBar{
        visible: 6 <= numAxis
        x:890
        y:80
        height: 480
        topButtonPressImage: resourceManager.imagePath + "/05_ORG/d050_25" + ".png"
        topButtonReleaseImage: resourceManager.imagePath + "/05_ORG/d050_23" + ".png"
        botomButtonPressImage: resourceManager.imagePath + "/05_ORG/d050_24" + ".png"
        botomButtonReleaseImage: resourceManager.imagePath + "/05_ORG/d050_22" + ".png"
        pageLabal: parseInt(page + 1, 10) + "/" + page_max
        onScrollUpClicked: {
            page = Math.max(page - 1, 0);
        }
        onScrollDownClicked: {
            page = Math.min(page + 1, 1);
        }
    }//换页按钮
}
