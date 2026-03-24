import QtQuick 2.0

Item {
    id: touchPage
    anchors.fill: parent

    signal componentDestructed(string source)
    signal folderSelected(int index)                                         // dummy. not use
    signal folderOpen(string folderPath, int depth)                          // dummy. not use
    signal fileSelected(int index)                                           // dummy. not use
    signal childFolderOpened(int index)                                      // dummy. not use
    signal showColorPattern()                                                // dummy. not use
    signal closeColorPattern()                                               // dummy. not use
    signal inputTestExecute()                                                // dummy. not use
    signal outputTestExecute()                                               // dummy. not use
    signal retClicked(string ipAddr, string subnetMask)                      // dummy. not use
    signal calibrationClicked()                                              // dummy. not use
    signal testAndSetClicked()                                               // dummy. not use
    signal executeClicked(string currentPath)                                // dummy. not use
    signal directionClicked()                                                // dummy. not use
    signal createFolderClicked()                                             // dummy. not use
    signal eraceFolderClicked()                                              // dummy. not use
    signal displaySelectClicked()                                            // dummy. not use
    signal touchClicked()                                                    // dummy. not use
    signal exitTouch()
    signal backLightCountDown()                                              // dummy. not use
    signal backLightCountUp()                                                // dummy. not use

    Component.onDestruction: {
        componentDestructed("Touch.qml")
    }

    Rectangle {
        id: point_rect
        width: 10
        height: 30
        visible: false

        Rectangle{
            anchors.verticalCenter: point_rect.verticalCenter
            width: 30
            height: 4

            color: "black"
        }

        Rectangle{
            anchors.horizontalCenter: point_rect.horizontalCenter
            width: 4
            height: 30

            color: "black"
        }
    }

    Rectangle {
        id: rect
        x: 10
        y: 10
        width: 300
        height: 160
        border.color: "black"
        visible: false

        Text {
            id: xText
            anchors.verticalCenter: rect.verticalCenter
            anchors.horizontalCenter: rect.horizontalCenter
            anchors.horizontalCenterOffset: -(rect.width/4)
            font.pixelSize: 32

            text: "X:"
        }

        Text {
            id: yText
            anchors.verticalCenter: rect.verticalCenter
            anchors.horizontalCenter: rect.horizontalCenter
            anchors.horizontalCenterOffset: (rect.width/4)
            font.pixelSize: 32

            text: "Y:"
        }
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent

        onPressed: {
            updatePosition();

            rect.visible = true;
            point_rect.visible = true;
        }

        onPositionChanged: {
            updatePosition();
        }

        onReleased: {
            rect.visible = false;
            point_rect.visible = false;
        }
    }

    function updatePosition(){
        var touch_x = mouseArea.mouseX;
        var touch_y = mouseArea.mouseY;

        var rect_x = touch_x;
        var rect_y = touch_y + 30;

        if(touchPage.width / 2 < touch_x){
            rect_x = rect_x - rect.width;
        }

        if(touchPage.height / 2 < touch_y){
            rect_y = rect_y - rect.height - 60;
        }

        rect.x = rect_x;
        rect.y = rect_y;

        point_rect.x = touch_x - 15;
        point_rect.y = touch_y - 15;

        xText.text = "X:" + touch_x.toString();
        yText.text = "Y:" + touch_y.toString();
    }

    Connections
    {
        target:touchManager
        onIsPressedKey01Changed:{
            if(touchManager.isPressedKey01 == true){
                exitTouch();
            }
        }
    }
}
