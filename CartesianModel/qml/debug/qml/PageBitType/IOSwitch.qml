import QtQuick 2.1
import QtQuick.Controls 2.12

IOSwitchForm {
    Component.onCompleted: {
        rowBackTile.anchors.TopAnchor = gridLayoutIo.anchors.TopAnchor
        rowBackTile.anchors.topMargin = gridLayoutIo.anchors.topMargin
        rowBackTile.anchors.BottonAnchor = gridLayoutIo.anchors.BottonAnchor
        rowBackTile.anchors.bottomMargin = gridLayoutIo.anchors.bottomMargin
        rowBackTile.anchors.LeftAnchor = gridLayoutIo.anchors.LeftAnchor
        rowBackTile.anchors.leftMargin = gridLayoutIo.anchors.leftMargin
        rowBackTile.anchors.RightAnchor = gridLayoutIo.anchors.RightAnchor
        rowBackTile.anchors.rightMargin = gridLayoutIo.anchors.rightMargin
        rowBackTile.x = gridLayoutIo.x
        rowBackTile.y = gridLayoutIo.y
        rowBackTile.width = gridLayoutIo.width
        rowBackTile.height = gridLayoutIo.height

        gridLayoutIo.visible = true;
        changeRowSpace();
        changeColumnSpace();
    }

    //repeaterIoSwitch.onModelChanged: {
    //}
    scrollView.onContentHeightChanged: {
        changeRowSpace();
        //console.log("implicitContentHeight:" + iOSwitchForm.scrollView.implicitContentHeight);
    }

    onHeightChanged: {
        changeRowSpace();
        //console.log("gridLayoutIo.height: " + gridLayoutIo.height + " gridLayoutIo.columnSpacing: " + gridLayoutIo.columnSpacing);
    }

    onWidthChanged: {
        changeColumnSpace();
    }

    function changeRowSpace()
    {
        if (null !== dbgManager && true === gridLayoutIo.visible) {
            var rows = dbgManager.ioListDisp.length / gridLayoutIo.columns;
            gridLayoutIo.rowSpacing = (scrollView.contentHeight - (21 * rows)) / rows;
            //console.log("scrollView.contentHeight: " + scrollView.contentHeight + " dbgManager.ioListDisp.length / gridLayoutIo.columns: " + dbgManager.ioListDisp.length / gridLayoutIo.columns);
            //gridLayoutIo.topPadding = (500 <= gridLayoutIo.height) ? (gridLayoutIo.rowSpacing / 2) : 2;
            gridLayoutIo.topPadding = gridLayoutIo.rowSpacing / 2;
        }
    }

    function changeColumnSpace()
    {
        if (null !== dbgManager.ioListDisp && true === gridLayoutIo.visible) {
            gridLayoutIo.columnSpacing = (gridLayoutIo.width - (21 * gridLayoutIo.columns)) / gridLayoutIo.columns;
            gridLayoutIo.leftPadding = gridLayoutIo.columnSpacing / 2;
        }
    }
}
