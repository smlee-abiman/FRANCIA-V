import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Enums 1.0
import "../Widget"

Item {
    id: timerPage
    width: parent.width
    height: parent.height

    property bool pageActive: false
    property var pageModel: autoViewModel.timerView
    property int page: 0
    property int maxpage: 17

    QtObject {
        id: local
        property int language: 0
        property bool dataLoaded: pageModel.dataLoaded
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
    Component.onDestruction: pageModel.stopMonitoring()

    Column{
        x:12
        y:10
        spacing: 7
        Repeater{
            model: pageModel.getDataList(page)
            Rectangle{
                width: 340
                height: 38
                radius: 25
                border.color: "#B5B5B6"
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    font.pixelSize: 17
                    color: (local.dataLoaded && modelData.start) ? "red" : "black"
                    text: "[" + modelData.mark + "]"
                }
                Text {
                    x:54
                    width: 240
                    height: 38
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    wrapMode: Text.WordWrap
                    lineHeight: 0.7
                    font.pixelSize: 17
                    color: (local.dataLoaded && modelData.start) ? "red" : "black"
                    text: modelData.msg
                }
                Text {
                    visible: local.dataLoaded
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    font.pixelSize: 18
                    color: (modelData.up ? "red" : resourceManager.dataColor(7, 61))
                    text: resourceManager.formatData(modelData.timer, resourceManager.getPenSetIntValue("7", 68, 0), 2)
                }
            }
        }
    }

    ImageScrollBar{
        width: 48
        height: 262
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 6
        topButtonPressImage: resourceManager.imagePath + "/07_AUTO/a070_17.png"
        topButtonReleaseImage: resourceManager.imagePath + "/07_AUTO/a070_15.png"
        botomButtonPressImage: resourceManager.imagePath + "/07_AUTO/a070_18.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/07_AUTO/a070_16.png"
        pageLabal: (page + 1) + "/" + maxpage
        pageLabelColor: "black"
        onScrollUpClicked: page = Math.max(page - 1, 0)
        onScrollDownClicked: page = Math.min(page + 1, maxpage - 1)
    }//换页按钮
}
