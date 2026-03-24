import QtQuick 2.0
import "../ComboboxEx"

Item {
    width: 630
    height: 450

    Item {
        id: rowHeader
        width: 608
        height: 27
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 4

        Text {
            id: element
            text: qsTr("カテゴリ： ")
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.topMargin: 2
        }

        ComboboxEx {
            id: comboBoxCategory
            anchors.left: element.right
            anchors.leftMargin: 61
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            clip: false
            hoverEnabled: true
            // model: (dbgManager !== null) ? dbgManager.pointTypeCategoryGroupCapt : null;
            model: ["グループ1 ヘッダー表示1", "グループ1 ヘッダー表示2", "グループ1 ポイントデータ表示", "グループ2 ヘッダー表示1", "グループ2 ヘッダー表示2", "グループ2 ポイントデータ表示", "グループ3 ヘッダー表示1", "グループ3 ヘッダー表示2", "グループ3 ポイントデータ表示", "グループ4 ヘッダー表示1", "グループ4 ヘッダー表示2", "グループ4 ポイントデータ表示"]
            onCurrentIndexChanged: {
                var category = currentIndex % 3
                switch(category) {
                case 0:
                    // Display header 01
                    displayArea.source = "CategoryHeader01.qml"
                    break

                case 1:
                    // Display header 02
                    displayArea.source = "CategoryHeader02.qml"
                    break

                case 2:
                    // Display point data
                    displayArea.source = "CategoryPointData.qml"
                    break
                }
            }
        }
    }

    Loader {
        id: displayArea
        anchors.top: rowHeader.bottom
        anchors.topMargin: 6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
    }
}
