import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../Widget"
import Enums 1.0
import "../../Program/PosSet/PosSetUtils.js" as Utils
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_4_3.qml
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
    id:pass
    property bool pageActive: false
    color: "transparent"

    QtObject {
        id: local
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[3].subMenuList[2]
        property var itemList: menuModel.itemList
        property bool passOn: menuModel.passOn
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

    Rectangle{
        x:25
        y:10
        width: 120
        height: 50
        border.color: "#999999"
        color: local.passOn ? "#cc3333" : "#cccccc"
        Text{
            anchors.centerIn: parent
            font.pixelSize: 18
            font.bold: true
            text: resourceManager.label(41,525, language.value)
            color: local.passOn ? "white" : "#666666"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                local.menuModel.clearSelect()
                local.menuModel.passOn = true
            }
        }
    }
    Rectangle{
        width: 120
        height: 50
        x:149
        y:10
        border.color: "#999999"
        color: !local.passOn ? "#cc3333" : "#cccccc"
        Text{
            anchors.centerIn: parent
            font.pixelSize: 18
            font.bold: true
            text: resourceManager.label(41,526, language.value)
            color: !local.passOn ? "white" : "#666666"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                local.menuModel.clearSelect()
                local.menuModel.passOn = false
            }
        }
    }

    ImageScrollBar{
        height: 333
        anchors.right: parent.right
        y:54
        anchors.rightMargin: -56
        visible: local.maxPageNo >= 2
        opacity: !local.passOn ? 0.3 : 1
        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
        pageLabal: local.page+1 + "/" + local.maxPageNo
        onScrollUpClicked: local.page = Math.max(local.page - 1, 0)
        onScrollDownClicked: local.page = Math.min(local.page + 1, local.maxPageNo - 1)
    }
    Component{
        id:listdelegate_5
        Rectangle{
            property int axisType: local.axisOrder[index]
            property var axisData: local.itemList[axisType]
            x: 1
            width: 262
            height: 55
            color: "#eaeaea"
            border.color: "#999999"
            opacity: !local.passOn ? 0.3 : 1
            CheckBox{
                width: 100
                height: 36
                x:14
                anchors.verticalCenter: parent.verticalCenter
                checked: axisData.select
                indicator:Image{
                    scale: 1.3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    source: axisData.select ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
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
                    if (local.passOn) {
                        axisData.select = checked
                        checked = Qt.binding(function() { return axisData.select })
                    }
                }
            }

            LimitNumericEditBox {
                width: 120
                height: 40
                x:120
                anchors.verticalCenter: parent.verticalCenter
                borderColor: "#999999"
                color: "white"
                enabled: axisData.select
                keyLayoutType: 6
                value: local.menuModel.formatPosition(axisData.position)
                onReady: {
                    max = "100000"
                    min = "0"
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
        width: 262
        height: 30
        x:26
        y:65
        color: "#cc3333"
        border.color: "#cc3333"
        opacity: !local.passOn ? 0.5 : 1
        Text{
            x:130
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 19
            font.bold: true
            text: resourceManager.label(41,487, language.value)
            color: "white"
        }
    }//标题栏
    ListView{
        width: 263
        height:296
        x:25
        y:95
        property int pg: local.page
        spacing: 2
        clip: true
        model: local.axisOrder.length
        delegate: listdelegate_5
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
    }
}
