import QtQuick 2.12
import QtQuick.Controls 2.5

BitTypePageForm {
    Component.onCompleted: {
    }

    comboBoxCategory.onCurrentIndexChanged: {
        if (-1 == comboBoxCategory.currentIndex) return;
        if (dbgManager.ioCategoryReadOnly[comboBoxCategory.currentIndex]) {
            rectReadOnly.visible = true;
            iOSwitchForm.gridLayoutIo.checkable = false;
        }
        else {
            rectReadOnly.visible = false;
            iOSwitchForm.gridLayoutIo.checkable = true;
        }
        dbgManager.ioCategoryId = comboBoxCategory.currentIndex;
        // <Note> Call setCaptWordAdr() after updating dbgManager.ioCategoryId.
        commonFunc.setCaptWordAdr(iOSwitchForm.repeaterAdr, dbgManager.ioCategoryOffset[comboBoxCategory.currentIndex]);
    }
}
