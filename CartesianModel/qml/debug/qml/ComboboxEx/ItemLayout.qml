// ItemLayout.qml
import QtQuick 2.12

Rectangle {
    id: _root
    //property alias itemLayout: _root
    height: 24
    anchors.left: parent.left
    anchors.right: parent.right
    color: "transparent"    // need
    // 枠線
    //border.color: "#dddddd"
    //border.width: 1
    // 実際に表示する部分へのエイリアス
    property alias text: _layoutText.text
    property alias textBold: _layoutText.font.bold
    // 上位への通知用のシグナル
    signal clicked(string itemText)     // いらんかも
    // 表示アイテムを配置
    Text {
        id: _layoutText
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        text: ""
        font.bold: false
        //font.pixelSize: comboBoxCategory.font.pixelSize
        font.pixelSize: comboBoxBody.font.pixelSize
    }

    // クリック判定
    MouseArea{
        anchors.fill: parent
        onClicked: {
            // クリックシグナルを上位へ
            _root.clicked(_layoutText.text);    // いらんかも
            clickListItem(_root);
        }
    }
}
