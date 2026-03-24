import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Prog_select_Dia.qml
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
Dialog {
    id:root
    width: 683
    height: 411
    focus: true
    modal : Qt. WindowModal
    closePolicy: Popup.NoAutoClose
    background: bac

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property var autoprogrealIndex: programViewModel.ncp.autoprogIndex
    property var manualprogrealIndex: programViewModel.ncp.manualprogIndex
    property var systemprogIndex: programViewModel.ncp.systemprogIndex
    property int addprogindex: -1
    signal progChanged()

    Image{
        id:bac
        source: resourceManager.imagePath + "/16_program/d161_97.png"
        //title bar
        Text{
            width: 683
            height: 53
            x:0
            y:0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 26
            color: "white"
            text: "Program_Select"
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
            x:635
            y:9
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
            downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
            onClicked: root.close()
        }
    }

    Image {
        x:-7
        y:48
        source: resourceManager.imagePath + "/16_program/d161_98.png"
        StackLayout{
            id:stack_prog
            width: parent.width
            height: parent.height-42
            y:43
            currentIndex: prog_tab.currentIndex

            Rectangle{
                id:auto_prog
                width: parent.width
                height: parent.height
                color: "transparent"
                property int page: 0

                Component{
                    id:listdelegate_1
                    Rectangle{
                        width: 170
                        height: 43
                        color: "#C1C2F4"
                        border.color: "#036EB8"
                        radius: 6
                        property int zzz: autoprogrealIndex[index]
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            leftPadding: 6
                            font.pixelSize: 20
                            color: resourceManager.rgb(41, 36 + parent.zzz)
                            text: resourceManager.label(41, 36 + parent.zzz, resourceManager.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onPressed:{
                                parent.color = "#FFDC2E"
                                bac.progChanged()
                            }
                            onReleased: {
                                parent.color = "#C1C2F4"
                            }
                            onClicked: {
                                programViewModel.progMenu4.selectProgIndex = autoprogrealIndex[index]
                                root.close();
                            }
                        }
                    }
                }
                GridView{
                    id:autoprogview
                    width: 526
                    height: 294
                    property int pg: auto_prog.page
                    x:6
                    y:6
                    cellWidth: 176
                    cellHeight: 49
                    interactive: false
                    clip: true
                    flow:GridView.FlowTopToBottom
                    model:programViewModel.ncp.autoProgNum
                    delegate: listdelegate_1
                    onPgChanged: {
                        positionViewAtIndex(pg*18,GridView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg*18, GridView.Beginning)
                    }
                }
                Rectangle {
                    width: 128
                    height: 340
                    x:542
                    y:-40
                    radius: 6
                    color: "#8faadc"
                    Rectangle {
                        width: 20
                        height: 340
                        color: "#8faadc"
                        anchors.left: parent.left
                    }
                    //切换按钮
                    ImageScrollBar{
                        height: 200
                        anchors.horizontalCenter: parent.horizontalCenter
                        y:135
                        property int zz: (autoprogview.model/18) + 1

                        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                        pageLabal: auto_prog.page+1 + "/" + zz
                        onScrollUpClicked:auto_prog.page = Math.max(auto_prog.page-1 ,0)
                        onScrollDownClicked:auto_prog.page = Math.min(auto_prog.page+1, zz-1)
                    }
                }//右侧菜单栏 우측메뉴표시줄
            }//自动运行 자동운전

            Rectangle{
                id:manual_prog
                width: parent.width
                height: parent.height
                color: "transparent"
                property int page: 0

                Component{
                    id:listdelegate_2
                    Rectangle{
                        width: 170
                        height: 43
                        color: "#C1C2F4"
                        border.color: "#036EB8"
                        radius: 6
                        property int zzzz: manualprogrealIndex[index]
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            leftPadding: 6
                            font.pixelSize: 20
                            color: resourceManager.rgb(41, 36 + parent.zzzz)
                            text: resourceManager.label(41, 36 + parent.zzzz, resourceManager.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.color = "#FFDC2E"
                                bac.progChanged()
                            }
                            onReleased: {
                                parent.color = "#C1C2F4"
                            }
                            onClicked: {
                                programViewModel.progMenu4.selectProgIndex = manualprogrealIndex[index]
                                root.close();
                            }
                        }
                    }
                }
                GridView{
                    id:manualprogview
                    width: 526
                    height: 294
                    property int pg: manual_prog.page
                    x:6
                    y:6
                    cellWidth: 176
                    cellHeight: 49
                    interactive: false
                    clip: true
                    flow:GridView.FlowTopToBottom
                    model:programViewModel.ncp.manualProgNum
                    delegate: listdelegate_2
                    onPgChanged: {
                        positionViewAtIndex(pg*18,GridView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg*18, GridView.Beginning)
                    }
                }
                Rectangle {
                    width: 128
                    height: 340
                    x:542
                    y:-40
                    radius: 6
                    color: "#8faadc"
                    Rectangle {
                        width: 20
                        height: 340
                        color: "#8faadc"
                        anchors.left: parent.left
                    }
                    //切换按钮
                    ImageScrollBar{
                        height: 200
                        anchors.horizontalCenter: parent.horizontalCenter
                        y:135
                        property int zz: (manualprogview.model/19) + 1

                        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                        pageLabal: manual_prog.page+1 + "/" + zz
                        onScrollUpClicked:manual_prog.page = Math.max(manual_prog.page-1 ,0)
                        onScrollDownClicked:manual_prog.page = Math.min(manual_prog.page+1, zz-1)
                    }
                }//右侧菜单栏
            }//手动操作

            Rectangle{
                id:system_prog
                width: parent.width
                height: parent.height
                color: "transparent"
                property int page: 0

                Component{
                    id:listdelegate_3
                    Rectangle{
                        width: 170
                        height: 43
                        color: "#C1C2F4"
                        border.color: "#036EB8"
                        radius: 6
                        property int zzzzz: systemprogIndex[index]
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            leftPadding: 6
                            font.pixelSize: 20
                            color: resourceManager.rgb(41, 36+parent.zzzzz)
                            text: resourceManager.label(41, 36+parent.zzzzz, resourceManager.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onPressed:{
                                parent.color = "#FFDC2E"
                                bac.progChanged()
                            }
                            onReleased: {
                                parent.color = "#C1C2F4"
                            }
                            onClicked: {
                                programViewModel.progMenu4.selectProgIndex = systemprogIndex[index]
                                root.close();
                            }
                        }
                    }
                }
                GridView{
                    id:sysprogview
                    width: 526
                    height: 294
                    property int pg: system_prog.page
                    x:6
                    y:6
                    cellWidth: 176
                    cellHeight: 49
                    interactive: false
                    clip: true
                    flow:GridView.FlowTopToBottom
                    model:programViewModel.ncp.systemProgNum
                    delegate: listdelegate_3
                    onPgChanged: {
                        positionViewAtIndex(pg*18,GridView.Beginning)
                    }
                    Component.onCompleted: {
                        positionViewAtIndex(pg*18, GridView.Beginning)
                    }
                }
                Rectangle {
                    width: 128
                    height: 340
                    x:542
                    y:-40
                    radius: 6
                    color: "#8faadc"
                    Rectangle {
                        width: 20
                        height: 340
                        color: "#8faadc"
                        anchors.left: parent.left
                    }
                    //切换按钮
                    ImageScrollBar{
                        height: 200
                        anchors.horizontalCenter: parent.horizontalCenter
                        y:135
                        property int zz: (sysprogview.model/19) + 1

                        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                        pageLabal: system_prog.page+1 + "/" + zz
                        onScrollUpClicked:system_prog.page = Math.max(system_prog.page-1 ,0)
                        onScrollDownClicked:system_prog.page = Math.min(system_prog.page+1, zz-1)
                    }
                }//右侧菜单栏
            }//系统程序
        }
    }

    TabBar{
        id:prog_tab
        width: 335
        height: 42
        x:-7
        y:48
        spacing: 10
        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
        }
        TabButton{
            background: Image {
                source: (prog_tab.currentIndex  == 0) ? resourceManager.imagePath + "/16_program/d161_100.png" : resourceManager.imagePath + "/16_program/d161_99.png"
            }
            Text{
                anchors.centerIn: parent
                font.pixelSize: 20
                text: resourceManager.label(41,604, resourceManager.language)
                color: resourceManager.rgb(41,604)
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    prog_tab.currentIndex = 0
                }
            }
        }
        TabButton{
            background: Image {
                source: (prog_tab.currentIndex  == 1) ? resourceManager.imagePath + "/16_program/d161_100.png" : resourceManager.imagePath + "/16_program/d161_99.png"
            }
            Text{
                anchors.centerIn: parent
                font.pixelSize: 20
                text: resourceManager.label(41,605, resourceManager.language)
                color: resourceManager.rgb(41,605)
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    prog_tab.currentIndex = 1
                }
            }
        }
        TabButton{
            background: Image {
                source: (prog_tab.currentIndex  == 2) ? resourceManager.imagePath + "/16_program/d161_100.png" : resourceManager.imagePath + "/16_program/d161_99.png"
            }
            Text{
                anchors.centerIn: parent
                font.pixelSize: 20
                text: resourceManager.label(41,606, resourceManager.language)
                color: resourceManager.rgb(41,606)
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    prog_tab.currentIndex = 2
                }
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
