import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0

Item {
    id: root
    width: 1024
    height: 768

    property bool pageActive: true

    QtObject {
        id: local
        readonly property bool jogDialSwOpe: (commonViewModel.topPopupDialog === Enums.PopupDialogStatusNone)
        property int numOfTabs: 0
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdFunctionControllerSetSystem
        property int language: 0
    }

    QtObject {
        id: penSet
        readonly property int groupNo: 17
        readonly property var selectedTabTextColorRGB: resourceManager.dataColor(groupNo, 1)
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    Component.onCompleted: {
        tab_bar.setCurrentIndex(systemTabButton.TabBar.index)
        tab_stack.currentIndex = 0
    }

    onPageActiveChanged: {
        let tabIndex = 0
        if (pageActive) {
            let numOfValidTabs = 0

            // Process from the button on the right.
            let visibleNetwork = local.userSettingView.getScreenSettings(local.dp1, 2).visibles[local.userLevel]
            if (visibleNetwork) {
                networkTabButton.visible = true
                tab_bar.moveItem(networkTabButton.TabBar.index, 0)
                tabIndex = networkTabButton.TabBar.index
                numOfValidTabs++
            }

            let visibleUser = local.userSettingView.getScreenSettings(local.dp1, 1).visibles[local.userLevel]
            if (visibleUser) {
                userTabButton.visible = true
                tab_bar.moveItem(userTabButton.TabBar.index, 0)
                tabIndex = userTabButton.TabBar.index
                numOfValidTabs++
            }

            let visibleSystem = local.userSettingView.getScreenSettings(local.dp1, 0).visibles[local.userLevel]
            if (visibleSystem) {
                systemTabButton.visible = true
                tab_bar.moveItem(systemTabButton.TabBar.index, 0)
                tabIndex = systemTabButton.TabBar.index
                numOfValidTabs++
            }

            local.numOfTabs = numOfValidTabs
        } else {
            // hide tab.
            networkTabButton.visible = false
            userTabButton.visible = false
            systemTabButton.visible = false
            // restore the order of tabs.
            tab_bar.moveItem(networkTabButton.TabBar.index, 0)
            tab_bar.moveItem(userTabButton.TabBar.index, 0)
            tab_bar.moveItem(systemTabButton.TabBar.index, 0)
        }
        tab_bar.setCurrentIndex(tabIndex)
    }

    // =========================================================
    // ✅ 여기부터: 사용자가 넣은 "배경" + "흰색 컨텐츠 영역" 구조
    //     그리고 그 안에 기존 UI(TabBar/StackLayout)를 전부 넣음
    // =========================================================

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "#EAEAEA"

        // 센터 내용표시 배경(흰 박스)
        Rectangle {
            id: contentArea
            width: 1000
            height: 655
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            color: "#FFFFFF"
            border.color: "#cccccc"
            // radius: 6

            // -----------------------------
            // ✅ 1) 탭 바 (contentArea 안으로)
            // -----------------------------
            TabBar {
                id: tab_bar
                width: 491
                height: 78
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 50
                anchors.leftMargin: 12
                spacing: 16

                background: Rectangle { color: "transparent" }

                onCurrentIndexChanged: {
                    switch (currentIndex) {
                    case systemTabButton.TabBar.index:  tab_stack.currentIndex = 0; break
                    case userTabButton.TabBar.index:    tab_stack.currentIndex = 1; break
                    case networkTabButton.TabBar.index: tab_stack.currentIndex = 2; break
                    default:                            tab_stack.currentIndex = 0; break
                    }
                }

                TabButton {
                    id: systemTabButton
                    width: 150
                    height: 60
                    background: Rectangle {
                        anchors.fill: parent
                        color: (tab_bar.currentIndex === systemTabButton.TabBar.index) ? "#cc3333" : "#cccccc"
                        border.color: "#999999"
                    }
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.pixelSize: systemTabButton.pressed ? 24 : 23
                        text: resourceManager.label(42, 74, local.language)
                        color: (tab_bar.currentIndex === systemTabButton.TabBar.index) ? "#ffffff" : "black"
                    }
                    onClicked: {
                        tab_bar.setCurrentIndex(systemTabButton.TabBar.index)
                        tab_stack.currentIndex = 0
                        phyKeyManager.ctrlBuzzer()
                    }
                }

                TabButton {
                    id: userTabButton
                    width: 150
                    height: 60
                    background: Rectangle {
                        anchors.fill: parent
                        color: (tab_bar.currentIndex === userTabButton.TabBar.index) ? "#cc3333" : "#cccccc"
                        border.color: "#999999"
                    }
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.pixelSize: userTabButton.pressed ? 24 : 23
                        text: resourceManager.label(42, 75, local.language)
                        color: (tab_bar.currentIndex === userTabButton.TabBar.index) ? "#ffffff" : "black"
                    }
                    onClicked: {
                        tab_bar.setCurrentIndex(userTabButton.TabBar.index)
                        tab_stack.currentIndex = 1
                        phyKeyManager.ctrlBuzzer()
                    }
                }

                TabButton {
                    id: networkTabButton
                    width: 150
                    height: 60
                    background: Rectangle {
                        anchors.fill: parent
                        color: (tab_bar.currentIndex === networkTabButton.TabBar.index) ? "#cc3333" : "#cccccc"
                        border.color: "#999999"
                    }
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.pixelSize: networkTabButton.pressed ? 24 : 23
                        text: resourceManager.label(42, 76, local.language)
                        color: (tab_bar.currentIndex === networkTabButton.TabBar.index) ? "#ffffff" : "black"
                    }
                    onClicked: {
                        tab_bar.setCurrentIndex(networkTabButton.TabBar.index)
                        tab_stack.currentIndex = 2
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }

            // -----------------------------
            // ✅ 2) 페이지 스택 (contentArea 안으로)
            //    TabBar 아래 영역을 자동으로 채움
            // -----------------------------
            StackLayout {
                id: tab_stack
                anchors.top: tab_bar.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: 4
                anchors.rightMargin: 5
                anchors.bottomMargin: 6

                currentIndex: 0

                onCurrentIndexChanged: {
                    switch (currentIndex) {
                    case 1:
                        userSetLoader.active = true
                        break
                    case 2:
                        internetSetLoader.active = true
                        break
                    case 0:
                    default:
                        break
                    }
                }

                System_set {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    property bool isActive: root.pageActive && (tab_stack.currentIndex === 0)
                    pageActive: isActive
                }

                Loader {
                    id: userSetLoader
                    source: "qrc:/qml/Functions/ControllerSettings/User_set.qml"
                    active: true

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    property bool isActive: root.pageActive && (tab_stack.currentIndex === 1)

                    onLoaded: {
                        // 로드된 페이지가 Loader 영역을 꽉 채우도록
                        item.anchors.fill = parent
                        item.pageActive = isActive
                    }
                    onIsActiveChanged: {
                        if (status === Loader.Ready) {
                            item.pageActive = isActive
                        }
                    }
                }

                Loader {
                    id: internetSetLoader
                    source: "qrc:/qml/Functions/ControllerSettings/Internet_set.qml"
                    active: true

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    property bool isActive: root.pageActive && (tab_stack.currentIndex === 2)

                    onLoaded: {
                        item.anchors.fill = parent
                        item.pageActive = isActive
                    }
                    onIsActiveChanged: {
                        if (status === Loader.Ready) {
                            item.pageActive = isActive
                        }
                    }
                }
            }
        }
    }

    /***********************************************************************/
    // Connections { ... }  // 필요하면 그대로 사용
}
