import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Select_Variable_Dialog.qml
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
/******选择变量*****/
Dialog {
    id:root
    closePolicy: Popup.CloseOnEscape

    property int menu_index: 0
    property var menu_state: variableViewmodel.menu_State
    property var menu1Mark: variableViewmodel.menu1_Mark
    property var menu1Msg: variableViewmodel.menu1_Msg
    property var menu2Mark: variableViewmodel.menu2_Mark
    property var menu2Msg: variableViewmodel.menu2_Msg
    property var menu3Mark: variableViewmodel.menu3_Mark
    property var menu3Msg: variableViewmodel.menu3_Msg
    property var menu4Mark: variableViewmodel.menu4_Mark
    property var menu4Msg: variableViewmodel.menu4_Msg
    property var menu5Mark: variableViewmodel.menu5_Mark
    property var menu5Msg: variableViewmodel.menu5_Msg
    property var menu6Mark: variableViewmodel.menu6_Mark
    property var menu6Msg: variableViewmodel.menu6_Msg
    property var menu7Mark: variableViewmodel.menu7_Mark
    property var menu7Msg: variableViewmodel.menu7_Msg

    background: bac
    Image{
        id:bac
        source: resourceManager.imagePath + "/08_MENU/d084_03.png"
        Text{
            width: 550
            height: 52
            x:0
            y:0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 26
            color: resourceManager.rgb(20, 29)
            text: resourceManager.label(20,29, resourceManager.language)
            MouseArea{
                anchors.fill: parent
                property point clickpoint: "0,0"
                onPressed: {
                    clickpoint = Qt.point(mouse.x,mouse.y)
                }
                onPositionChanged: {
                    var offset = Qt.point(mouse.x-clickpoint.x,mouse.y-clickpoint.y)
                    setDlgPoint(offset.x, offset.y)
                }
            }
        }
        ImageButton{
            x:500
            y:5
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
            downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
            onClicked: root.close()
        }
    }

    //框线
    Rectangle{
        width: 548
        height: 1
        x:-8
        y:356
        color: "#ffffff"
    }

    Rectangle{
        width: 120
        height: 306
        x:-4
        y:46
        color: "transparent"
        border.color: "#999999"
        Column{
            anchors.centerIn: parent
            spacing: 4
            Repeater{
                model: 7
                Rectangle{
                    width: 110
                    height: 39
                    color: (menu_index == index && menu_state[index] == 1) ? "white"
                                               : menu_state[index] == 0 ? "#cccccc" : "#cc3333"
                    border.color: "#999999"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 20
                        color: resourceManager.rgb(20, 30+index)
                        text: resourceManager.label(20,30+index, resourceManager.language)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if(menu_state[index] == 1){
                                menu_index = index
                            }else{
                                return
                            }
                        }
                    }
                }
            }
        }

    }//菜单栏
    Rectangle{
        width: 414
        height: 307
        x:120
        y:46
        color: "transparent"
        border.color: "#999999"
        StackLayout{
            width: parent.width-2
            height: parent.height-2
            anchors.centerIn: parent
            currentIndex: menu_index
            Rectangle{
                id:input
                width: parent.width
                height: parent.height
                color: "transparent"
                property int select_index: -1
                property int page: 0

                //切换按钮
                ImageScrollBar{
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    property int zz: (input_view.model/9) + 1

                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: input.page+1 + "/" + zz
                    onScrollUpClicked:input.page = Math.max(input.page-1 ,0)
                    onScrollDownClicked:input.page = Math.min(input.page+1, zz-1)
                }
                Component{
                    id:listdelegate_1
                    Row{
                        spacing: 4
                        Rectangle{
                            width: 50
                            height: 34
                            border.width: input.select_index == index ? 2 : 1
                            border.color:input.select_index == index ? "#cc3333" : "#cccccc"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: input.select_index == index ? input.select_index = -1 : input.select_index = index
                            }
                        }
                        Rectangle{
                            width: 290
                            height: 34
                            border.width: input.select_index == index ? 2 : 1
                            border.color:input.select_index == index ? "#cc3333" : "#cccccc"
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#999999"
                            }

                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: menu1Mark[index]
                            }
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: menu1Msg[index]
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: input.select_index == index ? input.select_index = -1 : input.select_index = index
                            }
                        }
                    }
                }
                ListView{
                    id:input_view
                    width: 346
                    height: 300
                    x:2
                    y:2
                    property int pg: parent.page
                    spacing: 4
                    clip: true
                    interactive: false
                    model: variableViewmodel.menu1_Num
                    delegate: listdelegate_1
                    onPgChanged: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                }
            }//输入

            Rectangle{
                id:output
                width: parent.width
                height: parent.height
                color: "transparent"
                property int select_index: -1
                property int page: 0

                //切换按钮
                ImageScrollBar{
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    property int zz: (output_view.model/9) + 1

                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: output.page+1 + "/" + zz
                    onScrollUpClicked:output.page = Math.max(output.page-1 ,0)
                    onScrollDownClicked:output.page = Math.min(output.page+1, zz-1)
                }
                Component{
                    id:listdelegate_2
                    Row{
                        spacing: 4
                        Rectangle{
                            width: 50
                            height: 34
                            border.width: output.select_index == index ? 2 : 1
                            border.color:output.select_index == index ? "#cc3333" : "#cccccc"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: output.select_index == index ? output.select_index = -1 : output.select_index = index
                            }
                        }
                        Rectangle{
                            width: 290
                            height: 34
                            border.width: output.select_index == index ? 2 : 1
                            border.color:output.select_index == index ? "#cc3333" : "#cccccc"
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#999999"
                            }

                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: menu2Mark[index]
                            }
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: menu2Msg[index]
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: output.select_index == index ? output.select_index = -1 : output.select_index = index
                            }
                        }
                    }
                }
                ListView{
                    id:output_view
                    width: 346
                    height: 300
                    x:2
                    y:2
                    property int pg: parent.page
                    spacing: 4
                    clip: true
                    interactive: false
                    model: variableViewmodel.menu2_Num
                    delegate: listdelegate_2
                    onPgChanged: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                }
            }//输出

            Rectangle{
                id:timer
                width: parent.width
                height: parent.height
                color: "transparent"
                property int select_index: -1
                property int page: 0

                //切换按钮
                ImageScrollBar{
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    property int zz: (timer_view.model/9) + 1

                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: timer.page+1 + "/" + zz
                    onScrollUpClicked:timer.page = Math.max(timer.page-1 ,0)
                    onScrollDownClicked:timer.page = Math.min(timer.page+1, zz-1)
                }
                Component{
                    id:listdelegate_3
                    Row{
                        spacing: 4
                        Rectangle{
                            width: 50
                            height: 34
                            border.width: timer.select_index == index ? 2 : 1
                            border.color:timer.select_index == index ? "#cc3333" : "#cccccc"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: timer.select_index == index ? timer.select_index = -1 : timer.select_index = index
                            }
                        }
                        Rectangle{
                            width: 290
                            height: 34
                            border.width: timer.select_index == index ? 2 : 1
                            border.color:timer.select_index == index ? "#cc3333" : "#cccccc"
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#999999"
                            }

                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: menu3Mark[index]
                            }
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: menu3Msg[index]
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: timer.select_index == index ? timer.select_index = -1 : timer.select_index = index
                            }
                        }
                    }
                }
                ListView{
                    id:timer_view
                    width: 346
                    height: 300
                    x:2
                    y:2
                    property int pg: parent.page
                    spacing: 4
                    clip: true
                    interactive: false
                    model: variableViewmodel.menu3_Num
                    delegate: listdelegate_3
                    onPgChanged: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                }
            }//计时器

            Rectangle{
                id:count
                width: parent.width
                height: parent.height
                color: "transparent"
                property int select_index: -1
                property int page: 0

                //切换按钮
                ImageScrollBar{
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    property int zz: (count_view.model/9) + 1

                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: count.page+1 + "/" + zz
                    onScrollUpClicked:count.page = Math.max(count.page-1 ,0)
                    onScrollDownClicked:count.page = Math.min(count.page+1, zz-1)
                }
                Component{
                    id:listdelegate_4
                    Row{
                        spacing: 4
                        Rectangle{
                            width: 50
                            height: 34
                            border.width: count.select_index == index ? 2 : 1
                            border.color:count.select_index == index ? "#cc3333" : "#cccccc"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: count.select_index == index ? count.select_index = -1 : count.select_index = index
                            }
                        }
                        Rectangle{
                            width: 290
                            height: 34
                            border.width: count.select_index == index ? 2 : 1
                            border.color:count.select_index == index ? "#cc3333" : "#cccccc"
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#999999"
                            }

                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: menu4Mark[index]
                            }
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: menu4Msg[index]
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: count.select_index == index ? count.select_index = -1 : count.select_index = index
                            }
                        }
                    }
                }
                ListView{
                    id:count_view
                    width: 346
                    height: 300
                    x:2
                    y:2
                    property int pg: parent.page
                    spacing: 4
                    clip: true
                    interactive: false
                    model: variableViewmodel.menu4_Num
                    delegate: listdelegate_4
                    onPgChanged: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                }
            }//计数器

            Rectangle{
                id:axisControl
                width: parent.width
                height: parent.height
                color: "transparent"
                property int select_index: -1
                property int page: 0

                //切换按钮
                ImageScrollBar{
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    property int zz: (axisControl_view.model/9) + 1

                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: axisControl.page+1 + "/" + zz
                    onScrollUpClicked:axisControl.page = Math.max(axisControl.page-1 ,0)
                    onScrollDownClicked:axisControl.page = Math.min(axisControl.page+1, zz-1)
                }
                Component{
                    id:listdelegate_5
                    Row{
                        spacing: 4
                        Rectangle{
                            width: 50
                            height: 34
                            border.width: axisControl.select_index == index ? 2 : 1
                            border.color:axisControl.select_index == index ? "#cc3333" : "#cccccc"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: axisControl.select_index == index ? axisControl.select_index = -1 : axisControl.select_index = index
                            }
                        }
                        Rectangle{
                            width: 290
                            height: 34
                            border.width: axisControl.select_index == index ? 2 : 1
                            border.color:axisControl.select_index == index ? "#cc3333" : "#cccccc"
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#999999"
                            }

                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: menu5Mark[index]
                            }
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: menu5Msg[index]
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: axisControl.select_index == index ? axisControl.select_index = -1 : axisControl.select_index = index
                            }
                        }
                    }
                }
                ListView{
                    id:axisControl_view
                    width: 346
                    height: 300
                    x:2
                    y:2
                    property int pg: parent.page
                    spacing: 4
                    clip: true
                    interactive: false
                    model: variableViewmodel.menu5_Num
                    delegate: listdelegate_5
                    onPgChanged: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                }
            }//轴控制

            Rectangle{
                id:user
                width: parent.width
                height: parent.height
                color: "transparent"
                property int select_index: -1
                property int page: 0

                //切换按钮
                ImageScrollBar{
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    property int zz: (user_view.model/9) + 1

                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: user.page+1 + "/" + zz
                    onScrollUpClicked:user.page = Math.max(user.page-1 ,0)
                    onScrollDownClicked:user.page = Math.min(user.page+1, zz-1)
                }
                Component{
                    id:listdelegate_6
                    Row{
                        spacing: 4
                        Rectangle{
                            width: 50
                            height: 34
                            border.width: user.select_index == index ? 2 : 1
                            border.color:user.select_index == index ? "#cc3333" : "#cccccc"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: user.select_index == index ? user.select_index = -1 : user.select_index = index
                            }
                        }
                        Rectangle{
                            width: 290
                            height: 34
                            border.width: user.select_index == index ? 2 : 1
                            border.color:user.select_index == index ? "#cc3333" : "#cccccc"
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#999999"
                            }

                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: menu6Mark[index]
                            }
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: menu6Msg[index]
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: user.select_index == index ? user.select_index = -1 : user.select_index = index
                            }
                        }
                    }
                }
                ListView{
                    id:user_view
                    width: 346
                    height: 300
                    x:2
                    y:2
                    property int pg: parent.page
                    spacing: 4
                    clip: true
                    interactive: false
                    model: variableViewmodel.menu6_Num
                    delegate: listdelegate_6
                    onPgChanged: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                }
            }//用户

            Rectangle{
                id:system
                width: parent.width
                height: parent.height
                color: "transparent"
                property int select_index: -1
                property int page: 0

                //切换按钮
                ImageScrollBar{
                    height: 306
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    property int zz: (system_view.model/9) + 1

                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: system.page+1 + "/" + zz
                    onScrollUpClicked:system.page = Math.max(system.page-1 ,0)
                    onScrollDownClicked:system.page = Math.min(system.page+1, zz-1)
                }
                Component{
                    id:listdelegate_7
                    Row{
                        spacing: 4
                        Rectangle{
                            width: 50
                            height: 34
                            border.width: system.select_index == index ? 2 : 1
                            border.color:system.select_index == index ? "#cc3333" : "#cccccc"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: system.select_index == index ? system.select_index = -1 : system.select_index = index
                            }
                        }
                        Rectangle{
                            width: 290
                            height: 34
                            border.width: system.select_index == index ? 2 : 1
                            border.color:system.select_index == index ? "#cc3333" : "#cccccc"
                            Rectangle{
                                width: 1
                                height: parent.height-4
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 100
                                color: "#999999"
                            }

                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 5
                                font.pixelSize: 18
                                text: menu7Mark[index]
                            }
                            Text{
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 105
                                font.pixelSize: 18
                                text: menu7Msg[index]
                            }

                            MouseArea{
                                anchors.fill: parent
                                onClicked: system.select_index == index ? system.select_index = -1 : system.select_index = index
                            }
                        }
                    }
                }
                ListView{
                    id:system_view
                    width: 346
                    height: 300
                    x:2
                    y:2
                    property int pg: parent.page
                    spacing: 4
                    clip: true
                    interactive: false
                    model: variableViewmodel.menu7_Num
                    delegate: listdelegate_7
                    onPgChanged: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg * 8, ListView.Beginning)
                    }
                }
            }//系统
        }
    }//内容栏

    Row{
        x:310
        y:360
        spacing: 20
        ImageButton{
            upImageSource: resourceManager.imagePath + "/08_MENU/d084_06.png"
            downImageSource: resourceManager.imagePath + "/08_MENU/d084_07.png"
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                font.pixelSize: 20
                text: "取消"
            }
        }
        ImageButton{
            upImageSource: resourceManager.imagePath + "/08_MENU/d084_04.png"
            downImageSource: resourceManager.imagePath + "/08_MENU/d084_05.png"
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                font.pixelSize: 20
                text: "确定"
            }
            onClicked:{
                root.close()
                programViewModel.progMenu4.currenteditIndex++
            }
        }
    }

    function setDlgPoint(dlgX ,dlgY)
     {
         //设置窗口拖拽不能超过父窗口
         if(root.x + dlgX < 0){
             root.x = 0
         }else if(root.x + dlgX > root.parent.width - root.width){
             root.x = root.parent.width - root.width
         }else{
             root.x = root.x + dlgX
         }

         if(root.y + dlgY < 0){
             root.y = 0
         }else if(root.y + dlgY > root.parent.height - root.height){
             root.y = root.parent.height - root.height
         }else{
             root.y = root.y + dlgY
         }
     }
}
