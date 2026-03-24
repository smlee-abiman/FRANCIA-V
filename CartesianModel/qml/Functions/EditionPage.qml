import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0            // Enums.FunctionVersionMng, VersionNo***

/*************************************************************************************
* Title: UI/UX of AEK
* File: EditionPage.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.16
* Worker: SH.HONG
**************************************************************************************/

Item {
    property bool pageActive: false
    property var viewModel: functionViewModel.versionView
    width: parent.width
    height: parent.height

    Component.onDestruction: viewModel.deactivate()
    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.FunctionVersionMng)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }

    QtObject {
        id: local
        property int language: 0
        function getVersion(index, offset, isAxis, versionTbl) {
            if (isAxis) {
                let order = commonViewModel.axisOrder[index]
                switch (order) {
                case  0: return versionTbl[Enums.VersionNoAxis1]
                case  1: return versionTbl[Enums.VersionNoAxis2]
                case  2: return versionTbl[Enums.VersionNoAxis3]
                case  3: return versionTbl[Enums.VersionNoAxis4]
                case  4: return versionTbl[Enums.VersionNoAxis5]
                case  5: return versionTbl[Enums.VersionNoAxis6]
                case  6: return versionTbl[Enums.VersionNoAxis7]
                case  7: return versionTbl[Enums.VersionNoAxis8]
                case  8: return versionTbl[Enums.VersionNoAxis9]
                }
            }
            switch(index + offset) {
                case  0: return versionTbl[Enums.VersionNoQt]
                case  1: return versionTbl[Enums.VersionNoApplication]
                case  2: return versionTbl[Enums.VersionNoPenSet]
                case  3: return versionTbl[Enums.VersionNoPenMsg]
                case  4: return versionTbl[Enums.VersionNoIPL]
                case  5: return versionTbl[Enums.VersionNoMOT]
                case  6: return versionTbl[Enums.VersionNoSPC]
                case  7: return versionTbl[Enums.VersionNoINIT]
                case  8: return versionTbl[Enums.VersionNoTABLE]
                case  9: return versionTbl[Enums.VersionNoNCP]
                case 10: return ""
                case 11: return ""
                case 12: return ""
                case 13: return ""
                case 14: return ""
                case 15: return ""
                case 16: return ""
                case 17: return ""
                case 18: return "" // reserve.
                case 19: return "" // reserve.
            }
            console.log("unknown version. index:" + index)  // for debug.
            return ""
        }
    }
    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    Rectangle{
        x: 2
        y: 40
        width: 1018
        height: 590
        anchors.fill: parent.Center
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"
    }//背景

    //标题
    Rectangle{
        width: 130
        height: 46
        x:10
        y:50
        color: "#999999"
        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.bold: true
            text: resourceManager.label(49,1,local.language)
            color: resourceManager.rgb(49,1)
        }
    }
    Rectangle{
        width: 360
        height: 46
        x:142
        y:50
        color: "#999999"
        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.bold: true
            text: resourceManager.label(49,2,local.language)
            color: resourceManager.rgb(49,2)
        }
    }
    Rectangle{
        width: 130
        height: 46
        x:515
        y:50
        color: "#999999"
        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.bold: true
            text: resourceManager.label(49,1,local.language)
            color: resourceManager.rgb(49,1)
        }
    }
    Rectangle{
        width: 360
        height: 46
        x:647
        y:50
        color: "#999999"
        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
            font.bold: true
            text: resourceManager.label(49,2,local.language)
            color: resourceManager.rgb(49,2)
        }
    }
    //左侧
    ColumnLayout{
        width: 492
        x:10
        y:98
        spacing: 3
        Repeater{
            model:10
            Rectangle{
                width: parent.width
                height: 50
                color: "transparent"
                Rectangle{
                    width: 130
                    height: 50
                    color: "#eaeaea"
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        font.pixelSize: 20
                        font.bold: true
                        text: resourceManager.label(49,3+index,local.language)
                        color: resourceManager.rgb(49,3+index)
                    }
                }
                Rectangle{
                    width: 360
                    height: 50
                    x:132
                    color: "#cccccc"
                    border.color: "#999999"
                    border.width: 1
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 4
                        font.pixelSize: 20
                        font.bold: true
                        text: local.getVersion(index, 0, false, viewModel.versionTbl)
                    }
                }
            }
        }
    }
    //右侧
    ColumnLayout{
        width: 492
        x:515
        y:98
        spacing: 3
        Repeater{
            model:10
            Rectangle{
                width: parent.width
                height: 50
                color: "transparent"
                readonly property bool isAxis: (0 <= index && index < commonViewModel.axisOrder.length)
                Rectangle{
                    width: 130
                    height: 50
                    color: "#eaeaea"
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 4
                        font.pixelSize: 20
                        font.bold: true
                        wrapMode: Text.WordWrap
                        text: {
                            if (isAxis) {
                                return resourceManager.label(22, commonViewModel.axisOrder[index], local.language)
                            }
                            return resourceManager.label(49, 13 + index, local.language)
                        }
                        color: {
                            if (isAxis) {
                                return resourceManager.rgb(22, commonViewModel.axisOrder[index])
                            }
                            return resourceManager.rgb(49, 13 + index)
                        }
                    }
                }
                Rectangle{
                    width: 360
                    height: 50
                    x:132
                    color: "#cccccc"
                    border.color: "#999999"
                    border.width: 1
                    Text {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 20
                        font.bold: true
                        text: local.getVersion(index, 10, isAxis, viewModel.versionTbl)
                    }
                }
            }
        }
    }
}
