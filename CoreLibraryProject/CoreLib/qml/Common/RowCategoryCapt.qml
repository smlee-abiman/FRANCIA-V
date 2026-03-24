import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: rowCategoryCapt
    property alias rowCategoryCapt: rowCategoryCapt
    property alias textBody: textBody
    Layout.minimumWidth: 613    // Set from upper element
    width: Layout.minimumWidth
    height: 28
    color: "#393cec"
    gradient: Gradient {
        GradientStop {
            position: 1
            color: "#393cec"
        }

        GradientStop {
            position: 0.51
            color: "#3584a7"
        }

        GradientStop {
            position: 0
            color: "#30d2be"
        }
    }
    opacity: 0.8
    clip: true
    Text {
        id: textBody
        color: "#fbfbfb"
        text: "System Input"    // Set upper element
        clip: false
        anchors.fill: parent
        topPadding: 4
        bottomPadding: 4
        leftPadding: 4
        font.pointSize: 14
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        //elide: Text.ElideMiddle
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    }
}
