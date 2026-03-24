import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import TeachingModePageModel 1.0
import TeachingModeData 1.0
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_3.qml
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
    id:menu2_3
    color: "transparent"
    border.width: 2
    border.color: "#999999"

    property bool pageActive: false

    QtObject {
        id: local
        property var stackPageModel: teachingViewModel.pageModel[2]
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[2]
        property bool isfreestack:menuModel.isFreeStack
        property var stacktype: stackPageModel.stackType
        property int curStandardStackNum: stackPageModel.curStandardPalletizeNum //current standard stack number
        property int curFreeStackNum: stackPageModel.curFreePalletizeNum //current free stack number
        property int stackIndex: menuModel.menuIndex
        readonly property int numItemPerPage: 8
        property int page: 0
        property var axisOrder: stackPageModel.axisOrder
        onStackIndexChanged: {
            page = stackIndex / numItemPerPage
        }
        onStacktypeChanged: {
            stackPageModel.modbusGetStackType(curStandardStackNum)
            menuModel.standardStackTypeList[curStandardStackNum] = local.stacktype[curStandardStackNum]
        }
    }

    onPageActiveChanged: {
        if (pageActive) {
            local.menuModel.activate()
            local.stackPageModel.modbusGetStackType(local.curStandardStackNum)
            local.stackPageModel.editFromProgEditScreenIng = true
            local.menuModel.standardStackTypeList[local.curStandardStackNum] = local.stacktype[local.curStandardStackNum]
        } else {
            local.menuModel.deactivate()
            local.menuModel.isFreeStack = false
            local.stackPageModel.curStandardPalletizeNum = 0
            if(!local.stackPageModel.editFromProgEditScreen){
                local.menuModel.menuIndex = 0
            }
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

    Rectangle{
        width: 120
        height: parent.height
        color: "#eaeaea"
        border.color: "#999999"
        Component{
            id:listdelegate_11
            Rectangle{
                width: 100
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "#999999"
                //color: ((local.curStandardStackNum === index) && local.menuModel.menuIndex < 8) || ((local.curFreeStackNum === (index - 8)&& local.menuModel.menuIndex >= 8)) ? "#FAEE00" : "#36A3DE"
                color: local.curStandardStackNum === index ? "#cc0000" : "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    font.bold: true
                    wrapMode: Text.WordWrap
                    color: local.curStandardStackNum === index ? "white" : "#666666"
                    text: resourceManager.label(41,496+index, language.value)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        local.menuModel.menuIndex = index
                        local.menuModel.isFreeStack = false  // 항상 고정 스택으로 간주
                        if(index >7){
                            local.menuModel.isFreeStack = true
                        }else{
                            local.menuModel.isFreeStack = false
                        }
                        if(index >= 0 && index < 8){
                            local.stackPageModel.curStandardPalletizeNum = index
                            local.menuModel.standardStackTypeList[local.curStandardStackNum] = local.stacktype[local.curStandardStackNum]
                        }else{
                            local.stackPageModel.curFreePalletizeNum = index - 8
                        }
                    }
                }
            }
        }
        ListView{
            width: 120
            height: 378
            y:10
            property int pg: local.page
            spacing: 4
            clip: true
            model: 4
            delegate: listdelegate_11
            interactive: false
            onPgChanged: {
                positionViewAtIndex(pg * local.numItemPerPage, ListView.Beginning)
            }
            Component.onCompleted: {
                positionViewAtIndex(pg * local.numItemPerPage, ListView.Beginning)
            }
        }
        // Rectangle{
        //     width: 100
        //     height: 40
        //     border.color: "#999999"
        //     color: "transparent"
        //     x: 10
        //     y: 5
        //     Image{
        //         id:up
        //         anchors.centerIn: parent
        //         source: upMouseArea.pressed ? resourceManager.imagePath + "/16_program/d161_89.png" : resourceManager.imagePath + "/16_program/d161_87.png"
        //     }
        //     MouseArea{
        //         id: upMouseArea
        //         anchors.fill: parent
        //         onClicked: local.page = 0
        //     }
        // }
        // Rectangle{
        //     anchors.bottom: parent.bottom
        //     anchors.bottomMargin: 5
        //     width: 100
        //     height: 40
        //     x: 10
        //     border.color: "#999999"
        //     color: "transparent"
        //     Image{
        //         id:down
        //         anchors.centerIn: parent
        //         source: downMouseArea.pressed ? resourceManager.imagePath + "/16_program/d161_90.png" : resourceManager.imagePath + "/16_program/d161_88.png"
        //     }
        //     MouseArea{
        //         id: downMouseArea
        //         anchors.fill: parent
        //         onClicked: local.page = 1
        //     }
        // }
    }//菜单栏

    Item{
        id: info
        property int index: local.stackIndex
        width: 310
        height: 426
        x: 142
        y: 20

        Column{
            spacing: 10
            Rectangle{
                width: 258
                height: 40
                border.color: "#999999"
                color: "#eaeaea"
                radius: 5
                visible: !local.isfreestack
                CheckBox{
                    width: 200
                    height: 36
                    x:10
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: local.stacktype[local.curStandardStackNum] === 0 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }
                    contentItem: Text{
                        leftPadding: 30
                        font.pixelSize: 17
                        font.bold: true
                        color: resourceManager.rgb(41, 552)
                        text: resourceManager.label(41,552, language.value)
                    }
                    onClicked: local.stackPageModel.modbusSetStackType(local.curStandardStackNum, 0)
                }
            }//main axis
            Rectangle{
                width: 258
                height: 40
                border.color: "#999999"
                color: "#eaeaea"
                radius: 5
                visible: !local.isfreestack
                CheckBox{
                    width: 200
                    height: 36
                    x:10
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: local.stacktype[local.curStandardStackNum] === 1 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }
                    contentItem: Text{
                        leftPadding: 30
                        font.pixelSize: 17
                        font.bold: true
                        color: resourceManager.rgb(41, 553)
                        text: resourceManager.label(41,553, language.value)
                    }
                    onClicked: local.stackPageModel.modbusSetStackType(local.curStandardStackNum, 1)
                }
            }//sub axis
            Rectangle{
                width: 258
                height: 40
                border.color: "#999999"
                color: "#eaeaea"
                radius: 5
                visible: !local.isfreestack
                CheckBox{
                    width: 200
                    height: 36
                    x:10
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: local.stacktype[local.curStandardStackNum] === 2 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }
                    contentItem: Text{
                        leftPadding: 30
                        font.pixelSize: 17
                        font.bold: true
                        color: resourceManager.rgb(41, 554)
                        text: resourceManager.label(41,554, language.value)
                    }
                    onClicked: local.stackPageModel.modbusSetStackType(local.curStandardStackNum, 2)
                }
            }//main sub axis
        }
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                width: 258
                height: 44
                border.color: "#999999"
                color: "#eaeaea"
                radius: 5

                Rectangle {
                    width: 80
                    height: 30
                    color: "#cccccc"
                    border.color: "#999999"
                    radius: 5
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    Text {
                        anchors.centerIn: parent
                        text: resourceManager.label(41,639, language.value)
                        font.pixelSize: 17
                        font.bold: true
                        color: resourceManager.rgb(41,639)
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (!local.isfreestack) {
                                local.stackPageModel.curTabIndex = 0
                            } else {
                                local.stackPageModel.curTabIndex = 1
                            }
                            local.stackPageModel.editFromProgEditScreen = true
                            viewModel.currentPageTab = 2
                        }
                    }
                }
            }//stack setting button
    }
}
