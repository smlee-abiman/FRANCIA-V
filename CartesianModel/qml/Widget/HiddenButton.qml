import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQml 2.12

Item {
    id:root
    property color rectColor:"transparent"
    property int rectWidth:65
    property int rectHeight:56
    property bool isEnablePressingSignal:false
    property int timerCount:0
    property int pressMonitorTime:0

    signal pressed()
    signal released()
    signal clicked()
    signal pressing()

    Timer {
        id: pressTimer
        interval: 1000
        running: false
        repeat: false
        onTriggered: {
            root.timerCount++
            if (root.isEnablePressingSignal === true && root.timerCount >= root.pressMonitorTime){
                root.pressing()
                pressTimer.repeat = false
                pressTimer.running = false
                root.timerCount = 0
            }
        }
    }
    Rectangle {
        id:rectArea
        width:root.rectWidth
        height:root.rectHeight
        color:root.rectColor
        MouseArea{
            anchors.fill: parent
            onPressed: {
                root.pressed()
                pressTimer.repeat = true
                pressTimer.running = true
            }
            onReleased: {
                pressTimer.repeat = false
                pressTimer.repeat = false
                root.timerCount = 0
                root.released()
            }
            onClicked: {
                root.clicked()
            }
        }
    }
}