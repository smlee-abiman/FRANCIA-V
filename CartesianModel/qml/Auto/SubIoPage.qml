import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Enums 1.0
import "../Widget"

Item {
    id: ioPage
    width: parent.width
    height: parent.height

    property bool pageActive: false
    property var pageModel: autoViewModel.ioView
    property int page: 0
    property var iodataList: pageModel.ioDataList
    property int maxpage: (iodataList.length - 1)/6 + 1
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

    Component {
        id: listDelegate
        Rectangle{
            width: 492
            height: 60
            color: "transparent"
            Rectangle{
                width: 460
                height: 46
                anchors.centerIn: parent
                radius: 25
                border.color: "#B5B5B6"
                Text {
                    width: 60
                    height: 46
                    x:4
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 18
                    color: "black"
                    text: (modelData.index !== -1) ? "[" + resourceManager.symbolAt(modelData.index) + "]" : ""
                }
                Text {
                    width: 340
                    height: 46
                    x:65
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 18
                    color: "black"
                    lineHeight: 0.7
                    wrapMode: Text.WordWrap
                    text: (modelData.index !== -1) ? resourceManager.getMessageStringValue("1", modelData.index, local.language) : ""
                }
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    source: (local.dataLoaded && modelData.value) ? resourceManager.imagePath + "/07_AUTO/d070_23.png" :
                                                                    resourceManager.imagePath + "/07_AUTO/d070_22.png"
                }
            }
        }
    }
    ListView {
        width: 492
        height: 360
        property int pg: page
        clip: true
        model: iodataList
        delegate: listDelegate
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * 6, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(0, ListView.Beginning)
        }
    }

    ImageScrollBar{
        width: 48
        height: 340
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 10
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
