import QtQuick 2.12
import QtQuick.Controls 2.5

ComboBox {
    id: comboBoxBody
    width: 200
    clip: false
    font.pointSize: 12
    wheelEnabled: true
    anchors.fill: parent
    topPadding: 1
    bottomPadding: 1
    leftPadding: 1
    rightPadding: 1
    //model: dbgManager.ioCategoryCapt;    // set upper module

    contentItem: Text {
        id: comboBoxCategoryContext
        text: comboBoxBody.displayText
        leftPadding: 4
        font: comboBoxBody.font
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        //focus: true
    }
    background: Rectangle {
        border.width: 2
        border.color: "skyblue"
        //color: "palegreen"
        color: "lightcyan"
    }

    popup: ComboPopup {
        id: comboPopupComp
        y: comboBoxCategoryContext.height
        width: comboBoxCategory.width
        //height: contentHeight / 2     // Test
        leftPadding: comboBoxCategoryContext.leftPadding
    }

    onCurrentIndexChanged: {
/*
        if (-1 == comboBoxCategory.currentIndex) return;
        if (dbgManager.ioCategoryReadOnly[comboBoxCategory.currentIndex]) {
            rectReadOnly.visible = true;
            iOSwitchForm.gridLayoutIo.checkable = false;
        }
        else {
            rectReadOnly.visible = false;
            iOSwitchForm.gridLayoutIo.checkable = true;
        }
        dbgManager.ioCategoryId = comboBoxBody.currentIndex;
        // <Note> Call setCaptWordAdr() after updating dbgManager.ioCategoryId.
        commonFunc.setCaptWordAdr(iOSwitchForm.repeaterAdr, dbgManager.ioCategoryOffset[comboBoxCategory.currentIndex]);
*/
    }


    focus: false    // ignore ?
    Keys.onPressed: {
        //console.log("comboEx key press:" + event.key);
    }
}
