import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Shapes 1.12
import QtQml 2.12
import Enums 1.0

Item {
    id: manualPage
    width: 1019
    height: 591
    property int mnpage: 0
    property bool pageActive: false

    Text {
        anchors.centerIn: parent
        font.pixelSize: 22
        text: "autopage"
    }

//    onPageActiveChanged: {
//        if (pageActive) {
//            manual_tab.changeManualPage(manual_tab.currentIndex)
//            io_operate_page.active = true
//        } else {
//            manual_tab.currentIndex = 0;
//        }
//    }

//    Image{
//        width: 1019
//        height: 591
//        anchors.fill: parent.Center
//        source: resourceManager.imagePath + "/06_MANU/d060_01.png"
//    }//背景

//    TabBar{
//        id: manual_tab
//        width:326
//        height: 78
//        anchors.top: parent.top
//        anchors.left: parent.left
//        anchors.topMargin: 4
//        z:1
//        spacing: 16
//        background: Rectangle{
//            color: "transparent"
//        }
//        TabButton{
//            id:axis_tab
//            z: (manual_tab.currentIndex  == 1)
//            background: Image {
//                source: (manual_tab.currentIndex  == 0 || axis_tab.pressed) ? resourceManager.imagePath + "/06_MANU/d060_03.png" : resourceManager.imagePath + "/06_MANU/d060_02.png"
//            }
//            Text {
//                anchors.fill: parent
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//                font.pixelSize: axis_tab.pressed ? 24 : 23
//                lineHeight: 0.7
//                text: resourceManager.label(31,2, resourceManager.language)
//                color: manual_tab.currentIndex === 0 ? "black" : resourceManager.rgb(31,2)
//            }
//            onClicked: {
//                manual_tab.currentIndex = 0
//                manual_tab.changeManualPage(manual_tab.currentIndex)
//                phyKeyManager.ctrlBuzzer()
//            }
//        }
//        TabButton{
//            id:io_tab
//            z: (manual_tab.currentIndex  == 1)
//            background: Image {
//                source: (manual_tab.currentIndex  == 1 || io_tab.pressed) ? resourceManager.imagePath + "/06_MANU/d060_03.png" : resourceManager.imagePath + "/06_MANU/d060_02.png"
//            }
//            Text {
//                anchors.fill: parent
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//                lineHeight: 0.7
//                font.pixelSize: io_tab.pressed ? 24 : 23
//                color: manual_tab.currentIndex === 1 ? "black" : resourceManager.rgb(31,1)
//                text: resourceManager.label(31,1, resourceManager.language)
//            }
//            onClicked: {
//                manual_tab.currentIndex = 1
//                manual_tab.changeManualPage(manual_tab.currentIndex)
//                phyKeyManager.ctrlBuzzer()
//            }
//        }

//        function changeManualPage(page) {
//            switch(page) {
//            case 0:
//                commonViewModel.pageChange(Enums.ManualAxisOpe)    // 軸操作
//                break
//            case 1:
//                commonViewModel.pageChange(Enums.ManualIO)          // I/O操作
//                break
//            default:
//                break
//            }
//        }
//    }

//    StackLayout{
//        id: stack_manual
//        width: parent.width
//        height: 590
//        anchors.top: parent.top
//        anchors.left: parent.left
//        anchors.topMargin: 80
//        currentIndex: manual_tab.currentIndex

//        Axis_operate{
//            id: axis_operate_page
//            property bool isActive: (manualPage.pageActive && (manual_tab.currentIndex == 0))
//            pageActive: isActive
//        }

//        Loader {
//            id: io_operate_page
//            property bool isActive: (manualPage.pageActive && (manual_tab.currentIndex == 1))
//            active: false
//            asynchronous: !isActive
//            sourceComponent: IO_operate {
//                pageActive: io_operate_page.isActive
//            }
//        }
//    }
}


