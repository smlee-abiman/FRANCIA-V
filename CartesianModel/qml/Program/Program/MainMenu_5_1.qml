import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Components"
import "../../Widget/ComboBox"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_5_1.qml
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
    id:menu5_1

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property int menuIndex: programViewModel.progMenu5.Menu5_Index
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu5.subMenuList[0]
    property int curSelectIndex:menuModel.curSelectIndex
    property var selectProgramNo:programViewModel.selectedProgramNo
    property bool isValidStopCmd:(selectProgramNo >= 251 && selectProgramNo <= 255)
    property int itemSelect:menuModel.itemSelect
    property int almItemSelect:menuModel.curAlarmSelectIndex

    onPageActiveChanged: {
        if (pageActive) {
            //console.log("MainMenu_5_1 onPageActiveChanged:"+pageActive)
            menuModel.activate()
        } else {
            //console.log("MainMenu_5_1 onPageActiveChanged:"+pageActive)
            menuModel.deactivate()
        }
    }

    Component.onDestruction: menuModel.deactivate()

    Language { id: language }

    Rectangle{
        id:menu5_1_0
        width: parent.width
        height: parent.height
        color: "transparent"
        border.width: 2
        border.color: "#999999"
        property int select: menu5_1.itemSelect
        Column{
            x:20
            y:20
            spacing: 2
            Rectangle{
                width: 380
                height: 50
                color:(menu5_1.isValidStopCmd === true) ? menu5_1_0.select == 1 ? "#cc3333" : "#eaeaea" : "#cccccc"
                border.color: "#999999"
                CheckBox{
                    width: 100
                    height: 36
                    x:15
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        visible:(menu5_1.isValidStopCmd)
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: menu5_1_0.select == 1 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }
                    contentItem: Text{
                        text: resourceManager.label(41,545, language.value)
                        color: resourceManager.rgb(41,545)
                        font.pixelSize: 17
                        font.bold: true
                        leftPadding: 30
                    }
                    onClicked: {
                        if (menu5_1.isValidStopCmd === true){
                            if(menu5_1_0.select === 1){
                                menu5_1_0.select = 0;
                                menuModel.curSelectIndex = -1;
                            } else {
                                menu5_1_0.select = 1
                                menuModel.curSelectIndex = (menu5_1_0.select - 1);
                            }
                        }
                    }
                }
                Rectangle{
                    visible:!(menu5_1.isValidStopCmd)
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin:13
                    width: 27
                    height: 27
                    border.color: "#999999"
                    border.width: 2
                    color: "#eaeaea"
                }
            }
            Rectangle{
                width: 380
                height: 54
                color:(menu5_1.isValidStopCmd === true) ? menu5_1_0.select == 2 ? "#cc3333" : "#eaeaea" : "#cccccc"
                border.color: "#999999"
                CheckBox{
                    width: 100
                    height: 36
                    x:15
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        visible:(menu5_1.isValidStopCmd)
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: menu5_1_0.select == 2 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }
                    contentItem: Text{
                        text: resourceManager.label(41,546, language.value)
                        color: resourceManager.rgb(41,546)
                        font.pixelSize: 17
                        font.bold: true
                        leftPadding: 30
                    }
                    onClicked: {
                        if (menu5_1.isValidStopCmd === true){
                            if(menu5_1_0.select === 2){
                                menu5_1_0.select = 0;
                                menuModel.curSelectIndex = -1;
                            } else {
                                menu5_1_0.select = 2
                                menuModel.curSelectIndex = (menu5_1_0.select - 1);
                            }
                        }
                    }
                }
                Rectangle{
                    visible:!(menu5_1.isValidStopCmd)
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin:13
                    width: 27
                    height: 27
                    border.color: "#999999"
                    border.width: 2
                    color: "#eaeaea"
                }
            }
            Rectangle{
                width: 380
                height: 54
                color:menu5_1_0.select == 3 ? "#cc3333" : "#eaeaea"
                border.color: "#999999"
                CheckBox{
                    width: 100
                    height: 36
                    x:15
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: menu5_1_0.select == 3 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }
                    contentItem: Text{
                        text: resourceManager.label(41,547, language.value)
                        color: resourceManager.rgb(41,547)
                        font.pixelSize: 17
                        font.bold: true
                        leftPadding: 30
                    }
                    onClicked: {
                        if(menu5_1_0.select === 3){
                            menu5_1_0.select = 0;
                            menuModel.curSelectIndex = -1;
                        } else {
                            menu5_1_0.select = 3
                            menuModel.curSelectIndex = (menu5_1_0.select - 1);
                        }
                    }
                }
            }
            Rectangle{
                width: 380
                height: 54
                color:menu5_1_0.select == 4 ? "#cc3333" : "#eaeaea"
                border.color: "#999999"
                CheckBox{
                    width: 100
                    height: 36
                    x:15
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: menu5_1_0.select == 4 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }
                    contentItem: Text{
                        text: resourceManager.label(41,548, language.value)
                        color: resourceManager.rgb(41,548)
                        font.pixelSize: 17
                        font.bold: true
                        leftPadding: 30
                    }
                    onClicked: {
                        if(menu5_1_0.select === 4){
                            menu5_1_0.select = 0;
                            menuModel.curSelectIndex = -1;
                        } else {
                            menu5_1_0.select = 4
                            menuModel.curSelectIndex = (menu5_1_0.select - 1);
                        }
                    }
                }
                StringComboBox_style5{
                    id:alm_select
                    width: 220
                    height: 40
                    x:140
                    anchors.verticalCenter: parent.verticalCenter
                    visible: menu5_1_0.select == 4
                    bgRectBorderWidth: 2
                    bgPopupBorderWidth:2
                    popupDelegateNum: 55
                    fontSize: 16
                    resourceGroupId: 41
                    resourceStartIndex: 668
                    bgRectColor: "#ffffff"
                    bgIndicatorColor: "#cccccc"
                    currentIndex: menu5_1.almItemSelect
                    onCurrentIndexChanged:{
                        if (menu5_1.pageActive){
                            menuModel.curAlarmSelectIndex = alm_select.currentIndex
                        }
                    }
                }
            }
        }
    }//系统
}
