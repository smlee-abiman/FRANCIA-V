import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import "../Widget"
import "../Widget/ComboBox"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Internal_Monitor.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: IJ.YI
**************************************************************************************/
Item {
    id: internal_monitor_page
    width: 1012
    height: 520
    property bool pageActive: false
    property var pageModel:monitorViewModel.internalMonitorView
    property var bankIOList: pageModel.bankIOList
    property var bankIoByteList: pageModel.bankIOByteList

    Component.onDestruction: pageModel.deactivate()

    onPageActiveChanged: {
        if (pageActive) {
            pageModel.activate()
        } else {
            pageModel.deactivate()
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height - 60
        anchors.top: parent.top
        anchors.topMargin: 60
        color: "#FFFFFF"
        border.color: "#999999"
        border.width: 2
    }

    Row{
        anchors.top: parent.top
        anchors.topMargin: 90
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 25
        // 좌뱅크
        StringComboBox_style1{
            id: bankNo
            width: 200
            height: 48
            bgRectBorderWidth: 2
            bgPopupBorderWidth:2
            popupDelegateNum: 5
            resourceGroupId: 33
            resourceStartIndex: 34
            bgRectColor: "#FFFFFF"
            bgIndicatorColor: "transparent"
            bgRectRadius: 10
            bgIndicatorRadius: 10
            currentIndex: 0
            onCurrentIndexChanged:{
                if (currentIndex !== -1){
                    console.debug("ResourceStringComboBoxDefaultIndicator bankNo currentIndex:"+currentIndex, "curBankNo:"+pageModel.curBankNo)
                    pageModel.curBankNo = currentIndex;
                }
            }
        }//bank选择

        // 우뱅크
        StringComboBox_style1{
            id: bankContents
            width: 200
            height: 48
            bgRectBorderWidth: 2
            bgPopupBorderWidth:2
            popupDelegateNum: 2
            resourceGroupId: 33
            resourceStartIndex: 39
            bgRectColor: "#FFFFFF"
            bgIndicatorColor: "transparent"
            bgRectRadius: 10
            bgIndicatorRadius: 10
            currentIndex: 0
            onCurrentIndexChanged:{
                if (currentIndex !== -1){
                    console.debug("ResourceStringComboBoxDefaultIndicator bankContents currentIndex:"+currentIndex, "curBankContents:"+pageModel.curBankContents)
                    pageModel.curBankContents = currentIndex;
                }
            }
        }//类型选择
    }

    Rectangle {
        width: 540
        height: 430
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        color: "transparent"

    ListModel{
        id:listmodel
        ListElement{index: "0"}
        ListElement{index: "1"}
        ListElement{index: "2"}
        ListElement{index: "3"}
        ListElement{index: "4"}
        ListElement{index: "5"}
        ListElement{index: "6"}
        ListElement{index: "7"}
        ListElement{index: "8"}
        ListElement{index: "9"}
        ListElement{index: "A"}
        ListElement{index: "B"}
        ListElement{index: "C"}
        ListElement{index: "D"}
        ListElement{index: "E"}
        ListElement{index: "F"}
    }
    // 상부 열
    Rectangle{
        width: 402
        height: 24
        x: 24
//        x:164
//        y:183
        color: "#999999"
//        border.color: "#898989"
        Row{
            anchors.centerIn: parent
            spacing: 15.6
            Repeater{
                model: listmodel
                Text{
                    font.pixelSize: 16
                    font.bold: true
                    text: index
                }
            }
        }
    }
    // 왼쪽 열
    Rectangle{
        width: 24
        height: 401
//        x:41
//        y:106
        y:24
        color: "#999999"
//        border.color: "#898989"
        Column{
            anchors.centerIn: parent
            spacing: 7
            Repeater{
                model: listmodel
                Text{
                    font.pixelSize: 16
                    font.bold: true
                    text: index + "0"
                }
            }
        }
    }
    // 내부 표
    Rectangle{
        width: 402
        height: 401
//        x:64
//        y:106
        y:24
        x: 24
        color: "#FFFFFF"
        border.color: "#999999"
        Rectangle{
            width: 1
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#999999"
        }
        Grid{
            anchors.centerIn: parent
            rows: 16
            columns: 16
            rowSpacing: 7
            columnSpacing: 7
            Repeater{
                model: internal_monitor_page.bankIOList
                Rectangle{
                    width: 18
                    height: 18
                    border.width: 2
                    border.color: "#999999"
                    color: (modelData.value === true && pageModel.dataLoaded === true) ? "green" : "#FFFFFF"
                }
            }
        }
    }

    // 십진법
    Rectangle{
        width: 109
        height: 24
        anchors.right: parent.right
        anchors.top: parent.top
//        x:465
//        y:24
        color: "#999999"
//        border.color: "#898989"
        Text{
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
            color: resourceManager.dataColor(33, 19)
            text: resourceManager.label(33,19,local.language)
        }
    }

    // 오른쪽 숫자값
    Column{
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 24
//        x:465
//        y:106
        spacing: -1
        Repeater{
            model: 16
            Row{
                spacing: -1
                Rectangle{
                    width: 55
                    height: 26
                    color: "#FFFFFF"
                    border.color: "#999999"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 18
                        font.bold: true
                        text: bankIoByteList[index*2]
                    }
                }
                Rectangle{
                    width: 55
                    height: 26
                    color: "#FFFFFF"
                    border.color: "#999999"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 18
                        font.bold: true
                        text: bankIoByteList[index*2+1]
                    }
                }
            }
        }
    }
    }
}
