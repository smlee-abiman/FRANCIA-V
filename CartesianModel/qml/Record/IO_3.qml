import QtQuick 2.12
import QtQuick.Layouts 1.3
import "../Widget"
//RECORD-IO-IO이력
/*************************************************************************************
* Title: UI/UX of AEK
* File: IO_3.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.18
* Worker: SH.HONG
**************************************************************************************/
Item {
    id: io3_view
    property bool pageActive: false
    property int ioPage: 0
    property int currentHistoryIndex: 0
    property var ioHistoryDataList: ioHistoryViewModel.ioHistoryDataList

    function prevHistory(skipNum)
    {
        currentHistoryIndex = Math.max(currentHistoryIndex - skipNum, 0)
    }

    function nextHistory(skipNum)
    {
        if (ioHistoryDataList.length < 2) {
            currentHistoryIndex = 0
        } else {
            currentHistoryIndex = Math.min(currentHistoryIndex + skipNum, ioHistoryDataList.length - 2)
        }
    }


    Rectangle{
        width: 1002
        height: 515
        x:4
        y:4
        color: "transparent"
        border.color: "#999999"
        Row{
            spacing: -1
            IoHistoryItem {
                id: leftIoHistoryArea
                currentHistoryIndex: io3_view.currentHistoryIndex
                page: ioPage
                onCurrentPageChanged: ioPage = currentPage
            }//左边
            IoHistoryItem {
                currentHistoryIndex: io3_view.currentHistoryIndex + 1
                page: ioPage
                onCurrentPageChanged: ioPage = currentPage
            }//右边
        }
        //换页按钮
        Rectangle {
            width: 60
            height: 515
            x: 942
            color: "transparent"
            border.color: "#999999"

            // 10개 이전
            Rectangle {
                width: parent.width
                height: 60
                y: 0
                color: "transparent"

                ImageButton {
                    anchors.centerIn: parent
                    upImageSource: resourceManager.imagePath + "/35_IO_history/a350_02.png"
                    downImageSource: resourceManager.imagePath + "/35_IO_history/a350_04.png"
                    onClicked: prevHistory(10)
                }
            }

            // 1개 이전
            Rectangle {
                width: parent.width
                height: 60
                y: 60
                color: "transparent"

                ImageButton {
                    anchors.centerIn: parent
                    upImageSource: resourceManager.imagePath + "/35_IO_history/a350_06.png"
                    downImageSource: resourceManager.imagePath + "/35_IO_history/a350_08.png"
                    onClicked: prevHistory(1)
                }
            }

            // 1개 다음
            Rectangle {
                width: parent.width
                height: 60
                y: 397
                color: "transparent"

                ImageButton {
                    anchors.centerIn: parent
                    upImageSource: resourceManager.imagePath + "/35_IO_history/a350_07.png"
                    downImageSource: resourceManager.imagePath + "/35_IO_history/a350_09.png"
                    onClicked: nextHistory(1)
                }
            }

            // 10개 다음
            Rectangle {
                width: parent.width
                height: 70
                y: 451
                color: "transparent"

                ImageButton {
                    anchors.centerIn: parent
                    upImageSource: resourceManager.imagePath + "/35_IO_history/a350_03.png"
                    downImageSource: resourceManager.imagePath + "/35_IO_history/a350_05.png"
                    onClicked: nextHistory(10)
                }
            }
        }

    }

    Connections {
        target: phyKeyManager
        enabled: io3_view.pageActive
        onJogScrollUpChanged: leftIoHistoryArea.prevPage()
        onJogScrollDownChanged: leftIoHistoryArea.nextPage()
    }
}

