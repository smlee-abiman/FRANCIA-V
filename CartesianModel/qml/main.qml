import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import "debug"
import "Components"
import "Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: main.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.19
* Worker: IJ.YI
**************************************************************************************/
ApplicationWindow {
    visible: true
//    width: 1024
//    height: 768
    maximumWidth: 1288
    maximumHeight: 810
    minimumWidth: 1288
    minimumHeight: 810
    property int opeCount: 0
    property int errorCount: 0
    property int sysCount: 0
    property int alarmCount: 0
    property int axisNumber: 0
    property int axis1_AlarmCount: 0
    property int axis2_AlarmCount: 0
    property int axis3_AlarmCount: 0
    property int axis4_AlarmCount: 0
    property int axis5_AlarmCount: 0
    property int axis6_AlarmCount: 0
    property int axis7_AlarmCount: 0
    property int axis8_AlarmCount: 0

    title: qsTr("AEK Pendant Demo")

    Row {
        spacing: 10
        MasterView {
            clip: true
        }
        Frame {
            width: 254
            height: 769
            Rectangle {
                id: key01
                width: 75
                height: 35
                anchors.top: parent.top
                anchors.topMargin: 5
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key01Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,38)
                    text: "key01" //resourceManager.label(20,38,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key01.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(1)
                    }
                    onReleased: {
                        key01.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key02
                width: 75
                height: 35
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.left: key01.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key02Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,39)
                    text: "key02" //resourceManager.label(20,39,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key02.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(2)
                    }
                    onReleased: {
                        key02.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key03
                width: 75
                height: 35
                anchors.top: key01.bottom
                anchors.topMargin: 10
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key03Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,40)
                    text: "key03" //resourceManager.label(20,40,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key03.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(3)
                    }
                    onReleased: {
                        key03.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key04
                width: 75
                height: 35
                anchors.top: key01.bottom
                anchors.topMargin: 10
                anchors.left: key03.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key04Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,41)
                    text: "key04" //resourceManager.label(20,41,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key04.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(4)
                    }
                    onReleased: {
                        key04.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key05
                width: 75
                height: 35
                anchors.top: key03.bottom
                anchors.topMargin: 48
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key05Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,45)
                    text: "key05" //resourceManager.label(20,45,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key05.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(5)
                    }
                    onReleased: {
                        key05.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key06
                width: 75
                height: 35
                anchors.top: key03.bottom
                anchors.topMargin: 10
                anchors.left: key03.right
                anchors.leftMargin: 4
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key06Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,42)
                    text: "key06" //resourceManager.label(20,42,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key06.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(6)
                    }
                    onReleased: {
                        key06.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key07
                width: 75
                height: 35
                anchors.top: key06.bottom
                anchors.topMargin: 40
                anchors.left: key05.right
                anchors.leftMargin: 4
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key07Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,43)
                    text: "key07" //resourceManager.label(20,43,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key07.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(7)
                    }
                    onReleased: {
                        key07.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key08
                width: 75
                height: 35
                anchors.top: key04.bottom
                anchors.topMargin: 48
                anchors.left: key05.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key08Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,44)
                    text: "key08" //resourceManager.label(20,44,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key08.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(8)
                    }
                    onReleased: {
                        key08.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key09
                width: 75
                height: 35
                anchors.top: key05.bottom
                anchors.topMargin: 86
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key09Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,45)
                    text: "key09" //resourceManager.label(20,45,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key09.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(9)
                    }
                    onReleased: {
                        key09.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key10
                width: 75
                height: 35
                anchors.top: key07.bottom
                anchors.topMargin: 10
                anchors.left: key05.right
                anchors.leftMargin: 5
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key10Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,42)
                    text: "key10" //resourceManager.label(20,42,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key10.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(10)
                    }
                    onReleased: {
                        key10.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key11
                width: 75
                height: 35
                anchors.top: key10.bottom
                anchors.topMargin: 40
                anchors.left: key09.right
                anchors.leftMargin: 5
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key11Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,43)
                    text: "key11" //resourceManager.label(20,43,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key11.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(11)
                    }
                    onReleased: {
                        key11.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key12
                width: 75
                height: 35
                anchors.top: key08.bottom
                anchors.topMargin: 86
                anchors.left: key09.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key12Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,44)
                    text: "key12" //resourceManager.label(20,44,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key12.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(12)
                    }
                    onReleased: {
                        key12.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key13
                width: 75
                height: 35
                anchors.top: key11.bottom
                anchors.topMargin: 10
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key13Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,46)
                    text: "key13" //resourceManager.label(20,46,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key13.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(13)
                    }
                    onReleased: {
                        key13.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key14
                width: 75
                height: 35
                anchors.top: key11.bottom
                anchors.topMargin: 10
                anchors.left: key13.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key14Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,47)
                    text: "key14" //resourceManager.label(20,47,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key14.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(14)
                    }
                    onReleased: {
                        key14.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key15
                width: 75
                height: 35
                anchors.top: key13.bottom
                anchors.topMargin: 10
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key15Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,46)
                    text: "key15" //resourceManager.label(20,46,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key15.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(15)
                    }
                    onReleased: {
                        key15.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key16
                width: 75
                height: 35
                anchors.top: key13.bottom
                anchors.topMargin: 10
                anchors.left: key15.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key16Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,47)
                    text: "key16" //resourceManager.label(20,47,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key16.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(16)
                    }
                    onReleased: {
                        key16.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key17
                width: 75
                height: 35
                anchors.top: key15.bottom
                anchors.topMargin: 10
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key17Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,48)
                    text: "key17" //resourceManager.label(20,48,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key17.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(17)
                    }
                    onReleased: {
                        key17.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }
            Rectangle {
                id: key18
                width: 75
                height: 35
                anchors.top: key15.bottom
                anchors.topMargin: 10
                anchors.left: key17.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: key18Text
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    //color: resourceManager.rgb(20,49)
                    text: "key18" //resourceManager.label(20,49,resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        key18.color = "#D1D1D1"
                        phyKeyManager.setEmulatedMemblemSwStatus(18)
                    }
                    onReleased: {
                        key18.color = "#EEEEEE"
                        phyKeyManager.setEmulatedMemblemSwStatus(0)
                    }
                }
            }

            Text {
                anchors.top: key06.bottom
                anchors.topMargin: 12
                anchors.left: key03.right
                anchors.leftMargin: 28
                font.pointSize: 7
                color: resourceManager.rgb(20,50)
                text: resourceManager.label(20,50,resourceManager.language)
            }
            Text {
                anchors.top: key10.bottom
                anchors.topMargin: 12
                anchors.left: key03.right
                anchors.leftMargin: 28
                font.pointSize: 7
                color: resourceManager.rgb(20,51)
                text: resourceManager.label(20,51,resourceManager.language)
            }
            Text {
                anchors.top: key11.bottom
                anchors.topMargin: 18
                anchors.left: key03.right
                anchors.leftMargin: 30
                font.pointSize: 7
                color: resourceManager.rgb(20,52)
                text: resourceManager.label(20,52,resourceManager.language)
            }
            Text {
                anchors.top: key13.bottom
                anchors.topMargin: 18
                anchors.left: key03.right
                anchors.leftMargin: 30
                font.pointSize: 7
                color: resourceManager.rgb(20,53)
                text: resourceManager.label(20,53,resourceManager.language)
            }
            Text {
                anchors.top: key15.bottom
                anchors.topMargin: 18
                anchors.left: key03.right
                anchors.leftMargin: 30
                font.pointSize: 7
                color: resourceManager.rgb(20,54)
                text: resourceManager.label(20,54,resourceManager.language)
            }

            Rectangle {
                id: deadmanSw
                property bool deadmanSwStatus: false
                width: 75
                height: 35
                anchors.top: key17.bottom
                anchors.topMargin: 20
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: deadmanSwText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    text: "DEADMAN"
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        if (deadmanSw.deadmanSwStatus === true){
                            deadmanSw.color = "#EEEEEE"
                            deadmanSw.deadmanSwStatus = false
                        } else {
                            deadmanSw.color = "#D1D1D1"
                            deadmanSw.deadmanSwStatus = true
                        }
                        phyKeyManager.setEmulatedDeadManSwStatus(deadmanSw.deadmanSwStatus)
                    }
                }
            }
            Rectangle {
                id: jogDialSw
                width: 75
                height: 35
                anchors.top: key17.bottom
                anchors.topMargin: 20
                anchors.left: deadmanSw.right
                anchors.leftMargin: 80
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: jogDialText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    text: "JOG-SW"
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        jogDialSw.color = "#D1D1D1"
                        phyKeyManager.setEmulatedJogDialSwStatus(true)
                    }
                    onReleased: {
                        jogDialSw.color = "#EEEEEE"
                        phyKeyManager.setEmulatedJogDialSwStatus(false)
                    }
                }
            }
            Rectangle {
                id: selectOrgSw
                width: 75
                height: 35
                anchors.top: deadmanSw.bottom
                anchors.topMargin: 10
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: selectOrgSwText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    text: "SEL-Org"
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        selectOrgSw.color = "#D1D1D1"
//                        phyKeyManager.setEmulatedSelectSwStatus(3)
                    }
                    onReleased: {
                        selectOrgSw.color = "#EEEEEE"
//                        phyKeyManager.setEmulatedSelectSwStatus(0)
                    }
                    onClicked: {
                        phyKeyManager.selectSwMode = 3
                    }
                }
            }
            Rectangle {
                id: selectManualSw
                width: 75
                height: 35
                anchors.top: deadmanSw.bottom
                anchors.topMargin: 10
                anchors.left: selectOrgSw.right
                anchors.leftMargin: 4
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: selectManualSwText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    text: "SEL-Manu"
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        selectManualSw.color = "#D1D1D1"
//                        phyKeyManager.setEmulatedSelectSwStatus(2)
                    }
                    onReleased: {
                        selectManualSw.color = "#EEEEEE"
//                        phyKeyManager.setEmulatedSelectSwStatus(0)
                    }
                    onClicked: {
                        phyKeyManager.selectSwMode = 2
                    }
                }
            }
            Rectangle {
                id: selectAutoSw
                width: 75
                height: 35
                anchors.top: deadmanSw.bottom
                anchors.topMargin: 10
                anchors.left: selectManualSw.right
                anchors.leftMargin: 4
                radius: 3
                color: "#EEEEEE"
                border.color: "#A3A3A3"
                border.width: 1
                Text {
                    id: selectAutoSwText
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 8
                    text: "SEL-Auto"
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        selectAutoSw.color = "#D1D1D1"
//                        phyKeyManager.setEmulatedSelectSwStatus(1)
                    }
                    onReleased: {
                        selectAutoSw.color = "#EEEEEE"
//                        phyKeyManager.setEmulatedSelectSwStatus(0)
                    }
                    onClicked: {
                        phyKeyManager.selectSwMode = 1
                    }
                }
            }

            Row{
                y:570
                spacing: 5
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 45
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 15
                    text: "keyNo:"
                }
                Rectangle{
                    width: 60
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    border.color: "#898989"
                    color: "#b5b5b6"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        text: commonViewModel.currentKeyNo === -1 ? "" : commonViewModel.currentKeyNo
                    }
                }
            }

            Row{
                spacing: 10
                Row{
                    y:610
                    spacing: 5
                    Text{
                        width: 45
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: 15
                        text: "dp1:"
                    }
                    Rectangle{
                        width: 60
                        height: 30
                        anchors.verticalCenter: parent.verticalCenter
                        border.color: "#898989"
                        color: "#b5b5b6"
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            text:commonViewModel.currentDp1
                        }
                    }
                }
                Row{
                    y:610
                    spacing: 5
                    Text{
                        width: 45
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: 15
                        text: "dp2:"
                    }
                    Rectangle{
                        width: 60
                        height: 30
                        anchors.verticalCenter: parent.verticalCenter
                        border.color: "#898989"
                        color: "#b5b5b6"
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            text:commonViewModel.currentDp2
                        }
                    }
                }
            }

            Row{
                spacing: 5
                y:655
                Rectangle {
                    width: 80
                    height: 40
                    color: "#EEEEEE"
                    border.color: "#A3A3A3"
                    enabled: teachingViewModel.currentPageTab === 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 9
                        text: "ncp import"
                        color: "black"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressed: parent.color = "#D1D1D1"
                        onReleased: parent.color = "#EEEEEE"
                        onClicked: {
                            ncpfile.open()
                        }
                    }
                }
                Rectangle {
                    width: 80
                    height: 40
                    color: "#EEEEEE"
                    border.color: "#A3A3A3"
                    enabled: teachingViewModel.currentPageTab === 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 9
                        text: "ncp output"
                        color: "black"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressed: parent.color = "#D1D1D1"
                        onReleased: parent.color = "#EEEEEE"
                        onClicked: {
                            ncpfilesave.open()
                        }
                    }
                }
                Rectangle {
                    width: 80
                    height: 40
                    color: "#EEEEEE"
                    border.color: "#A3A3A3"
                    enabled: teachingViewModel.currentPageTab === 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 9
                        text: "compile"
                        color: "black"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressed: parent.color = "#D1D1D1"
                        onReleased: parent.color = "#EEEEEE"
                        onClicked: {
                            teachingViewModel.pageModel[6].demoComplieNcp()
                        }
                    }
                }

            }

            Row{
                y:706
                spacing: 5
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 15
                    text: "ncp name:"
                }
                Rectangle{
                    width: 155
                    height: 35
                    anchors.verticalCenter: parent.verticalCenter
                    border.color: "#898989"
                    color: "#b5b5b6"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 15
                        text: teachingViewModel.pageModel[6].editNcp
                    }
                }
            }
        }
    }

    Rectangle{
        width: 252
        height: 1
        x:1035
        y:482
        color: "#c9c9c8"
    }
    Rectangle{
        width: 252
        height: 1
        x:1035
        y:660
        color: "#c9c9c8"
    }
    Rectangle{
        width: 1288
        height: 42
        border.color: "#B5B5B6"
        y:768
        Row{
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2
            Row{
                spacing: 4
                Text{
                    width: 60
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 13
                    color: resourceManager.rgb(50,92)
                    text: resourceManager.label(50,92,resourceManager.language)+":"
                }
                CheckBox {
                    id:emg
                    anchors.verticalCenter: parent.verticalCenter
                    width:25
                    height: 50
                    onClicked: {
                        if(emg.checked == 1){
                            commonViewModel.trigger_EmergencyStates(true);
                        }else{
                            commonViewModel.trigger_EmergencyStates(false);
                        }
                    }
                }
            }//急停
            Row{
                spacing: 4
                Text{
                    width: 60
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 13
                    text: resourceManager.label(53,1,resourceManager.language)+":"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    value: opeCount
                    fontPixelSize: 12
                    onReady: {
                        commonViewModel.trigger_ope(false)
                    }
                    onEnter: {
                        let edittedValue = Number(newValue)
                        opeCount = edittedValue
                        commonViewModel.trigger_ope(opeCount)
                    }
                }
            }//动作表示
            Row{
                spacing: 4
                Text{
                    width: 60
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(53,0,resourceManager.language)+":"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: errorCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        errorCount = edittedValue
                        commonViewModel.trigger_CurrentState(errorCount,alarmCount,sysCount)
                    }
                }
            }//操作错误
            Row{
                spacing: 4
                Text{
                    width: 60
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(50,10,resourceManager.language)+":"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: sysCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        sysCount = edittedValue
                        commonViewModel.trigger_CurrentState(errorCount,alarmCount,sysCount)
                    }
                }
            }//系統警報
            Row{
                spacing: 4
                Text{
                    width: 40
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(41,548,resourceManager.language)+":"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: alarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        alarmCount = edittedValue
                        commonViewModel.trigger_CurrentState(errorCount,alarmCount,sysCount)
                    }
                }
            }//警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,0,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis1_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis1_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(0,axis1_AlarmCount)
                    }
                }
            }//軸1警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,1,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis2_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis2_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(1,axis2_AlarmCount)
                    }
                }
            }//軸2警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,2,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis3_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis3_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(2,axis3_AlarmCount)
                    }
                }
            }//軸3警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,3,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis4_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis4_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(3,axis4_AlarmCount)
                    }
                }
            }//軸4警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,4,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis5_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis5_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(4,axis5_AlarmCount)
                    }
                }
            }//軸5警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,5,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis6_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis6_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(5,axis6_AlarmCount)
                    }
                }
            }//軸6警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,6,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis7_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis7_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(6,axis7_AlarmCount)
                    }
                }
            }//軸7警報
            Row{
                spacing: 4
                Text{
                    width: 58
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    font.pixelSize: 12
                    text: resourceManager.label(22,7,resourceManager.language) + "alm:"
                }
                LimitNumericEditBox{
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36
                    height: 30
                    borderWidth: 1
                    rectRadius: 0
                    fontPixelSize: 12
                    value: axis8_AlarmCount
                    onEnter: {
                        let edittedValue = Number(newValue)
                        axis8_AlarmCount = edittedValue
                        commonViewModel.trigger_AxisState(7,axis8_AlarmCount)
                    }
                }
            }//軸8警報
        }
    }

    Connections {
        id: fmWindow
        onVisibleChanged: {
            if(!target.visible)
                edittbl.value = target.digits
        }
    }

    FileDialog {
        id: fmDialog
        title: "Select test data"
        folder: shortcuts.home
        onAccepted: {
            debugtool.feedTestData(fmDialog.fileUrl)
        }
    }
    FileDialog{
        id:ncpfile
        title: "ncp file select"
        nameFilters: ["ncp file(*.ncp)"]
        onAccepted: {
            teachingViewModel.pageModel[6].anysReloadNcp(ncpfile.fileUrl)
        }
    }
    FileDialog{
        id:ncpfilesave
        title: "ncp file save"
        selectExisting: false
        nameFilters: ["ncp file(*.ncp)"]
        onAccepted: {
            teachingViewModel.pageModel[6].saveNCPFile1(ncpfilesave.fileUrl)
        }
    }

//    DebugWindow{

//    }
}
