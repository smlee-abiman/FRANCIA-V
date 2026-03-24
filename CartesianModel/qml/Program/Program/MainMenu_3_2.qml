import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Enums 1.0
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_3_2.qml
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
    id:menu3_2
    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property int menuIndex: programViewModel.progMenu3.Menu3_Index
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu3.subMenuList[1]
    property int curPage:menuModel.curPageTimerDataList
    property int selectIndex:menuModel.selectIndexTimerDataList
    property int targetCoopPage:menuModel.targetCoopPage
    property int targetCoopSelectIndex:menuModel.targetCoopSelectIndex
    property bool isLinkedStartup:menuModel.isLinkedStartup
    property bool isHoldingTimerValue:menuModel.isHoldingTimerValue
    property int holdingTimerValueIndex:menuModel.holdingTimerValueIndex
    property string holdingTimerValue:menuModel.holdingTimerValue

    function initProperty(){
        //menuModel.curPageTimerDataList = 1
        menuModel.selectIndexTimerDataList = 0
        menuModel.targetCoopPage = 0
        menuModel.targetCoopSelectIndex = 0
        menuModel.isLinkedStartup = false
    }
    function initHoldSysProperty(){
        menuModel.isHoldingTimerValue = false
        menuModel.holdingTimerValueIndex = -1
        menuModel.holdingTimerValue = ""
    }

    onPageActiveChanged: {
        if (pageActive) {
            //console.log("MainMenu_3_2 onPageActiveChanged:"+pageActive, "isLinkedStartup:"+menu3_2.isLinkedStartup)
            menuModel.activate()
            menuModel.modbusGetValues(menuModel.curPageTimerDataList);
        } else {
            //console.log("MainMenu_3_2 onPageActiveChanged:"+pageActive)
            menu3_2.initProperty()
            menu3_2.initHoldSysProperty()
            menuModel.deactivate()
        }
    }

    Component.onDestruction: menuModel.deactivate()

    Language { id: language }

    Rectangle{
        id:menu3_2_0
        width: parent.width
        height: parent.height
        color: "transparent"
        border.width: 2
        border.color: "#999999"
        property int select:menu3_2.selectIndex
        property var timeraddr: menu3_2.menuModel.timerAddr
        property int timerNumber: menu3_2.menuModel.gettimerNum()
        property var timerDataList:menu3_2.menuModel.timerDataList

        ImageScrollBar{
            height: 440
            anchors.right: parent.right
            anchors.rightMargin: 15
            y:6
            property int maxpage:menu3_2_0.timerNumber%8 > 0 ? menu3_2_0.timerNumber/8+1 :menu3_2_0.timerNumber/8
            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
            pageLabal: menu3_2.curPage + "/" + maxpage
            onScrollUpClicked:{
                //console.log("onScrollUpClicked curPage:"+menu3_2.curPage, "curPageTimerDataList:"+menuModel.curPageTimerDataList)
                menuModel.clearTimerValueKeepStatus(menu3_2.curPage);
                if (menu3_2.curPage > 1) {
                    menuModel.curPageTimerDataList = menu3_2.curPage - 1
                } else {
                    menuModel.curPageTimerDataList = maxpage
                }
                if (menu3_2.targetCoopPage <= 0){
                    menuModel.selectIndexTimerDataList = 0;
                } else {
                    if (menu3_2.targetCoopPage !== menuModel.curPageTimerDataList){
                        menuModel.selectIndexTimerDataList = 0;
                    } else {
                        menuModel.selectIndexTimerDataList = menu3_2.targetCoopSelectIndex
                    }
                }
                menuModel.isLinkedStartup = false
                menuModel.modbusGetValues(menuModel.curPageTimerDataList);
            }
            onScrollDownClicked:{
                //console.log("onScrollDownClicked curPage:"+menu3_2.curPage, "curPageTimerDataList:"+menuModel.curPageTimerDataList)
                menuModel.clearTimerValueKeepStatus(menu3_2.curPage);
                if (menu3_2.curPage >= maxpage) {
                    menuModel.curPageTimerDataList = 1
                } else {
                    menuModel.curPageTimerDataList = menu3_2.curPage + 1
                }
                if (menu3_2.targetCoopPage <= 0){
                    menuModel.selectIndexTimerDataList = 0;
                } else {
                    if (menu3_2.targetCoopPage !== menuModel.curPageTimerDataList){
                        menuModel.selectIndexTimerDataList = 0;
                    } else {
                        menuModel.selectIndexTimerDataList = menu3_2.targetCoopSelectIndex
                    }
                }
                menuModel.isLinkedStartup = false
                menuModel.modbusGetValues(menuModel.curPageTimerDataList);
            }
        }
        Component{
            id:listdelegate_1
            Rectangle{
                x:1
                width: 352
                height: 56
                visible:(((menu3_2.curPage - 1)*8+index) < menu3_2_0.timerNumber) ? true : false
                color: "#eaeaea"
                border.color: "#999999"
                CheckBox{
                    width: 214
                    height: 50
                    x:8
                    anchors.verticalCenter: parent.verticalCenter
                    indicator:Image{
                        scale: 1.3
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        source: menu3_2_0.select === index+1 ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                    }

                    contentItem: Text{
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        property int timerIndex: (menu3_2.curPage-1)*8+index
                        text: (timerIndex < menu3_2_0.timerNumber) ? resourceManager.symbolAt(menu3_2_0.timeraddr[timerIndex]) + "\n" + resourceManager.getMessageStringValue("1", menu3_2_0.timeraddr[timerIndex] ,language.value) : ""
                        color: (timerIndex < menu3_2_0.timerNumber) ? resourceManager.rgb(32, 38 + timerIndex) : "transparent"
                        font.pixelSize: 16
                        leftPadding: 25
                        font.bold: true
                    }
                    onClicked: {
                        menuModel.isLinkedStartup = false
                        menuModel.isHoldingTimerValue = false
                        //console.log("listdelegate_1 checkbox select:"+menu3_2_0.select, "index:"+index, "curPage:"+menu3_2.curPage)
                        let selectIndex = menu3_2_0.select === index+1 ? menuModel.selectIndexTimerDataList = 0 : menuModel.selectIndexTimerDataList = index+1
                        menuModel.setCurSelectInfo(menu3_2.curPage, selectIndex);
                        return selectIndex;
                    }
                }
                LimitNumericEditBox{
                    id:time_item
                    width: 120
                    height: 40
                    x:215
                    anchors.verticalCenter: parent.verticalCenter
                    max: "99.99"
                    min: "0.00"
                    textColor:(menu3_2.isHoldingTimerValue === true && menu3_2.holdingTimerValueIndex === index) ? "red" :"black"
                    value: (menu3_2.isHoldingTimerValue === true && menu3_2.holdingTimerValueIndex === index) ? menu3_2.holdingTimerValue:
                            (((menu3_2.curPage - 1)*8+index) < menu3_2_0.timerNumber) ? resourceManager.formatData(menu3_2_0.timerDataList[index], 2) : ""
                    fontPixelSize: 18
                    borderColor:"#999999"
                    keyLayoutType:6
                    enabled: (menu3_2_0.select === index + 1)
                    onEnter: {
                        if (menu3_2_0.select === index + 1){
                            menuModel.keepTimerValue(menu3_2.curPage, index, newValue);
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                    }
                    onReady: {
                    }
                }
            }
        }
        ListView{
            width: 360
            height:442
            x:10
            y:6
            property int pg:menu3_2.curPage
            spacing: -1
            clip: true
            model: 8
            delegate: listdelegate_1
            interactive: false
            onPgChanged: {
                positionViewAtIndex(pg * 8, ListView.Beginning)
            }
            Component.onCompleted: {
                positionViewAtIndex(pg * 8, ListView.Beginning)
            }
        }
    }//计时器(タイマー)

    Connections {
        target:menuModel
        onIsReleaseHoldTimerValueChanged:{
            //console.log("MainMenu_3_2 onIsReleaseHoldTimerValueChanged isReleaseHoldTimerValue:"+isReleaseHoldTimerValue, "pageActive:"+menu3_2.pageActive)
            if (menu3_2.pageActive === true){
                menu3_2.initHoldSysProperty();
            }
        }
    }
}
