import QtQml 2.12
import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Column {
    id: root
    spacing: 10

    property string title: "title"
    property int keyPressInterval: 150

    signal prevClicked()
    signal enterClicked()
    signal nextClicked()

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        text: root.title
    }

    Column {
        id: buttonColum
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 13

        // prev button.
        Shape {
            id: keyPrev
            width: 44; height: 35
            anchors.horizontalCenter: parent.horizontalCenter
            antialiasing: true
            containsMode: Shape.FillContains
            ShapePath {
                id: keyPrevPath
                strokeWidth: 2
                strokeColor: "black"
                fillColor: keyPrevMouse.pressed ? "limegreen" : "lightgreen"
                startX: keyPrev.width / 2; startY: 0
                PathLine { x: keyPrev.width;      y: keyPrev.height }
                PathLine { x: 0;                  y: keyPrev.height }
                PathLine { x: keyPrevPath.startX; y: keyPrevPath.startY }
            }
            MouseArea {
                id: keyPrevMouse
                anchors.fill: parent
                // onClicked: prevClicked()
            }
            Timer {
                interval: keyPressInterval
                repeat: true
                triggeredOnStart: true
                running: keyPrevMouse.pressed
                onTriggered: prevClicked()
            }
        }
        // enter button.
        Rectangle {
            width: 44; height: width
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: 2
            border.color: "black"
            Shape {
                id: enterShape
                anchors.fill: parent
                property real r: 9
                ShapePath {
                    fillColor: enterMouse.pressed ? "darkorange" : "orange"
                    startX: enterShape.width  / 2 - enterShape.r
                    startY: enterShape.height / 2 - enterShape.r
                    PathArc {
                        x: enterShape.width  / 2 + enterShape.r
                        y: enterShape.height / 2 + enterShape.r
                        radiusX: enterShape.r; radiusY: radiusX
                        useLargeArc: true
                    }
                }
                ShapePath {
                    fillColor: enterMouse.pressed ? "darkorange" : "orange"
                    startX: enterShape.width  / 2 + enterShape.r
                    startY: enterShape.height / 2 + enterShape.r
                    PathArc {
                        x: enterShape.width  / 2 - enterShape.r
                        y: enterShape.height / 2 - enterShape.r
                        radiusX: enterShape.r; radiusY: radiusX
                        useLargeArc: true
                    }
                }
            }
            MouseArea {
                id: enterMouse
                anchors.fill: parent
                onClicked: enterClicked()
            }
        }
        // next  button.
        Shape {
            id: keyNext
            width: 44; height: 35
            antialiasing: true
            containsMode: Shape.FillContains
            ShapePath {
                id: keyNextPath
                strokeWidth: 2
                strokeColor: "black"
                fillColor: keyNextMouse.pressed ? "limegreen" : "lightgreen"
                startX: keyNext.width; startY: 0
                PathLine { x: keyNext.width / 2; y: keyNext.height }
                PathLine { x: 0;                  y: 0 }
                PathLine { x: keyNext.width;      y: 0 }
            }
            MouseArea {
                id: keyNextMouse
                anchors.fill: parent
                // onClicked: nextClicked()
            }
            Timer {
                interval: keyPressInterval
                repeat: true
                triggeredOnStart: true
                running: keyNextMouse.pressed
                onTriggered: nextClicked()
            }
        }
    }
}
