import QtQuick 2.12
import QtQuick.Controls 2.12
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Remind_save.qml
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
Dialog  {
    id:remind_save
    width:620
    height: 320
    closePolicy: Popup.NoAutoClose
    modal: true
    anchors.centerIn: parent
    background: Rectangle{
        width:620
        height: 320
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 620
            height: 55
            border.color: "#999999"
            color: "#999999"
            Row{
                anchors.centerIn: parent
                spacing: 10
                Image {
                    x:200
                    y:2
                    source: resourceManager.imagePath + "/26_machine_p/d260_11.png"
                }
                Text{
                    font.pixelSize: 26
                    font.weight: Font.Bold
                    font.bold: true
                    color: "white"
                    text: resourceManager.label(43,2,resourceManager.language)
                }
            }
            ImageButton{
                id:error_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    remind_save.close()
                }
            }//关闭弹窗
        }//标题栏
    }
    property var modelSelectView: functionViewModel.mechanicalParamView.modelSelectView
    property var mechaParamView: functionViewModel.mechanicalParamView.mechaParamView
    property var drvPrmView: functionViewModel.mechanicalParamView.drvParamView.drvPrmView
    property var simpleParamView: functionViewModel.mechanicalParamView.drvParamView.simpleSetView
    property int remindindex: 0
    property int remindtype: 0
    property int savevalue: 0
    property int t1: 0

    signal clickSaveButton()

    Timer{
        id:timer
        interval: 1000
        onTriggered: {
            switch(remindindex){
            case 1:
                modelSelectView.saveStatus = 0
                break;
            case 2:
                mechaParamView.saveStatus = 0
                break;
            case 3:
                mechaParamView.saveStatus = 0
                break;
            case 4:
                drvPrmView.saveStatus = 0
                break;
            case 5:
                drvPrmView.saveStatus = 0
                break;
            case 6:
                simpleParamView.saveStatus = 0
                break;
            default:
                break;
            }

            remind_save.close();
        }
        running: (remindtype == 2 ? 1 : 0)
    }

    //机种保存
    Text{
        id:type_save
        anchors.centerIn: parent
        font.pixelSize: 30
        text:{
            switch(remindindex){
            case 1:
                (remindtype == 0) ? resourceManager.label(43,3,resourceManager.language) : (remindtype == 1) ? resourceManager.label(43,4,resourceManager.language) : resourceManager.label(43,5,resourceManager.language)
                break
            case 2:
                (remindtype == 0) ? resourceManager.label(43,6,resourceManager.language) : (remindtype == 1) ? resourceManager.label(43,7,resourceManager.language) : resourceManager.label(43,8,resourceManager.language)
                break
            case 3:
                (remindtype == 0) ? resourceManager.label(43,9,resourceManager.language) : (remindtype == 1) ? resourceManager.label(43,10,resourceManager.language) : resourceManager.label(43,11,resourceManager.language)
                break
            case 4:
                (remindtype == 0) ? resourceManager.label(43,273,resourceManager.language) : (remindtype == 1) ? resourceManager.label(43,274,resourceManager.language) : resourceManager.label(43,275,resourceManager.language)
                break
            case 5:
                (remindtype == 0) ? resourceManager.label(43,12,resourceManager.language) : (remindtype == 1) ? resourceManager.label(43,13,resourceManager.language) : resourceManager.label(43,14,resourceManager.language)
                break
            case 6:
                (remindtype == 0) ? resourceManager.label(43,12,resourceManager.language) : (remindtype == 1) ? resourceManager.label(43,13,resourceManager.language) : resourceManager.label(43,14,resourceManager.language)
                break
            default:
                ""
                break
            }
        }

        //            visible: remindindex == 1
    }

    Rectangle{
        width: 108
        height: 50
        x:488
        y:245
        color: "#cccccc"
        border.color: "#999999"
        Image {
            x:10
            anchors.verticalCenter: parent.verticalCenter
            source: resourceManager.imagePath + "/00_COMMON/d000_75.png"
        }
        Text {
            x:50
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 23
            text: resourceManager.label(43,15,resourceManager.language)
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {
                remind_save.clickSaveButton()
                parent.color = "#dae000"
            }
            onReleased: {
                parent.color = "#e6f6fc"
            }
            onClicked: {
                switch(remindindex){
                case 1:
                    modelSelectView.saveModelType(savevalue);
                    break;
                case 2:
                    mechaParamView.initParameter();
                    break;
                case 3:
                    mechaParamView.saveParameter();
                    break;
                case 4:
                    drvPrmView.initParameter();
                    break;
                case 5:
                    drvPrmView.saveParameter();
                    break;
                default:
                    break
                }
            }
        }
    }
}
