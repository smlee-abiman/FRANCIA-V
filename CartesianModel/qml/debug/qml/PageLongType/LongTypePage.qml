import QtQuick 2.4
import QtQuick.Layouts 1.3


LongTypePageForm {
    Component.onCompleted: {
    }

    comboBoxCategory.onCurrentIndexChanged: {
        if (-1 == comboBoxCategory.currentIndex) return;
        dbgManager.longTypeCategoryGroupId = comboBoxCategory.currentIndex;
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
                    numberCellLoader.setSource("../Common/NumberCell.qml", {
                                               //"text": modelData.value,
                                               //"readOnly": "false",
                                               // 入力可能範囲を 0～65535とする場合、以下を変更してください。有効桁数は自動計算されます。
                                               //"maxValue": 65535,
                                               //"minValue": 0,
                                               "maxValue": 0x7fffffff,
                                               "minValue": 0,
                                               "Layout.minimumWidth": 100    // WORD Typeデータの表示幅は、ここを調整
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
