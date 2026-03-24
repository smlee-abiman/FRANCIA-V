import QtQuick 2.12
import QtQml 2.12
import Enums 1.0
import "../../Widget/"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Maintain_1.qml
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
    id:maintain1
    property var viewModel: maintainViewModel
    property bool pageActive:false
    property int pageIndex: 0
    property int maxPageIndex: 1
    property var maintainCompleteList: viewModel.maintainCompleteList
    property var lastInspectionDateList: viewModel.lastInspectionDateList

    //函数
    function resetsave(){
        viewModel.saveLastInspectionDate(getDate())
        operationRecord.recordInspectionSaved()
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
            commonViewModel.pageChange(Enums.FunctionMaintainCheck)
        }
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "20"
        property string completeButtonOnColor: ""
        property string lastInspectionDateColor: ""
        property string pageColor: ""
        Component.onCompleted: {
            completeButtonOnColor = resourceManager.dataColor(groupNo, 2)
            lastInspectionDateColor = resourceManager.dataColor(groupNo, 3)
            pageColor = resourceManager.dataColor(groupNo, 4)
        }
    }

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

    Rectangle{
        width: 100
        height: 46
        x:906
        y:-42
        border.width: 2
        border.color: "#999999"
        color: savemouse1.pressed ? "#cc3333" : "#cccccc"
        radius: 6
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: savemouse1.pressed ? 23 : 22
            color: savemouse1.pressed ? "white" : "#666666"
            text: resourceManager.label(45,6,local.language)
        }
        MouseArea{
            id:savemouse1
            anchors.fill: parent
            onClicked:{
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
            y:243
            color: "#999999"
        }
        Rectangle{
            width:parent.width-70
            height: 1
            y:335
            color: "#999999"
        }
        Rectangle{
            width:parent.width-70
            height: 1
            y:427
            color: "#999999"
        }

        Rectangle{
            width:1
            height: parent.height
            x:80
            color: "#999999"
        }
        Rectangle{
            width:1
            height: parent.height
            x:650
            color: "#999999"
        }
        Rectangle{
            width:1
            height: parent.height
            x:800
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
        x:6
        y:8
        spacing: 3
        Text {
            width: 78
            height: 60
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 21
            text: resourceManager.label(45,3, local.language)
            color: resourceManager.rgb(45,3)
        }
        Text {
            width: 568
            height: 60
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 21
            text: resourceManager.label(45,4, local.language)
            color: resourceManager.rgb(45,4)
        }
        Text {
            width: 148
            height: 60
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 21
            text: resourceManager.label(45,5, local.language)
            color: resourceManager.rgb(45,5)
        }
    }//标题

    ListModel{
        id:listmodel
        ListElement{index:0}
        ListElement{index:1}
        ListElement{index:2}
        ListElement{index:3}
        ListElement{index:4}
        ListElement{index:5}
        ListElement{index:6}
        ListElement{index:7}
        ListElement{index:8}
        ListElement{index:9}
    }

    Component{
        id:delegate1
        Row{
            spacing: 3
            Text{
                width: 78
                height: 90
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                text: index+1
            }
            Text{
                width: 568
                height: 90
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: 4
                font.pixelSize: 19
                color: resourceManager.rgb(45,7+index)
                text: resourceManager.label(45,7+index,local.language)
            }
            Text{
                width: 148
                height: 90
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: 4
                font.pixelSize: 19
                color: penSet.lastInspectionDateColor
                text: lastInspectionDateList[index]
            }
            Rectangle{
                width: 128
                height: 90
                color: "transparent"
                Rectangle{
                    property bool complete: maintainCompleteList[index]
                    width: 100
                    height: 42
                    anchors.centerIn: parent
                    border.width: 2
                    border.color: "#999999"
                    color: (savemouse.pressed || complete) ? "#cc3333" : "#cccccc"
                    radius: 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: savemouse.pressed ? 23 : 22
                        color: parent.complete ? penSet.completeButtonOnColor : resourceManager.rgb(45,17)
                        text: resourceManager.label(45,17,local.language)
                    }
                    MouseArea{
                        id:savemouse
                        anchors.fill: parent
                        onClicked: {
                            viewModel.maintainCompleteList[index] = !parent.complete
                            if (parent.complete) {
                                operationRecord.recordInspectionCompleted(index)
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
        x:4
        y:68
        clip: true
        spacing: 2
        interactive: false
        model: listmodel
        delegate: delegate1
        onPgChanged: {
            positionViewAtIndex(pg * 5,ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * 5, ListView.Beginning)
        }
    }

    ImageScrollBar{
        x:952
        y:16
        height: 490
        botomButtonPressImage: resourceManager.imagePath + "/25_controller/a250_03.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/25_controller/a250_01.png"
        topButtonPressImage: resourceManager.imagePath + "/25_controller/a250_02.png"
        topButtonReleaseImage: resourceManager.imagePath + "/25_controller/a250_00.png"
        pageLabal: pageIndex+1 + "/" + "2"
        pageLabelColor: penSet.pageColor
        onScrollUpClicked: prevPage()
        onScrollDownClicked: nextPage()
    }//切换按钮
/***********************************************************************/
//    Connections {
//        target: phyKeyManager
//        enabled: maintain1.pageActive
//        onJogScrollUpChanged: prevPage()
//        onJogScrollDownChanged: nextPage()
//    }
}
