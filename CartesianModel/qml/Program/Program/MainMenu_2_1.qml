import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../../Widget"
import TeachingModePageModel 1.0
import TeachingModeData 1.0
import Enums 1.0
import "../../Program/PosSet/PosSetUtils.js" as Utils
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_1.qml
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
    id:menu2_1
    color: "transparent"
    border.width: 2
    border.color: "#999999"

    property bool pageActive: false
    property int usrlevel: commonViewModel.userLevel//用户等级

    QtObject {
        id: local
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[0]
        property var pointDataList: menuModel.pointDataList
        property bool relative: menuModel.relative
        property bool noWaiting: menuModel.noWaiting
        property int currentPointNo: menuModel.currentPointNo
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

    ImageScrollBar{
        width: 60
        height: 352
        anchors.right: parent.right
        anchors.rightMargin: 5
        y:5
        // visible: local.maxPageNo >= 2    //축갯수가 5축이 넘어가면 보임
        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
        pageLabal: local.page+1 + "/" + local.maxPageNo
        onScrollUpClicked: local.page = Math.max(local.page - 1, 0)
        onScrollDownClicked: local.page = Math.min(local.page + 1, local.maxPageNo - 1)
    }
    Component{
        id:listdelegate_1
        Rectangle{
            property int axisType: local.axisOrder[index]
            property var pointData: local.pointDataList[axisType]
            width: 340
            height: 55
            // color: pointData.select ? "#CC3333" : "#EAEAEA"
            color: "#eaeaea"
            border.color: "#cccccc"
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
                    verticalAlignment: Text.AlignVCenter
                    readonly property int axisLabelIndex: Utils.axisLabelIndex(axisType)
                    text: resourceManager.label(22, axisLabelIndex, language.value)
                    color: resourceManager.rgb(22, axisLabelIndex)
                    font.pixelSize: 16
                    leftPadding: 22
                }
                onClicked: {
                    pointData.select = checked
                    checked = Qt.binding(function() { return pointData.select })
                }
            }
            LimitNumericEditBox {
                width: 110
                height: 44
                x:90
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
            LimitNumericEditBox {
                width: 110
                height: 44
                x:210
                anchors.verticalCenter: parent.verticalCenter
                borderColor: "#999999"
                color: "white"
                enabled: pointData.select
                keyLayoutType: 3
                value: local.menuModel.formatSpeed(pointData.speed)
                onReady: {
                    max = "100"
                    min = "1"
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
        width: 339
        height: 30
        x:11
        y:14
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
            x:237
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 19
            text: resourceManager.label(41,488, language.value)
            color: "white"
        }
    }//标题栏
    ListView{
        width: 340
        height:320
        x:10
        y:46
        property int pg: local.page
        spacing: 2
        clip: true
        model: local.numAxis
        delegate: listdelegate_1
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * 5, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * 5, ListView.Beginning)
        }
    }
    Rectangle{
        width: parent.width-4
        height: 60
        anchors.horizontalCenter: parent.horizontalCenter
        y:391
        color: "#DCDDDD"
        Row{
            x:10
            anchors.verticalCenter: parent.verticalCenter
            spacing: 50
            visible: commonViewModel.manipulateMode !== 2
//            CheckBox{
//                id: relativeCheckBox
//                width: 80
//                height: 36
//                anchors.verticalCenter: parent.verticalCenter
//                checked: local.relative
//                indicator:Image{
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.left: parent.left
//                    source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
//                }
//                contentItem: Text{
//                    text: resourceManager.label(41,489, language.value)
//                    color: resourceManager.rgb(41, 489)
//                    font.pixelSize: 18
//                    leftPadding: 30
//                    topPadding: -2
//                }
//                onClicked: {
//                    local.menuModel.relative = checked
//                    checked = Qt.binding(function() { return local.relative })
//                }
//            }
            CheckBox{
                id: noWaitingCheckBox
                width: 80
                height: 36
                anchors.verticalCenter: parent.verticalCenter
                checked: local.noWaiting
                visible: usrlevel >= 3
                indicator:Image{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                }
                contentItem: Text{
                    text: resourceManager.label(41,490, language.value)
                    color: resourceManager.rgb(41, 490)
                    font.pixelSize: 18
                    leftPadding: 30
                    topPadding: -2
                }
                onClicked: {
                    local.menuModel.noWaiting = checked
                    checked = Qt.binding(function() { return local.noWaiting })
                }
            }
            Rectangle {
                width: 120
                height: 60
                color: positionMemoryButtonMouseArea.pressed ? "#cc3333" : "#cccccc"
                border.color: "#999999"

                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: positionMemoryButtonMouseArea.pressed ? 17 : 18
                    color: positionMemoryButtonMouseArea.pressed ? "#ffffff" : resourceManager.rgb(41,491)
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
                    border.color: "#b5b5b6"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "#036EB8"
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
                        onPressed: parent.color = "yellow"
                        onReleased: parent.color = "white"
                    }
                }
            }
        }
    }
}
