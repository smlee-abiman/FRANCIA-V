import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"

Item {
    id: root
    property bool pageActive: false
    width: 1012
    height: 520
    property int currentPage: 0
    readonly property var viewModel: monitorViewModel.ioForceView

    QtObject {
        id: penSet
        readonly property int category1StartPage: viewModel.category1StartPage
        readonly property int category2StartPage: viewModel.category2StartPage
        readonly property int category3StartPage: viewModel.category3StartPage
        readonly property var ioNameTextColorRBG: viewModel.ioNameTextColorRBG
        readonly property var pageTextColorRGB: viewModel.pageTextColorRGB
    }
    QtObject {
        id: local
        readonly property var forceIoDataTbl: viewModel.forceIoDataTbl
        readonly property int numOfItemsParPage: 6
        readonly property int numOfPages: Math.ceil(forceIoDataTbl.length / numOfItemsParPage)
        readonly property string selectedButtonColor: "yellow"
    }

    Component.onDestruction: viewModel.deactivate()
    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.MonitorIOForced)
            viewModel.activate();
        } else {
            viewModel.deactivate()
        }
    }

    function forceIoPageUp() {
        let page = currentPage + local.numOfPages - 1
        currentPage = page % local.numOfPages
    }

    function forceIoPageDown() {
        let page = currentPage + local.numOfPages + 1
        currentPage = page % local.numOfPages
    }

    // jog dial.
    MouseArea {
        anchors.fill: parent
        enabled: pageActive
        propagateComposedEvents: true
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        onClicked: mouse.accepted = false
        onWheel: {
            if(!REAL_TARGET) {
                if(wheel.angleDelta.y > 0) {
                    //console.log("mouse wheel up")
                    phyKeyManager.decrementEmulatedJogCount()
                } else if(wheel.angleDelta.y < 0) {
                    //console.log("mouse wheel down")
                    phyKeyManager.incrementEmulatedJogCount()
                }
            }
        }
    }
    Connections {
        target: phyKeyManager
        enabled: pageActive
        onJogScrollUpChanged: forceIoPageUp()
        onJogScrollDownChanged: forceIoPageDown()
    }

    //标题栏
    Item {
        x:100
        y:25
        Text{
            x:26
            font.pixelSize: 23
            color: resourceManager.rgb(33, 8)
            text: resourceManager.label(33, 8, resourceManager.language)
        }
        Text{
            x:252
            font.pixelSize: 22
            color: resourceManager.rgb(33, 9)
            text: resourceManager.label(33, 9, resourceManager.language)
        }
        Text{
            x:474
            font.pixelSize: 23
            color: resourceManager.rgb(33, 10)
            text: resourceManager.label(33, 10, resourceManager.language)
        }
        Text{
            x:580
//            y:-18
            font.pixelSize: 23
            color: resourceManager.rgb(33, 11)
            text: "       " + resourceManager.label(33, 11, resourceManager.language)
        }
    }

    Item {
        id: ioForcePage
        property int pageIndex: currentPage
        width: 30+830
        height: (60*6)+(12*5)
        y:60
        Column {
            x:30
            spacing: 12
            Repeater{
                model: {
                    let remain = local.forceIoDataTbl.length - (ioForcePage.pageIndex * local.numOfItemsParPage)
                    return Math.min(local.numOfItemsParPage, remain)
                }
                Rectangle{
                    readonly property int ioDataIndex: (ioForcePage.pageIndex * local.numOfItemsParPage) + index
                    readonly property var forceIoData: local.forceIoDataTbl[ioDataIndex]
                    width: 830
                    height: 60
                    radius: 10
                    border.width: 2
                    border.color: "grey"
                    color: "#DCDDDD"
                    visible: (forceIoData.forcedDio !== 0xFFFF)
                    //分隔线 : Separator.
                    Rectangle{
                        width: 2
                        height: parent.height-4
                        x:210
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                    }
                    Rectangle{
                        width: 2
                        height: parent.height-4
                        x:500
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                    }
                    Rectangle{
                        width: 2
                        height: parent.height-4
                        x:634
                        anchors.verticalCenter: parent.verticalCenter
                        color: "white"
                    }
                    // first icon.
                    Image {
                        x:16
                        y:12
                        source: forceIoData.value ? resourceManager.imagePath + "/08_Monitor/d080_11.png" : resourceManager.imagePath + "/08_Monitor/d080_12.png"
                    }
                    // io name.
                    Rectangle{
                        width: 130
                        height: parent.height-4
                        x:70
                        anchors.verticalCenter: parent.verticalCenter
                        color: "transparent"
                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: forceIoData.value ? penSet.ioNameTextColorRBG : "black"
                            text: resourceManager.symbolAt(forceIoData.forcedDio) + " " + resourceManager.getMessageStringValue("1", forceIoData.forcedDio, resourceManager.language)
                        }
                    }
                    //强制操作 : forced operation.(off, on, release)
                    Row{
                        x:230
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 20
                        ImageButton{
                            upImageSource: forceIoData.forcedOn ? resourceManager.imagePath + "/08_Monitor/d080_34.png" : resourceManager.imagePath + "/08_Monitor/d080_33.png"
                            downImageSource: resourceManager.imagePath + "/08_Monitor/d080_34.png"
                            text: resourceManager.label(33, 12, resourceManager.language)
                            textColor: resourceManager.rgb(33, 12)
                            fontPixelSize: 16
                            onClicked: viewModel.forciblyTurnOn(ioDataIndex)
                        }
                        ImageButton{
                            upImageSource: forceIoData.forcedOff ? resourceManager.imagePath + "/08_Monitor/d080_34.png" : resourceManager.imagePath + "/08_Monitor/d080_33.png"
                            downImageSource: resourceManager.imagePath + "/08_Monitor/d080_34.png"
                            text: resourceManager.label(33, 13, resourceManager.language)
                            textColor: resourceManager.rgb(33, 13)
                            fontPixelSize: 16
                            onClicked: viewModel.forciblyTurnOff(ioDataIndex)
                        }
                        ImageButton{
                            upImageSource: resourceManager.imagePath + "/08_Monitor/d080_33.png"
                            downImageSource: resourceManager.imagePath + "/08_Monitor/d080_34.png"
                            text: resourceManager.label(33, 52, resourceManager.language)
                            textColor: resourceManager.rgb(33, 52)
                            fontPixelSize: 16
                            onClicked: viewModel.releaseForcedIO(ioDataIndex)
                        }
                    }

                    // invers operation.
                    ImageButton{
                        x:530
                        anchors.verticalCenter: parent.verticalCenter
                        upImageSource: forceIoData.invers ? (resourceManager.imagePath + "/08_Monitor/d080_34.png") : (resourceManager.imagePath + "/08_Monitor/d080_33.png")
                        downImageSource: resourceManager.imagePath + "/08_Monitor/d080_34.png"
                        text: resourceManager.label(33, 10, resourceManager.language)
                        textColor: resourceManager.rgb(33, 10)
                        fontPixelSize: 16
                        onClicked: forceIoData.invers ? viewModel.inversTurnOff(ioDataIndex) : viewModel.inversTurnOn(ioDataIndex);
                    }

                    // swap operation.
                    Rectangle{
                        id: ioSwapButton
                        width: 100
                        height: 48
                        x:650
                        anchors.verticalCenter: parent.verticalCenter
                        border.color: "#898989"
                        border.width: 2
                        radius: 4
                        color: (forceIoData.swapDi !== 0xFFFF) ? local.selectedButtonColor :
                                                                 (viewModel.canSwapIO) ? "#e6f6fc" : "gray"
                        states: [
                            State {
                                name: "IsPressed"
                                PropertyChanges { target: ioSwapButton; color: local.selectedButtonColor }
                            }
                        ]
                        Text {
                            anchors.fill: parent
                            font.pixelSize: 17
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: "black"
                            text: (forceIoData.swapDi === 0xFFFF) ? "" : resourceManager.symbolAt(forceIoData.swapDi)
                            MouseArea {
                                anchors.fill: parent
                                pressAndHoldInterval: 5000
                                onClicked: {
                                    if (forceIoData.swapDi === 0xFFFF) {
                                        if (!viewModel.canSwapIO) {
                                            return;
                                        }
                                    }
                                    ioForceSwapDialog.forceIoData = forceIoData
                                    ioForceSwapDialog.open()
                                }
                                onPressed: {
                                    if (!viewModel.canSwapIO) {
                                        return;
                                    }
                                    ioSwapButton.state = "IsPressed"
                                }
                                onReleased: ioSwapButton.state = ""
                                onPressAndHold: {
                                    if (forceIoData.swapDi === 0xFFFF) {
                                        return;
                                    }
                                    viewModel.releaseSwappedIO(forceIoData)
                                    ioSwapButton.state = ""
                                }
                            }
                        }
                    }
                    // last icon.
                    Image {
                        x:776
                        y:14
                        source: forceIoData.value ? resourceManager.imagePath + "/08_Monitor/d080_11.png" : resourceManager.imagePath + "/08_Monitor/d080_12.png"
                    }
                }
            }
        }
    }

    //IO分类选择
    Item{
        x:710
        y:-46

        Image {
            property bool inCategory: (penSet.category1StartPage <= currentPage && currentPage < penSet.category2StartPage)
            id: category1Img
            x:0
            source: (category1Img.inCategory) ? resourceManager.imagePath + "/08_Monitor/d080_04.png" : resourceManager.imagePath + "/08_Monitor/d080_05.png"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                text: resourceManager.label(33, 5, resourceManager.language)
                color: (category1Img.inCategory) ? resourceManager.dataColor(8, 2) : resourceManager.rgb(33, 5)
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    currentPage = penSet.category1StartPage
                }
            }
        }

        Image {
            property bool inCategory: (penSet.category2StartPage <= currentPage && currentPage < penSet.category3StartPage)
            id: category2Img
            x:100
            source: (category2Img.inCategory) ? resourceManager.imagePath + "/08_Monitor/d080_04.png" : resourceManager.imagePath + "/08_Monitor/d080_05.png"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                text: resourceManager.label(33, 6, resourceManager.language)
                color: (category2Img.inCategory) ? resourceManager.dataColor(8, 2) : resourceManager.rgb(33, 6)
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    currentPage = penSet.category2StartPage
                }
            }
        }

        Image {
            property bool inCategory: (penSet.category3StartPage <= currentPage)
            id: category3Img
            x:200
            source: (category3Img.inCategory) ? resourceManager.imagePath + "/08_Monitor/d080_04.png" : resourceManager.imagePath + "/08_Monitor/d080_05.png"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                text: resourceManager.label(33, 7, resourceManager.language)
                color: (category3Img.inCategory) ? resourceManager.dataColor(8, 2) : resourceManager.rgb(33, 7)
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    currentPage = penSet.category3StartPage
                }
            }
        }
    }
    //IO显示页面翻页
    Item {
        anchors {
            top: root.top
            bottom: ioForcePage.bottom
            left: ioForcePage.right
            right: root.right
        }
        ImageScrollBar{
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 25
                bottom: parent.bottom
                bottomMargin: -10
            }
            topButtonPressImage: resourceManager.imagePath + "/05_ORG/d050_25" + ".png"
            topButtonReleaseImage: resourceManager.imagePath + "/05_ORG/d050_23" + ".png"
            botomButtonPressImage: resourceManager.imagePath + "/05_ORG/d050_24" + ".png"
            botomButtonReleaseImage: resourceManager.imagePath + "/05_ORG/d050_22" + ".png"
            pageLabal: currentPage+1  + "/" + local.numOfPages
            pageLabelColor:penSet.pageTextColorRGB
            onScrollUpClicked: root.forceIoPageUp()
            onScrollDownClicked: root.forceIoPageDown()
        }
    }

    IO_ForceSwapDialog {
        id: ioForceSwapDialog
        visible: false
    }
}
