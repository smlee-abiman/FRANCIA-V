import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.2
import OriginViewModel 1.0
import Enums 1.0
import "OriginUtils.js" as Util
import "../Widget"
import "../Components/MainDialog"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Origin.qml
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
/******************原点复归画面使用重复器放置仪表盘****************************/

Item {
    id:originRoot
    width: 1018
    height: 590
    property var viewModel: originViewModel
    property var axisDataList: originViewModel.axisInfo.axisDataList
    property int timerCnt: 0
    property int axisCount: originViewModel.getAxisCount()
    property bool pageActive: false
    property bool isScrollLinkageStauts:false
    property bool isAlreadyOriginPopup:false
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property bool orgLampswitch: false

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
            originViewModel.startPageChanging()
            commonViewModel.pageChange(Enums.Origin)
            viewModel.activate()
        } else {
            org_dialog.close();
            originViewModel.stopOriginCarriageMonitor()
            viewModel.deactivate()
        }
    }

    Component.onDestruction: viewModel.deactivate(ss)

    Rectangle{
        width: 1018
        height: 590
        anchors.fill: parent.Center
        radius: 6
        border.color: "#898989"
        border.width: 2
        color: "#E5F4F9"
    }//background
///-------------------------------------------------------------------------


///-------------------------------------------------------------------------

///-------------------------------------------------------------------------







/******************************left area********************************/
    Rectangle{
        x:8
        y:numAxis <= 5 ? 78 :8
        width: 507
        height: 62
        color: "#156BB3"
        Rectangle{
            width: 80
            height: 62
            color: "transparent"
            border.color: "#9DA3A4"
        }
        Rectangle{
            width: 250
            height: 62
            x:79
            color: "transparent"
            border.color: "#9DA3A4"
            Text {
                width: 80
                height: 62
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: 22
                color: resourceManager.rgb(30, 13)
                text: resourceManager.label(30, 13, local.language)
            }
            Text {
                width: 80
                height: 62
                x:85
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: 20
                color: resourceManager.rgb(30, 14)
                text: resourceManager.label(30, 14, local.language)
            }
            Text {
                width: 80
                height: 62
                x:170
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: 20
                color: resourceManager.rgb(30, 15)
                text: resourceManager.label(30, 15, local.language)
            }
        }
        Rectangle{
            width: 90
            height: 62
            x:328
            color: "transparent"
            border.color: "#9DA3A4"
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: 20
                color: resourceManager.rgb(30, 16)
                text: resourceManager.label(30, 16, local.language) + "\n" + resourceManager.label(30, 17, local.language)
//                text: resourceManager.label(30, 17, local.language)
            }
        }
        Rectangle{
            width: 90
            height: 62
            x:417
            color: "transparent"
            border.color: "#9DA3A4"
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: 20
                color: resourceManager.rgb(30, 18)
                text: resourceManager.label(30, 18, local.language)
            }
        }
    }//titles
    Column{
        x:8
        y: numAxis <= 5 ? 138 : 68
        spacing: 1
        Repeater{
            model: numAxis
            Rectangle{
                id:data_base
                width: 507
                height: 70
                color: "#2AB0DD"
                border.color: "#9DA3A4"
                property int axisNum: originRoot.axisOrder[index]

                Rectangle{
                    width: 80
                    height: 70
                    color: "transparent"
                    border.color: "#9DA3A4"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 6
                        font.pixelSize: 20
                        color: resourceManager.rgb(22, data_base.axisNum)
                        text: resourceManager.label(22, data_base.axisNum, local.language)
                    }//axis name
                }
                Rectangle{
                    width: 250
                    height: 70
                    x:79
                    color: "transparent"
                    border.color: "#9DA3A4"
                    Column{
                        anchors.centerIn: parent
                        spacing: 10
                        Row{
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 70
                            Rectangle{
                                width: 18
                                height: 18
                                radius: 9
                                border.width: 2
                                border.color: "#336699"
                                color: axisDataList[data_base.axisNum].home ? "#F0EB4C" : "#003366"
                            }//home
                            Rectangle{
                                width: 18
                                height: 18
                                radius: 9
                                border.width: 2
                                border.color: "#336699"
                                color: axisDataList[data_base.axisNum].over ? "#F0EB4C" : "#003366"
                            }//limit
                            Rectangle{
                                width: 18
                                height: 18
                                radius: 9
                                border.width: 2
                                border.color: "#336699"
                                color: axisDataList[data_base.axisNum].brake ? "#F0EB4C" : "#003366"
                            }//brake
                        }
                        ProgressBar{
                            width: 220
                            height: 27
                            anchors.horizontalCenter: parent.horizontalCenter
                            background: Rectangle{
                                width: parent.width
                                height: parent.height
                                color: "white"
                            }
                            contentItem: Item {
                                Rectangle{
                                    implicitWidth:parseInt(axisDataList[originRoot.axisOrder[index]].origin * parent.width / 100, 10)
                                    height: parent.height
                                    color:((axisDataList[originRoot.axisOrder[index]].origin < 20) || (axisDataList[originRoot.axisOrder[index]].origin > 80)) ? "#fff200" : "#00d656"
                                    Text {
                                        id: text1
                                        anchors.centerIn: parent
                                        z:2
                                        font.bold: true
                                        font.pixelSize: 15
                                        color: "black"
                                        text: axisDataList[originRoot.axisOrder[index]].originPercent
                                    }
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: 90
                    height: 70
                    x:328
                    color: "transparent"
                    border.color: "#9DA3A4"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 18
                        color: "black"
                        text:  axisDataList[data_base.axisNum].position + "\n" + axisDataList[data_base.axisNum].encoder
                    }
                }
                Rectangle{
                    width: 90
                    height: 70
                    x:417
                    color: "transparent"
                    border.color: "#9DA3A4"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 18
                        color: "black"
                        text: axisDataList[data_base.axisNum].torque
                    }
                }
            }
        }
    }//axis data

/******************************right area********************************/
    Rectangle{
        id: org_start
        x: mouse.pressed ? 524 : 522
        y: mouse.pressed ? 80 : 78
        width: 180
        height: 80
        enabled: !originRoot.isAlreadyOriginPopup
        border.width: 3
        border.color: "#898989"
        color: mouse.pressed ? "yellow" : "#DAE3F3"
        Text {
            anchors.centerIn: parent
            font.pixelSize: mouse.pressed ? 30 : 28
            color: resourceManager.rgb(30, 19)
            font.bold: true
            text: resourceManager.label(30, 19, local.language)
        }
        MouseArea{
            id: mouse
            anchors.fill: parent
            onClicked: {
                if (originRoot.pageActive === true){
                    commonViewModel.modbusMemblemSwControl(true, Enums.OriginStartKeyNo)
                    originViewModel.originStatus = 0;
                    commonViewModel.modbusMemblemSwControl(false, Enums.OriginStartKeyNo)
                    originViewModel.startOriginCarriageMonitor()
                }
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//原点复归开始按钮

    Org_Dialog{
        id:org_dialog
        x:522
        y:182
        closePolicy: Popup.NoAutoClose
        //modal: true
        timer_switch: orgLampswitch
        onOpened: {
            timer_switch = true
        }
        onClosed: {
            originRoot.isAlreadyOriginPopup = false
        }
    }//原点复归窗口

/***********************************************************************/
    Connections{
        target:originViewModel
        enabled: originRoot.pageActive
        onOriginStatusChanged :{
            switch(originViewModel.originStatus){
            case 1:
                if (originRoot.pageActive === true){
                    org_dialog.open()
                    originRoot.isAlreadyOriginPopup = true
                }
                break
            case 2:
                break
            case 3:
                break
            case 4:
                orgLampswitch = false
                break
            default:
                break
            }
        }
    }
    Connections{
        target: commonViewModel
        enabled: originRoot.pageActive
        onTopPopupDialogChanged: {
            if(originRoot.isAlreadyOriginPopup === true && (commonViewModel.topPopupDialog === 6 || commonViewModel.topPopupDialog === 4)){
                originViewModel.stopOriginCarriageMonitor()
                org_dialog.close()
            }
        }
    }
    Connections{
        target:phyKeyManager
        enabled: originRoot.pageActive
        onMemblemSwKey01PressChanged:{
            if(phyKeyManager.memblemSwKey01Press){
                originViewModel.stopOriginCarriageMonitor()
                org_dialog.close()
            }
        }
        onMemblemSwKey02PressChanged:{
            if(phyKeyManager.memblemSwKey02Press){
                originViewModel.originStatus = 0;
                originViewModel.startOriginCarriageMonitor()
            }
        }
    }
}



