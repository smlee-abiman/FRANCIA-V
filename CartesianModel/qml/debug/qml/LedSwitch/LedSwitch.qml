import QtQuick 2.12
import QtQuick.Controls 2.12

LedSwitchForm {
    Component.onCompleted: {
        if (true == _checkable) {
            switchBody.checked = ("0" === modelData.value) ? false : true
        }
    }

    switchBody.onCheckedChanged: {
        modelData.value = (true === switchBody.checked) ? "1" : "0"
    }

    switchPopupTrig.onContainsMouseChanged: {
        if (true === usePopupTips) {
            if (switchPopupTrig.containsMouse) {
                switchPopup.popupMsg.text = modelData.label
                switchPopup.parent = elemSwitch
                switchPopup.y = -(switchPopup.height / 2 - 3)
                switchPopup.x = (elemSwitch.x <= (gridLayoutIo.width / 2))
                    ? (elemSwitch.width - 3)
                    : -(switchPopup.width - 3)
                switchPopup.open()
            }
            else {
                switchPopup.close()
            }
        }
    }
}
