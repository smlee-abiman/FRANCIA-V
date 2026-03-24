import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Record_Operate.qml
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
    id: record_operate
    property var viewModel: operationHistoryViewModel
    property bool pageActive: false
    property var operationList: operationHistoryList.operationList
    property int page: 0
    property int view_type: 0
    property int view_index: 0
    readonly property int numListItemsPerPage: 7
    readonly property int numDetailItemsPerPage: 3
    property int numOperationItem: operationList.length
    property int numPages: 0
    property bool isExportProcessing: viewModel.isExportProcessing
    property int exportResult: viewModel.exportResult

    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.HistoryOpe)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }
    onIsExportProcessingChanged: {
        if (!isExportProcessing) {
            if (export_processing_dialog.opened) {
                export_processing_dialog.close()
                if (exportResult == 0) {
                    export_completed_dialog.open()
                }
            }
        }
    }
    onOperationListChanged: {
        let prevPage = page
        let numItem = operationList.length
        if (numItem === 0) {
            page = 0
        } else {
            if (page === 0) {
                page = 1
            } else {
                let numItemsPerPage = 1
                if (view_type === 0) {
                    numItemsPerPage = numListItemsPerPage
                } else if (view_type === 1) {
                    numItemsPerPage = numDetailItemsPerPage
                }
                if ((page * numItemsPerPage) > numItem) {
                    page = Math.ceil(numItem / numItemsPerPage)
                }
            }
        }
        numPages = getNumPages(view_type, numItem)
    }

    Component.onCompleted: numPages = getNumPages(view_type, numOperationItem)
    onView_typeChanged: numPages = getNumPages(view_type, numOperationItem)

    function getNumPages(viewType, numItems){
        if (numItems === 0) {
            return 0
        }
        let numItemsPerPage = 1
        if (viewType === 0) {
            numItemsPerPage = numListItemsPerPage
        } else if (viewType === 1) {
            numItemsPerPage = numDetailItemsPerPage
        }
        return Math.ceil(numItems / numItemsPerPage)
    }
    function nextPage(){
        page = Math.min(page + 1, numPages)
    }
    function prevPage(){
        if (numPages === 0) {
            page = 0
        } else {
            page = Math.max(page - 1, 1)
        }
    }

    QtObject {
        id: penSet
        readonly property int groupNo: 25
        readonly property string resetButtonPushColor: resourceManager.dataColor(groupNo, 2)
        readonly property string writeUsbButtonPushColor: resourceManager.dataColor(groupNo, 3)
        readonly property string pageNoColor: resourceManager.dataColor(groupNo, 4)
    }
// 제목 바 전체를 감싸는 사각형
Rectangle {
    x: 10
    y: 10
    width: 796
    height: 40 // 높이는 필요에 따라 조정
    color: "#999999"
    Text {
        x:10
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 24
        font.bold: true
        text: resourceManager.label(50,4, resourceManager.language)
        color: "white"
        visible: view_type == 0
    }
    Text {
        x:170
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 24
        font.bold: true
        text: resourceManager.label(50,5,resourceManager.language)
        color: "white"
        visible: view_type == 0
    }
    Text {
        x:480
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 24
        font.bold: true
        text: resourceManager.label(50,6,resourceManager.language)
        color: "white"
        visible: view_type == 0
    }
}
    Component{
        id:listdelegate
        Rectangle{
            width: 750
            height: 60
            border.color: "#999999"
            color: (index%2 == 0) ? "#ffffff": "#cccccc"
            Text{
                width: 60
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                text: index+1
            }
            Text{
                width: 260
                height: parent.height
                x:60
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                text: Qt.formatDateTime(modelData.dateTime, "yyyy/MM/dd hh:mm:ss")
            }
            Text{
                width: 420
                height: parent.height
                x:330
                wrapMode: Text.WordWrap
                font.pixelSize: 20
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                text :viewModel.toOperateString(modelData, resourceManager.language)
            }
        }
    }
    ListView{
        id: list_view
        property int pg: page
        function setPosition(page)
        {
            if (view_type == 0) {
                positionViewAtIndex((page - 1)*numListItemsPerPage,ListView.Beginning)
            } else if (view_type == 1) {
                positionViewAtIndex((page - 1)*numDetailItemsPerPage,ListView.Beginning)
            }
        }
        x:10
        y:50
        width: 750
        height: (view_type === 0) ? 456 : 458
        clip: true
        spacing: (view_type === 0) ? 6 : 4
        model: operationList
        delegate: (view_type === 0) ? listdelegate : listdelegate1
        interactive: false
        onModelChanged: setPosition(pg)
        onPgChanged: setPosition(pg)
    }

    Component{
        id:listdelegate1
        Rectangle{
            width: 750
            height: 150
            border.color: "#999999"
            color: (index%2 == 0) ? "#ffffff": "#cccccc"
            Text{
                width: parent.width-4
                height: 40
                x:4
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 22
                text: index+1
            }
            Text{
                width: parent.width-4
                height: 40
                x:4
                y:50
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 22
                text: Qt.formatDateTime(modelData.dateTime, "yyyy/MM/dd hh:mm:ss")
            }
            Text{
                width:parent.width-4
                height:125
                x:4
                y:100
                wrapMode: Text.WordWrap
                font.pixelSize: 20
                verticalAlignment: Text.AlignTop
                horizontalAlignment: Text.AlignLeft
                text :viewModel.toOperateString(modelData, resourceManager.language)
            }
        }
    }

Rectangle {
    x: 759
    y: 50
    width: 47
    height: 456
    color: "#eaeaea"
    border.color: "#999999"

    // ◀ 이전 페이지 버튼
    Rectangle {
        width: 47
        height: 47
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 3
        color: "transparent"

        Image {
            anchors.centerIn: parent
            source: prev_page_mouse_area.pressed
                    ? resourceManager.imagePath + "/33_ALM_history/a330_02.png"
                    : resourceManager.imagePath + "/33_ALM_history/a330_00.png"
        }

        MouseArea {
            id: prev_page_mouse_area
            anchors.fill: parent
            onClicked: {
                prevPage()
                phyKeyManager.ctrlBuzzer()
            }
        }
    }

    // 🔢 페이지 번호 (가운데 정중앙)
    Rectangle {
        width: 60
        height: 30
        anchors.centerIn: parent
        color: "transparent"

        PageText {
            anchors.centerIn: parent
            page: record_operate.page
            totalPage: record_operate.numPages
            color: "#666666"
        }
    }

    // ▶ 다음 페이지 버튼
    Rectangle {
        width: 47
        height: 47
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 3
        color: "transparent"

        Image {
            anchors.centerIn: parent
            source: next_page_mouse_area.pressed
                    ? resourceManager.imagePath + "/33_ALM_history/a330_03.png"
                    : resourceManager.imagePath + "/33_ALM_history/a330_01.png"
        }

        MouseArea {
            id: next_page_mouse_area
            anchors.fill: parent
            onClicked: {
                nextPage()
                phyKeyManager.ctrlBuzzer()
            }
        }
    }
}

    //우측
    Rectangle{
        width: 140
        height: 496
        x:826
        y:10
        border.color: "#999999"
        color: "#eaeaea"

        Rectangle {
            width: 110
            height: 60
            color: "#cccccc"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 60

            Text {
                id: res
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 23
                color: "#666666"
                text: resourceManager.label(50, 8, resourceManager.language)
            }

            Rectangle {
                width: 90
                height: 48
                anchors.centerIn: parent
                color: "transparent"

                MouseArea {
                    id: resetButtonMouseArea
                    anchors.fill: parent
                    onPressed: {
                        res.font.pixelSize = 22
                        commonViewModel.modbusResetControl(true)
                    }
                    onReleased: {
                        res.font.pixelSize = 23
                        commonViewModel.modbusResetControl(false)
                    }
                }
            }
        }

        Rectangle {
            width: 110
            height: 60
            color: "#cccccc"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 140

            Text {
                id: exp
                anchors.centerIn: parent
                font.pixelSize: writeUsbButtonMouseArea.pressed ? 22 : 23
                color: "#666666"
                text: resourceManager.label(50, 7, resourceManager.language)
            }

            Rectangle {
                width: 90
                height: 48
                anchors.centerIn: parent
                color: "transparent"

                MouseArea {
                    id: writeUsbButtonMouseArea
                    anchors.fill: parent
                    onClicked: {
                        export_dialog.open()
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }

        Rectangle{
            width: 120
            height: 1
            anchors.horizontalCenter: parent.horizontalCenter
            y:240
            color: "white"
        }
        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            y:280
            source:(view_type == 0) ? resourceManager.imagePath + "/33_ALM_history/a330_08.png" : resourceManager.imagePath + "/33_ALM_history/a330_07.png"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    let topItemIndex
                    let lastPage = numPages
                    if (view_type == 0) {
                        view_type = 1
                        if (operationList.length === 0) {
                            page = 0
                        } else {
                            if (page === lastPage) {
                                topItemIndex = Math.max(operationList.length - numListItemsPerPage - 1, 0)
                            } else {
                                topItemIndex = (numListItemsPerPage * (page - 1))
                            }
                            page = (topItemIndex / numDetailItemsPerPage) + 1
                        }
                    } else if (view_type == 1){
                        view_type = 0
                        if (operationList.length === 0) {
                            page = 0
                        } else {
                            if (page === lastPage) {
                                topItemIndex = Math.max(operationList.length - numDetailItemsPerPage - 1, 0)
                            } else {
                                topItemIndex = (numDetailItemsPerPage * (page - 1))
                            }
                            page = (topItemIndex / numListItemsPerPage) + 1
                        }
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }

    Loader {
        id: export_dialog
        function open()
        {
            if (item == null) {
                active = true
            } else {
                item.open()
            }
        }
        active: false
        onLoaded: item.open()
        sourceComponent: RecordDialog {
            msg: resourceManager.label(50, 31, resourceManager.language)
            msgColor: resourceManager.rgb(50, 31)
            language: resourceManager.language
            onCancelClicked: close()
            onOkClicked: {
                close()
                let ret = viewModel.exportData()
                if (ret) {
                    export_processing_dialog.open()
                }
            }
        }
    }

    Loader {
        id: export_processing_dialog
        property bool opened: item == null ? false : item.opened
        function open()
        {
            if (item == null) {
                active = true
            } else {
                item.open()
            }
        }

        function close()
        {
            if (item != null) {
                item.close()
            }
        }
        active: false
        onLoaded: item.open()
        sourceComponent: RecordDialog {
            msg: resourceManager.label(50, 186, resourceManager.language)
            msgColor: resourceManager.rgb(50, 186)
            language: resourceManager.language
            cancelButtonVisible: false
            okButtonVisible: false
        }
    }

    Loader {
        id: export_completed_dialog
        function open()
        {
            if (item == null) {
                active = true
            } else {
                item.open()
            }
        }

        active: false
        onLoaded: item.open()
        sourceComponent: RecordDialog {
            msg: resourceManager.label(50, 89, resourceManager.language)
            msgColor: resourceManager.rgb(50, 89)
            language: resourceManager.language
            okButtonVisible: false
            onCancelClicked: close()
        }
    }

    Connections {
        target: phyKeyManager
        enabled: record_operate.pageActive
        onJogScrollUpChanged: prevPage()
        onJogScrollDownChanged: nextPage()
    }
}
