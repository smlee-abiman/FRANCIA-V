import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: numberCell
    property alias numberCell: numberCell
    property alias numberEdit: numberEdit
    property alias text: numberEdit.text
    property alias readOnly: numberEdit.readOnly
    property int maxValue: 0xFFFF      // set from upper element
    property int minValue: 0        // set from upper element
    Layout.minimumWidth: (numberEdit.font.pixelSize / 2 + numberEdit.font.wordSpacing) * numberEdit.maximumLength
                                    // Different from expectations. set from upper element.
    width: Layout.minimumWidth
    //height: 28
    height: (numberEdit.font.pixelSize + 3 * 2)
    border.width: 1
    color: "transparent"
    clip: true

    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true      // necessary for ToolTip
        TextInput {
            id: numberEdit
            text: modelData.value
            //focus: true
            selectByMouse: true
            anchors.right: parent.right
            anchors.rightMargin: 3
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 3
            anchors.top: parent.top     // Set from upper element
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 14
            cursorVisible: activeFocus
            maximumLength: (numberCell.minValue.toString().length <= numberCell.maxValue.toString().length)
                           ? numberCell.maxValue.toString().length : numberCell.minValue.toString().length
            //xx inputMask: "000"                  // Cannot be used because it is not right-aligned
            //inputMethodComposing: true           // Disappear

            //inputMethodHints: Qt.ImhDigitsOnly          // invalid
            //inputMethodHints: Qt.ImhPreferNumbers       // invalid

            ToolTip.visible: ma.containsMouse
            ToolTip.delay: 200
            ToolTip.timeout: 3000
            ToolTip.text: modelData.label

            Keys.onPressed: {
                event.accepted = true;
                switch (event.key) {
                case Qt.Key_Backspace:
                case Qt.Key_Delete:
                case Qt.Key_Left:
                case Qt.Key_Right:
                case Qt.Key_Home:
                case Qt.Key_End:
                    event.accepted = false;     // through to numberEdit
                    break;
                case Qt.Key_Minus:
                    if ((numberCell.minValue < 0)
                     && (false === numberEdit.text.startsWith("-"))
                     && (0 == numberEdit.cursorPosition || text.length == selectedText.length)
                    ) {
                        event.accepted = false; // through to numberEdit
                    }

                    break;
                default:
                    event.accepted = !(Qt.Key_0 <= event.key && event.key <= Qt.Key_9);
                    break;
                }
            }

            onTextChanged: {
                var iVal = parseInt(text, 10);
                if (numberCell.maxValue < iVal) {
                    text = numberCell.maxValue;
                }
                else if (iVal < numberCell.minValue) {
                    text = numberCell.minValue
                }
                if (text !== modelData.value) {
                    modelData.value = text;
                }
            }

            onActiveFocusChanged: {
                if ((0 == text.length) || (0 == text.length && "-" == text)) {
                    text = 0;
                }
            }
        }
    }

}

