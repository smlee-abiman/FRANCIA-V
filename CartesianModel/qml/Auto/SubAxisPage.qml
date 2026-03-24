import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml 2.12
import "../Widget"

Item {
    id: axisPage
    width: parent.width
    height: parent.height

    property bool pageActive: false
    property var pageModel: autoViewModel.axisView
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property var axisDataList: commonViewModel.axisInfo.axisDataList


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
            pageModel.startMonitoring()
        } else {
            pageModel.stopMonitoring()
        }
    }

    Row{
        x:110
        spacing: 5
        Text {
            width: 100
            height: 35
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 20
            color: resourceManager.rgb(32,22)
            text: resourceManager.label(32,22,local.language)
        }
        Text {
            width: 100
            height: 35
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 20
            color: resourceManager.rgb(32,23)
            text: resourceManager.label(32,23,local.language)
        }
        Text {
            width: 100
            height: 35
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 20
            color: resourceManager.rgb(32,24)
            text: resourceManager.label(32,24,local.language)
        }
    }//标题

    Column{
        x:30
        y:35
        spacing: numAxis > 5 ? 2 : 17
        Repeater{
            model: numAxis
            Rectangle{
                id:data_base
                width: 420
                height: 44
                color: "white"
                radius: 30
                border.color: "#B5B5B6"
                property int axisNum: axisOrder[index]//轴号
                Text {
                    width: 80
                    height: 44
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 15
                    font.pixelSize: 20
                    color: resourceManager.rgb(22,data_base.axisNum)
                    text: resourceManager.label(22,data_base.axisNum,local.language)
                }//轴名称
                Text {
                    width: 100
                    height: 44
                    x:82
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 20
                    color: "black"
                    text: axisDataList[data_base.axisNum].position
                }//位置
                Text {
                    width: 100
                    height: 44
                    x:184
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 20
                    color: "black"
                    text: axisDataList[data_base.axisNum].encoder
                }//编码器
                Text {
                    width: 100
                    height: 50
                    x:290
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 20
                    color: "black"
                    text: axisDataList[data_base.axisNum].torque
                }//扭矩
            }
        }
    }
}
