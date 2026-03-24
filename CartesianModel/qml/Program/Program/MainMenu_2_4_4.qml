import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../Widget"
import Enums 1.0
import "../../Program/PosSet/PosSetUtils.js" as Utils
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_4_4.qml
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
        property var menuModel: programViewModel.progMenu2.subMenuList[3].subMenuList[3]
        property var itemList: menuModel.itemList
        property bool noWaiting: menuModel.noWaiting
        property bool plusDirection: menuModel.plusDirection
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
        id:listdelegate_6
        Rectangle{
            property int axisType: local.axisOrder[index]
            property var pointData: local.itemList[axisType]
            x:1
            width: 262
            height: 60
            color: "#eaeaea"
            border.color: "#999999"
            CheckBox{
                width: 100
                height: 36
                x:14
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
                    leftPadding: 30
                }
                onClicked: {
                    pointData.select = checked
                    checked = Qt.binding(function() { return pointData.select })
                }
            }
            LimitNumericEditBox {
                width: 120
                height: 40
                x:120
                anchors.verticalCenter: parent.verticalCenter
                borderColor: "#999999"
                color: "white"
                enabled: pointData.select
                keyLayoutType: 3
                value: local.menuModel.formatSpeed(pointData.speed)
                max: "100"
                min: "1"
                onEnter: {
                    local.menuModel.setSpeed(axisType, newValue)
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }
    }
    Rectangle{
        width: 262
        height: 32
        x:26
        y:10
        color: "#cc3333"
        border.color: "#cc3333"
        Text{
            x:155
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 19
            font.bold: true
            text: resourceManager.label(41,488, language.value)
            color: "white"
        }
    }//标题栏
    ListView{
        width: 263
        height:310
        x:25
        y:46
        property int pg: local.page
        spacing: 2
        clip: true
        model: local.axisOrder.length
        delegate: listdelegate_6
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
    }
    Rectangle{
        width: 270
        height: 40
        x:25
        y:360
        border.color: "#999999"
        color: "#eaeaea"
        CheckBox{
            width: 100
            height: 36
            x:14
            checked: local.noWaiting
            scale: 1.1
            indicator:Image{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
            }
            contentItem: Text{
                text: resourceManager.label(41,527, language.value)
                color: resourceManager.rgb(41,527)
                font.pixelSize: 17
                font.bold: true
                leftPadding: 22
                topPadding: 4
            }
            onClicked: {
                local.menuModel.noWaiting = checked
                checked = Qt.binding(function() { return local.noWaiting })
            }
        }
    }
    Rectangle{
        width: 270
        height: 40
        x:25
        y:405
        border.color: "#999999"
        color: "#eaeaea"
        Row{
            x:10
            spacing: 20
            anchors.verticalCenter: parent.verticalCenter
            Rectangle{
                width:100
                height: 35
                color: !local.plusDirection ? "#cccccc" : "#cc3333"
                border.color: "#999999"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 17
                    text: resourceManager.label(41,528, language.value)
                    color: !local.plusDirection ? "#666666" : "white"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: local.menuModel.plusDirection = true
                }
            }
            Rectangle{
                width:100
                height: 35
                color: local.plusDirection ? "#cccccc" : "#cc3333"
                border.color: "#999999"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 17
                    text: resourceManager.label(41,529, language.value)
                    color: local.plusDirection ? "#666666" : "white"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: local.menuModel.plusDirection = false
                }
            }
        }
    }
}//轴复归
