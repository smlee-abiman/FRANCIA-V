import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import TeachingModePageModel 1.0
import TeachingModeData 1.0
import Enums 1.0
import "../../Program/PosSet/PosSetUtils.js" as Utils
import "../../Widget"
import "../../Components"
import "../../Widget/ComboBox"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_2.qml
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
// program servo position point page
Rectangle{
    id:menu2_2
    border.width: 2
    color: "transparent"
    border.color: "#999999"
    property bool pageActive: false

    QtObject {
        id: local
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[1]
        property var axisOrder: commonViewModel.axisOrder
        property var pointList: menuModel.pointList
        property var pointDataList: menuModel.pointDataList
        property bool relative: menuModel.relative
        property bool noWaiting: menuModel.noWaiting
        property int currentPointLabelIndex: menuModel.currentPointLabelIndex
        property int page: 0
        property int numAxis: axisOrder.length
        property int maxPageNo: Math.ceil(numAxis / numAxisPerPage)
        property int numAxisPerPage: 5

        onCurrentPointLabelIndexChanged: {
            pos_select.currentIndex = local.currentPointLabelIndex
        }
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "16"
        readonly property string positionColor: resourceManager.dataColor(groupNo, 170)
        readonly property string speedColor: resourceManager.dataColor(groupNo, 173)
        readonly property string selectedItemColor: resourceManager.dataColor(groupNo, 174)
    }

    onPageActiveChanged: {
        if (pageActive) {
            pos_select.currentIndex = local.currentPointLabelIndex
            local.menuModel.activate()
        } else {
            local.menuModel.deactivate()
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

    Row{
        x:11
        y:11
        spacing: 40
        Text{
            width: 80
            height: 42
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
            font.bold: true
            text: resourceManager.label(41,492, language.value)
            color: resourceManager.rgb(41, 492)
        }
        StringComboBox_style4{
            id: pos_select
            width: 237
            height: 42
            resourceGroupId: 36
            fontSize: 18
            comboBoxMode: local.pointList
            Component.onCompleted: {
                currentIndex = local.currentPointLabelIndex
            }
            onCurrentIndexChanged: {
                if (pos_select.currentIndex >= 0) {
                    local.menuModel.setCurrentPointLabelIndex(currentIndex)
                }
            }
        }//位置选择
    }//固定点选择
    // right scrollbar
    ImageScrollBar{
        height: 288
        anchors.right: parent.right
        anchors.rightMargin: 12
        y : 92
        visible: local.maxPageNo >= 2
        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
        pageLabal: local.page+1 + "/" + local.maxPageNo
        onScrollUpClicked: local.page = Math.max(local.page - 1, 0)
        onScrollDownClicked: local.page = Math.min(local.page + 1, local.maxPageNo - 1)
    }
    Component{
        id:listdelegate_2
        // axis select check box
        Rectangle{
            property int axisType: local.axisOrder[index]
            property var pointData: local.pointDataList[axisType]
            x: 1
            width: 357
            height: 56
            color: pointData.select ? "#CCCCCC" : "#CCCCCC"
            border.color: "#999999"
            CheckBox{
                width: 100
                height: 36
                x:8
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
                    font.pixelSize: 16
                    font.bold: true
                    leftPadding: 25
                }
                onClicked: {
                    pointData.select = checked
                    checked = Qt.binding(function() { return pointData.select })
                }
            }

            LimitNumericEditBox {
                width: 115
                height: 44
                x:100
                anchors.verticalCenter: parent.verticalCenter
                borderColor: "#999999"
                color: "white"
                enabled: pointData.select
                keyLayoutType: 6
                enablePressAndHoldClearKey: true
                textColor: penSet.positionColor
                value: local.menuModel.formatPosition(axisType, pointData.position)
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
            LimitNumericEditBox {
                width: 115
                height: 44
                x:220
                anchors.verticalCenter: parent.verticalCenter
                borderColor: "#999999"
                color: "white"
                enabled: pointData.select
                keyLayoutType: 3
                textColor: penSet.speedColor
                value: local.menuModel.formatSpeed(axisType, pointData.speed)
                onReady: {
                    max = "65535"
                    min = "0"
                }
                onEnter: {
                    local.menuModel.setSpeed(axisType, newValue)
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }
    }
    Rectangle{
        width: 357
        height: 30
        x:11
        y:60
        color: "#cc3333"
        border.color: "#cc3333"
        Text{
            x:105
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 19
            text: resourceManager.label(41,487, language.value)
            color: "white"
        }
        Text{
            x:247
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 19
            text: resourceManager.label(41,488, language.value)
            color: "white"
        }
    }//标题栏
    ListView{
        width: 358
        height:302
        x:10
        y:92
        property int pg: local.page
        spacing: 2
        clip: true
        model: local.numAxis
        delegate: listdelegate_2
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * 5, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * 5, ListView.Beginning)
        }
    }
    Rectangle{
        width: 358
        height: 60
        x:10
        y:386
        color: "#eaeaea"
        border.color: "#999999"
        Row{
            x:10
            anchors.verticalCenter: parent.verticalCenter
            spacing: 140
            visible: commonViewModel.manipulateMode !== 2
            CheckBox{
                id: noWaitingCheckBox
                width: 81
                height: 60
                anchors.verticalCenter: parent.verticalCenter
                checked: local.noWaiting
                scale: 1.1
                indicator:Image{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                }
                contentItem: Text{
                    text: resourceManager.label(41,490, language.value)
                    color: resourceManager.rgb(41, 490)
                    font.pixelSize: 18
                    font.bold: true
                    leftPadding: 22
                    topPadding: 14
                }
                onClicked: {
                    local.menuModel.noWaiting = checked
                    checked = Qt.binding(function() { return local.noWaiting })
                }
            }
            Rectangle {
                width: 120
                height: 45
                color: "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: positionMemoryButtonMouseArea.pressed ? 17 : 18
                    font.bold: true
                    color: "black"
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
        Row{
            x:-1
            y:1
            spacing: -1
            visible: commonViewModel.manipulateMode === 2
            Repeater{
                model: 8
                Rectangle{
                    width: 47
                    height: 60
                    color: "white"
                    border.color: "#999999"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "black"
                        text: {
                            switch(index){
                            case 0:
                                "-0.01";
                                break;
                            case 1:
                                "-0.1";
                                break;
                            case 2:
                                "-1";
                                break;
                            case 3:
                                "-10";
                                break;
                            case 4:
                                "+0.01";
                                break;
                            case 5:
                                "+0.1";
                                break;
                            case 6:
                                "+1";
                                break;
                            case 7:
                                "+10";
                                break;
                            }
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onPressed: parent.color = "#cc3333"
                        onReleased: parent.color = "white"
                    }
                }
            }
        }
    }
}
