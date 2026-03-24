import QtQuick 2.5
import QtQuick.Layouts 1.3

OtherTypePageForm {

    Component.onCompleted: {
    }

    comboBoxCategory.onCurrentIndexChanged: {
        if (-1 == comboBoxCategory.currentIndex) return;
        dbgManager.otherTypeCategoryGroupId = comboBoxCategory.currentIndex;
    }

    gridStrings.onHeightChanged: {
        scrollView.contentHeight = gridStrings.height + gridStrings.anchors.topMargin + gridStrings.anchors.bottomMargin;
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
                var iGridWidth = (0 !== gridStrings.width) ? gridStrings.width : 608;
                if ("H" === modelData.value) {
                    numberRow.Layout.columnSpan = gridStrings.columns;
                    numberRow.Layout.minimumWidth = iGridWidth;
                    //numberCellLoader.item.height = 50;              // valid
                    numberCellLoader.setSource("../Common/RowCategoryCapt.qml", {
                                               "textBody.text": modelData.label,
                                               "Layout.minimumWidth": iGridWidth
                                             } );
                }
                else if ("_ADR" === modelData.label) {
                    var iRowAdr = parseInt(modelData.value);
                    dbgManager.otherTypeIsString = modelData.isString;
                    adrCellLoader.setSource("../Common/LabelAddress.qml", {
                                               "text": qsTr(("0000" + iRowAdr.toString(16).toUpperCase()).slice(-4) + ":"),
                                               "font.pointSize": 16,
                                               "Layout.minimumWidth": 55
                                             });
                }
                else {
                    if (dbgManager.otherTypeIsString == false){
                        numberRow.Layout.columnSpan = 1;
                        numberCellLoader.setSource("../Common/NumberCell.qml", {
                                                   //"text": modelData.value,
                                                   //"readOnly": "false",
                                                   "Layout.minimumWidth": 38
                                                 });
                    }
                    else{
                        numberRow.Layout.columnSpan = 8;
                        numberCellLoader.setSource("../Common/StringCell.qml", {
                                                   //"text": modelData.value,
                                                   "maxLength": 6,
                                                   "Layout.minimumWidth": 543    // WORD Typeデータの表示幅は、ここを調整
                                                 });
                    }
                }
            }
        }
    }

    gridStrings.onWidthChanged: {

    }
}
