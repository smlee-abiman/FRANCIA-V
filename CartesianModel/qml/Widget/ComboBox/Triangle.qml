import QtQuick 2.12

Canvas {
    id: canvasId
    property color triangleColor: "black"
    width: parent.width
    height: parent.height
    contextType: "2d"

    onPaint: {
        //context.lineWidth = 0
        context.strokeStyle = "#00000000"
        context.fillStyle = triangleColor
        context.beginPath();
        context.moveTo(0, 0)
        context.lineTo(canvasId.width/2, canvasId.height);
        context.lineTo(canvasId.width, 0);
        context.closePath();
        context.fill()
        context.stroke();
    }
    Connections{
        target: canvasId
        onTriangleColorChanged: {
            requestPaint()
        }
    }
}
