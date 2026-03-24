import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../../Widget"
import Enums 1.0
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Variable_Dia.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.21
* Worker: SH.HONG
**************************************************************************************/
/******选择变量*****/
Dialog {
    id:root
    width: 600
    height: 433
    closePolicy: Popup.CloseOnEscape

    signal enter(string newValue, string symbol, string msg, string operator, int value, int addr); // ToDo: newValue, msgは削除予定.

    property int menu_index: variableViewmodel.menuIndex
    property var menu_state: variableViewmodel.menu_State
    property int variable1Selectindex: variableViewmodel.variable1SelectIndex
    property int variable2_1Selectindex: variableViewmodel.variable2_1SelectIndex
    property int variable2_2Selectindex: variableViewmodel.variable2_2SelectIndex
    property int variable3Selectindex: variableViewmodel.variable3SelectIndex
    property int variable4Selectindex: variableViewmodel.variable4SelectIndex
    property int variable5Selectindex: variableViewmodel.variable5SelectIndex
    property int variable6_1Selectindex: variableViewmodel.variable6_1SelectIndex
    property int variable6_2Selectindex: variableViewmodel.variable6_2SelectIndex
    property int variable7_1Selectindex: variableViewmodel.variable7_1SelectIndex
    property int variable7_2Selectindex: variableViewmodel.variable7_2SelectIndex
    property int variable8_1Selectindex: variableViewmodel.variable8_1SelectIndex
    property int variable8_2Selectindex: variableViewmodel.variable8_2SelectIndex
    property var menu1Addr: variableViewmodel.menu1Addr
    property var menu2_1Addr: variableViewmodel.menu2_1Addr
    property var menu2_2Addr: variableViewmodel.menu2_2Addr
    property var menu3Addr: variableViewmodel.menu3Addr
    property var menu4Addr: variableViewmodel.menu4Addr
    property var menu5Addr: variableViewmodel.menu5Addr
    property var menu6_1Addr: variableViewmodel.menu6_1Addr
    property var menu6_2Addr: variableViewmodel.menu6_2Addr
    property var menu7_1Addr: variableViewmodel.menu7_1Addr
    property var menu7_2Addr: variableViewmodel.menu7_2Addr
    property var menu8_1Addr: variableViewmodel.menu8_1Addr
    property var menu8_2Addr: variableViewmodel.menu8_2Addr
    property var menu1_state: variableViewmodel.menu1_State
    property var menu2_1_state: variableViewmodel.menu2_1_State
    property var menu3_state: variableViewmodel.menu3_State
    property var menu4_state: variableViewmodel.menu4_State
    property var menu5_state: variableViewmodel.menu5_State
    property var menu6_1_state: variableViewmodel.menu6_1_State
    property var menu7_1_state: variableViewmodel.menu7_1_State
    property var menu8_1_state: variableViewmodel.menu8_1_State
    property var menu6_2_symbol: variableViewmodel.menu6_2_Symbol//0:=; 1:≠; 2:<; 3:>;
    property var menu7_2_symbol: variableViewmodel.menu7_2_Symbol//0:=; 1:≠; 2:<; 3:>;
    property var menu8_2_symbol: variableViewmodel.menu8_2_Symbol//0:=; 1:≠; 2:<; 3:>;

    Language { id: language }
    // 완전 전체 배경
    background: Rectangle{
        id:bac
        width: 600
        height: 433
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        // 제목 타이틀
        Rectangle{
            width: 600
            height: 50
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                leftPadding: 2
                font.pixelSize: 22
                color: resourceManager.rgb(41, 465)
                text: resourceManager.label(41,465, language.value)
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
        }
        ImageButton{
            x:530
            y:9
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
            downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
            onClicked: root.close()
        }
    }

    // 좌측 리스트 백그라운드
    Rectangle{
        width: 120
        height: 330
        x:-7
        y:41
        color: "transparent"
        border.color: "#999999"
        Column{
            anchors.centerIn: parent
            spacing: 2
            Repeater{
                model: 8
                Rectangle{
                    width: 114
                    height: 39
                    color:(menu_index == index && menu_state[index] == 1) ? "white"
                                                                          : menu_state[index] == 0 ? "#cccccc" : "#cccccc"
                    border.color: "#999999"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 2

                        font.pixelSize: 18
                        color: resourceManager.rgb(41, 466+index)
                        text: resourceManager.label(41,466+index, language.value)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if(menu_state[index]){
                                variableViewmodel.menuIndex = index
                            }else{
                                return
                            }
                        }
                    }
                }
            }
        }
    }//菜单栏
    // 우측 리스트 전체
    StackLayout{
        width: 456
        height: 330
        x:117
        y:41
        clip: true
        currentIndex: menu_index
        //输入
        Component {
            id: component_variable1
            Item{
                property alias listdelegate_1: inner_listdelegate_1
                property alias input_view: inner_input_view
                id:variable1
                width: parent.width
                height: parent.height
                property int page: 0
                ImageScrollBar{
                    height: 326
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    property int zz: input_view.model%8 >0 ? input_view.model/8+1 : input_view.model/8
                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: variable1.page+1 + "/" + zz
                    onScrollUpClicked:variable1.page = Math.max(variable1.page-1 ,0)
                    onScrollDownClicked:variable1.page = Math.min(variable1.page+1, zz-1)
                }//切换按钮
                Component{
                    id:inner_listdelegate_1
                    Row{
                        x:1
                        spacing: 2
                        Rectangle{
                            width: 39
                            height: 39
                            color: "#ffffff"
                            border.color:variable1Selectindex == index ? "#999999" : "#999999"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    variable1Selectindex == index ? variableViewmodel.variable1SelectIndex = -1 : variableViewmodel.variable1SelectIndex = index
                                    variableViewmodel.setVariableStr(menu_index)
                                }
                            }
                        }
                        Rectangle{
                            width: 354
                            height: 39
                            color: "#ffffff"
                            border.color:variable1Selectindex == index ? "#999999" : "#999999"
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.symbolAt(menu1Addr[index])
                            }
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                y:19
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.getMessageStringValue("1", menu1Addr[index], language.value)
                            }
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 200

                                visible: variable1Selectindex == index
                                color:menu1_state[variable1Selectindex] == 1 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"ON"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu1_State[variable1Selectindex] = 1
                                }
                            }
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 275

                                visible: variable1Selectindex == index
                                color:menu1_state[variable1Selectindex] == 0 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"OFF"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu1_State[variable1Selectindex] = 0
                                }
                            }
                            MouseArea{
                                width: variable1Selectindex == index ? 180 : parent.width
                                height: parent.height
                                onClicked:{
                                    variable1Selectindex == index ? variableViewmodel.variable1SelectIndex = -1 : variableViewmodel.variable1SelectIndex = index
                                }
                            }
                        }
                    }
                }
                ListView{
                    id:inner_input_view
                    width: 408
                    height: 326
                    y:2
                    property int pg: parent.page
                    spacing: 2
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
            }
        }
        Loader {
            id: loader_variable1
            asynchronous: (menu_index !== 0)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable1
        }

        //内部输出
        Component {
            id: component_variable2
            Item{
                property alias variable2_1: inner_variable2_1
                property alias listdelegate_2_1: inner_listdelegate_2_1
                property alias variable2_1_view: inner_variable2_1_view
                property alias variable2_2: inner_variable2_2
                property alias listdelegate_2_2: inner_listdelegate_2_2
                property alias variable2_2_view: inner_variable2_2_view
                id:variable2
                width: parent.width
                height: parent.height
                property bool type: variableViewmodel.menu2_Type
                Rectangle{
                    x : 1
                    width: parent.width
                    height: 40
                    color: "#cccccc"
                    border.color: "#999999"
                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        font.pixelSize: 18
                        color: resourceManager.rgb(41, 476)
                        text: resourceManager.label(41,476, language.value)
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 130
                        color:variable2.type == 0 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 477)
                            text: resourceManager.label(41,477, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu2_Type = 0
                        }
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 240

                        color:variable2.type == 1 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 478)
                            text: resourceManager.label(41,478, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu2_Type = 1
                        }
                    }
                }
                StackLayout{
                    width: parent.width
                    height: 288
                    y:41
                    clip: true
                    currentIndex: variable2.type
                    Item{
                        id:inner_variable2_1
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 284
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 2
                            property int zz: variable2_1_view.model%7 >0 ? variable2_1_view.model/7+1 : variable2_1_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable2_1.page+1 + "/" + zz
                            onScrollUpClicked:variable2_1.page = Math.max(variable2_1.page-1 ,0)
                            onScrollDownClicked:variable2_1.page = Math.min(variable2_1.page+1, zz-1)
                        }
                        Component{
                            id:inner_listdelegate_2_1
                            Row{
                                spacing: 4
                                x: 1
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable2_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 18
                                        text: index + 1
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked:variable2_1Selectindex == index ? variableViewmodel.variable2_1SelectIndex = -1 : variableViewmodel.variable2_1SelectIndex = index
                                    }
                                }
                                Rectangle{
                                    width: 340
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable2_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.symbolAt(menu2_1Addr[index])
                                    }
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        y:19
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.getMessageStringValue("1", menu2_1Addr[index], language.value)
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 181

                                        visible: variable2_1Selectindex == index
                                        color:menu2_1_state[index] == 1 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"ON"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu2_1_State[index] = 1
                                        }
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 255

                                        visible: variable2_1Selectindex == index
                                        color:menu2_1_state[index] == 0 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"OFF"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu2_1_State[index] = 0
                                        }
                                    }
                                    MouseArea{
                                        width: variable2_1Selectindex == index ? 180 : parent.width
                                        height: parent.height
                                        onClicked:variable2_1Selectindex == index ? variableViewmodel.variable2_1SelectIndex = -1 : variableViewmodel.variable2_1SelectIndex = index
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable2_1_view
                            width: 394
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu2_1_Num
                            delegate: listdelegate_2_1
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//位
                    Item{
                        id:inner_variable2_2
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 284
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 2
                            property int zz: variable2_2_view.model%7 >0 ? variable2_2_view.model/7+1 : variable2_2_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable2_2.page+1 + "/" + zz
                            onScrollUpClicked:variable2_2.page = Math.max(variable2_2.page-1 ,0)
                            onScrollDownClicked:variable2_2.page = Math.min(variable2_2.page+1, zz-1)
                        }
                        Component{
                            id:inner_listdelegate_2_2
                            Row{
                                spacing: 2
                                Row{
                                    spacing: 2
                                    Rectangle{
                                        width: 48
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable2_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 18
                                            text: index + 1
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked:variable2_2Selectindex == index ? variableViewmodel.variable2_2SelectIndex = -1 : variableViewmodel.variable2_2SelectIndex = index
                                        }
                                    }
                                    Rectangle{
                                        width: 210
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable2_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.symbolAt(menu2_2Addr[index])
                                        }
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            y:19
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.getMessageStringValue("1", menu2_2Addr[index], language.value)
                                        }
                                        MouseArea{
                                            width: variable2_2Selectindex == index ? 180 : parent.width
                                            height: parent.height
                                            onClicked:variable2_2Selectindex == index ? variableViewmodel.variable2_2SelectIndex = -1 : variableViewmodel.variable2_2SelectIndex = index
                                        }
                                    }
                                }
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#999999"
                                    border.color:"#999999"
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 20
                                        text: "="
                                    }
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 39
                                    color: enabled == false ? "#999999" : opened ? "#cc3333" : "white"
                                    fontPixelSize: 22
                                    enabled: variable2_2Selectindex == index
                                    rectRadius: 2
                                    value: enabled == false ? "" : variableViewmodel.menu2_2_Value[index]
                                    onReady: {
                                        max = 255
                                        min = 0
                                    }
                                    onEnter: {
                                        variableViewmodel.menu2_2_Value[index] = newValue;
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable2_2_view
                            width: 394
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu2_2_Num
                            delegate: listdelegate_2_2
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//字节
                }
            }
        }
        Loader {
            id: loader_variable2
            asynchronous: (menu_index !== 1)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable2
        }

        //外部输出
        Component {
            id: component_variable3
            Item{
                property alias listdelegate_3: inner_listdelegate_3
                property alias output_view: inner_output_view

                id:variable3
                width: parent.width
                height: parent.height
                property int page: 0
                ImageScrollBar{
                    height: 324
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 6
                    property int zz: output_view.model%8 >0 ? output_view.model/8+1 : output_view.model/8
                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: variable3.page+1 + "/" + zz
                    onScrollUpClicked:variable3.page = Math.max(variable3.page-1 ,0)
                    onScrollDownClicked:variable3.page = Math.min(variable3.page+1, zz-1)
                }//切换按钮
                // 외부출력 전체 Ext output
                Component{
                    id:inner_listdelegate_3
                    Row{
                        spacing: 4
                        x : 1
                        Rectangle{
                            width: 50
                            height: 39
                            color: "#ffffff"
                            border.color:variable3Selectindex == index ? "#999999" : "#999999"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: variable3Selectindex == index ? variableViewmodel.variable3SelectIndex = -1 : variableViewmodel.variable3SelectIndex = index
                            }
                        }
                        Rectangle{
                            width: 330
                            height: 39
                            color: "#ffffff"
                            border.color:variable3Selectindex == index ? "#999999" : "#999999"
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 180

                                visible: variable3Selectindex == index
                                color:menu3_state[index] == 1 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"ON"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu3_State[index] = 1
                                }
                            }
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 255

                                visible: variable3Selectindex == index
                                color:menu3_state[index] == 0 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"OFF"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu3_State[index] = 0
                                }
                            }
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.symbolAt(menu3Addr[index])
                            }
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                y:19
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.getMessageStringValue("1", menu3Addr[index], language.value)
                            }
                            MouseArea{
                                width: variable3Selectindex == index ? 180 : parent.width
                                height: parent.height
                                onClicked:variable3Selectindex == index ? variableViewmodel.variable3SelectIndex = -1 : variableViewmodel.variable3SelectIndex = index
                            }
                        }
                    }
                }
                ListView{
                    id:inner_output_view
                    width: 384
                    height: 326
                    y:2
                    property int pg: parent.page
                    spacing: 2
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
            }
        }
        Loader {
            id: loader_variable3
            asynchronous: (menu_index !== 2)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable3
        }

        //计时器
        Component {
            id: component_variable4
            Item{
                property alias listdelegate_4: inner_listdelegate_4
                property alias timer_view: inner_timer_view
                id:variable4
                width: parent.width
                height: parent.height
                property int page: 0
                ImageScrollBar{
                    height: 324
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 6
                    property int zz: timer_view.model%8 >0 ? timer_view.model/8+1 : timer_view.model/8
                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: variable4.page+1 + "/" + zz
                    onScrollUpClicked:variable4.page = Math.max(variable4.page-1 ,0)
                    onScrollDownClicked:variable4.page = Math.min(variable4.page+1, zz-1)
                }//切换按钮
                // 타이머 Timer
                Component{
                    id:inner_listdelegate_4
                    Row{
                        spacing: 4
                        x : 1
                        Rectangle{
                            width: 50
                            height: 39
                            color: "#ffffff"
                            border.color:variable4Selectindex == index ? "#999999" : "#999999"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: variable4Selectindex == index ? variableViewmodel.variable4SelectIndex = -1 : variableViewmodel.variable4SelectIndex = index
                            }
                        }
                        Rectangle{
                            width: 330
                            height: 39
                            color: "#ffffff"
                            border.color:variable4Selectindex == index ? "#999999" : "#999999"
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 180

                                visible: variable4Selectindex == index
                                color:menu4_state[index] == 1 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"ON"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu4_State[index] = 1
                                }
                            }
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 255

                                visible: variable4Selectindex == index
                                color:menu4_state[index] == 0 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"OFF"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu4_State[index] = 0
                                }
                            }
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.symbolAt(menu4Addr[index])
                            }
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                y:19
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.getMessageStringValue("1", menu4Addr[index], language.value)
                            }
                            MouseArea{
                                width: variable4Selectindex == index ? 180 : parent.width
                                height: parent.height
                                onClicked:variable4Selectindex == index ? variableViewmodel.variable4SelectIndex = -1 : variableViewmodel.variable4SelectIndex = index
                            }
                        }
                    }
                }
                ListView{
                    id:inner_timer_view
                    width: 384
                    height: 326
                    y:2
                    property int pg: parent.page
                    spacing: 2
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
            }
        }
        Loader {
            id: loader_variable4
            asynchronous: (menu_index !== 3)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable4
        }

        //计数器
        Component {
            id: component_variable5
            Item{
                property alias listdelegate_5: inner_listdelegate_5
                property alias counter_view: inner_counter_view

                id:variable5
                width: parent.width
                height: parent.height
                property int page: 0
                ImageScrollBar{
                    height: 324
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 6
                    property int zz: counter_view.model%8 >0 ? counter_view.model/8+1 : counter_view.model/8
                    topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                    topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                    botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                    botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                    pageLabal: variable5.page+1 + "/" + zz
                    onScrollUpClicked:variable5.page = Math.max(variable5.page-1 ,0)
                    onScrollDownClicked:variable5.page = Math.min(variable5.page+1, zz-1)
                }//切换按钮
                // 카운터 counter
                Component{
                    id:inner_listdelegate_5
                    Row{
                        spacing: 4
                        x : 1
                        Rectangle{
                            width: 50
                            height: 39
                            color: "#ffffff"
                            border.color:variable5Selectindex == index ? "#999999" : "#999999"
                            Text{
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                text: index + 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: variable5Selectindex == index ? variableViewmodel.variable5SelectIndex = -1 : variableViewmodel.variable5SelectIndex = index
                            }
                        }
                        Rectangle{
                            width: 330
                            height: 39
                            color: "#ffffff"
                            border.color:variable5Selectindex == index ? "#999999" : "#999999"
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 180

                                visible: variable5Selectindex == index
                                color:menu5_state[index] == 1 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"ON"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu5_State[index] = 1
                                }
                            }
                            Rectangle{
                                width:70
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 255

                                visible: variable5Selectindex == index
                                color:menu5_state[index] == 0 ? "#cc3333" : "#999999"
                                border.color: "#ffffff"
                                border.width: 2
                                Text{
                                    anchors.centerIn: parent
                                    font.pixelSize: 17
                                    text:"OFF"
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: variableViewmodel.menu5_State[index] = 0
                                }
                            }
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.symbolAt(menu5Addr[index])
                            }
                            Text{
                                height: 19
                                anchors.left: parent.left
                                anchors.leftMargin: 5
                                y:19
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 15
                                text: resourceManager.getMessageStringValue("1", menu5Addr[index], language.value)
                            }
                            MouseArea{
                                width: variable5Selectindex == index ? 180 : parent.width
                                height: parent.height
                                onClicked:variable5Selectindex == index ? variableViewmodel.variable5SelectIndex = -1 : variableViewmodel.variable5SelectIndex = index
                            }
                        }
                    }
                }
                ListView{
                    id:inner_counter_view
                    width: 384
                    height: 326
                    y:2
                    property int pg: parent.page
                    spacing: 2
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
            }
        }
        Loader {
            id: loader_variable5
            asynchronous: (menu_index !== 4)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable5
        }

        //轴控制
        // 축제어 axis control
        Component {
            id: component_variable6
            Item{
                property alias variable6_1: inner_variable6_1
                property alias listdelegate_6_1: inner_listdelegate_6_1
                property alias variable6_1_view: inner_variable6_1_view
                property alias variable6_2: inner_variable6_2
                property alias listdelegate_6_2: inner_listdelegate_6_2
                property alias variable6_2_view: inner_variable6_2_view

                id:variable6
                width: parent.width
                height: parent.height
                property bool type: variableViewmodel.menu6_Type
                Rectangle{
                    x : 1
                    width: parent.width
                    height: 40
                    color: "#cccccc"
                    border.color: "#999999"

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        font.pixelSize: 18
                        color: resourceManager.rgb(41, 476)
                        text: resourceManager.label(41,476, language.value)
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 90

                        color:variable6.type == 0 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 477)
                            text: resourceManager.label(41,477, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu6_Type = 0
                        }
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 200
                        color:variable6.type == 1 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 478)
                            text: resourceManager.label(41,478, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu6_Type = 1
                        }
                    }
                }
                StackLayout{
                    width: parent.width
                    height: 288
                    y:41
                    clip: true
                    currentIndex: variable6.type
                    Item{
                        id:inner_variable6_1
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 290
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 6
                            property int zz: variable6_1_view.model%7 >0 ? variable6_1_view.model/7+1 : variable6_1_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable6_1.page+1 + "/" + zz
                            onScrollUpClicked:variable6_1.page = Math.max(variable6_1.page-1 ,0)
                            onScrollDownClicked:variable6_1.page = Math.min(variable6_1.page+1, zz-1)
                        }
                        // 축제어 axis control
                        Component{
                            id:inner_listdelegate_6_1
                            Row{
                                spacing: 4
                                x : 1
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable6_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 18
                                        text: index + 1
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked:variable6_1Selectindex == index ? variableViewmodel.variable6_1SelectIndex = -1 : variableViewmodel.variable6_1SelectIndex = index
                                    }
                                }
                                Rectangle{
                                    width: 330
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable6_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.symbolAt(menu6_1Addr[index])
                                    }
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        y:19
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.getMessageStringValue("1", menu6_1Addr[index], language.value)
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 180

                                        visible: variable6_1Selectindex == index
                                        color:menu6_1_state[index] == 1 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"ON"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu6_1_State[index] = 1
                                        }
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 255

                                        visible: variable6_1Selectindex == index
                                        color:menu6_1_state[index] == 0 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"OFF"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu6_1_State[index] = 0
                                        }
                                    }
                                    MouseArea{
                                        width: variable6_1Selectindex == index ? 180 : parent.width
                                        height: parent.height
                                        onClicked:variable6_1Selectindex == index ? variableViewmodel.variable6_1SelectIndex = -1 : variableViewmodel.variable6_1SelectIndex = index
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable6_1_view
                            width: 384
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu6_1_Num
                            delegate: listdelegate_6_1
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//位
                    Item{
                        id:inner_variable6_2
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 290
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 6
                            property int zz: variable6_2_view.model%7 >0 ? variable6_2_view.model/7+1 : variable6_2_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable6_2.page+1 + "/" + zz
                            onScrollUpClicked:variable6_2.page = Math.max(variable6_2.page-1 ,0)
                            onScrollDownClicked:variable6_2.page = Math.min(variable6_2.page+1, zz-1)
                        }
                        // user 사용사 설정
                        Component{
                            id:inner_listdelegate_6_2
                            Row{
                                spacing: 2
                                Row{
                                    spacing: 4
                                    Rectangle{
                                        width: 50
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable6_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 18
                                            text: index + 1
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked:variable6_2Selectindex == index ? variableViewmodel.variable6_2SelectIndex = -1 : variableViewmodel.variable6_2SelectIndex = index
                                        }
                                    }
                                    Rectangle{
                                        width: 190
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable6_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.symbolAt(menu6_2Addr[index])
                                        }
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            y:19
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.getMessageStringValue("1", menu6_2Addr[index], language.value)
                                        }
                                        MouseArea{
                                            width: variable6_2Selectindex == index ? 180 : parent.width
                                            height: parent.height
                                            onClicked:variable6_2Selectindex == index ? variableViewmodel.variable6_2SelectIndex = -1 : variableViewmodel.variable6_2SelectIndex = index
                                        }
                                    }
                                }
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#999999"
                                    border.color:"#999999"
                                    enabled: variable6_2Selectindex == index
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 20
                                        text: menu6_2_symbol[index]
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onPressed: parent.color = "yellow"
                                        onReleased: {
                                            parent.color = "#999999"
                                        }
                                        onClicked: {
                                            if(menu6_2_symbol[index] === "="){
                                                variableViewmodel.menu6_2_Symbol[index] = "≠"
                                            }else if(menu6_2_symbol[index] === "≠"){
                                                variableViewmodel.menu6_2_Symbol[index] = ">"
                                            }else if(menu6_2_symbol[index] === ">"){
                                                variableViewmodel.menu6_2_Symbol[index] = "<"
                                            }else{
                                                variableViewmodel.menu6_2_Symbol[index] = "="
                                            }
                                        }
                                    }
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 39
                                    color: enabled == false ? "#999999" : opened ? "#cc3333" : "white"
                                    fontPixelSize: 22
                                    enabled: variable6_2Selectindex == index
                                    value: enabled == false ? "" : variableViewmodel.menu6_2_Value[index]
                                    onReady: {
                                        max = 255
                                        min = 0
                                    }
                                    onEnter: {
                                        variableViewmodel.menu6_2_Value[index] = newValue;
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable6_2_view
                            width: 384
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu6_2_Num
                            delegate: listdelegate_6_2
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//字节
                }
            }
        }
        Loader {
            id: loader_variable6
            asynchronous: (menu_index !== 5)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable6
        }

        //用户
        Component {
            id: component_variable7
            Item{
                property alias variable7_1: inner_variable7_1
                property alias listdelegate_7_1: inner_listdelegate_7_1
                property alias variable7_1_view: inner_variable7_1_view
                property alias variable7_2: inner_variable7_2
                property alias listdelegate_7_2: inner_listdelegate_7_2
                property alias variable7_2_view: inner_variable7_2_view

                id:variable7
                width: parent.width
                height: parent.height
                property bool type: variableViewmodel.menu7_Type
                Rectangle{
                    width: parent.width
                    height: 40
                    color: "#cccccc"
                    border.color: "#999999"
                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        font.pixelSize: 18
                        color: resourceManager.rgb(41, 476)
                        text: resourceManager.label(41,476, language.value)
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 90

                        color:variable7.type == 0 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 477)
                            text: resourceManager.label(41,477, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu7_Type = 0
                        }
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 200

                        color:variable7.type == 1 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 478)
                            text: resourceManager.label(41,478, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu7_Type = 1
                        }
                    }
                }
                StackLayout{
                    width: parent.width
                    height: 288
                    y:41
                    clip: true
                    currentIndex: variable7.type
                    Item{
                        id:inner_variable7_1
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 290
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 6
                            property int zz: variable7_1_view.model%7 >0 ? variable7_1_view.model/7+1 : variable7_1_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable7_1.page+1 + "/" + zz
                            onScrollUpClicked:variable7_1.page = Math.max(variable7_1.page-1 ,0)
                            onScrollDownClicked:variable7_1.page = Math.min(variable7_1.page+1, zz-1)
                        }
                        Component{
                            id:inner_listdelegate_7_1
                            Row{
                                spacing: 4
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable7_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 18
                                        text: index + 1
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked:variable7_1Selectindex == index ? variableViewmodel.variable7_1SelectIndex = -1 : variableViewmodel.variable7_1SelectIndex = index
                                    }
                                }
                                Rectangle{
                                    width: 330
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable7_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.symbolAt(menu7_1Addr[index])
                                    }
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        y:19
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.getMessageStringValue("1", menu7_1Addr[index], language.value)
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 180

                                        visible: variable7_1Selectindex == index
                                        color:menu7_1_state[index] == 1 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"ON"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu7_1_State[index] = 1
                                        }
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 255

                                        visible: variable7_1Selectindex == index
                                        color:menu7_1_state[index] == 0 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"OFF"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu7_1_State[index] = 0
                                        }
                                    }
                                    MouseArea{
                                        width: variable7_1Selectindex == index ? 180 : parent.width
                                        height: parent.height
                                        onClicked:variable7_1Selectindex == index ? variableViewmodel.variable7_1SelectIndex = -1 : variableViewmodel.variable7_1SelectIndex = index
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable7_1_view
                            width: 384
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu7_1_Num
                            delegate: listdelegate_7_1
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//位
                    Item{
                        id:inner_variable7_2
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 290
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 6
                            property int zz: variable7_2_view.model%7 >0 ? variable7_2_view.model/7+1 : variable7_2_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable7_2.page+1 + "/" + zz
                            onScrollUpClicked:variable7_2.page = Math.max(variable7_2.page-1 ,0)
                            onScrollDownClicked:variable7_2.page = Math.min(variable7_2.page+1, zz-1)
                        }
                        Component{
                            id:inner_listdelegate_7_2
                            Row{
                                spacing: 2
                                Row{
                                    spacing: 4
                                    Rectangle{
                                        width: 50
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable7_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 18
                                            text: index + 1
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked:variable7_2Selectindex == index ? variableViewmodel.variable7_2SelectIndex = -1 : variableViewmodel.variable7_2SelectIndex = index
                                        }
                                    }
                                    Rectangle{
                                        width: 190
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable7_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.symbolAt(menu7_2Addr[index])
                                        }
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            y:19
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.getMessageStringValue("1", menu7_2Addr[index], language.value)
                                        }
                                        MouseArea{
                                            width: variable7_2Selectindex == index ? 180 : parent.width
                                            height: parent.height
                                            onClicked:variable7_2Selectindex == index ? variableViewmodel.variable7_2SelectIndex = -1 : variableViewmodel.variable7_2SelectIndex = index
                                        }
                                    }
                                }
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#999999"
                                    border.color:"#999999"
                                    enabled: variable7_2Selectindex == index
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 20
                                        text: menu7_2_symbol[index]
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onPressed: parent.color = "yellow"
                                        onReleased: {
                                            parent.color = "#999999"
                                        }
                                        onClicked: {
                                            if(menu7_2_symbol[index] === "="){
                                                variableViewmodel.menu7_2_Symbol[index] = "≠"
                                            }else if(menu7_2_symbol[index] === "≠"){
                                                variableViewmodel.menu7_2_Symbol[index] = ">"
                                            }else if(menu7_2_symbol[index] === ">"){
                                                variableViewmodel.menu7_2_Symbol[index] = "<"
                                            }else{
                                                variableViewmodel.menu7_2_Symbol[index] = "="
                                            }
                                        }
                                    }
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 39
                                    color: enabled == false ? "#999999" : opened ? "#cc3333" : "white"
                                    fontPixelSize: 22
                                    enabled: variable7_2Selectindex == index
                                    value: enabled == false ? "" : variableViewmodel.menu7_2_Value[index]
                                    onReady: {
                                        max = 255
                                        min = 0
                                    }
                                    onEnter: {
                                        variableViewmodel.menu7_2_Value[index] = newValue;
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable7_2_view
                            width: 384
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu7_2_Num
                            delegate: listdelegate_7_2
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//字节
                }
            }
        }
        Loader {
            id: loader_variable7
            asynchronous: (menu_index !== 6)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable7
        }

        //系统
        Component {
            id: component_variable8
            Item{
                property alias variable8_1: inner_variable8_1
                property alias listdelegate_8_1: inner_listdelegate_8_1
                property alias variable8_1_view: inner_variable8_1_view
                property alias variable8_2: inner_variable8_2
                property alias listdelegate_8_2: inner_listdelegate_8_2
                property alias variable8_2_view: inner_variable8_2_view

                id:variable8
                width: parent.width
                height: parent.height
                property bool type: variableViewmodel.menu8_Type
                Rectangle{
                    width: parent.width
                    height: 40
                    color: "#cccccc"
                    border.color: "#999999"

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 5
                        font.pixelSize: 18
                        color: resourceManager.rgb(41, 476)
                        text: resourceManager.label(41,476, language.value)
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 90

                        color:variable8.type == 0 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 477)
                            text: resourceManager.label(41,477, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu8_Type = 0
                        }
                    }
                    Rectangle{
                        width:100
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 200

                        color:variable8.type == 1 ? "#cc3333" : "#999999"
                        border.color: "#ffffff"
                        border.width: 2
                        Text{
                            anchors.centerIn: parent
                            font.pixelSize: 17
                            color: resourceManager.rgb(41, 478)
                            text: resourceManager.label(41,478, language.value)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: variableViewmodel.menu8_Type = 1
                        }
                    }
                }
                StackLayout{
                    width: parent.width
                    height: 288
                    y:41
                    clip: true
                    currentIndex: variable8.type
                    Item{
                        id:inner_variable8_1
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 290
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 6
                            property int zz: variable8_1_view.model%7 >0 ? variable8_1_view.model/7+1 : variable8_1_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable8_1.page+1 + "/" + zz
                            onScrollUpClicked:variable8_1.page = Math.max(variable8_1.page-1 ,0)
                            onScrollDownClicked:variable8_1.page = Math.min(variable8_1.page+1, zz-1)
                        }
                        Component{
                            id:inner_listdelegate_8_1
                            Row{
                                spacing: 4
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable8_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 18
                                        text: index + 1
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked:variable8_1Selectindex == index ? variableViewmodel.variable8_1SelectIndex = -1 : variableViewmodel.variable8_1SelectIndex = index
                                    }
                                }
                                Rectangle{
                                    width: 330
                                    height: 39
                                    color: "#ffffff"
                                    border.color:variable8_1Selectindex == index ? "#999999" : "#999999"
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.symbolAt(menu8_1Addr[index])
                                    }
                                    Text{
                                        height: 19
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        y:19
                                        verticalAlignment: Text.AlignVCenter
                                        font.pixelSize: 15
                                        text: resourceManager.getMessageStringValue("1", menu8_1Addr[index], language.value)
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 180

                                        visible: variable8_1Selectindex == index
                                        color:menu8_1_state[index] == 1 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"ON"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu8_1_State[index] = 1
                                        }
                                    }
                                    Rectangle{
                                        width:70
                                        height: 28
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 255

                                        visible: variable8_1Selectindex == index
                                        color:menu8_1_state[index] == 0 ? "#cc3333" : "#999999"
                                        border.color: "#ffffff"
                                        border.width: 2
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 17
                                            text:"OFF"
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: variableViewmodel.menu8_1_State[index] = 0
                                        }
                                    }
                                    MouseArea{
                                        width: variable8_1Selectindex == index ? 180 : parent.width
                                        height: parent.height
                                        onClicked:variable8_1Selectindex == index ? variableViewmodel.variable8_1SelectIndex = -1 : variableViewmodel.variable8_1SelectIndex = index
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable8_1_view
                            width: 384
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu8_1_Num
                            delegate: listdelegate_8_1
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//位
                    Item{
                        id:inner_variable8_2
                        width: parent.width
                        height: parent.height
                        property int page: 0
                        ImageScrollBar{
                            height: 290
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 6
                            property int zz: variable8_2_view.model%7 >0 ? variable8_2_view.model/7+1 : variable8_2_view.model/7
                            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                            pageLabal: variable8_2.page+1 + "/" + zz
                            onScrollUpClicked:variable8_2.page = Math.max(variable8_2.page-1 ,0)
                            onScrollDownClicked:variable8_2.page = Math.min(variable8_2.page+1, zz-1)
                        }
                        Component{
                            id:inner_listdelegate_8_2
                            Row{
                                spacing: 2
                                Row{
                                    spacing: 4
                                    Rectangle{
                                        width: 50
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable8_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            anchors.centerIn: parent
                                            font.pixelSize: 18
                                            text: index + 1
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked:variable8_2Selectindex == index ? variableViewmodel.variable8_2SelectIndex = -1 : variableViewmodel.variable8_2SelectIndex = index
                                        }
                                    }
                                    Rectangle{
                                        width: 190
                                        height: 39
                                        color: "#ffffff"
                                        border.color:variable8_2Selectindex == index ? "#999999" : "#999999"
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.symbolAt(menu8_2Addr[index])
                                        }
                                        Text{
                                            height: 19
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            y:19
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 15
                                            text: resourceManager.getMessageStringValue("1", menu8_2Addr[index], language.value)
                                        }
                                        MouseArea{
                                            width: variable8_2Selectindex == index ? 180 : parent.width
                                            height: parent.height
                                            onClicked:variable8_2Selectindex == index ? variableViewmodel.variable8_2SelectIndex = -1 : variableViewmodel.variable8_2SelectIndex = index
                                        }
                                    }
                                }
                                Rectangle{
                                    width: 50
                                    height: 39
                                    color: "#999999"
                                    border.color:"#999999"
                                    enabled: variable8_2Selectindex == index
                                    Text{
                                        anchors.centerIn: parent
                                        font.pixelSize: 20
                                        text: menu8_2_symbol[index]
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        onPressed: parent.color = "yellow"
                                        onReleased: {
                                            parent.color = "#999999"
                                        }
                                        onClicked: {
                                            if(menu8_2_symbol[index] === "="){
                                                variableViewmodel.menu8_2_Symbol[index] = "≠"
                                            }else if(menu8_2_symbol[index] === "≠"){
                                                variableViewmodel.menu8_2_Symbol[index] = ">"
                                            }else if(menu8_2_symbol[index] === ">"){
                                                variableViewmodel.menu8_2_Symbol[index] = "<"
                                            }else{
                                                variableViewmodel.menu8_2_Symbol[index] = "="
                                            }
                                        }
                                    }
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 39
                                    color: enabled == false ? "#999999" : opened ? "#cc3333" : "white"
                                    fontPixelSize: 22
                                    enabled: variable8_2Selectindex == index
                                    value: enabled == false ? "" : variableViewmodel.menu8_2_Value[index]
                                    onReady: {
                                        max = 255
                                        min = 0
                                    }
                                    onEnter: {
                                        variableViewmodel.menu8_2_Value[index] = newValue;
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid);
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid);
                                    }
                                }
                            }
                        }
                        ListView{
                            id:inner_variable8_2_view
                            width: 384
                            height: 285
                            y:2
                            property int pg: parent.page
                            spacing: 2
                            clip: true
                            interactive: false
                            model: variableViewmodel.menu8_2_Num
                            delegate: listdelegate_8_2
                            onPgChanged: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                            Component.onCompleted: {
                                positionViewAtIndex(pg * 7, ListView.Beginning)
                            }
                        }
                    }//字节
                }
            }
        }
        Loader {
            id: loader_variable8
            asynchronous: (menu_index !== 7)
            visible: (state === Loader.Ready)
            sourceComponent: component_variable8
        }
    }

    Rectangle{
        width: 110
        height: 40
        x:450
        y:374
        border.width: 2
        border.color: "#999999"
        color: mouse.pressed ? "#cc3333" : "#cccccc"
        // Image {
        //     x:5
        //     anchors.verticalCenter: parent.verticalCenter
        //     source: resourceManager.imagePath + "/16_program/d161_104.png"
        // }
        Text {
            anchors.centerIn: parent
            font.pixelSize: mouse.pressed ? 24 : 23
            color: mouse.pressed ? "white" : "black"
            text: resourceManager.label(41,292,language.value)
        }
        MouseArea{
            id:mouse
            anchors.fill: parent
            onClicked:{
                if(variable1Selectindex!= -1 || variable2_1Selectindex != -1 || variable2_2Selectindex != -1
                        || variable3Selectindex != -1 || variable4Selectindex != -1
                        || variable5Selectindex != -1 || variable6_1Selectindex != -1
                        || variable6_2Selectindex != -1 || variable7_1Selectindex != -1
                        || variable7_2Selectindex != -1 || variable8_1Selectindex != -1 || variable8_2Selectindex != -1){
                    variableViewmodel.setVariableStr(menu_index)
                    let newValue = variableViewmodel.outVariableStr
                    root.enter(newValue, variableViewmodel.outMark, variableViewmodel.outMsg, variableViewmodel.outSymbol, variableViewmodel.outValue, variableViewmodel.outAddr)
                    root.close()
                }else{
                    return
                }
            }
        }
    }//确定按钮

    function setDlgPoint(dlgX ,dlgY){
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
