import QtQuick 2.12

Item {
    id: _root
    width: 21
    height: 21
    //property bool lightOn: false

    Rectangle {
        id: rectLed
        width: 21
        height: 21
        color: "#020202"
        radius: 16
        border.width: 0
        transformOrigin: Item.Center
        rotation: -40
        gradient: Gradient {
            GradientStop {
                id: gradientStop1
                position: 0
                color: "#d4dad8"
            }
            GradientStop {
                id: gradientStop
                position: 1
                color: "#020202"
            }
        }
        anchors.fill: parent
    }
    states: [
        State {
            name: "lightOnStatus"
            //when: _root.lightOn
            when: modelData.boolValue

            PropertyChanges {
                target: gradientStop
                color: "#54bdfc"
            }
            PropertyChanges {
                target: gradientStop1
                color: "#39fea3"
            }

            PropertyChanges {
                target: rectLed
                color: "#05cf73"
            }
        }
    ]
}

/*##^##
Designer {
    D{i:1;anchors_height:200;anchors_width:200;anchors_x:0;anchors_y:0}
}
##^##*/
