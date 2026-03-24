import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Datetime_Dialog.qml
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
    id:date_dialog
    width: 642
    height: 310
    modal: false
    closePolicy: Popup.CloseOnEscape
    background: Rectangle{
        width: 642
        height: 310
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 642
            height: 55
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.centerIn: parent
                font.pixelSize: 26
                color: resourceManager.rgb(55,0)
                text: resourceManager.label(55,0,resourceManager.language)
            }
            ImageButton{
                id:date_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    date_dialog.close()
                }
            }
        }//标题栏
    }

    Image{
        id:clockImage
        width: 198
        height: 198
        x:40
        y:70
        source: resourceManager.imagePath + "/08_MENU/d080_04" + ".png"
        Image{
            id:secImage
            x:94
            y:38
            source: resourceManager.imagePath + "/08_MENU/d080_08.png"
            transform: Rotation {
                id: secondRotation
                origin.x: 5.5
                origin.y: 62
                angle: commonViewModel.seconds * 6
            }
        }//秒针
        Image{
            id:minuteImage
            x:92
            y:45
            source: resourceManager.imagePath + "/08_MENU/d080_07.png"
            transform: Rotation {
                id: minuteRotation
                origin.x: 7
                origin.y: 55
                angle: commonViewModel.minutes * 6
            }
        }//分针
        Image{
            id:hourImage
            x:89
            y:50
            source: resourceManager.imagePath + "/08_MENU/d080_06.png"
            transform: Rotation {
                id: hourRotation
                origin.x: 10
                origin.y: 50
                angle: (commonViewModel.hour * 30)+(commonViewModel.minutes * 0.5)
            }
        }//时针
    }

    Text{
        id:dateText
        anchors.top:clockImage.top
        anchors.topMargin:-10
        anchors.left:clockImage.right
        anchors.leftMargin:40
        font.pixelSize: 20
        text: resourceManager.label(55,1,resourceManager.language)
        color: resourceManager.rgb(55,1)
    }
    LimitNumericEditBox{
        id:yearArea
        anchors.top:dateText.bottom
        anchors.topMargin:2
        anchors.left:clockImage.right
        anchors.leftMargin:40
        value:commonViewModel.year
        textColor:"black"
        width:90
        height:38
        borderWidth: 1
        borderColor: "#999999"
        rectRadius: 0
        color: opened ? "#cc3333" : "white"
        onReady: {
            dialogDisplayX = 160
            dialogDisplayY = 150
        }
        noLimit:true
        keyLayoutType:0
        onEnter: {
            let yearValue = parseInt(newValue)
            if (yearValue >= 0 && yearValue <= 99){
                if((yearValue % 4 !== 0) && monthArea.value === "02" && dayArea.value === "29"
                    || (yearValue % 4 !== 0) && monthArea.value === "2" && dayArea.value === "29"){
                    console.log("yearArea onEnter:"+newValue, "month:"+monthArea.value, "day:"+dayArea.value)
                    dayArea.enter("28");
                }
                commonViewModel.modbusUpdateDateTime(Enums.DateTimeYear, newValue)
                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
            } else {
                console.log("!!!!! invalid yearValue:"+yearValue)
            }
        }
    }
    LimitNumericEditBox{
        id:monthArea
        anchors.top:dateText.bottom
        anchors.topMargin:2
        anchors.left:yearArea.right
        anchors.leftMargin:10
        value:commonViewModel.month
        textColor:"black"
        width:90
        height:38
        borderWidth: 1
        borderColor: "#999999"
        rectRadius: 0
        color: opened ? "#cc3333" : "white"
        onReady: {
            dialogDisplayX = 160
            dialogDisplayY = 150
        }
        noLimit:true
        keyLayoutType:0
        property variant shortMonth: /4|6|9|11/g
        onEnter: {
            let monthValue = parseInt(newValue)
            if (monthValue >= 1 && monthValue <= 12){
                if(newValue === "2" || newValue === "02"){
                    if((parseInt(yearArea.value) % 4 === 0) && (parseInt(dayArea.value) > 29)){
                        dayArea.enter("29");
                    } else if((parseInt(yearArea.value) % 4 !== 0) && (parseInt(dayArea.value) > 28)){
                        dayArea.enter("28");
                    }
                } else if(shortMonth.test(newValue) && (dayArea.value === "31")){
                    dayArea.enter("30");
                }
                commonViewModel.modbusUpdateDateTime(Enums.DateTimeMonth, newValue)
                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
            } else {
                console.log("!!!!! invalid monthValue:"+monthValue)
            }
        }
    }
    LimitNumericEditBox{
        id:dayArea
        anchors.top:dateText.bottom
        anchors.topMargin:2
        anchors.left:monthArea.right
        anchors.leftMargin:10
        value:commonViewModel.day
        textColor:"black"
        width:90
        height:38
        borderWidth: 1
        borderColor: "#999999"
        rectRadius: 0
        color: opened ? "#cc3333" : "white"
        onReady: {
            dialogDisplayX = 160
            dialogDisplayY = 150
        }
        noLimit:true
        keyLayoutType:0
        onEnter: {
            let dayValue = parseInt(newValue)
            if (dayValue >= 1 && dayValue <= 31){
                if(monthArea.value === "2" || monthArea.value === "02"){
                    if((parseInt(yearArea.value) % 4 === 0) && dayValue > 29){
                        newValue = "29"
                    } else if((parseInt(yearArea.value) % 4 !== 0) && dayValue > 28){
                        newValue = "28"
                    }
                } else if(monthArea.shortMonth.test(monthArea.value) && (newValue === "31")){
                    newValue = "30"
                }
                commonViewModel.modbusUpdateDateTime(Enums.DateTimeDay, newValue)
                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
            } else {
                console.log("!!!!! invalid dayValue:"+dayValue)
            }
        }
    }
    Text{
        id:timeText
        anchors.top:yearArea.bottom
        anchors.topMargin:2
        anchors.left:clockImage.right
        anchors.leftMargin:40
        font.pixelSize: 20
        text: resourceManager.label(55,2,resourceManager.language)
        color: resourceManager.rgb(55,2)
    }
    LimitNumericEditBox{
        id:hourArea
        anchors.top:timeText.bottom
        anchors.topMargin:2
        anchors.left:clockImage.right
        anchors.leftMargin:40
        width:90
        height:38
        value:commonViewModel.hour
        textColor:"black"
        borderWidth: 1
        borderColor: "#999999"
        rectRadius: 0
        color: opened ? "#cc3333" : "white"
        onReady: {
            dialogDisplayX = 160
            dialogDisplayY = 150
        }
        noLimit:true
        keyLayoutType:0
        onEnter: {
            let hourValue = parseInt(newValue)
            if (hourValue >= 0 && hourValue <= 23){
                commonViewModel.modbusUpdateDateTime(Enums.DateTimeHour, newValue)
                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
            } else {
                console.log("!!!!! invalid hourValue:"+hourValue)
            }
        }
    }
    LimitNumericEditBox{
        id:minutesArea
        anchors.top:timeText.bottom
        anchors.topMargin:2
        anchors.left:hourArea.right
        anchors.leftMargin:10
        width:90
        height:38
        value:commonViewModel.minutes
        textColor:"black"
        borderWidth: 1
        borderColor: "#999999"
        rectRadius: 0
        color: opened ? "#cc3333" : "white"
        onReady: {
            dialogDisplayX = 160
            dialogDisplayY = 150
        }
        noLimit:true
        keyLayoutType:0
        onEnter: {
            let minutesValue = parseInt(newValue)
            if (minutesValue >= 0 && minutesValue <= 59){
                commonViewModel.modbusUpdateDateTime(Enums.DateTimeMinutes, newValue)
                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
            } else {
                console.log("!!!!! invalid minutesValue:"+minutesValue)
            }
        }
    }
    LimitNumericEditBox{
        id:secondArea
        anchors.top:timeText.bottom
        anchors.topMargin:2
        anchors.left:minutesArea.right
        anchors.leftMargin:10
        width:90
        height:38
        value:commonViewModel.seconds
        textColor:"black"
        borderWidth: 1
        borderColor: "#999999"
        rectRadius: 0
        color: opened ? "#cc3333" : "white"
        onReady: {
            dialogDisplayX = 160
            dialogDisplayY = 150
        }
        noLimit:true
        keyLayoutType:0
        onEnter: {
            let secondValue = parseInt(newValue)
            if (secondValue >= 0 && secondValue <= 59){
                commonViewModel.modbusUpdateDateTime(Enums.DateTimeSeconds, newValue)
                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
            } else {
                console.log("!!!!! invalid secondValue:"+secondValue)
            }
        }
    }
}
