import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Enums 1.0
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_1_3.qml
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
Rectangle {
    id: menu1_3
    border.width: 2
    border.color: "#999999"
    color: "transparent"

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu1.subMenuList[2]
    property var timerViewModel: teachingViewModel.pageModel[3]
    property int open: menuModel.open
    property bool singleChoice: menuModel.singleChoice
    readonly property int numItems: menuModel.itemList.length
    property int page: 0
    property int stackIndex: 0

    function getMaxTime(timerIndex){
        let ret
        let digits = timerViewModel.getIntegerDigits(timerIndex)
        switch (digits) {
        case 2:
            ret = "99.99"
            break
        case 3:
            ret = "655.35"
            break
        default:
            console.warn("getMaxTime: Invalid value.", index, digits)
            ret = "655.35"
            break
        }
        return ret
    }

    function sendKey(itemIndex, keyIndex){
        let keyNo = menuModel.getKeyNo(itemIndex, keyIndex)
        if (keyNo > 0) {
            commonViewModel.modbusMemblemSwControl(true, keyNo)
            commonViewModel.modbusMemblemSwControl(false, keyNo)
            menuModel.modbusGetSignal()
        }
    }

    onPageActiveChanged: {
        if (pageActive) {
            menuModel.activate()
            timer.running = true
        } else {
            menuModel.deactivate()
            timer.running = false
        }
    }

    Component.onDestruction: menuModel.deactivate()

    Timer{
        id:timer
        running: false
        interval: 100
        repeat: true
        onTriggered: {
            menuModel.modbusGetSignal()
        }
    }

    Language { id: language }

    Rectangle{
        width: 120
        height: parent.height
        color: "#eaeaea"
        border.color: "#999999"
        property int maxpage: (numItems - 1) / 8
        Component{
            id:listdelegate_3
            Rectangle{
                width: 100
                height: 35
                x: 10
                y: 5
                radius: 5
                border.color: "#999999"
                color: menu1_3.stackIndex == index ? "#cc3333" : "#cccccc"
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: 4
                    scale: 0.8
                    source: modelData.state ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                }
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 35
                    font.pixelSize: 17
                    wrapMode: Text.WordWrap
                    text: resourceManager.label(41,modelData.titleIndex, language.value)
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        menu1_3.stackIndex = index
                    }
                }
            }
        }
        ListView{
            id:list1_3
            width: 120
            height: 378
            y: 48
            property int pg: menu1_3.page
            spacing: 4
            clip: true
            model: menuModel.itemList
            delegate: listdelegate_3
            interactive: false
            onPgChanged: {
                let topIndex = pg * 8
                menu1_3.stackIndex = topIndex
                positionViewAtIndex(topIndex, ListView.Beginning)
            }
            Component.onCompleted: {
                let topIndex = pg * 8
                menu1_3.stackIndex = topIndex
                positionViewAtIndex(topIndex, ListView.Beginning)
            }
        }
        Rectangle{
            width: 100
            height: 40
            border.color: "#999999"
            color: "transparent"
            x: 10
            y: 5
            Image{
                id:up2
                anchors.centerIn: parent
                source: resourceManager.imagePath + "/16_program/d161_87.png"
            }
            MouseArea{
                anchors.fill: parent
                onPressed: up2.source = resourceManager.imagePath + "/16_program/d161_89.png"
                onReleased: up2.source = resourceManager.imagePath + "/16_program/d161_87.png"
                onClicked: menu1_3.page = Math.max(menu1_3.page - 1, 0)
            }
        }
        Rectangle{
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            width: 100
            height: 40
            x: 10
            border.color: "#B5B5B6"
            color: "transparent"
            Image{
                id:down2
                anchors.centerIn: parent
                source: resourceManager.imagePath + "/16_program/d161_88.png"
            }
            MouseArea{
                anchors.fill: parent
                onPressed: down2.source = resourceManager.imagePath + "/16_program/d161_90.png"
                onReleased: down2.source = resourceManager.imagePath + "/16_program/d161_88.png"
                onClicked: menu1_3.page = Math.min(menu1_3.page + 1, parent.parent.maxpage);
            }
        }
    }//菜单栏
    StackLayout{
        width:268
        height: 453
        x:118
        currentIndex: menu1_3.stackIndex

        Repeater{
            model: menuModel.itemList
            Rectangle{
                width: parent.width
                height: parent.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 56
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.width: 2
                    border.color: "#B5B5B6"
                    color: "#C1C2F4"
                    Row{
                        x:5
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 50
                        CheckBox{
                            width: 80
                            height: 36
                            indicator:Image{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                scale: 0.9
                                source: (open === 1) ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                            }
                            contentItem: Text{
                                text: resourceManager.label(41,609, language.value)
                                color: resourceManager.rgb(41, 609)
                                font.pixelSize: 18
                                leftPadding: 36
                                topPadding: -2
                            }
                            onClicked: menuModel.open = 1;
                        }
                        CheckBox{
                            width: 80
                            height: 36
                            indicator:Image{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                scale: 0.9
                                source: (open === 0) ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                            }
                            contentItem: Text{
                                text: resourceManager.label(41,610, language.value)
                                color: resourceManager.rgb(41, 610)
                                font.pixelSize: 18
                                leftPadding: 36
                                topPadding: -2
                            }
                            onClicked: menuModel.open = 0;
                            //                                        onClicked: programViewModel.Jig_Out_State = 0
                        }
                    }
                }//输出状态
                Row{
                    x:10
                    y:70
                    spacing: 50
                    CheckBox{
                        width: 80
                        height: 36
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.1
                            source:{
                                if(menuModel.isSArmClampItem(index)){
                                    (modelData.state && singleChoice) ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                                }else{
                                    (modelData.state && !singleChoice) ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                                }
                            }
                        }
                        contentItem: Text{
                            text: resourceManager.label(41,modelData.titleIndex, language.value)
                            color: resourceManager.rgb(41, modelData.titleIndex)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked:{
                            modelData.state = modelData.state ? false : true
                            if(menuModel.isSArmClampItem(index)){
                                menuModel.setJigSingleChoice();
                            }else{
                                menuModel.setJigMultipleChoice();
                            }
                        }
                    }
                    CheckBox{
                        width: 80
                        height: 36
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.1
                            source: modelData.detect ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            text: resourceManager.label(41,611, language.value)
                            color: resourceManager.rgb(41, 611)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: modelData.detect = !modelData.detect
                    }
                }
                Row{
                    x:10
                    y:120
                    spacing: 50
                    Column{
                        width: 100
                        spacing: 2
                        Text{
                            width: 110
                            height: 40
                            anchors.horizontalCenter: parent.horizontalCenter
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap

                            font.pixelSize: 18
                            color: resourceManager.rgb(41,360+ index*2)
                            text: resourceManager.label(41,360+ index*2, language.value)
                        }
                        LimitNumericEditBox {
                            readonly property int timeIndex: {
                                if (menuModel.open === 0) {
                                    2
                                } else {
                                    0
                                }
                            }

                            property int time: modelData.timeList[timeIndex]
                            width: 100
                            height: 45
                            anchors.horizontalCenter: parent.horizontalCenter
                            rectRadius: 6
                            borderColor: "#898989"
                            borderWidth: 2
                            fontPixelSize: 22
                            keyLayoutType: 4
                            max: menu1_3.getMaxTime(timeIndex)
                            min: "0"
                            value: timerViewModel.formatSpeed(timeIndex, time)
                            onEnter: {
                                menuModel.setTime(index, timeIndex, parseFloat(newValue))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                    }
                    Column{
                        width: 100
                        spacing: 2
                        Text{
                            width: 110
                            height: 40
                            anchors.horizontalCenter: parent.horizontalCenter
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap

                            font.pixelSize: 18
                            color: resourceManager.rgb(41,361+ index*2)
                            text: resourceManager.label(41,361+ index*2, language.value)
                        }
                        LimitNumericEditBox {
                            readonly property int timeIndex: {
                                if (menuModel.open === 0) {
                                    3
                                } else {
                                    1
                                }
                            }
                            property int time: modelData.timeList[timeIndex]
                            width: 100
                            height: 45
                            anchors.horizontalCenter: parent.horizontalCenter
                            rectRadius: 6
                            borderColor: "#898989"
                            borderWidth: 2
                            fontPixelSize: 22
                            keyLayoutType: 4
                            max: menu1_3.getMaxTime(timeIndex)
                            min: "0"
                            value: timerViewModel.formatSpeed(timeIndex, time)
                            onEnter: {
                                menuModel.setTime(index, timeIndex, parseFloat(newValue))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                    }
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:365
                    spacing: 5
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 10
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        ImageButton{
                            readonly property int keyIndex: 0
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,609)
                                text: resourceManager.label(41,609, language.value)
                            }
                            onClicked: sendKey(index, keyIndex)
                        }
                        Image {
                            readonly property bool status: modelData.signal
                            anchors.verticalCenter: parent.verticalCenter
                            source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                        }
                        ImageButton{
                            readonly property int keyIndex: 1
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,610)
                                text: resourceManager.label(41,610, language.value)
                            }
                            onClicked: sendKey(index, keyIndex)
                        }
                    }
                }//操作
            }
        }
    }
}

