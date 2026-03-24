import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../Widget"
import "../Program/PosSet/PosSetUtils.js" as Utils
//RECORD-IO-전체이력
/*************************************************************************************
* Title: UI/UX of AEK
* File: IO_2.qml
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
    id: io2_view
    property var viewModel: ioHistoryViewModel
    property bool pageActive: false
    property int bankno: ioHistoryViewModel.bankNo
    property int banktype: ioHistoryViewModel.bankType
    property int bankIndex: bankno*2+banktype
    property var bankdatatype: ioHistoryViewModel.bankDataType
    property var ioHistoryDataList: ioHistoryViewModel.ioHistoryDataList
    property var ioHistoryData: ioHistoryDataList.length === 0 ? null : ioHistoryDataList[currentHistoryIndex]
    property int numIoHistoryData: ioHistoryDataList.length
    property int currentHistoryIndex: 0

    function prevHistory(skipNum)
    {
        currentHistoryIndex = Math.max(currentHistoryIndex - skipNum, 0)
    }

    function nextHistory(skipNum)
    {
        currentHistoryIndex = Math.min(currentHistoryIndex + skipNum, ioHistoryDataList.length - 1)
    }

    function openIoHistoryFileLoadingDialog() {
        if (pageActive) {
            ioHistoryFileLoadingDialog.open()
        }
    }

    function closeIoHistoryFileLoadingDialog() {
        if (pageActive) {
            ioHistoryFileLoadingDialog.close()
        }
    }

    onPageActiveChanged: {
        if (pageActive) {
            if (!local.isFtpFileAcquired) {
                openIoHistoryFileLoadingDialog()
            }
        } else {
            closeIoHistoryFileLoadingDialog()
        }
    }

    QtObject {
        id: local
        property bool isFtpFileAcquired: viewModel.isFtpFileAcquired
        onIsFtpFileAcquiredChanged: {
            if (isFtpFileAcquired) {
                io2_view.closeIoHistoryFileLoadingDialog()
            } else {
                io2_view.openIoHistoryFileLoadingDialog()
            }
        }
    }

    Rectangle{
        width: 1002
        height: 515
        x:4
        y:4
        color: "transparent"
        border.color: "#999999"

        //左侧
        ResourceStringComboBox1{
            id:bankNo
            x:17
            y:20
            fontSize: 20
            resourceGroupId: 33
            resourceStartIndex: 34
            listItemCount: 5
            currentIndex:bankno
            selectedColor: "#000000"
            onCurrentIndexChanged:{
                if (currentIndex !== -1){
                    ioHistoryViewModel.bankNo = currentIndex;
                }
            }
        }
        ResourceStringComboBox1{
            id:bankContents
            x:242
            y:20
            fontSize: 20
            resourceGroupId: 33
            resourceStartIndex: 39
            listItemCount: 2
            currentIndex:banktype
            selectedColor: "#000000"
            onCurrentIndexChanged:{
                if (currentIndex !== -1){
                    ioHistoryViewModel.bankType = currentIndex;
                }
            }
        }
        ListModel{
            id:listmodel
            ListElement{index: "0"}
            ListElement{index: "1"}
            ListElement{index: "2"}
            ListElement{index: "3"}
            ListElement{index: "4"}
            ListElement{index: "5"}
            ListElement{index: "6"}
            ListElement{index: "7"}
            ListElement{index: "8"}
            ListElement{index: "9"}
            ListElement{index: "A"}
            ListElement{index: "B"}
            ListElement{index: "C"}
            ListElement{index: "D"}
            ListElement{index: "E"}
            ListElement{index: "F"}
        }
        Rectangle{
            width: 402
            height: 24
            x:40
            y:83
            color: "#cccccc"
            border.color: "#999999"
            Row{
                anchors.centerIn: parent
                spacing: 15.6
                Repeater{
                    model: listmodel
                    Text{
                        font.pixelSize: 16
                        text: index
                    }
                }
            }
        }
        Rectangle{
            width: 24
            height: 402
            x:17
            y:106
            color: "#cccccc"
            border.color: "#999999"
            Column{
                anchors.centerIn: parent
                spacing: 7
                Repeater{
                    model: listmodel
                    Text{
                        font.pixelSize: 16
                        text: index + "0"
                    }
                }
            }
        }

        Rectangle{
            id: io_status_area
            property var ioStatusList: ioHistoryData === null ? null : ioHistoryData.ioStatusList[bankIndex].status
            width: 402
            height: 402
            x:40
            y:106
            color: "#FFFFFF"
            border.color: "#999999"
            Rectangle{
                width: 1
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#999999"
            }
            Grid{
                anchors.centerIn: parent
                rows: 16
                columns: 2
                rowSpacing: 7
                columnSpacing: 7
                Repeater{
                    model: 32
                    Rectangle{
                        id: io_status_byte_field
                        property int ioStatus: io_status_area.ioStatusList === null ? 0 : io_status_area.ioStatusList[index]
                        width: 193
                        height: 18
                        color: "transparent"
                        Loader {
                            anchors.fill: parent
                            sourceComponent: bankdatatype[bankIndex*32+index] ? io_status_byte_component : io_status_bit_component
                        }
                        Component {
                            id: io_status_bit_component
                            Row{
                                spacing: 7
                                anchors.fill: parent
                                Repeater{
                                    model: 8
                                    Rectangle{
                                        width: 18
                                        height: 18
                                        border.width: 2
                                        color: (io_status_byte_field.ioStatus & (1 << (7 - index))) == 0 ? "#ffffff" : "#cc3333"
                                        border.color: "#999999"
                                    }
                                }
                            }//bit显示
                        }
                        Component {
                            id: io_status_byte_component
                            Text{
                                font.pixelSize: 16
                                x:5
                                verticalAlignment: Text.AlignVCenter
                                text: io_status_byte_field.ioStatus
                            }//byte显示
                        }
                    }
                }
            }
        }

        //右侧
        Rectangle {
            width: 140
            height: 42
            x:480
            y:20
            color: "#cccccc"
            border.color: "#999999"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 60
                font.pixelSize: 17
                font.bold: true
                color: resourceManager.rgb(50,180)
                text: resourceManager.label(50,180,resourceManager.language)
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
                    text: io2_view.numIoHistoryData === 0 ? "" : currentHistoryIndex + 1
                }
            }
        }
        Rectangle {
            width: 280
            height: 42
            x:630
            y:20
            color: "#cccccc"
            border.color: "#999999"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 235
                font.pixelSize: 17
                font.bold: true
                color: resourceManager.rgb(50,184)
                text: resourceManager.label(50,184,resourceManager.language)
            }
            Rectangle{
                width: 220
                height: 32
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: "white"
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: ioHistoryData === null ? "" : Qt.formatDateTime(io2_view.ioHistoryData.dateTime, "yyyy/MM/dd hh:mm:ss")
                }
            }
        }

        Rectangle{
            width: 180
            height: 400
            x:480
            y:83
            radius: 5
            color: "#cccccc"
            border.color: "#999999"
            Text {
                x:90
                font.pixelSize: 20
                color: resourceManager.rgb(50,183)
                text: resourceManager.label(50,183,resourceManager.language)
            }
            Column{
                id: axisPositionArea
                property var axisPositionList: ioHistoryData === null ? null : io2_view.ioHistoryData.axisPositionList
                property var axisOrder: commonViewModel.axisOrder
                property int numAxis: axisOrder.length
                x:10
                y:30
                spacing: 15
                Repeater{
                    model: axisPositionArea.numAxis
                    Rectangle{
                        property int axisType: axisPositionArea.axisOrder[index]
                        width: 160
                        height: 32
                        color: "transparent"
                        Text {
                            readonly property int axisLabelIndex: Utils.axisLabelIndex(parent.axisType)
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 110
                            font.pixelSize: 20
                            color: resourceManager.rgb(22, axisLabelIndex)
                            text: resourceManager.label(22, axisLabelIndex, resourceManager.language)
                        }
                        Rectangle{
                            width: 100
                            height: 32
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 6
                            radius: 16
                            color: "white"
                            Text {
                                readonly property int positionFormat: 66
                                readonly property int minValue: -8388608
                                function formatPosition(position, positionFormat)
                                {
                                    let ret = ""
                                    if (position === minValue) {
                                        ret = "****.**"
                                    } else {
                                        ret = resourceManager.formatData(position, positionFormat)
                                    }
                                    return ret
                                }
                                anchors.centerIn: parent
                                font.pixelSize: 20
                                text: axisPositionArea.axisPositionList === null ? "" : formatPosition(axisPositionArea.axisPositionList[axisType], positionFormat)
                            }
                        }
                    }
                }
            }
        }
        Rectangle{
            width: 160
            height: 400
            x:700
            y:83
            radius: 5
            color: "#cccccc"
            border.color: "#999999"
            Rectangle{
                width: 1
                height: 398
                anchors.centerIn: parent
                color: "white"
            }
            Text {
                x:22
                font.pixelSize: 20
                color: resourceManager.rgb(50,181)
                text: resourceManager.label(50,181,resourceManager.language)
            }
            Text {
                x:98
                font.pixelSize: 20
                color: resourceManager.rgb(50,182)
                text: resourceManager.label(50,182,resourceManager.language)
            }
            Column{
                id: seq_step_area
                property var seqList: ioHistoryData === null ? null : io2_view.ioHistoryData.seqList
                property var stepList: ioHistoryData === null ? null : io2_view.ioHistoryData.stepList
                x:10
                y:30
                spacing: 15
                Repeater{
                    model: 8
                    Row{
                        spacing: 22
                        Rectangle{
                            id: seq
                            width: 60
                            height: 32
                            radius: 16
                            color: "white"
                            Text {
                                anchors.centerIn: parent
                                font.pixelSize: 20
                                text: seq_step_area.seqList === null ? "" : seq_step_area.seqList[index]
                            }
                        }
                        Rectangle{
                            id: step
                            width: 60
                            height: 32
                            radius: 16
                            color: "white"
                            Text {
                                anchors.centerIn: parent
                                font.pixelSize: 20
                                text: seq_step_area.stepList === null ? "" : seq_step_area.stepList[index] + 1
                            }
                        }
                    }
                }
            }
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

    Loader {
        id: ioHistoryFileLoadingDialog
        width: parent.width
        height: parent.height
        readonly property bool opened: (status === Loader.Ready) && item.opened
        function open() {
            asynchronous = false
            if (status === Loader.Ready) {
                item.open()
            }
        }
        function close() {
            if (status === Loader.Ready) {
                item.close()
            }
            asynchronous = true
        }
        active: io2_view.pageActive
        asynchronous: true
        visible: (status === Loader.Ready) && io2_view.pageActive
        onActiveChanged: {
            if (!active) {
                asynchronous = true
            }
        }
        onLoaded: {
            if (!asynchronous) {
                item.open()
            }
            asynchronous = true
        }
        sourceComponent: RecordDialog {
            msg: resourceManager.label(22, 11, resourceManager.language)
            msgColor: resourceManager.rgb(22, 11)
            language: resourceManager.language
            cancelButtonVisible: false
            okButtonVisible: false
        }
    }
}
