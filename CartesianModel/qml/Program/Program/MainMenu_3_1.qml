import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Enums 1.0
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_3_1.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: SH.HONG
**************************************************************************************/
Item {
    id:menu3_1

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property int menuIndex: programViewModel.progMenu3.Menu3_Index
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu3.subMenuList[0]
    property string curDelayTimeValue:menuModel.curDelayTimeValue

    onPageActiveChanged: {
        if (pageActive) {
            //console.log("MainMenu_3_1 onPageActiveChanged:"+pageActive)
            menuModel.activate()
        } else {
            //console.log("MainMenu_3_1 onPageActiveChanged:"+pageActive)
            menuModel.deactivate()
        }
    }

    Component.onDestruction: menuModel.deactivate()

    Language { id: language }

    Rectangle{
        id:menu3_1_0
        width: parent.width
        height: parent.height
        color: "transparent"
        border.width: 2
        border.color: "#999999"
        Rectangle {
            x: 20
            y: 20
            width: 280   // 필요에 따라 조정
            height: 60
            color: "#cccccc"
            border.color: "#999999"
            Row {
                x: 15
                spacing: 10
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 18
                    font.bold: true
                    color: resourceManager.rgb(41,533)
                    text: resourceManager.label(41,533, language.value)
                }

                LimitNumericEditBox {
                    anchors.verticalCenter: parent.verticalCenter
                    id: delay_time
                    width: 140
                    height: 40
                    min: "0.00"
                    max: "99.99"
                    textColor: "black"
                    value: menu3_1.curDelayTimeValue
                    fontPixelSize: 18
                    borderColor: "#999999"
                    keyLayoutType: 6
                    onEnter: {
                        menuModel.curDelayTimeValue = newValue
                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    }
                    onReady: {
                    }
                }
            }
        }

    }//延时(遅延)
}
