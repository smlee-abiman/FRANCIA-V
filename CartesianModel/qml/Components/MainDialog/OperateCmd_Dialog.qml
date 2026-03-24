import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: OperateCmd_Dialog.qml
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
// NOTE : 동작표시 같은 팝업창 코드
Dialog  {
    id:operateCmd_dialog
    width: 642
    height: 310
    modal: false
    closePolicy:Popup.NoAutoClose

    signal dialogClose()
    property var opeCmdStatusMsg:commonViewModel.opeCmdStatusMsg
    property var opeCmdStatusColor:commonViewModel.opeCmdStatusColor

    function set_DialogPosition(diaX,diaY){
        if(operateCmd_dialog.x + diaX < 0){
            operateCmd_dialog.x = 0
        }else if(operateCmd_dialog.x + diaX > 1024 - operateCmd_dialog.width){
            operateCmd_dialog.x = 1024 - operateCmd_dialog.width
        }else{
            operateCmd_dialog.x = operateCmd_dialog.x + diaX
        }
        if(operateCmd_dialog.y + diaY < 90){
            operateCmd_dialog.y  = 90
        }else if(operateCmd_dialog.y  + diaY > 685 - operateCmd_dialog.height){
            operateCmd_dialog.y  = 685 - operateCmd_dialog.height
        }else{
            operateCmd_dialog.y  = operateCmd_dialog.y  + diaY
        }
    }

    background: Rectangle{
        width: 642
        height: 310
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
                spacing: 10
                Image {
                    x:200
                    y:2
                    source: resourceManager.imagePath + "/08_MENU/d084_02.png"
                }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 26
                    font.weight: Font.Bold
                    font.bold: true
                    color: resourceManager.rgb(53,1)
                    text: resourceManager.label(53,1,resourceManager.language)
                }
            }
            MouseArea{
                anchors.fill: parent
                property point clickpoint: "0,0"
                onPressed: clickpoint = Qt.point(mouse.x,mouse.y)
                onPositionChanged: {
                    var offset = Qt.point(mouse.x - clickpoint.x, mouse.y - clickpoint.y)
                    set_DialogPosition(offset.x,offset.y)
                }
            }
        }//标题栏
    }
    ImageButton{
        x:570
        y:-1
        scale: 1.5
        upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        onClicked: {
            operateCmd_dialog.dialogClose()
            operateCmd_dialog.close()
        }
    }

    Text{
        x:10
        y:60
        font.pixelSize: 22
        color:operateCmd_dialog.opeCmdStatusColor
        text: operateCmd_dialog.opeCmdStatusMsg
    }
}
