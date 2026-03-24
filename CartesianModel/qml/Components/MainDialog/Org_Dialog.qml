import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.12
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Org_Dialog.qml
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
    id:org_dialog
    width: 485
    height: 310

    property int org_timerCnt: 0
    property bool timer_switch: false
    property int originstatus: originViewModel.originStatus

    background: Rectangle{
        width: 485
        height: 310
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 485
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
                    source: (originViewModel.originStatus === 4 ) ? resourceManager.imagePath + "/05_ORG/d050_28.png" : resourceManager.imagePath + "/05_ORG/d050_27.png"
                }
                Text{
                    font.pixelSize: 24
                    font.weight: Font.Bold
                    font.bold: true
                    color: resourceManager.rgb(30, 21)
                    text: resourceManager.label(30, 21,resourceManager.language)
                }
            }
        }//标题栏
    }
    ImageButton{
        id:language_dialog_close
        x:415
        y:-10
        scale: 1.5
        upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        onClicked: {
            originViewModel.stopOriginCarriageMonitor()
            commonViewModel.modbusMemblemSwControl(true, Enums.OriginCancelKeyNo)
            commonViewModel.modbusMemblemSwControl(false, Enums.OriginCancelKeyNo)
            org_dialog.close()
        }
    }//关闭弹窗

    Timer{
        id:org_timer
        interval: 200 //间隔(单位毫秒):1000毫秒=1秒
        repeat: true //重复
        running: originViewModel.originStatus === 4 ? false : timer_switch
        onTriggered:{
            if(org_timerCnt<8){
                org_timerCnt++
            }else{
                org_timerCnt=0
            }
        }
    }
    Text {
        x:2
        y:80
        width: 480
        height: 60
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 23
        color: resourceManager.rgb(30, 22)
        text: (originstatus == 1 || originstatus == 3) ? resourceManager.label(30, 22, resourceManager.language)
                                 : originstatus == 2 ? resourceManager.label(30, 23, resourceManager.language)
                                                      : resourceManager.label(30, 24, resourceManager.language)
    }//提示信息
    Item {
        x:100
        y:140
        Image {
            x:2
            y:10
            source: resourceManager.imagePath + "/05_ORG/d050_14.png"
        }
        Row{
            x:30
            y:7
            spacing: 12
            Image {
                source:(org_timerCnt===1) ? resourceManager.imagePath + "/05_ORG/d050_17.png" : resourceManager.imagePath + "/05_ORG/d050_16.png"
            }
            Image {
                source:(org_timerCnt===2) ? resourceManager.imagePath + "/05_ORG/d050_17.png" : resourceManager.imagePath + "/05_ORG/d050_16.png"
            }
            Image {
                source:(org_timerCnt===3) ? resourceManager.imagePath + "/05_ORG/d050_17.png" : resourceManager.imagePath + "/05_ORG/d050_16.png"
            }
            Image {
                source:(org_timerCnt===4) ? resourceManager.imagePath + "/05_ORG/d050_17.png" : resourceManager.imagePath + "/05_ORG/d050_16.png"
            }
            Image {
                source:(org_timerCnt===5) ? resourceManager.imagePath + "/05_ORG/d050_17.png" : resourceManager.imagePath + "/05_ORG/d050_16.png"
            }
            Image {
                source:(org_timerCnt===6) ? resourceManager.imagePath + "/05_ORG/d050_17.png" : resourceManager.imagePath + "/05_ORG/d050_16.png"
            }
        }
        Image {
            x:222
            y:-14
            source:(originViewModel.originStatus === 4) ? resourceManager.imagePath + "/05_ORG/d050_18.png" : resourceManager.imagePath + "/05_ORG/d050_15.png"
        }
    }//提示图标
    Rectangle{
        width: 120
        height: 55
        x:342
        y:232
        color: "#ffffff"
        border.width: 2
        border.color: "#999999"

        visible: !(originViewModel.originStatus === 4)
        Row{
            anchors.centerIn: parent
            spacing: 2
            Image {
                anchors.verticalCenter: parent.verticalCenter
                source: org_timer.running ? resourceManager.imagePath + "/05_ORG/d050_29.png" : resourceManager.imagePath + "/05_ORG/d050_30.png"
            }
            Text {
                width: 70
                height: 55
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 23
                color: resourceManager.rgb(30, 23)
                text: org_timer.running ? resourceManager.label(30, 25, resourceManager.language) : resourceManager.label(30, 26, resourceManager.language)
            }
        }
        MouseArea{
            anchors.fill: parent
            onPressed: parent.color = "#cc3333"
            onReleased: parent.color = "#ffffff"
            onClicked: {
                if (org_timer.running){
                    commonViewModel.modbusMemblemSwControl(true, Enums.OriginPauseKeyNo)
                    originViewModel.pauseOriginCarriageMonitor()
                    commonViewModel.modbusMemblemSwControl(false, Enums.OriginPauseKeyNo)
                    timer_switch = false
                } else {
                    commonViewModel.modbusMemblemSwControl(true, Enums.OriginStartKeyNo)
                    originViewModel.startOriginCarriageMonitor()
                    commonViewModel.modbusMemblemSwControl(false, Enums.OriginStartKeyNo)
                    timer_switch = true
                }
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//暂停按钮
}
