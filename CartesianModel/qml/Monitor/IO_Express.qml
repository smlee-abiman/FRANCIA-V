import QtQuick 2.12
import QtQuick.Layouts 1.3
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: IO_Express.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.01
* Worker: IJ.YI
**************************************************************************************/
Item {
    id: io_express_page
    width: 1012
    height: 520
    property int page: monitorViewModel.ioView.pageNum
    property int pageMax: monitorViewModel.ioView.pageMaxNum
    property bool pageActive: false
    property int type1StartPage: parseInt(resourceManager.getPenSetStringValue("8",6,0)) / 3
    property int type2StartPage: parseInt(resourceManager.getPenSetStringValue("8",7,0)) / 3
    property int type3StartPage: parseInt(resourceManager.getPenSetStringValue("8",8,0)) / 3

    Rectangle{
        width: parent.width
        height: parent.height
        color: "transparent"
        Rectangle{
            width: parent.width
            height: 588
            z: -3
            anchors.top: parent.top
            anchors.topMargin: 60
            border.width: 2
            color: "#FFFFFF"
            border.color: "#CCCCCC"
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
    onPageActiveChanged: {
        if (pageActive) {
            monitorViewModel.ioView.activate()
        } else {
            monitorViewModel.ioView.deactivate()
        }
    }
    // Top select IO
    Rectangle{
        width: 350
        height: 60
        anchors.right: parent.right
        color: "transparent"
        Image {
            z:-2
            anchors.top: parent.top
            source: resourceManager.imagePath + "/08_Monitor/a080_07.png"
        }

        // IO section IMM,robot,Internal
        Row {
            spacing: 10
            leftPadding: 15
            // Robot io
            Rectangle {
                width: 100
                height: 40
                anchors.top: parent.top
                anchors.topMargin: 11
                color: page < type2StartPage || but_mouse1.pressed ? "#CC3333" : "#CCCCCC"
                border.width: 1
                border.color: "#999999"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    color: page < type2StartPage || but_mouse1.pressed ? "#FFFFFF" : "#666666"
                    text: resourceManager.label(33, 5, local.language)
                }
                MouseArea{
                    id:but_mouse1
                    anchors.fill: parent
                    onClicked: {
                        page = type1StartPage
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
            // Robot EOAT io
            Rectangle {
                width: 100
                height: 40
                anchors.top: parent.top
                anchors.topMargin: 11
                color: (page >= type2StartPage && page < type3StartPage) || but_mouse2.pressed ? "#CC3333" : "#CCCCCC"
                border.width: 1
                border.color: "#999999"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    color: (page >= type2StartPage && page < type3StartPage) || but_mouse2.pressed ? "#FFFFFF" : "#666666"
                    text: resourceManager.label(33, 6, local.language)
                }
                MouseArea {
                    id:but_mouse2
                    anchors.fill: parent
                    onClicked:{
                        page = type2StartPage
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
            // Imm interface
            Rectangle {
                width: 100
                height: 40
                anchors.top: parent.top
                anchors.topMargin: 11
                color: (page >= type3StartPage && page <= 5 ) || but_mouse3.pressed ? "#CC3333" : "#CCCCCC"
                border.width: 1
                border.color: "#999999"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    color: (page >= type3StartPage && page <= 5 ) || but_mouse3.pressed ? "#FFFFFF" : "#666666"
                    text: resourceManager.label(33, 7, local.language)
                }
                MouseArea {
                    id:but_mouse3
                    anchors.fill: parent
                    onClicked:{
                        page = type3StartPage
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }
    }
    // list area
    Grid {
        x:20
        y:85
        rows: 6
        columns: 3
        rowSpacing: 35
        columnSpacing: 28
        flow: Grid.TopToBottom
        Repeater{
            model: monitorViewModel.ioView.getDataList(page)
            Rectangle {
                visible: (modelData.index !== -1) ? true : false
                width: 265
                height: 60
                radius: 6
                border.color: "#898989"
                color: "#dcdddc"
                Image {
                    x:10
                    anchors.verticalCenter: parent.verticalCenter
                    source: (modelData.value == 1) ? resourceManager.imagePath + "/08_Monitor/d080_11.png" : resourceManager.imagePath + "/08_Monitor/d080_12.png"
                }
                Text {
                    width: 215
                    height: 60
                    x:50
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    wrapMode:Text.WrapAnywhere
                    font.pixelSize: 23
                    font.bold: true
                    text: (modelData.index != -1) ? resourceManager.symbolAt(modelData.index) + " " + resourceManager.getMessageStringValue("1", modelData.index, local.language) : ""
                    color: (modelData.value == 1) ? resourceManager.dataColor(8, 4) : "black"
                }
            }
        }
    }
    // scrollbar background
    Rectangle {
        width: 66
        height: 500
        radius: 30
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.top: parent.top
        anchors.topMargin: 110
        color: "#eaeaea"
    }

//    // 구분선
//    Rectangle {
//        x : 236
//        y : 351
//        width: 430
//        height: 2
//        radius: 1
//        color: "#cc3333"
//    }

    ImageScrollBar{
        height: 480
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 120
        topButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_03.png"
        topButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_04.png"
        botomButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_05.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_06.png"
        pageLabal: page+1  + "/" + pageMax
        pageLabelColor:resourceManager.dataColor(8, 3)
        onScrollUpClicked: {
            if(0 < page){
                page --
            }else{
                page = (pageMax-1)
            }
        }
        onScrollDownClicked: {
            if(page < (pageMax-1)){
                page ++
            }else{
                page = 0
            }
        }
    }
}
