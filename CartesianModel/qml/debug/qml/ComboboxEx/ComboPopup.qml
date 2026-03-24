// ComboPopup.qml
import QtQuick 2.12
import QtQuick.Controls 2.5

Popup {
    id: comboPopup
    topPadding: 3
    bottomPadding: 3
    rightPadding: 4
    focus: true     // nessesary
    clip: true      // nessesary
    opacity: 0.95
    background: Rectangle {
        border.width: 2
        border.color: "skyblue"
        color: "lightcyan"
    }

    onOpened: {
        listViewCategory.currentIndex = comboBoxBody.currentIndex;
    }

    contentItem: ListView {
        id: listViewCategory
        implicitHeight: contentHeight
        //currentIndex: comboBoxCategory.highlightedIndex
        currentIndex: comboBoxBody.highlightedIndex
        // comboBoxBody.delegateModel を参照すると、既定のPopup，LiteViewのLookと
        // Behaivorとなるので、comboBoxBody.model を参照すること
        //model: comboBoxBody.popup.visible ? comboBoxCategory.delegateModel : null
        model: comboBoxBody.popup.visible ? comboBoxBody.model : null
        delegate: ItemLayout {
            text: modelData
            textBold: comboBoxBody.currentIndex === index ? true : false
        }

        highlightFollowsCurrentItem: false      // nessesary
        highlight: Rectangle {
            color: "lightsteelblue"; opacity: 1.00
            radius: 3
            width: listViewCategory.currentItem != null ? listViewCategory.currentItem.width : 0
            height: listViewCategory.currentItem != null ? listViewCategory.currentItem.height : 0
            y: listViewCategory.currentItem != null ? listViewCategory.currentItem.y : 0
            Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
        }

        onCurrentIndexChanged: {
            if (-1 == listViewCategory.currentIndex) return;
            //console.log("onCurrentIndexChanged: " + listViewCategory.currentIndex)

            //　ポップアップ直後に、ハイライトボックスが移動するアニメーションが実行される時に、
            // CurrentIndexChangeedが発生するので、ここでComboboxのCurrentIndex
            // との同期を実行してはならない。
            /****************************************************************
            if (comboBoxCategory.currentIndex != listViewCategory.currentIndex) {
                comboBoxCategory.currentIndex = listViewCategory.currentIndex;
            }
            ****************************************************************/
        }

        //onCurrentItemChanged: {
        //    console.log("onCurrentItemChanged: " + listViewCategory.currentIndex)
        //}

        focus: true     // nessesary
        Keys.onPressed: {
            switch (event.key) {
            case Qt.Key_Space:
                // enterキーを自身に遅れないか？;
                event.accepted = true;
                break;
            case Qt.Key_Enter:
            case Qt.Key_Return:
                fixedSelectItem();
                event.accepted = true;
                break;
            default:
                break;
            }
        }
    }

    function clickListItem(object)
    {
        if (-1 == listViewCategory.currentIndex) return;
        var idxNew = listViewCategory.indexAt(object.x, object.y);
        //listViewCategory.currentIndex = idxNew;
        comboPopup.close();
        comboBoxBody.currentIndex = idxNew;
    }

    function fixedSelectItem()
    {
        if (-1 == listViewCategory.currentIndex) return;
        var idxNew = listViewCategory.currentIndex;
        comboBoxBody.currentIndex = idxNew;
        comboPopup.close();
    }

    function sleep(waitMsec) {
      var startMsec = new Date();

      // 指定ミリ秒間だけループさせる（CPUは常にビジー状態）
      while (new Date() - startMsec < waitMsec);
    }
}
