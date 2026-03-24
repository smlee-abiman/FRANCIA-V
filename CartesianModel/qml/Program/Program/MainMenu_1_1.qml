import QtQuick 2.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_1_1.qml
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
Rectangle{
    id:menu1_1

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property int page: 0
    property int menuIndex: menuModel.menuIndex
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu1.subMenuList[0]
    property var timerViewModel: teachingViewModel.pageModel[3]
    property var programNoList: menuModel.programNoList
    property bool detect: menuModel.detect

    color: "transparent"
    border.width: 2
    border.color: "#999999"

    onPageActiveChanged: {
        if (pageActive) {
            menuModel.activate()
            m_stack.load_page(programNoList[menuIndex])
        } else {
            menuModel.menuIndex = 0
            menuModel.deactivate()
        }
    }

    onMenuIndexChanged: {
        m_stack.load_page(programNoList[menuIndex])
    }

    Component.onDestruction: menuModel.deactivate()

    Language { id: language }

    Rectangle{
        width: 120
        height: parent.height
        anchors.left: parent.left
        color: "#eaeaea"
        border.color: "#999999"
        Component{
            id:listdelegate_1
            Rectangle{
                readonly property int programNo: programNoList[index]
                readonly property int titleIndex: 36 + programNo
                width: 100
                height: 40
                x: 10
                radius: 5
                border.color: "#999999"
                color: menu1_1.menuIndex == index ? "#cc3333" : "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    wrapMode: Text.WordWrap
                    text: resourceManager.label(41,titleIndex, language.value)
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        menuModel.menuIndex = index
                        m_stack.load_page(programNo)
                    }
                }
            }
        }
        ListView{
            id:list1_1
            width: 120
            height: 378
            y:48
            property int pg: menu1_1.page
            spacing: 4
            clip: true
            model: programNoList.length
            delegate: listdelegate_1
            interactive: false
            onPgChanged: {
                positionViewAtIndex(pg * 8, ListView.Beginning)
            }
            Component.onCompleted: {
                positionViewAtIndex(pg * 8, ListView.Beginning)
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
                id:up
                anchors.centerIn: parent
                source: resourceManager.imagePath + "/16_program/d161_87.png"
            }
            MouseArea{
                anchors.fill: parent
                onPressed: up.source = resourceManager.imagePath + "/16_program/d161_89.png"
                onReleased: {
                    menu1_1.page = 0
                    up.source = resourceManager.imagePath + "/16_program/d161_87.png"
                }
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
                id:down
                anchors.centerIn: parent
                source: resourceManager.imagePath + "/16_program/d161_88.png"
            }
            MouseArea{
                anchors.fill: parent
                onPressed: down.source = resourceManager.imagePath + "/16_program/d161_90.png"
                onReleased: {
                    menu1_1.page = 1
                    down.source = resourceManager.imagePath + "/16_program/d161_88.png"
                }
            }
        }
    }//菜单栏
    StackView{
        id:m_stack
        width: 310
        height: 453
        x:118
        clip: true

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

        Component{
            id:prog102
            Rectangle{
                id:prog102_1
                property int programNo: 102
                property int itemIndex: menuModel.getItemIndex(programNo)
                property var item: menuModel.itemList[itemIndex]
                property var timeList: item.timeList
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#999999"
                Rectangle{
                    width: parent.width
                    height: 100
                    color: "transparent"
                    Row{
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 20
                        Column{
                            spacing:4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,294)
                                text: resourceManager.label(41,294, language.value)
                            }
                            LimitNumericEditBox{
                                readonly property int index: 0
                                width: 120
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                borderColor: "#898989"
                                fontPixelSize: 22
                                keyLayoutType: 4
                                max: m_stack.getMaxTime(index)
                                min: "0"
                                value: timerViewModel.formatSpeed(index, prog102_1.timeList[index])
                                onEnter: {
                                    menuModel.setTime(prog102_1.programNo, index, parseFloat(newValue))
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,295)
                                text: resourceManager.label(41,295, language.value)
                            }
                            LimitNumericEditBox{
                                readonly property int index: 1
                                width: 120
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                borderColor: "#898989"
                                fontPixelSize: 22
                                keyLayoutType: 4
                                max: m_stack.getMaxTime(index)
                                min: "0"
                                value: timerViewModel.formatSpeed(index, prog102_1.timeList[index])
                                onEnter: {
                                    menuModel.setTime(prog102_1.programNo, index, parseFloat(newValue))
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
            }
        }//Prog#102 MOPS

        Component{
            id:prog103
            Rectangle{
                id:prog103_1
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#999999"
            }
        }//Prog#103 成型许可

        Component{
            id:prog148
            Rectangle{
                id:prog148_1
                property int programNo: 148
                property int itemIndex: menuModel.getItemIndex(programNo)
                property var item: menuModel.itemList[itemIndex]
                property var timeList: item.timeList
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                property bool detect: menu1_1.detect
                Rectangle{
                    width: parent.width
                    height: 100
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 20
                        CheckBox{
                            width: 100
                            height: 40
                            x:20
                            anchors.verticalCenter: parent.verticalCenter
                            indicator:Image{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                scale: 1.3
                                source: prog148_1.detect ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                            }
                            contentItem: Text{
                                text: resourceManager.label(41,301, language.value)
                                color: resourceManager.rgb(41,301)
                                font.pixelSize: 18
                                leftPadding: 30
                                topPadding: -2
                            }
                            onClicked: {
                                menuModel.setDetect(!prog148_1.detect)
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,298)
                                text: resourceManager.label(41,298, language.value)
                            }
                            LimitNumericEditBox{
                                readonly property int index: 0
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 5
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: 4
                                max: m_stack.getMaxTime(index)
                                min: "0"
                                value: timerViewModel.formatSpeed(index, prog148_1.timeList[index])
                                onEnter: {
                                    menuModel.setTime(prog148_1.programNo, index, parseFloat(newValue))
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y: 99
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,299)
                                text: resourceManager.label(41,299, language.value)
                            }
                            LimitNumericEditBox{
                                readonly property int index: 1
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 5
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: 4
                                max: m_stack.getMaxTime(index)
                                min: "0"
                                value: timerViewModel.formatSpeed(index, prog148_1.timeList[index])
                                onEnter: {
                                    menuModel.setTime(prog148_1.programNo, index, parseFloat(newValue))
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,300)
                                text: resourceManager.label(41,300, language.value)
                            }
                            LimitNumericEditBox{
                                readonly property int index: 2
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 5
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: 4
                                max: m_stack.getMaxTime(index)
                                min: "0"
                                value: timerViewModel.formatSpeed(index, prog148_1.timeList[index])
                                onEnter: {
                                    menuModel.setTime(prog148_1.programNo, index, parseFloat(newValue))
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
            }
        }//Prog#148 ejector forward

        Component{
            id:prognull
            Rectangle{
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 40
                    text: "NULL"
                }
            }
        }//空画面

        function load_page(page){
            let component
            switch(page){
            case 102: component = prog102; break;
            case 103: component = prog103; break;
            case 148: component = prog148; break;
            default: component = prognull; break;
            }
            if (depth >= 1) {
                m_stack.replace(component,StackView.Immediate);
            } else {
                m_stack.push(component,StackView.Immediate);
            }
        }
    }
}



