import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../Widget"
import Enums 1.0
import "../../Program/PosSet/PosSetUtils.js" as Utils
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_4_1.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: SH.HONG
**************************************************************************************/
Rectangle{
    property bool pageActive: false
    color: "transparent"

    QtObject {
        id: local
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[3].subMenuList[0]
        property var pointDataList: menuModel.pointDataList
        property bool consecutive: menuModel.consecutive
        property int currentPointNo: menuModel.currentPointNo
        property int speed: menuModel.speed
        property int page: 0
        property var axisOrder: commonViewModel.axisOrder
        property int numAxis: axisOrder.length
        property int maxPageNo: Math.ceil(numAxis / numAxisPerPage)
        property int numAxisPerPage: 5
    }

    onPageActiveChanged: {
        if (pageActive) {
            local.menuModel.activate()
        } else {
            local.menuModel.deactivate()
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

    Component{
        id:listdelegate_3
        Rectangle{
            property int axisType: local.axisOrder[index]
            property var pointData: local.pointDataList[axisType]
            width: 280
            height: 60
            color: "#eaeaea"
            border.color: "#999999"
            x: 1
            CheckBox{
                width: 100
                height: 36
                x:9
                anchors.verticalCenter: parent.verticalCenter
                checked: pointData.select
                indicator:Image{
                    scale: 1.3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    source: pointData.select ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                }

                contentItem: Text{
                    readonly property int axisLabelIndex: Utils.axisLabelIndex(axisType)
                    text: resourceManager.label(22, axisLabelIndex, language.value)
                    color: resourceManager.rgb(22, axisLabelIndex)
                    font.pixelSize: 17
                    font.bold: true
                    leftPadding: 25
                }
                onClicked: {
                    pointData.select = checked
                    checked = Qt.binding(function() { return pointData.select })
                }
            }
            LimitNumericEditBox {
                width: 130
                height: 44
                x:120
                anchors.verticalCenter: parent.verticalCenter
                borderColor: "#999999"
                borderWidth: 2
                color: "white"
                enabled: pointData.select
                keyLayoutType: 6
                enablePressAndHoldClearKey: true
                value: local.menuModel.formatPosition(pointData.position)
                onReady: {
                    max = commonViewModel.formatPointMaxValue(axisType, local.currentPointNo)
                    min = commonViewModel.formatPointMinValue(axisType, local.currentPointNo)
                }
                onEnter: {
                    let position = parseFloat(newValue)
                    if (position !== Utils.INVALID_POSITION_VALUE) {
                        position = (position * 100).toFixed(2)
                    }
                    local.menuModel.setPosition(axisType, position)
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }
    }
    Rectangle{
        width: 280
        height: 32
        x:26
        y:10
        color: "#cc3333"
        border.color: "#cc3333"
        Text{
            x:135
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 19
            font.bold: true
            text: resourceManager.label(41,487, language.value)
            color: "white"
        }
    }//标题栏
    ListView{
        width: 281
        height:296
        x:25
        y:45
        property int pg: local.page
        spacing: -1
        clip: true
        model: local.axisOrder.length
        delegate: listdelegate_3
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
    }
    Rectangle{
        width: 280
        height: 40
        x:25
        y:350
        border.color: "#999999"
        color: "#eaeaea"
        CheckBox{
            width: 100
            height: 36
            x:12
            checked: local.consecutive
            scale:1.1
            indicator:Image{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
            }
            contentItem: Text{
                text: resourceManager.label(41,521, language.value)
                color: resourceManager.rgb(41,521)
                font.pixelSize: 17
                font.bold: true
                leftPadding: 22
                topPadding: 4
            }
            onClicked: {
                local.menuModel.consecutive = checked
                checked = Qt.binding(function() { return local.consecutive })
            }
        }
    }
    Rectangle{
        width: 280
        height: 40
        x:25
        y:395
        border.color: "#999999"
        color: "#eaeaea"
        Row{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 14
            spacing: 3
            Text{
                width: 80
                height: 36
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                anchors.verticalCenter: parent.verticalCenter
                wrapMode:Text.WordWrap
                font.pixelSize: 17
                font.bold: true
                text: resourceManager.label(41,522, language.value)
                color: resourceManager.rgb(41, 522)
            }
            LimitNumericEditBox {
                width: 85
                height: 32
                anchors.verticalCenter: parent.verticalCenter
                borderColor: "#999999"
                borderWidth: 2
                color: "white"
                keyLayoutType: 3
                value: local.menuModel.formatSpeed(local.speed)
                onReady: {
                    max = "100"
                    min = "1"
                }
                onEnter: {
                    local.menuModel.setSpeed(newValue)
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }
        Rectangle {
            width: 80
            height: 30
            color: positionMemoryButtonMouseArea.pressed ? "#cc3333" : "#cccccc"
            border.color: "#999999"
            radius: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            y: 5
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                font.pixelSize: positionMemoryButtonMouseArea.pressed ? 17 : 17
                color: positionMemoryButtonMouseArea.pressed ? "white" : resourceManager.rgb(41,491)
                text: resourceManager.label(41,491, language.value)
            }

            MouseArea {
                id: positionMemoryButtonMouseArea
                anchors.fill: parent
                onClicked: {
                    local.menuModel.setCurrentPosition()
                }
            }
        }
    }
}
