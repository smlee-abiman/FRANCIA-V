import QtQuick 2.12
import Enums 1.0
import "../../Widget/"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Maintain_2.qml
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
Item {
    id:maintain2
    property var viewModel: maintainViewModel
    property bool pageActive:false
    property int pageIndex: 0
    property int maxPageIndex: 1
    property var refuelingCompleteList: viewModel.refuelingCompleteList
    property var maintainDataList: viewModel.maintainDataList
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property int maxPageNo: Math.ceil(numAxis / numAxisPerPage)
    property int numAxisPerPage: 5

    function resetsave(){
        viewModel.saveLastRefuelingDate(getDate())
        operationRecord.recordRefuelingSaved()
    }

    function getDate(){
         if(REAL_TARGET) {
             return commonViewModel.curDate
         } else {
             return Qt.formatDateTime(new Date(), "yyyy/MM/dd");
         }
     }

    function prevPage() {
        pageIndex = Math.max(pageIndex-1,0)
    }

    function nextPage() {
        pageIndex = Math.min(pageIndex+1,maxPageIndex)
    }

    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.FunctionMaintain)
        }
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "20"
        property string completeButtonOnColor: ""
        property string dataColor: ""
        Component.onCompleted: {
            completeButtonOnColor = resourceManager.dataColor(groupNo, 10)
            dataColor = resourceManager.dataColor(groupNo, 11)
        }
    }

    Rectangle{
        width: 100
        height: 46
        x:906
        y:-42
        border.width: 2
        border.color: "#999999"
        color: savemouse2.pressed ? "#cc3333" : "#cccccc"
        radius: 6
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: savemouse2.pressed ? 23 : 22
            color: resourceManager.rgb(45,6)
            text: resourceManager.label(45,6,resourceManager.language)
        }
        MouseArea{
            id:savemouse2
            anchors.fill: parent
            onClicked: {
                resetsave()
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//保存按钮

    Rectangle{
        width: 1000
        height: 510
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 6
        anchors.bottomMargin: 2
        color: "transparent"
        border.color: "#999999"
        border.width: 1
        Rectangle{
            width:parent.width-70
            height: 1
            y: 60
            color: "#999999"
        }
        Rectangle{
            width:parent.width-70
            height: 1
            y:151
            color: "#999999"
        }
        Rectangle{
            width:parent.width-70
            height: 1
            y:242
            color: "#999999"
        }
        Rectangle{
            width:parent.width-70
            height: 1
            y:334
            color: "#999999"
        }
        Rectangle{
            width:parent.width-70
            height: 1
            y:426
            color: "#999999"
        }

        Rectangle{
            width:1
            height: parent.height
            x:185
            color: "#999999"
        }
        Rectangle{
            width:1
            height: parent.height
            x:370
            color: "#999999"
        }
        Rectangle{
            width:1
            height: parent.height
            x:557
            color: "#999999"
        }
        Rectangle{
            width:1
            height: parent.height
            x:742
            color: "#999999"
        }
        Rectangle{
            width:1
            height: parent.height
            x:930
            color: "#999999"
        }
    }//表格

    Row{
        x:192
        y:8
        spacing: 2
        Text {
            width: 184
            height: 60
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 21

            text: resourceManager.label(45,18, resourceManager.language)
            color: resourceManager.rgb(45,18)
        }
        Text {
            width: 184
            height: 60
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 21

            text: resourceManager.label(45,19, resourceManager.language)
            color: resourceManager.rgb(45,19)
        }
        Text {
            width: 184
            height: 60
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 21

            text: resourceManager.label(45,20, resourceManager.language)
            color: resourceManager.rgb(45,20)
        }
    }//标题

    Component{
        id:delegate1
        Row{
            property int axis: axisOrder[index]
            property var axisData: maintainDataList[axis]
            spacing: 2
            Text{
                width: 184
                height: 90
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                text: resourceManager.label(22,axis,resourceManager.language)
                color: resourceManager.rgb(22,axis)
            }
            Text{
                width: 184
                height: 90
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                color: penSet.dataColor
                text: viewModel.formatRefuelingDistance(axisData.refuelingDistance)
            }
            Text{
                width: 184
                height: 90
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                color: penSet.dataColor
                text: viewModel.formatCumulativeDistance(axisData.cumulativeDistance)
            }
            Text{
                width: 184
                height: 90
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                color: penSet.dataColor
                text: axisData.lastRefuelingDate
            }
            Rectangle{
                width: 184
                height: 90
                color: "transparent"
                Rectangle{
                    property bool complete: refuelingCompleteList[axis]
                    width: 115
                    height: 50
                    anchors.centerIn: parent
                    border.width: 2
                    border.color: "#999999"
                    color: (savemouse1.pressed || complete) ? "#cc3333" : "#cccccc"
                    radius: 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: savemouse1.pressed ? 23 : 22

                        text: resourceManager.label(45,21,resourceManager.language)
                        color: parent.complete ? penSet.completeButtonOnColor : resourceManager.rgb(45,21)
                    }
                    MouseArea{
                        id:savemouse1
                        anchors.fill: parent
                        onClicked: {
                            viewModel.refuelingCompleteList[axis] = !parent.complete
                            if (parent.complete) {
                                operationRecord.recordRefuelingCompleted(axis)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//保存按钮
            }
        }
    }

    ListView{
        property int pg: pageIndex
        width: 940
        height: 450
        x:6
        y:68
        clip: true
        spacing: 2
        interactive: false
        model: axisOrder.length
        delegate: delegate1
        onPgChanged: {
            positionViewAtIndex(pg * 5,ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * 5, ListView.Beginning)
        }
    }

    ImageScrollBar{
        x:944
        y:12
        height: 502
        botomButtonPressImage: resourceManager.imagePath + "/25_controller/d250_03.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/25_controller/d250_01.png"
        topButtonPressImage: resourceManager.imagePath + "/25_controller/d250_02.png"
        topButtonReleaseImage: resourceManager.imagePath + "/25_controller/d250_00.png"
        visible: maxPageNo >= 2
        pageLabal: pageIndex+1 + "/" + maxPageNo
        onScrollUpClicked: prevPage()
        onScrollDownClicked: nextPage()
    }//切换按钮
/***********************************************************************/
//    Connections {
//        target: phyKeyManager
//        enabled: maintain2.pageActive
//        onJogScrollUpChanged: prevPage()
//        onJogScrollDownChanged: nextPage()
//    }
}
