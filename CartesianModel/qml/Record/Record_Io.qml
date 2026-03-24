import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: Record_Io.qml
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
    id: record_io
    property var viewModel: ioHistoryViewModel
    property bool pageActive: false
    property int page: 0

    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.HistoryIO)
            viewModel.activate()
        } else {
            viewModel.deactivate()
            page = 0
        }
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
                text: resourceManager.label(50,13, local.language)
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
                text: resourceManager.label(50,14, local.language)
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
                text: resourceManager.label(50,12, local.language)
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

        IO_2 {
            pageActive: record_io.pageActive && (page == 0)
        }
        Loader {
            source: "qrc:/qml/Record/IO_3.qml";
            active: false
            property bool isActive: record_io.pageActive && (page == 1)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if (isActive)
                    active = true
                if(status == Loader.Ready)
                    item.pageActive = isActive;
            }
        }
        Loader {
            source: "qrc:/qml/Record/IO_1.qml";
            active: false
            property bool isActive: record_io.pageActive && (page == 2)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if (isActive)
                    active = true
                if(status == Loader.Ready)
                    item.pageActive = isActive;
            }
        }
    }
}
