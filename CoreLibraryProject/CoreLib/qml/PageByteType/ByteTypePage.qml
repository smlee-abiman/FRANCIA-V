import QtQuick 2.5
import QtQuick.Layouts 1.3
//import "../Common"

ByteTypePageForm {

    Component.onCompleted: {
    }

    comboBoxCategory.onCurrentIndexChanged: {
        if (-1 == comboBoxCategory.currentIndex) return;
        dbgManager.byteTypeCategoryGroupId = comboBoxCategory.currentIndex;
        //commonFunc.setCaptWordAdr(iOSwitchForm.repeaterAdr, dbgManager.ioCategoryOffset[comboBoxCategory.currentIndex]);
    }

    gridBytes.onHeightChanged: {
        scrollView.contentHeight = gridBytes.height + gridBytes.anchors.topMargin + gridBytes.anchors.bottomMargin;
    }

    Component {
        id: delegateNumberCell
        Row {
            id: numberRow
            Loader {
                id: numberCellLoader
            }
            Loader {
                id: adrCellLoader
            }

            Component.onCompleted: {
                var iGridWidth = (0 !== gridBytes.width) ? gridBytes.width : 608;
                if ("H" === modelData.value) {
                    numberRow.Layout.columnSpan = gridBytes.columns;
                    numberRow.Layout.minimumWidth = iGridWidth;
                    //numberCellLoader.item.height = 50;              // valid
                    numberCellLoader.setSource("../Common/RowCategoryCapt.qml", {
                                               "textBody.text": modelData.label,
                                               "Layout.minimumWidth": iGridWidth
                                             } );
                }
                else if ("_ADR" === modelData.label) {
                    var iRowAdr = parseInt(modelData.value);
                    adrCellLoader.setSource("../Common/LabelAddress.qml", {
                                               "text": qsTr(("0000" + iRowAdr.toString(16).toUpperCase()).slice(-4) + ":"),
                                               "font.pointSize": 16,
                                               "Layout.minimumWidth": 55
                                             });
                }
                else {
                    numberRow.Layout.columnSpan = 1;
/*
                    if (1 === (index % gridBytes.columns)) {
                        var iBaseAdr = 0;
                        var iRowAdr = iBaseAdr + ((index - 1) / gridBytes.columns * (gridBytes.columns - 1));
                        adrCellLoader.setSource("../Common/LabelAddress.qml", {
                                                   "text": qsTr(("0000" + iRowAdr.toString(16).toUpperCase()).slice(-4) + ":"),
                                                   "font.pointSize": 16,
                                                   "Layout.minimumWidth": 55
                                                 });
                    }
*/
                    numberCellLoader.setSource("../Common/NumberCell.qml", {
                                               //"text": modelData.value,
                                               //"readOnly": "false",
                                               "Layout.minimumWidth": 38
                                             });
                }
            }
        }
    }

    gridBytes.onWidthChanged: {
/*
        for (var idx=0; idx < gridBytes.rows; idx++) {
            if (gridBytes[idx].columnSpan === gridBytes.columns) {
                gridBytes[idx].Layout.minimumWidth = gridBytes.width;
            }
        }
*/
    }
}
