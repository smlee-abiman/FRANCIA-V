import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Widget"

Dialog {
    id: root
    closePolicy: Popup.CloseOnEscape
    onOpened: {
        root.x = (parent.width - width) / 2
        root.y = (parent.height - height) / 2
    }
    onClosed: {
        root.forceIoData = null
        root.ioSwapConfig = null
        viewModel.ioSwapConfigTblsIndex = 0
        local.currentPage = 0
        listview2.currentIndex = -1
    }

    readonly property var viewModel: monitorViewModel.ioForceView
    property var forceIoData: null
    property var ioSwapConfig: null

    QtObject {
        id: local
        readonly property int numOfItemsParPage: 8
        readonly property int numOfPages: Math.ceil(viewModel.ioSwapConfigTbl.length / numOfItemsParPage)
        property int currentPage: 0
        readonly property var ioSwapConfigTblsStatus: viewModel.getIoSwapConfigTblsStatus(root.forceIoData);
        onIoSwapConfigTblsStatusChanged: {
            for (let i = 0; i < ioSwapConfigTblsStatus.length; i++) {
                if (ioSwapConfigTblsStatus[i]) {
                    viewModel.ioSwapConfigTblsIndex = i
                }
            }
        }
    }
    function pageUp() {
        let page = local.currentPage + local.numOfPages - 1
        local.currentPage = page % local.numOfPages
        listview2.positionViewAtIndex(local.currentPage * local.numOfItemsParPage, ListView.Beginning)
    }
    function pageDown() {
        let page = local.currentPage + local.numOfPages + 1
        local.currentPage = page % local.numOfPages
        listview2.positionViewAtIndex(local.currentPage * local.numOfItemsParPage, ListView.Beginning)
    }

    // see : Select_Variable_Dialog.qml
    function setDlgPoint(dlgX ,dlgY)
     {
         //设置窗口拖拽不能超过父窗口
         if(root.x + dlgX < 0){
             root.x = 0
         }else if(root.x + dlgX > root.parent.width - root.width){
             root.x = root.parent.width - root.width
         }else{
             root.x = root.x + dlgX
         }

         if(root.y + dlgY < 0){
             root.y = 0
         }else if(root.y + dlgY > root.parent.height - root.height){
             root.y = root.parent.height - root.height
         }else{
             root.y = root.y + dlgY
         }
     }

    // see : Select_Variable_Dialog.qml
    background: bac
    Image{
        id:bac
        source: resourceManager.imagePath + "/08_MENU/d084_03.png"
        Text{
            width: 550
            height: 52
            x:0
            y:0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 26
            color: resourceManager.rgb(33, 53)
            text: resourceManager.label(33, 53, resourceManager.language)
            MouseArea{
                anchors.fill: parent
                property point clickpoint: "0,0"
                onPressed: {
                    clickpoint = Qt.point(mouse.x,mouse.y)
                }
                onPositionChanged: {
                    var offset = Qt.point(mouse.x-clickpoint.x,mouse.y-clickpoint.y)
                    setDlgPoint(offset.x, offset.y)
                }
            }
        }
        ImageButton{
            x:500
            y:5
            sacle: 1.5
            upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
            downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
            onClicked: root.close()
        }
    }

    // see : Select_Variable_Dialog.qml
    //框线
    Rectangle{
        width: 548
        height: 1
        x:-8
        y:356
        color: "#1FA7E4"
    }

    // see : Select_Variable_Dialog.qml
    // left side.
    Rectangle {
        width: 120
        height: 306
        x:-4
        y:46
        color: "transparent"
        border.color: "#1FA7E4"
        Component {
            id: leftSideListDelegate
            Rectangle {
                width: 110
                height: 34
                anchors.horizontalCenter: parent.horizontalCenter
                color: {
                    if (local.ioSwapConfigTblsStatus.length <= index) {
                        return "#B5B5B6"
                    } else if (!local.ioSwapConfigTblsStatus[index]) {
                        return "#B5B5B6"
                    } else if (index === listview.currentIndex) {
                        return "white"
                    } else {
                        return "#2EA7E0"
                    }
                }
                border.color: "#1FA7E4"
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    color: resourceManager.rgb(33, 44 + index)
                    text: resourceManager.label(33, 44 + index, resourceManager.language)
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (local.ioSwapConfigTblsStatus.length <= index) {
                            return;
                        } else if (!local.ioSwapConfigTblsStatus[index]) {
                            return;
                        }
                        viewModel.ioSwapConfigTblsIndex = index
                        local.currentPage = 0
                    }
                }
            }
        }
        ListView {
            id: listview
            anchors {
                fill: parent
                topMargin: 3
            }
            interactive: false
            spacing: 4
            model: 8
            currentIndex: viewModel.ioSwapConfigTblsIndex
            delegate: leftSideListDelegate
        }
    }

    // right side.
    Rectangle {
        width: 414
        height: 306
        x:120
        y:46
        color: "transparent"
        border.color: "#1FA7E4"
        Item {
            id: workArea
            width: parent.width-2
            height: parent.height-2
            anchors.centerIn: parent
            // rigth side.
            Rectangle {
                width: workArea.width
                height: workArea.height
                color: "transparent"
                ImageScrollBar {
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: (local.currentPage + 1) + "/" + local.numOfPages
                    onScrollUpClicked:   pageUp()
                    onScrollDownClicked: pageDown()
                }
                Component {
                    id: rightSideListDelegate
                    Row {
                        spacing: 4
                        visible: modelData.isValid
                        // index.
                        Rectangle {
                            width: 50; height: 34
                            border.width: (index === listview2.currentIndex) ? 2 : 1
                            border.color: (index === listview2.currentIndex) ? "orange" : "#B5B5B6"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: ('00' + (index + 1)).slice(-2)
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    root.ioSwapConfig = modelData
                                    listview2.currentIndex = index
                                }
                            }
                        }
                        Rectangle {
                            width: 290; height: 34
                            border.width: (index === listview2.currentIndex) ? 2 : 1
                            border.color: (index === listview2.currentIndex) ? "orange" : "#B5B5B6"
                            // separator.
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#B5B5B6"
                            }
                            // io symbol.
                            Text {
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: modelData.isValid ? resourceManager.symbolAt(modelData.diAddress) : ""
                            }
                            // io name.
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: modelData.isValid ? resourceManager.getMessageStringValue("1", modelData.diAddress, resourceManager.language) : ""
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    root.ioSwapConfig = modelData
                                    listview2.currentIndex = index
                                }
                            }
                        }
                    }
                }
                ListView {
                    id: listview2
                    width: 346
                    height: 300
                    x:2
                    y:2
                    interactive: false
                    spacing: 4
                    clip: true
                    snapMode: ListView.SnapToItem
                    model: {
                        if (local.ioSwapConfigTblsStatus.length <= viewModel.ioSwapConfigTblsIndex) {
                            return 0;
                        } else if (!local.ioSwapConfigTblsStatus[viewModel.ioSwapConfigTblsIndex]) {
                            return 0;
                        }
                        return viewModel.ioSwapConfigTbl;
                    }
                    onModelChanged: {
                        root.ioSwapConfig = null
                        currentIndex = -1
                    }
                    delegate: rightSideListDelegate
                }
            }
        }
    }

    // cancel, apply button.
    Row{
        x:310
        y:360
        spacing: 20
        ImageButton{
            upImageSource: resourceManager.imagePath + "/08_MENU/d084_06.png"
            downImageSource: resourceManager.imagePath + "/08_MENU/d084_07.png"
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                font.pixelSize: 20
                color: resourceManager.rgb(41,293)
                text: resourceManager.label(41,293,resourceManager.language)
            }
            onClicked: root.close()
        }
        ImageButton{
            upImageSource: resourceManager.imagePath + "/08_MENU/d084_04.png"
            downImageSource: resourceManager.imagePath + "/08_MENU/d084_05.png"
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                font.pixelSize: 20
                color: resourceManager.rgb(41,292)
                text: resourceManager.label(41,292,resourceManager.language)
            }
            onClicked: {
                if ((root.forceIoData === null) || (root.ioSwapConfig === null)) {
                    return;
                }
                viewModel.swapIO(root.forceIoData, root.ioSwapConfig);
                root.close()
            }
        }
    }
}
