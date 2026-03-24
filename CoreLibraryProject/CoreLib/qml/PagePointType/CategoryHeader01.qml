import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "../Common"

Item {
    ScrollView {
        anchors.fill: parent
        clip: true
        contentWidth: local.rowWidth
        contentHeight: debugDataListView.contentHeight
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        QtObject {
            id: local
            property int rowCountPerPage: 10
            property int rowHeight: parent.height / rowCountPerPage
            property int rowWidth: parent.width
            property int rowSpacing: 10
            property int itemCountPerRow: 1 + 8 // Addr + Item(8)
        }

        ListView {
            property int itemWidth: (width - ((local.itemCountPerRow - 1) * local.rowSpacing)) / local.itemCountPerRow
            id: debugDataListView
            anchors.fill: parent
            anchors.rightMargin: 10
            model: dbgManager.pointHeaderList01
            delegate: Item {
                id: rowRoot
                width: local.rowWidth
                height: local.rowHeight

                Row {
                    id: listRow
                    spacing: local.rowSpacing

                    Repeater {
                        model: dbgManager.getRowPointHeaderList01(index)
                        delegate: Item {
                            id: rowRepeater
                            height: local.rowHeight

                            Loader {
                                id: cellLoader
                                anchors.fill: parent
                                anchors.bottomMargin: 5
                            }

                            Component.onCompleted: {
                                if ("H" === modelData.value) {
                                    rowRepeater.width = debugDataListView.width
                                    cellLoader.setSource("../Common/RowCategoryCapt.qml", {
                                                               "textBody.text": modelData.label,
                                                               "Layout.minimumWidth": debugDataListView.width
                                                             } );
                                }
                                else if ("_ADR" === modelData.label) {
                                    rowRepeater.width = debugDataListView.itemWidth
                                    var iRowAdr = parseInt(modelData.value);
                                    cellLoader.setSource("../Common/LabelAddress.qml", {
                                                             "text": qsTr(("0000" + iRowAdr.toString(16).toUpperCase()).slice(-4) + ":"),
                                                             "font.pointSize": 16,
                                                             "Layout.minimumWidth": debugDataListView.itemWidth
                                                         });
                                }
                                else {
                                    if(modelData.isPatternParam) {
                                        if(index <= 1) {
                                            rowRepeater.width = debugDataListView.itemWidth
                                            cellLoader.setSource("../Common/StringCell.qml", {
                                                                     //"text": modelData.value,
                                                                     "maxLength": 6,
                                                                     "Layout.minimumWidth": rowRepeater.width,
                                                                     "visible": false,
                                                                 });
                                        } else if(index == 2) {
                                            rowRepeater.width = debugDataListView.itemWidth
                                            cellLoader.setSource("../Common/StringCell.qml", {
                                                                     //"text": modelData.value,
                                                                     "maxLength": 6,
                                                                     "Layout.minimumWidth": rowRepeater.width,
                                                                 });
                                        } else {
                                            rowRepeater.width = debugDataListView.itemWidth * 2
                                            cellLoader.setSource("../Common/StringCell.qml", {
                                                                     //"text": modelData.value,
                                                                     "maxLength": 6,
                                                                     "Layout.minimumWidth": rowRepeater.width,
                                                                 });
                                        }
                                    } else {
                                        if(index == 2) {
                                            rowRepeater.width = debugDataListView.itemWidth * 3
                                        } else {
                                            rowRepeater.width = debugDataListView.itemWidth
                                        }
                                        cellLoader.setSource("../Common/StringCell.qml", {
                                                                 //"text": modelData.value,
                                                                 "maxLength": 6,
                                                                 "Layout.minimumWidth": rowRepeater.width,
                                                             });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
