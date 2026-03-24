import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Enums 1.0
import "../Widget"

Item {
    id: productionPage
    width: parent.width
    height: parent.height

    property bool pageActive: false
    property var pageModel: autoViewModel.prodView
    property int page: 0

    function getProduceData(index){
        switch(index) {
        case 0:
            return resourceManager.formatData(pageModel.fetchTime, resourceManager.getPenSetIntValue("7", 29, 0), 2)
        case 1:
            return resourceManager.formatData(pageModel.cycleTime, resourceManager.getPenSetIntValue("7", 30, 0), 2)
        case 2:
            return resourceManager.formatData(pageModel.moldingTime, resourceManager.getPenSetIntValue("7", 31, 0))
        case 3:
            return resourceManager.formatData(pageModel.oneFetchNum, resourceManager.getPenSetIntValue("7", 32, 0)) + "/" +  resourceManager.formatData(autoViewModel.prodView.fetchNum, resourceManager.getPenSetIntValue("7", 32, 0))
        case 4:
            return resourceManager.formatData(pageModel.prodNum, resourceManager.getPenSetIntValue("7", 33, 0)) + "/" +  resourceManager.formatData(autoViewModel.prodView.progNum, resourceManager.getPenSetIntValue("7", 33, 0))
        case 5:
            return resourceManager.formatData(pageModel.badRate, resourceManager.getPenSetIntValue("7", 34, 0)) + "/" +  resourceManager.formatData(autoViewModel.prodView.achievementRate, resourceManager.getPenSetIntValue("7", 34, 0))
        case 6:
            return resourceManager.formatData(pageModel.notificationTime, resourceManager.getPenSetIntValue("7", 35, 0))
        case 7:
            return resourceManager.formatData(pageModel.badCount, resourceManager.getPenSetIntValue("7", 36, 0))
        case 8:
            return pageModel.prodEnd
        }
    }

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
                    x: 6
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 20
                    color: resourceManager.rgb(32, 25+index)
                    text: resourceManager.label(32, 25+index, local.language)
                }
                Text {
                    visible: local.dataLoaded
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    font.pixelSize: 20
                    color: resourceManager.dataColor(7, 19)
                    text: getProduceData(index)
                }
            }
        }
    }
    ListView {
        width: 492
        height: 360
        property int pg: page
        clip: true
        model: 9
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
        pageLabal: (page + 1) + "/" + "2"
        pageLabelColor: "black"
        onScrollUpClicked: page = 0
        onScrollDownClicked: page = 1
    }//换页按钮
}
