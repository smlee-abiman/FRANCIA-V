import QtQuick 2.12
import QtQuick.Layouts 1.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: IoHistoryItem.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.1
* Worker: SH.HONG
**************************************************************************************/
Rectangle{
    id: io_history_item
    property var ioHistoryDataList: ioHistoryViewModel.ioHistoryDataList
    property var ioHistoryData: ((currentHistoryIndex + 1) > numIoHistoryData) ? null : ioHistoryDataList[currentHistoryIndex]
    property int numIoHistoryData: ioHistoryDataList.length
    property int currentHistoryIndex: 0
    property int page: 0
    property int numItemsPerPage: 24
    property int maxPage: Math.ceil(ioHistoryViewModel.displayIoAddressList.length / numItemsPerPage)
    property var displayIoList: ioHistoryViewModel.getDisplayIoAddressList(page, numItemsPerPage)

    signal currentPageChanged(int currentPage)

    function prevPage()
    {
        let newPage = Math.max(page-1,0)
        if (newPage !== page) {
            currentPageChanged(newPage)
        }
    }

    function nextPage()
    {
        let newPage = Math.min(page+1,maxPage - 1)
        if (newPage !== page) {
            currentPageChanged(newPage)
        }
    }

    width: 472
    height: 515
    color: "transparent"
    border.color: "#999999"

    Rectangle {
        width: 140
        height: 42
        color: "#cccccc"
        border.color: "#999999"
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 60
            font.pixelSize: 17
            font.bold: true
            color: resourceManager.rgb(50, 180)
            text: resourceManager.label(50, 180,resourceManager.language)
        }
        Rectangle{
            width: 52
            height: 32
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 6
            color: "white"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                text: (currentHistoryIndex + 1) > numIoHistoryData ? "" : currentHistoryIndex + 1
            }
        }
    }
    Rectangle {
        width: 315
        height: 42
        x:142
        color: "#cccccc"
        border.color: "#999999"
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 265
            font.pixelSize: 17
            font.bold: true
            color: resourceManager.rgb(50, 184)
            text: resourceManager.label(50, 184,resourceManager.language)
        }
        Rectangle{
            width: 245
            height: 32
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 6
            color: "white"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                text: ioHistoryData === null ? "" : Qt.formatDateTime(ioHistoryData.dateTime, "yyyy/MM/dd hh:mm:ss")
            }
        }
    }
    Item{
        width: parent.width-6
        height: parent.height-102
        clip: true
        x:5
        y:46
        Grid{
            rows: 8
            columns: 3
            rowSpacing: 5
            columnSpacing: 4
            flow: Grid.TopToBottom
            Repeater{
                model: 24
                Rectangle {
                    property int ioAddress: io_history_item.displayIoList[index]
                    width: 148
                    height: 47
                    border{ width: 1; color: "#999999" }
                    color: "#eaeaea"
                    visible: (ioAddress >= 0)
                    Image {
                        x:-2
                        anchors.verticalCenter: parent.verticalCenter
                        scale: 0.8
                        source: ioHistoryViewModel.isIoStatusOn(currentHistoryIndex, ioAddress) ? resourceManager.imagePath + "/33_ALM_history/a330_09.png" : resourceManager.imagePath + "/33_ALM_history/a330_10.png"
                    }
                    Text {
                        x:25
                        width: parent.width-25
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        wrapMode:Text.WordWrap
                        font.pixelSize: 15
                        text: (ioAddress != -1) ? resourceManager.symbolAt(ioAddress) + " " + resourceManager.getMessageStringValue("1", ioAddress, resourceManager.language) : ""
                        color: (ioAddress != -1) ? resourceManager.dataColor(8, 4) : "black"
                    }
                }
            }
        }
    }
    Row{
        anchors.horizontalCenter: parent.horizontalCenter
        y:463
        spacing: 140
        ImageButton{
            upImageSource: resourceManager.imagePath + "/35_IO_history/a350_10.png"
            downImageSource: resourceManager.imagePath + "/35_IO_history/a350_12.png"
            onClicked: prevPage();
        }
        Text{
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 22
            text: page+1 + "/" + maxPage
        }

        ImageButton{
            upImageSource: resourceManager.imagePath + "/35_IO_history/a350_11.png"
            downImageSource: resourceManager.imagePath + "/35_IO_history/a350_13.png"
            onClicked: nextPage()
        }
    }
}

