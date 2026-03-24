import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: Record_Alm.qml
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
    id: record_alarm
    property var viewModel: alarmHistoryViewModel
    property bool pageActive: false
    property int page: 0

    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.HistoryAlarm)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }

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
    Row{
        x: 660
        y: -45
        spacing: 1
        Rectangle{
            width: 100
            height: 46
            color: page == 0 || but_mouse1.pressed ? "#cc3333" : "#cccccc"
            border.color: "#999999"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: but_mouse1.pressed ? 17 : 18
                text: resourceManager.label(50,9, local.language)
                color: (page == 0) ? "#ffffff" : "black"
            }
            MouseArea{
                id:but_mouse1
                anchors.fill: parent
                onClicked:{
                    page = 0
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            width: 100
            height: 46
            color: page == 1 || but_mouse2.pressed ? "#cc3333" : "#cccccc"
            border.color: "#999999"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: but_mouse2.pressed ? 17 : 18
                text: resourceManager.label(50,10, local.language)
                color: (page == 1) ? "#ffffff" : "black"
            }
            MouseArea{
                id:but_mouse2
                anchors.fill: parent
                onClicked:{
                    page = 1
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            width: 100
            height: 46
            color: page == 2 || but_mouse3.pressed ? "#cc3333" : "#cccccc"
            border.color: "#999999"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: but_mouse3.pressed ? 17 : 18
                text: resourceManager.label(50,11, local.language)
                color: (page == 2) ? "#ffffff" : "black"
            }
            MouseArea{
                id:but_mouse3
                anchors.fill: parent
                onClicked:{
                    page = 2
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }//菜单切换

    StackLayout{
        width: parent.width
        height: 520
        currentIndex: page

        Alarm_1{
            pageActive: record_alarm.pageActive && (page == 0)
        }
        Loader {
            source: "qrc:/qml/Record/Alarm_2.qml";
            asynchronous: true
            property bool isActive: record_alarm.pageActive && (page == 1)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }
        Loader {
            source: "qrc:/qml/Record/Alarm_3.qml";
            asynchronous: true
            property bool isActive: record_alarm.pageActive && (page == 2)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }
    }
}
