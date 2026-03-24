import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_1_4.qml
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
Rectangle {
    id:menu1_4
    width: parent.width
    height: parent.height
    color: "transparent"
    border.color: "#999999"
    border.width: 2
    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu1.subMenuList[3]
    property int editIndex: menuModel.editIndex//1-4数据编辑索引
    property var itemList: menuModel.itemList
    property int page: 0

    onPageActiveChanged: {
        if (pageActive) {
            menuModel.activate()
        } else {
            menuModel.deactivate()
        }
    }

    Component.onDestruction: menuModel.deactivate()

    Language { id: language }

    StackLayout{
        width:360
        height: 450
        x:5
        y:5
        currentIndex: menu1_4.page
        Repeater{
            model: 5
            Item {
                id:xx
                width: parent.width
                height: parent.height
                property int zz: index
                Column{
                    x:10
                    y:10
                    spacing: 4
                    Repeater{
                        model: 6
                        Rectangle{
                            id:base
                            property int editIndex: menu1_4.page * 6 + index
                            property var item: itemList[editIndex]
                            width: 358
                            height: 66
                            border.color: "#cccccc"
                            color: "#eaeaea"
                            CheckBox{
                                width: 34
                                height: 34
                                x:5
                                anchors.verticalCenter: parent.verticalCenter
                                indicator: Rectangle{
                                    width: parent.width
                                    height: parent.height
                                    border.color: "#999999"
                                    color: base.item.optional ? "white" : "#cccccc"
                                    Image {
                                        anchors.centerIn: parent
                                        visible: base.item.selectState
                                        source: base.item.optional ? resourceManager.imagePath + "/16_program/a160_80.png" : resourceManager.imagePath + "/16_program/a160_80.png"
                                    }
                                }
                                onClicked: {
                                    if(base.item.optional){
                                        if(base.item.selectState){
                                            menuModel.deselectItem(base.editIndex)
                                        }else{
                                            menuModel.selectItem(base.editIndex)
                                        }
                                    }else{
                                        return
                                    }
                                }
                            }
                            Rectangle{
                                width: 290
                                height: 56
                                x:45
                                anchors.verticalCenter: parent.verticalCenter
                                border.color: base.item.selectState ? "#999999" : "#999999"
                                color: menu1_4.editIndex == base.editIndex ? "#cc3333" : base.item.selectState ? "white" : "#cccccc"

                                Text{
                                    width: parent.width
                                    height: parent.height
                                    wrapMode: Text.WordWrap
                                    maximumLineCount: 2
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    leftPadding: 10
                                    font.pixelSize: lineCount>=2 ? 18 : 20
                                    text: {
                                        if (base.item.mark.length === 0) {
                                            return ""
                                        } else {
                                            let str =  base.item.mark + "(" + resourceManager.getMessageStringValue("1", base.item.msgLabelIndex ,language.value) + ")"
                                            str += base.item.value === 0 ? "OFF" : "ON"
                                        }
                                    }
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        if(base.item.selectState){
                                            menuModel.editIndex = base.editIndex
                                        }else{
                                            return
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

Rectangle {
        y:15
        anchors.right: parent.right
        anchors.rightMargin: 10
        width: 47
        height: 416
        color: "#EAEAEA"
        border.color: "#999999"

        ImageScrollBar {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: 410
            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
            pageLabal: menu1_4.page + 1 + "/" + 5
            onScrollUpClicked: menu1_4.page = Math.max(menu1_4.page - 1, 0)
            onScrollDownClicked: menu1_4.page = Math.min(menu1_4.page + 1, 4)
        }
}
}
