import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_5_2.qml
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
    id:menu5_2

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property int menuIndex: programViewModel.progMenu5.Menu5_Index
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu5.subMenuList[1]
    property int editIndex: menuModel.editIndex
    property var itemList: menuModel.itemList

    onPageActiveChanged: {
        if (pageActive) {
            //console.log("MainMenu_5_2 onPageActiveChanged:"+pageActive)
            menuModel.activate()
        } else {
            //console.log("MainMenu_5_2 onPageActiveChanged:"+pageActive)
            menuModel.deactivate()
        }
    }

    Component.onDestruction: menuModel.deactivate()

    Language { id: language }

    Rectangle{
        id:menu5_2_0
        width: parent.width
        height: parent.height
        color: "transparent"
        border.width: 2
        border.color: "#999999"
        property int page: 0

        Rectangle {
            y:15
            anchors.right: parent.right
            anchors.rightMargin: 10
            width: 47
            height: 417
            color: "#eaeaea"
            border.color: "#999999"
        ImageScrollBar{
            anchors.fill: parent
            anchors.margins: 3
            height: 417
            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
            pageLabal: menu5_2_0.page+1 + "/" + 5
            onScrollUpClicked:menu5_2_0.page = Math.max(menu5_2_0.page-1 ,0)
            onScrollDownClicked:menu5_2_0.page = Math.min(menu5_2_0.page+1, 4)
        }
        }

        StackLayout{
            width:360
            height: 450
            x:5
            y:5
            currentIndex: menu5_2_0.page
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
                                id:base1
                                property int editIndex:menu5_2_0.page * 6 + index
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
                                        color: base1.item.optional ? "white" : "#cccccc"
                                        Image {
                                            anchors.centerIn: parent
                                            visible: base1.item.selectState
                                            source: base1.item.optional ? resourceManager.imagePath + "/16_program/a160_80.png" : resourceManager.imagePath + "/16_program/a160_80.png"
                                        }
                                    }
                                    onClicked: {
                                        if(base1.item.optional){
                                            if(base1.item.selectState){
                                                menuModel.deselectItem(base1.editIndex)
                                            }else{
                                                menuModel.selectItem(base1.editIndex)
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
                                    border.color: base1.item.selectState ? "#999999" : "#999999"
                                    color: menu5_2.editIndex == base1.editIndex ? "#cc3333" : base1.item.selectState ? "white" : "#cccccc"
                                    Text{
                                        width: parent.width
                                        height: parent.height
                                        wrapMode: Text.WordWrap
                                        maximumLineCount: 2
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignLeft
                                        leftPadding: 15
                                        minimumPixelSize: 18
                                        font.pixelSize:(lineCount>=2 ? 18 : 20)
                                        text: {
                                            if (base1.item.mark.length === 0) {
                                                return ""
                                            } else {
                                                let str = base1.item.mark + "(" + resourceManager.getMessageStringValue("1", base1.item.msgLabelIndex ,language.value) + ")" + base1.item.opeStr
                                                str += (base1.item.opeStr === "") ? (base1.item.value === 0 ? "OFF" : "ON") : String(base1.item.value)
                                            }
                                        }
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked: {
                                            if(base1.item.selectState){
                                                menuModel.editIndex = base1.editIndex
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
    }//内部输出
}
