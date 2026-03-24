import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Program/PosSet/PosSetUtils.js" as Utils
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_4_6.qml
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
    id:acc_dec
    property bool pageActive: false
    color: "transparent"

    QtObject {
        id: local
        property int page: 0
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[3].subMenuList[5]
        property var itemList: menuModel.itemList
        property bool accOn: menuModel.accOn
        property var coordinateX:[72,70,67,65,63,60,58,56,53,51,48,46,43,41,38,36,33,31,28,26,23,21,18,16,13,11]
        property var accdectime:[0.70,0.69,0.68,0.67,0.66,0.65,0.64,0.63,0.62,0.61,0.59,0.56,0.53,0.50,0.47,0.44,0.41,0.38,0.35,0.32,0.29,0.26,0.23,0.22,0.21,0.20]
        property var axisOrder: commonViewModel.axisOrder
        property int numAxis: axisOrder.length
        property int maxPageNo: Math.ceil(numAxis / numAxisPerPage)
        property int numAxisPerPage: 5
    }

    function draw_line(ctx,index){
        ctx.save()
        ctx.beginPath()
        ctx.lineWidth = 2
        ctx.fillStyle = "yellow"
        ctx.strokeStyle = "yelow";
        ctx.moveTo(0,40)
        ctx.lineTo(local.coordinateX[index],8)
        ctx.moveTo(local.coordinateX[index],8)
        ctx.lineTo(80,8)
        //描边
        ctx.stroke()
        ctx.restore();
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
        x: 25
        y: 10
        width: 120
        height: 50
        border.color: "#999999"
        color: local.accOn ? "#cccccc" : "#cc3333"
        Text{
            anchors.centerIn: parent
            font.pixelSize: 18
            font.bold: true
            text: resourceManager.label(41,525, language.value)
            color: local.accOn ? "#666666" : "white"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                local.menuModel.clearSelect()
                local.menuModel.accOn = true
            }
        }
    }
    Rectangle{
        width: 120
        height: 50
        x:139
        y:10
        border.color: "#999999"
        color: local.accOn ? "#cccccc" : "#cc3333"
        Text{
            anchors.centerIn: parent
            font.pixelSize: 18
            font.bold: true
            text: resourceManager.label(41,526, language.value)
            color: local.accOn ? "#666666" : "white"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                local.menuModel.clearSelect()
                local.menuModel.accOn = false
            }
        }
    }

    Component{
        id:listdelegate_8
        Rectangle{
            id: listDelegate
            property int axisType: local.axisOrder[index]
            property var pointData: local.itemList[axisType]
            x: 1
            width: 280
            height: 60
            opacity: local.accOn ? 1 : 0.3
            color: "#eaeaea"
            border.color: "#999999"
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
                    if (local.accOn) {
                        pointData.select = checked
                        checked = Qt.binding(function() { return pointData.select })
                    }
                }
            }
            Row{
                x:100
                anchors.verticalCenter: parent.verticalCenter
                spacing: -4
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    scale: 0.7
                    source: minusButtonMouseArea.pressed ? resourceManager.imagePath + "/16_program/a160_77.png" : resourceManager.imagePath + "/16_program/a160_76.png"
                    MouseArea{
                        id: minusButtonMouseArea
                        anchors.fill: parent
                        onClicked: {
                            if(local.accOn){
                                pointData.acceleration = Math.max(pointData.acceleration - 1, 0)
                                canvas.requestPaint()
                            }else{
                                return
                            }
                        }
                    }
                }
                Rectangle{
                    width: 80
                    height: 40
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    border.color: "#999999"
                    border.width: 2
                    Canvas{
                        id:canvas
                        anchors.fill: parent
                        contextType: "2d"
                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.clearRect(0,0,canvas.width,canvas.height);
                            acc_dec.draw_line(ctx,pointData.acceleration)
                        }
                    }
                    Rectangle{
                        width: 20
                        height: 20
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: 2
                        anchors.bottomMargin: 2
                        color: "#cccccc"
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 16
                            text: pointData.acceleration + 1
                        }
                    }
                }
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    scale: 0.7
                    source: plusButtonMouseArea.pressed ? resourceManager.imagePath + "/16_program/a160_79.png" : resourceManager.imagePath + "/16_program/a160_78.png"
                    MouseArea{
                        id: plusButtonMouseArea
                        anchors.fill: parent
                        onClicked: {
                            if(local.accOn){
                                pointData.acceleration = Math.min(pointData.acceleration + 1, 25);
                                canvas.requestPaint()
                            }
                        }
                    }
                }
            }//加速度档位调整
        }
    }
    Rectangle{
        width: 280
        height: 30
        x:26
        y:65
        color: "#cc3333"
        border.color: "#cc3333"
        opacity: local.accOn ? 1 : 0.3
        Text{
            x:170
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 19
            font.bold: true
            text: resourceManager.label(41,531, language.value)
            color: "white"
        }
    }//标题栏
    ListView{
        width: 281
        height:310
        x:25
        y:95
        property int pg: local.page
        spacing: -1
        clip: true
        model: local.axisOrder.length
        delegate: listdelegate_8
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
    }
}
