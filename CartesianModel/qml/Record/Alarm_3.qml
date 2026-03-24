import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Alarm_3.qml
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
    id: axis_alarm_view
    property var viewModel: alarmHistoryViewModel
    property var alarmList: viewModel.axisAlarmList
    property bool pageActive: false
    property int page: 0
    property int view_type: 0
    property int view_index: 0
    readonly property int numListItemsPerPage: 7
    readonly property int numGridItemsPerPage: 4
    readonly property int numDetailItemsPerPage: 1
    property int numAlarmItem: alarmList.length
    property int numPages: 0
    readonly property int axis_alarm_msg_type_start_index: 1040
    property bool isExportProcessing: viewModel.isExportProcessing
    property int exportResult: viewModel.exportResult

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

    function getNumPages(viewType, numItems)
    {
        if (numItems === 0) {
            return 0
        }
        let numItemsPerPage = 1
        if (viewType === 0) {
            numItemsPerPage = numListItemsPerPage
        } else if (viewType === 1) {
            numItemsPerPage = numGridItemsPerPage
        }
        return Math.ceil(numItems / numItemsPerPage)
    }

    function nextPage()
    {
        page = Math.min(page + 1, numPages)
    }

    function prevPage()
    {
        if (numPages === 0) {
            page = 0
        } else {
            page = Math.max(page - 1, 1)
        }
    }

    Component.onCompleted: numPages = getNumPages(view_type, numAlarmItem)

    onView_typeChanged: numPages = getNumPages(view_type, numAlarmItem)

    onAlarmListChanged: {
        let prevPage = page
        let numItem = alarmList.length
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
                    numItemsPerPage = numGridItemsPerPage
                }
                if ((page * numItemsPerPage) > numItem) {
                    page = Math.ceil(numItem / numItemsPerPage)
                }
            }
        }
        numPages = getNumPages(view_type, numItem)
    }

    QtObject {
        id: penSet
        readonly property int groupNo: 25
        readonly property string resetButtonPushColor: resourceManager.dataColor(groupNo, 2)
        readonly property string writeUsbButtonPushColor: resourceManager.dataColor(groupNo, 3)
        readonly property string pageNoColor: resourceManager.dataColor(groupNo, 4)
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
        text: resourceManager.label(50,4, local.language)
        color: "white"
        visible: view_type == 0
    }
    Text {
        x:170
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 24
        font.bold: true
        text: resourceManager.label(50,5,local.language)
        color: "white"
        visible: view_type == 0
    }
    Text {
        x:480
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 24
        font.bold: true
        text: resourceManager.label(50,6,local.language)
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
                font.pixelSize: 24
                text: index+1
            }
            Text{
                width: 260
                height: parent.height
                x:60
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                text: Qt.formatDateTime(modelData.dateTime, "yyyy/MM/dd hh:mm:ss")
            }
            Text{
                property int alarmNo: axis_alarm_msg_type_start_index + modelData.alarmNo * 2 - 1
                width: 420
                height: parent.height
                x:330
                wrapMode: Text.WordWrap
                font.pixelSize: 22
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                text: resourceManager.getMessageStringValue("12", alarmNo, local.language) + "  " + viewModel.getAxisName(modelData.unitNo)
            }
        }
    }
    ListView{
        id: list_view
        property int pg: page
        function setPosition(page)
        {
            if (view_type == 0) {
                positionViewAtIndex((page - 1)*7,ListView.Beginning)
            } else if (view_type == 3) {
                positionViewAtIndex(page - 1,ListView.Beginning)
            }
        }
        x:10
        y:50
        width: 750
        height: (view_type == 0) ? 456 : 460
        clip: true
        spacing: 6
        model: alarmList
        visible: (view_type == 0) || (view_type == 3)
        delegate: {
            if (view_type == 0) {
                listdelegate
            } else if (view_type == 3) {
                listdelegate2
            } else {
                listdelegate
            }
        }
        interactive: false
        onModelChanged: {
            setPosition(pg)
        }

        onPgChanged: {
            setPosition(pg)
        }
    }

    Component{
        id:listdelegate1
        Rectangle{
            width: 370
            height: 225
            border.color: "#999999"
            color: (index%2 == 0) ? "#ffffff": "#cccccc"
            Text{
                width: parent.width-4
                height: 40
                x:4
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 24
                text: index+1
            }
            Text{
                width: parent.width-4
                height: 40
                x:4
                y:50
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 24
                text: Qt.formatDateTime(modelData.dateTime, "yyyy/MM/dd hh:mm:ss")
            }
            Text{
                property int alarmNo: axis_alarm_msg_type_start_index + modelData.alarmNo * 2 - 1
                width:parent.width-4
                height:125
                x:4
                y:100
                wrapMode: Text.WordWrap
                font.pixelSize: 22
                verticalAlignment: Text.AlignTop
                horizontalAlignment: Text.AlignLeft
                text: resourceManager.getMessageStringValue("12", alarmNo, local.language) + "  " + viewModel.getAxisName(modelData.unitNo)
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    view_type = 3
                    view_index = index
                    page = index + 1
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }
    GridView{
        id: grid_view
        property int pg1: page
        function setPosition(page)
        {
            positionViewAtIndex((page - 1)*4,GridView.Beginning)
        }
        x:10
        y:50
        width: 750
        height: 460
        cellWidth: 375
        cellHeight: 230
        clip: true
        visible: view_type == 1
        model: alarmList
        interactive: false
        delegate: listdelegate1
        onModelChanged: {
            setPosition(pg1)
        }
        onPg1Changed: {
            setPosition(pg1)
        }
    }

    Component{
        id:listdelegate2
        Rectangle{
            width: 750
            height: 460
            border.color: "#999999"
            color: (index%2 == 0) ? "#ffffff": "#cccccc"
            Text{
                width: parent.width-4
                height: 40
                x:4
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 24
                text: index+1
            }
            Text{
                width: parent.width-4
                height: 40
                x:4
                y:50
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 24
                text: Qt.formatDateTime(modelData.dateTime, "yyyy/MM/dd hh:mm:ss")
            }
            Text{
                property int alarmNo: axis_alarm_msg_type_start_index + modelData.alarmNo * 2 - 1
                width:parent.width-4
                height:125
                x:4
                y:100
                wrapMode: Text.WordWrap
                font.pixelSize: 22
                verticalAlignment: Text.AlignTop
                horizontalAlignment: Text.AlignLeft
                text: resourceManager.getMessageStringValue("12", alarmNo, local.language) + "  " + viewModel.getAxisName(modelData.unitNo)
            }
        }
    }
// 왼쪽 페이지 넘김 UI
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
            source: prev_page_mouse_area.pressed ? resourceManager.imagePath + "/33_ALM_history/a330_02.png" : resourceManager.imagePath + "/33_ALM_history/a330_00.png"
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

    // 🔢 페이지 번호
    Rectangle {
        width: 60
        height: 30
        anchors.centerIn: parent
        color: "transparent"

        PageText {
            anchors.centerIn: parent
            page: axis_alarm_view.page
            totalPage: axis_alarm_view.numPages
            color: penSet.pageNoColor
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
            source: next_page_mouse_area.pressed ? resourceManager.imagePath + "/33_ALM_history/a330_03.png" : resourceManager.imagePath + "/33_ALM_history/a330_01.png"
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

// 오른쪽 사이드바 UI
Rectangle {
    width: 140
    height: 496
    x: 826
    y: 10
    border.color: "#999999"
    color: "#eaeaea"

    // 리셋 버튼
    Rectangle {
        width: 110
        height: 60
        color: "#cccccc"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 60

        Text {
            id: res
            anchors.centerIn: parent
            font.pixelSize: 23
            color: "#666666"
            text: resourceManager.label(50, 8, local.language)
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
                    operationRecord.recordKeyResetAlarmHistory()
                }
            }
        }
    }

    // USB 저장 버튼
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
            text: resourceManager.label(50, 7, local.language)
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
                    phyKeyManager.ctrlBuzzer()
                    export_dialog.open()
                }
            }
        }
    }

    // 구분선
    Rectangle {
        width: 120
        height: 1
        anchors.horizontalCenter: parent.horizontalCenter
        y: 240
        color: "white"
    }

    // 모드 전환 버튼
    Rectangle {
        width: 48
        height: 48
        anchors.horizontalCenter: parent.horizontalCenter
        y: 280
        color: "transparent"

        Image {
            anchors.centerIn: parent
            source: (view_type === 0)
                    ? resourceManager.imagePath + "/33_ALM_history/a330_06.png"
                    : resourceManager.imagePath + "/33_ALM_history/a330_07.png"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                let topItemIndex
                let lastPage = numPages
                if (view_type === 0) {
                    view_type = 1
                    if (alarmList.length === 0) {
                        page = 0
                    } else {
                        if (page === lastPage) {
                            topItemIndex = Math.max(alarmList.length - numListItemsPerPage - 1, 0)
                        } else {
                            topItemIndex = (numListItemsPerPage * (page - 1))
                        }
                        page = (topItemIndex / numGridItemsPerPage) + 1
                    }
                } else if (view_type === 1) {
                    view_type = 0
                    if (alarmList.length === 0) {
                        page = 0
                    } else {
                        if (page === lastPage) {
                            topItemIndex = Math.max(alarmList.length - numGridItemsPerPage - 1, 0)
                        } else {
                            topItemIndex = (numGridItemsPerPage * (page - 1))
                        }
                        page = (topItemIndex / numListItemsPerPage) + 1
                    }
                } else if (view_type === 3) {
                    view_type = 0
                    topItemIndex = page - 1
                    page = (topItemIndex / numListItemsPerPage) + 1
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
            msg: resourceManager.label(50, 17, local.language)
            msgColor: resourceManager.rgb(50, 17)
            language: local.language
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
            msg: resourceManager.label(50, 185, local.language)
            msgColor: resourceManager.rgb(50, 185)
            language: local.language
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
            msg: resourceManager.label(50, 88, local.language)
            msgColor: resourceManager.rgb(50, 88)
            language: local.language
            okButtonVisible: false
            onCancelClicked: close()
        }
    }

        Connections {
        target: phyKeyManager
        enabled: axis_alarm_view.pageActive
        onJogScrollUpChanged: prevPage()
        onJogScrollDownChanged: nextPage()
    }
}
