import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"

import Enums 1.0            // Enums.FunctionControllerSetUser
/*************************************************************************************
* Title: UI/UX of AEK
* File: User_set.qml
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
    property var viewModel: functionViewModel.controllerSettingsView.userSettingView
    width: parent.width
    height: parent.height
    property int page: 0

    QtObject {
        id: local
        readonly property int currentUserLevel: commonViewModel.userLevel
        readonly property int numOfScreens: viewModel.numOfScreens
        readonly property int numOfItemsParPage: 6
        readonly property int numOfPages: Math.ceil(numOfScreens / numOfItemsParPage)
        property int currentPage: 0
        function pageUp() {
            local.currentPage = Math.max(local.currentPage - 1, 0);
            userSettingListView.positionViewAtIndex(local.currentPage * local.numOfItemsParPage, ListView.Beginning)
        }
        function pageDown() {
            local.currentPage = Math.min(local.currentPage + 1, local.numOfPages - 1);
            userSettingListView.positionViewAtIndex(local.currentPage * local.numOfItemsParPage, ListView.Beginning)
        }
    }

    Component.onDestruction: viewModel.deactivate()
    onPageActiveChanged: {
        // console.log(pageActive) // todo: debug.
        if (pageActive) {
            commonViewModel.pageChange(Enums.FunctionControllerSetUser)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }

    Row {
        x:315
        y:8
        spacing: 19
        Repeater {
            model: 5
            Text {
                width: 100
                height: 40
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 20
                text: resourceManager.label(42,63+index,resourceManager.language)
                color: resourceManager.rgb(42,63+index)
            }
        }
    }//标题栏

    Item {
        id: userSettingItem
        width: 920; height: 456
        y:50
        ListView {
            id: userSettingListView
            width: parent.width
            height: {
                const delegateHeigth = 65
                const numOfValidItems = Math.min(local.numOfScreens - (local.currentPage * local.numOfItemsParPage), local.numOfItemsParPage)
                return (delegateHeigth * numOfValidItems) + (spacing * (numOfValidItems - 1))
            }
            interactive: false
            clip: true
            spacing: 11
            snapMode: ListView.SnapToItem
            model: local.numOfScreens
            delegate: userSettingDelegate
        }
        Component {
            id: userSettingDelegate
            Rectangle {
                readonly property int screenIndex: index
                x:20
                width: 900; height: 65
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Rectangle {
                    width: 280; height: parent.height
                    color: "#999999"
                    border.color: "#999999"
                    border.width: 1
                    Image {
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        source: {
                            let imageFileIndex =  screenIndex + 8
                            let imageFileIndexString = ('00' + imageFileIndex).slice(-2)
                            return `${resourceManager.imagePath}/25_controller/d250_${imageFileIndexString}.png`
                        }
                    }
                    Text {
                        width: 218
                        height: 65
                        x:50
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        font.pixelSize: 23
                        color: resourceManager.rgb(42,23+screenIndex)
                        text: resourceManager.label(42,23+screenIndex,resourceManager.language)
                    }
                }
                Row {
                    x:320;
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 74
                    Repeater {
                        model: 5    // numof user levels.
                        CheckBox {
                            readonly property int userLevelIndex: index
                            readonly property var screenSettings: viewModel.screenSettingsTbl[screenIndex]
                            readonly property var screenVisible: screenSettings.visibles[userLevelIndex]
                            checked: screenVisible
                            enabled: (userLevelIndex <= local.currentUserLevel)
                            onClicked: {
                                viewModel.toggleScreenVisibility(screenSettings, userLevelIndex)
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
            }
        }
    }

    //换页按钮
    Item {
        x: 925
        y: 54
        width: 60          // ✅ "자리 폭" (필요한 값으로 조정)
        height: 440

        ImageScrollBar {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            topButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_03.png"
            topButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_04.png"
            botomButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_05.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_06.png"

            pageLabal: {
                let pageNum = 1
                let numOfPages
                pageNum += local.currentPage
                numOfPages = local.numOfPages
                return pageNum + "/" + numOfPages
            }
            pageLabelColor: "black"

            onScrollUpClicked:  local.pageUp()
            onScrollDownClicked: local.pageDown()
        }
    }
/***********************************************************************/
//    Connections {
//        target: phyKeyManager
//        enabled: pageActive
//        onJogScrollUpChanged: local.pageUp()
//        onJogScrollDownChanged: local.pageDown()
//    }
//    MouseArea {
//        anchors.fill: parent
//        enabled: pageActive
//        propagateComposedEvents: true
//        onPressed: mouse.accepted = false
//        onReleased: mouse.accepted = false
//        onClicked: mouse.accepted = false
//        onWheel: {
//            if(!REAL_TARGET) {
//                if(wheel.angleDelta.y > 0) {
//                    //console.log("mouse wheel up")
//                    phyKeyManager.decrementEmulatedJogCount()
//                } else if(wheel.angleDelta.y < 0) {
//                    //console.log("mouse wheel down")
//                    phyKeyManager.incrementEmulatedJogCount()
//                }
//            }
//        }
//    }
}
