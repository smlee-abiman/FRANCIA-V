import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: ComparePopup.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.23
* Worker: SH.HONG
**************************************************************************************/
Dialog{
    id:compare_result
    width: 642
    height: 340
    modal: true
    closePolicy:Popup.NoAutoClose
    property var viewModel:null
    property bool compareResult: false
    property int compareDiscordFactor:viewModel.compareDiscordFactor
    property string compareSuccessMsg:resourceManager.label(51,58,local.language)
    property string compareFailMsg:resourceManager.label(51,59,local.language)
    property string fileNumber:""
    property string fileName:""
    property var compareDetailResult:viewModel.compareDetailResult
    property var compareDetailResultColor:viewModel.compareDetailResultColor

    QtObject {
        id: local
        property int language: 0
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    background: Rectangle{
        width: parent.width
        height: parent.height
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 642
            height: 55
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Row{
                anchors.centerIn: parent
                spacing: 5
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/08_MENU/d082_11.png"
                }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 22
                    color: resourceManager.rgb(51,57)
                    text: resourceManager.label(51,57,resourceManager.language)
                }
            }
            ImageButton{
                id: compa2re_result_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    viewModel.compareDiscordFactor = -1
                    viewModel.clearCompareDetailResult()
                    compare_result.fileNumber = ""
                    compare_result.fileName = ""
                    compare_result.close()
                }
            }
        }//标题栏
    }

    Text{
        x:-6
        y:45
        font.pixelSize: 20
        color: compareResult ? resourceManager.rgb(51,58) : resourceManager.rgb(51,59)
        text: compareResult ?
            (compare_result.compareSuccessMsg).toString().replace("[no]", compare_result.fileNumber).replace("[file]", compare_result.fileName) :
            (compare_result.compareFailMsg).toString().replace("[no]", compare_result.fileNumber).replace("[file]", compare_result.fileName)
    }

    Grid{
        x:-6
        y:78
        columns: 4
        rows: 8
        columnSpacing: -2
        rowSpacing: -2
        flow: Grid.TopToBottom
        Repeater{
            model: 32
            Rectangle{
                width: 159
                height: 32
                border.color: "#999999"
                Text{
                    x:5
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 16
                    color: compare_result.compareDetailResultColor[index]
                    text: compare_result.compareDetailResult[index]
                }
                Text{
                    x:135
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 22
                    color: "red"
                    text: (compare_result.compareDetailResult[index] === "") ? "" : "≠"
                }
            }
        }
    }
}
