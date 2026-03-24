import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../../Monitor"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Drive_parameters.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.18
* Worker: SH.HONG
**************************************************************************************/
Item {
    id:drvParamPage
    width: parent.width
    height: parent.height

    property bool pageActive:false
    property int page: 0

    function changePage(page) {
        switch(page) {
        case 0:
            //commonViewModel.pageChange(Enums.FunctionMechanicalParamDrvParam)
            break
        case 1:
            //commonViewModel.pageChange(Enums.FunctionMPDrvParamSimpleSet)
            break
        case 2:
            commonViewModel.pageChange(Enums.FunctionDriverMonitor)
            break
        default:
            // NOP
            break
        }
    }
    Row{
        x: 551
        y: -40
        spacing: 1
        Rectangle{
            width: 130
            height: 45
            border.width: 2
            color: page == 0 || but_mouse1.pressed ? "#cc3333" : "#cccccc"
            border.color: "#999999"
            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: but_mouse1.pressed ? 17 : 19
                text: resourceManager.label(43,43, local.language)
                color: page == 0 ? "white" : "black"
            }
            MouseArea{
                id:but_mouse1
                anchors.fill: parent
                onClicked:{
                    page = 0
                    drvParamPage.changePage(0)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            width: 130
            height: 45
            border.width: 2
            color: page == 1 || but_mouse2.pressed ? "#cc3333" : "#cccccc"
            border.color: "#999999"
            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: but_mouse2.pressed ? 17 : 19
                text: resourceManager.label(43,44, local.language)
                color: page == 1 ? "white" : "black"
            }
            MouseArea{
                id:but_mouse2
                anchors.fill: parent
                onClicked:{
                    page = 1
                    drvParamPage.changePage(1)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
         Rectangle{
             width: 130
             height: 45
             border.width: 2
             color: page == 2 || but_mouse3.pressed ? "#cc3333" : "#cccccc"
             border.color: "#999999"
             Text{
                 anchors.fill: parent
                 horizontalAlignment: Text.AlignHCenter
                 verticalAlignment: Text.AlignVCenter
                 lineHeight: 0.7
                 font.pixelSize: but_mouse3.pressed ? 17 : 18
                 font.bold: true
                 text: resourceManager.label(43,45, local.language)
                 color: resourceManager.dataColor(43,45)
             }
             MouseArea{
                 id:but_mouse3
                 anchors.fill: parent
                 onClicked:{
                     page = 2
                     drvParamPage.changePage(2)
                     phyKeyManager.ctrlBuzzer()
                 }
             }
         }
    }//菜单切换

    StackLayout{
        id:stack_dri
        width: 1010
        height: 600
        x:10
        y:10
        currentIndex: page

        Driver_1{
            property bool isActive: drvParamPage.pageActive && (stack_dri.currentIndex === 0)
            pageActive: isActive
        }

        Loader {
            source: "qrc:/qml/Functions/MechanicalConfig/DriveParameters/Driver_2.qml";
            active: false
            property bool isActive: drvParamPage.pageActive && (stack_dri.currentIndex === 1)
            onLoaded: {
                item.pageActive = isActive;
                item.jogScrollOn = isActive;
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                    item.jogScrollOn = isActive;
                }
            }
        }

        Loader {
            source: "qrc:/qml/Monitor/Drive_Monitor.qml";
            active: false
            property bool isActive: drvParamPage.pageActive && (stack_dri.currentIndex === 2)
            onLoaded: {
                item.pageActive = isActive;
                item.scale = 0.9     // ← 비율로 줄이기
                item.x = -17
                item.y = -10
                //item.realMonJogScrollOn = isActive;
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                item.pageCallerDrvParam = isActive;
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                    //item.realMonJogScrollOn = isActive;
                }
            }
        }
    }
}
