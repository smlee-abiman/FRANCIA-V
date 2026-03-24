import QtQuick 2.0

Item {
    anchors.fill: parent

    signal componentDestructed(string source)                                // dummy. not use
    signal folderSelected(int index)                                         // dummy. not use
    signal folderOpen(string folderPath, int depth)                          // dummy. not use
    signal fileSelected(int index)                                           // dummy. not use
    signal childFolderOpened(int index)                                      // dummy. not use
    signal showColorPattern()                                                // dummy. not use
    signal closeColorPattern()
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
    signal exitTouch()                                                       // dummy. not use
    signal backLightCountDown()                                              // dummy. not use
    signal backLightCountUp()                                                // dummy. not use

    Rectangle {
        id: x0y0
        width: 113
        height: 256
        color: "#008000"
    }

    Rectangle {
        id: x1y0
        width: 113
        height: 256
        anchors.top: x0y0.top
        anchors.left: x0y0.right
        color: "#008080"
    }

    Rectangle {
        id: x2y0
        width: 113
        height: 256
        anchors.top: x1y0.top
        anchors.left: x1y0.right
        color: "#0080FF"
    }

    Rectangle {
        id: x3y0
        width: 113
        height: 256
        anchors.top: x2y0.top
        anchors.left: x2y0.right
        color: "#000000"
    }

    Rectangle {
        id: x4y0
        width: 113
        height: 256
        anchors.top: x3y0.top
        anchors.left: x3y0.right
        color: "#000080"
    }

    Rectangle {
        id: x5y0
        width: 113
        height: 256
        anchors.top: x4y0.top
        anchors.left: x4y0.right
        color: "#0000FF"
    }

    Rectangle {
        id: x6y0
        width: 113
        height: 256
        anchors.top: x5y0.top
        anchors.left: x5y0.right
        color: "#00FF00"
    }

    Rectangle {
        id: x7y0
        width: 113
        height: 256
        anchors.top: x6y0.top
        anchors.left: x6y0.right
        color: "#00FF80"
    }

    Rectangle {
        id: x8y0
        width: 120
        height: 256
        anchors.top: x7y0.top
        anchors.left: x7y0.right
        color: "#00FFFF"
    }

    Rectangle {
        id: x0y1
        width: 113
        height: 256
        anchors.top: x0y0.bottom
        anchors.left: x0y0.left
        color: "#FF8000"
    }

    Rectangle {
        id: x1y1
        width: 113
        height: 256
        anchors.top: x0y1.top
        anchors.left: x0y1.right
        color: "#FF8080"
    }

    Rectangle {
        id: x2y1
        width: 113
        height: 256
        anchors.top: x1y1.top
        anchors.left: x1y1.right
        color: "#FF80FF"
    }

    Rectangle {
        id: x3y1
        width: 113
        height: 256
        anchors.top: x2y1.top
        anchors.left: x2y1.right
        color: "#FFFF00"
    }

    Rectangle {
        id: x4y1
        width: 113
        height: 256
        anchors.top: x3y1.top
        anchors.left: x3y1.right
        color: "#FFFF80"
    }

    Rectangle {
        id: x5y1
        width: 113
        height: 256
        anchors.top: x4y1.top
        anchors.left: x4y1.right
        color: "#FFFFFF"
    }

    Rectangle {
        id: x6y1
        width: 113
        height: 256
        anchors.top: x5y1.top
        anchors.left: x5y1.right
        color: "#FF0000"
    }

    Rectangle {
        id: x7y1
        width: 113
        height: 256
        anchors.top: x6y1.top
        anchors.left: x6y1.right
        color: "#FF0080"
    }

    Rectangle {
        id: x8y1
        width: 120
        height: 256
        anchors.top: x7y1.top
        anchors.left: x7y1.right
        color: "#FF00FF"
    }

    Rectangle {
        id: x0y2
        width: 113
        height: 256
        anchors.top: x0y1.bottom
        anchors.left: x0y1.left
        color: "#808000"
    }

    Rectangle {
        id: x1y2
        width: 113
        height: 256
        anchors.top: x0y2.top
        anchors.left: x0y2.right
        color: "#808080"
    }

    Rectangle {
        id: x2y2
        width: 113
        height: 256
        anchors.top: x1y2.top
        anchors.left: x1y2.right
        color: "#8080FF"
    }

    Rectangle {
        id: x3y2
        width: 113
        height: 256
        anchors.top: x2y2.top
        anchors.left: x2y2.right
        color: "#800000"
    }

    Rectangle {
        id: x4y2
        width: 113
        height: 256
        anchors.top: x3y2.top
        anchors.left: x3y2.right
        color: "#800080"
    }

    Rectangle {
        id: x5y2
        width: 113
        height: 256
        anchors.top: x4y2.top
        anchors.left: x4y2.right
        color: "#8000FF"
    }

    Rectangle {
        id: x6y2
        width: 113
        height: 256
        anchors.top: x5y2.top
        anchors.left: x5y2.right
        color: "#80FF00"
    }

    Rectangle {
        id: x7y2
        width: 113
        height: 256
        anchors.top: x6y2.top
        anchors.left: x6y2.right
        color: "#80FF80"
    }

    Rectangle {
        id: x8y2
        width: 120
        height: 256
        anchors.top: x7y2.top
        anchors.left: x7y2.right
        color: "#80FFFF"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            closeColorPattern()
        }
    }
}
