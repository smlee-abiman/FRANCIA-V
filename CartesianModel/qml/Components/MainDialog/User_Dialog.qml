import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: User_Dialog.qml
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
    id:usr_switch_dialog
    width: 642
    height: 310
    modal: false
    closePolicy:Popup.NoAutoClose

    background: Rectangle{
        width: 642
        height: 310
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 642
            height: 55
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.centerIn: parent
                font.pixelSize: 26
                color: resourceManager.rgb(54,0)
                text: resourceManager.label(54,0,resourceManager.language)
            }
            ImageButton{
                id:usr_switch_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    usr_switch_dialog.close()
                    pass_te1.visible = false
                    pass_te2.visible = false
                    pass_te3_1.visible = false
                    pass_te3_2.visible = false
                }
            }
        }//标题栏
    }

    Image{
        x:-7
        y:46
        TabBar{
            id:usr_switch_tab
            y:12
            spacing: 6
            background: Rectangle{
                anchors.fill: parent
                color: "transparent"
            }

            TabButton{
                id:usr_tab1
                width: 100
                height: 36
                background: Rectangle {
                    anchors.fill: parent
                    color: (usr_switch_tab.currentIndex === 0) ? "#cc3333" : "#cccccc"
                    border.color: "#999999"
                }
                Text {
                id:usr_tab1_text
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 18
                font.bold: true
                wrapMode: Text.WordWrap
                text: resourceManager.label(54,1,resourceManager.language)
                color: (usr_switch_tab.currentIndex === 0) ? "#ffffff" : "black"
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        usr_switch_tab.currentIndex = 0
                        operationRecord.recordScreenSwitching(28, 0)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }

            TabButton{
                id:usr_tab2
                width: 100
                height: 36
                background: Rectangle {
                    anchors.fill: parent
                    color: (usr_switch_tab.currentIndex === 1) ? "#cc3333" : "#cccccc"
                    border.color: "#999999"
                }
                Text {
                    id:usr_tab2_text
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 18
                    font.bold: true
                    wrapMode: Text.WordWrap
                    text: resourceManager.label(54,2,resourceManager.language)
                    color: (usr_switch_tab.currentIndex === 1) ? "#ffffff" : "black"
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        usr_switch_tab.currentIndex = 1
                        operationRecord.recordScreenSwitching(28, 1)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }

        }

        StackLayout{
            width: 629
            height: 204
            y:40
            currentIndex: usr_switch_tab.currentIndex
            //用户切换
            Item{
                Item{
                    Image{
                        id:userLevel0
                        x:50
                        y:40
                        scale: 1.3
                        source:(usr_select === 0) ? resourceManager.imagePath + "/36_DIALOG/a360_12.png" : resourceManager.imagePath + "/36_DIALOG/a360_11.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if (usr_select !== 0){
                                    local.userLevelPassInput = "";
                                }
                                usr_select = 0
                                top_bar.userLevel5Visible = false
                                userLevel4CtrlButton.enabled = true
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                    Image{
                        id:userLevel1
                        x:170
                        y:40
                        scale: 1.3
                        source:(usr_select === 1) ? resourceManager.imagePath + "/36_DIALOG/a360_14.png" : resourceManager.imagePath + "/36_DIALOG/a360_13.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if (usr_select !== 1){
                                    local.userLevelPassInput = "";
                                }
                                usr_select = 1
                                top_bar.userLevel5Visible = false
                                userLevel4CtrlButton.enabled = true
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                    Image{
                        id:userLevel2
                        x:290
                        y:40
                        scale: 1.3
                        source:(usr_select === 2) ? resourceManager.imagePath + "/36_DIALOG/a360_16.png" : resourceManager.imagePath + "/36_DIALOG/a360_15.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if (usr_select !== 2){
                                    local.userLevelPassInput = "";
                                }
                                usr_select = 2
                                top_bar.userLevel5Visible = false
                                userLevel4CtrlButton.enabled = true
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                    Image{
                        id:userLevel3
                        x:410
                        y:40
                        scale: 1.3
                        source:(usr_select === 3) ? resourceManager.imagePath + "/36_DIALOG/a360_18.png" : resourceManager.imagePath + "/36_DIALOG/a360_17.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if (usr_select !== 3){
                                    local.userLevelPassInput = "";
                                }
                                usr_select = 3
                                top_bar.userLevel5Visible = false
                                userLevel4CtrlButton.enabled = true
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                    Image{
                        id:userLevel4
                        visible:top_bar.userLevel5Visible
                        x:530
                        y:40
                        scale: 1.3
                        source:(usr_select === 4) ? resourceManager.imagePath + "/36_DIALOG/a360_20.png" : resourceManager.imagePath + "/36_DIALOG/a360_19.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if (usr_select !== 4){
                                    local.userLevelPassInput = "";
                                }
                                usr_select = 4
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                    HiddenButton{
                        id:userLevel4CtrlButton
                        x:530
                        y:40
                        isEnablePressingSignal:true
                        rectColor:"transparent"
                        pressMonitorTime:5
                        onClicked:{
                            //console.log("userLevel4 onClicked:")
                        }
                        onPressing:{
                            //console.log("userLevel4 onPressing:")
                            if(top_bar.usr_select >= 3){
                                top_bar.userLevel5Visible = true
                                userLevel4CtrlButton.enabled = false
                            } else {
                                //console.log("invalid usr_select(to userLevel5):"+top_bar.usr_select)
                            }
                        }
                        onReleased:{
                            //console.log("userLevel4 onReleased:")
                        }
                    }
                }//1 2 3 4用户图标指示
                Row{
                    x:80
                    y:135
                    spacing: 4
                    Text {
                        width: 90
                        height: 40
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: 22
                        color: resourceManager.rgb(54,8)
                        text: resourceManager.label(54,8,resourceManager.language)
                    }
                    LimitNumericEditBox{
                        id:password_input
                        width: 300
                        height: 40
                        max: "9999"
                        min: "0"
                        textColor:"black"
                        value: ""
                        fontPixelSize: 18
                        borderColor:"#999999"
                        borderWidth:2
                        rectRadius:0
                        keyLayoutType:3
                        color: opened ? "#cc3333" : "white"
                        onEnter: {
                            //console.log("newValue:"+newValue, "userLevel:"+commonViewModel.userLevel, "usr_select:"+top_bar.usr_select, "password:"+top_bar.userLevelPass[commonViewModel.userLevel])
                            local.userLevelPassInput = newValue
                            value = top_bar.hiddenWords
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                        onReady: {
                            password_input.value = ""
                        }
                    }
                }//密码输入

                Rectangle{
                    width: 100
                    height: 30
                    x:240
                    y:174
                    color: "transparent"
                    Text {
                        id:pass_te3
                        anchors.centerIn: parent
                        text: resourceManager.label(54,13,resourceManager.language)
                        font.pixelSize: 24
                        color: resourceManager.rgb(54,13)
                        visible: false
                    }
                }//提示文字

                Rectangle{
                    x:480
                    y:130
                    width: 120
                    height: 50
                    border.width: 2
                    color: mouse_confirm.pressed ? "#cc3333" : "#ffffff"
                    border.color: "#999999"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 24
                        color: resourceManager.rgb(54,9)
                        text: resourceManager.label(54,9,resourceManager.language)
                    }
                    MouseArea{
                        id:mouse_confirm
                        anchors.fill: parent
                        onClicked: {
                            if (commonViewModel.userLevel < top_bar.usr_select){
                                if (local.userLevelPassInput === top_bar.userLevelPass[top_bar.usr_select]){
                                    let curUserLevel = commonViewModel.userLevel
                                    curUserLevel = top_bar.usr_select
                                    commonViewModel.modbusUpdateUserLevel(curUserLevel)
                                    if(!REAL_TARGET) {
                                        usr_select = curUserLevel;
                                        commonViewModel.userLevel = curUserLevel;
                                    }
                                    pass_te3.visible = false
                                    password_input.value = ""
                                    usr_switch_dialog.close()
                                } else {
                                    pass_te3.visible = true
                                }
                            } else {
                                let curUserLevel = commonViewModel.userLevel
                                curUserLevel = top_bar.usr_select
                                commonViewModel.modbusUpdateUserLevel(curUserLevel)
                                if(!REAL_TARGET) {
                                    usr_select = curUserLevel;
                                    commonViewModel.userLevel = curUserLevel;
                                }
                                pass_te3.visible = false
                                password_input.value = ""
                                usr_switch_dialog.close()
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//确认
            }
            //修改密码
            Item{
                Text {
                    anchors.right: password_old.left
                    y:14
                    anchors.rightMargin: 5
                    text: resourceManager.label(54,10,resourceManager.language)
                    font.pixelSize: 22
                    color: resourceManager.rgb(54,10)
                }
                Text {
                    anchors.right: password_new.left
                    y:62
                    anchors.rightMargin: 5
                    text: resourceManager.label(54,11,resourceManager.language)
                    font.pixelSize: 22
                    color: resourceManager.rgb(54,11)
                }
                Text {
                    anchors.right: password_new_verify.left
                    y:110
                    anchors.rightMargin: 5
                    text: resourceManager.label(54,12,resourceManager.language)
                    font.pixelSize: 22
                    color: resourceManager.rgb(54,12)
                }
                LimitNumericEditBox{
                    id:password_old
                    width: 290
                    height: 40
                    x:195
                    y:10
                    max: "9999"
                    min: "0"
                    textColor:"black"
                    value: ""
                    fontPixelSize: 18
                    borderColor:"#999999"
                    borderWidth:1
                    rectRadius:0
                    keyLayoutType:3
                    onEnter: {
                        if (newValue !== ""){
                            local.userLevelPassOld = newValue
                            value = top_bar.hiddenWords
                            local.passChgInputStatus[0] = true
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                    }
                    onReady: {
                        password_old.value = ""
                        local.passChgInputStatus[0] = false
                    }
                }
                LimitNumericEditBox{
                    id:password_new
                    width: 290
                    height: 40
                    x:195
                    y:60
                    max: "9999"
                    min: "0"
                    textColor:"black"
                    value: ""
                    fontPixelSize: 18
                    borderColor:"#999999"
                    borderWidth:1
                    rectRadius:0
                    keyLayoutType:3
                    onEnter: {
                        if (newValue !== ""){
                            local.userLevelPassNew = newValue
                            value = top_bar.hiddenWords
                            local.passChgInputStatus[1] = true
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                    }
                    onReady: {
                        password_new.value = ""
                        local.passChgInputStatus[1] = false
                    }
                }
                LimitNumericEditBox{
                    id:password_new_verify
                    width: 290
                    height: 40
                    x:195
                    y:110
                    max: "9999"
                    min: "0"
                    textColor:"black"
                    value: ""
                    fontPixelSize: 18
                    borderColor:"#999999"
                    borderWidth:1
                    rectRadius:0
                    keyLayoutType:3
                    onEnter: {
                        if (newValue !== ""){
                            local.userLevelPassVerify = newValue
                            value = top_bar.hiddenWords
                            local.passChgInputStatus[2] = true
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                    }
                    onReady: {
                        password_new_verify.value = ""
                        local.passChgInputStatus[2] = false
                    }
                }

                Rectangle{
                     width: 110
                     height: 48
                     x:495
                     y:150
                     border.width: 2
                     border.color: "#999999"
                     color: confirm_mouse.pressed ? "#cc3333" : "#ffffff"
                     Text {
                         anchors.fill: parent
                         verticalAlignment: Text.AlignVCenter
                         horizontalAlignment: Text.AlignHCenter
                         font.pixelSize: confirm_mouse.pressed ? 25 : 24
                         color: resourceManager.rgb(54,9)
                         text: resourceManager.label(54,9,resourceManager.language)
                     }
                     MouseArea{
                         id:confirm_mouse
                         anchors.fill: parent
                         onPressed: {
                             pass_te1.visible = false
                             pass_te2.visible = false
                             pass_te3_1.visible = false
                             pass_te3_2.visible = false
                         }
                         onClicked: {
                            if (local.passChgInputStatus[0] === true && local.passChgInputStatus[1] === true && local.passChgInputStatus[2] === true){
                                 if (local.userLevelPassOld === top_bar.userLevelPass[commonViewModel.userLevel]){
                                     if (local.userLevelPassNew === local.userLevelPassVerify){
                                         console.log("userLevel:"+commonViewModel.userLevel, "userLevelPassNew:"+local.userLevelPassNew)
                                         commonViewModel.modbusUpdateUserLevelPassword(commonViewModel.userLevel, local.userLevelPassNew)
                                         commonViewModel.getUserLevelPassword(commonViewModel.userLevel)
                                     } else {
                                         console.log("!!!invalid userLevelPassNew:"+local.userLevelPassNew, "userLevelPassVerify:"+local.userLevelPassVerify)
                                         pass_te1.visible = false
                                         pass_te3_1.visible = false
                                         pass_te3_2.visible = false
                                         pass_te2.visible = true
                                     }
                                 } else {
                                     console.log("!!!invalid userLevelPassOld:"+local.userLevelPassOld, "password:"+top_bar.userLevelPass[commonViewModel.userLevel])
                                     pass_te2.visible = false
                                     pass_te3_1.visible = false
                                     pass_te3_2.visible = false
                                     pass_te1.visible = true
                                 }
                             }
                            phyKeyManager.ctrlBuzzer()
                         }
                     }
                 }//确认按钮

                //提示文字
                Rectangle{
                    width: 120
                    height: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:160
                    color: "transparent"
                    Text {
                        id:pass_te1
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        visible: false
                        color: resourceManager.rgb(54,14)
                        text: resourceManager.label(54,14,resourceManager.language)
                    }
                    Text {
                        id:pass_te2
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        visible: false
                        color: resourceManager.rgb(54,15)
                        text: resourceManager.label(54,15,resourceManager.language)
                    }
                    Text {
                        id:pass_te3_1
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        visible: false
                        color: resourceManager.rgb(54,16)
                        text: resourceManager.label(54,16,resourceManager.language)
                    }
                    Text {
                        id:pass_te3_2
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        visible: false
                        color: resourceManager.rgb(54,17)
                        text: resourceManager.label(54,17,resourceManager.language)
                    }
                }
            }
        }
    }
    Connections {
        target: commonViewModel
        onUserLevelPasswordChanged: {
            if (userLevelPassword === true){
                pass_te1.visible = false
                pass_te3_2.visible = false
                pass_te2.visible = false
                pass_te3_1.visible = true
            } else {
                pass_te1.visible = false
                pass_te3_1.visible = false
                pass_te2.visible = false
                pass_te3_2.visible = true
            }
        }
    }
//    Connections {
//        target: phyKeyManager
//        enabled: local.userAreaDialogJogDialSw
//        onJogDialSwitchInvalidChanged:{
//            var pageIndex = usr_switch_tab.currentIndex
//            if (pageIndex >= 1){
//                pageIndex = 0
//            } else {
//                pageIndex++
//            }
//            usr_switch_tab.currentIndex = pageIndex
//        }
//    }
}
