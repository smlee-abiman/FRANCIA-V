import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../../Components/RemindDialog"
import "../../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: DrvParamSimpleData.qml
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
Item {
    id:root
    width: 1010
    height: 460
    property var pageModel
    property int totalPages: 4
    property int modeToggleIndex: 0
    property int page1modeToggleIndex: 0
    property int page2modeToggleIndex: 0
    property int page3modeToggleIndex: 0
    property string scrollTopButtonPressImage: ""
    property string scrollTopButtonReleaseImage: ""
    property string scrollBotomButtonPressImage: ""
    property string scrollBotomButtonReleaseImage: ""
    property bool jogScrollOn:false
    property var page1CalcValue:[0, 0, 0]
    property var page1BackupValue:[0, 0, 0]
    property var page3CalcValue:[0, 0, 0]
    property var page3BackupValue:[0, 0, 0]
    property var page4CalcValue:[0, 0]
    property var page4BackupValue:[0, 0]
    property int axisIndex:0
    property var axisDataList:pageModel.simpleSetAxisInfo.simpleSetAxisDataList
    property var pageDrvMonitorModel:monitorViewModel.drvMonitorView
    property var page1CalcFirstDisp:[false, false, false]
    property var page3CalcFirstDisp:[false, false, false]
    property var page4CalcFirstDisp:[false, false]
    property int axis0CurPage: 0
    property int axis1CurPage: 0
    property int axis2CurPage: 0
    property int axis3CurPage: 0
    property int axis4CurPage: 0
    property int axis5CurPage: 0
    property int axis6CurPage: 0
    property int axis7CurPage: 0
    property bool pageActive:false
    property bool page1CalcUpDownbtn1Clicked:false
    property bool page1CalcUpDownbtn2Clicked:false
    property bool page1CalcUpDownbtn3Clicked:false
    property bool page3CalcUpDownbtn1Clicked:false
    property bool page3CalcUpDownbtn2Clicked:false
    property bool page3CalcUpDownbtn3Clicked:false
    property bool page4CalcUpDownbtn1Clicked:false
    property bool page4CalcUpDownbtn2Clicked:false
    property bool isManual: commonViewModel.manipulateMode === 1

    signal remindPopupOpen()
    signal initPage(int page, int axis)
    signal updatePage(int page, int item, int axis)
    signal updatePageSR(int page, int item, int axis)
    signal updatePageHR(int page, int item, int axis)
    signal readPage(int page, int axis)

    function getKeyType(page, column, axis){
        let format = pageModel.getDispFormat(page, column, axis);
        if (format < 0){
            return 0
        } else {
            return ((format & 0x0F) !== 0) ? 4 : 0
        }
    }
    function initPageMode(page, axis) {
        switch(page){
        case 1:
            page2modeToggleIndex = 0
            page3modeToggleIndex = 0
            break
        case 2:
            page1modeToggleIndex = 0
            page3modeToggleIndex = 0
            break
        case 3:
            page1modeToggleIndex = 0
            page2modeToggleIndex = 0
            break
        default:
            return
        }
        root.initPage(page, axis)
    }
    function judgeUpdatePage(page){
        switch(page){
        case 1:
            root.updatePageHR(page, 0, root.axisIndex);
            root.updatePage(page, 1, root.axisIndex);
            root.updatePage(page, 2, root.axisIndex);
            root.updatePage(page, 3, root.axisIndex);
            break
        case 2:
            root.updatePageHR(page, 0, root.axisIndex);
            root.updatePageSR(page, 1, root.axisIndex);
            root.updatePageSR(page, 2, root.axisIndex);
            root.updatePageSR(page, 3, root.axisIndex);
            root.updatePageSR(page, 4, root.axisIndex);
            root.updatePageSR(page, 5, root.axisIndex);
            root.updatePageSR(page, 6, root.axisIndex);
            break
        case 3:
            root.updatePage(page, 1, root.axisIndex);
            root.updatePage(page, 2, root.axisIndex);
            root.updatePage(page, 3, root.axisIndex);
            break
        case 4:
            root.updatePage(page, 1, root.axisIndex);
            root.updatePageHR(page, 2, root.axisIndex);
            break
        default:
            return
        }
    }
    function getCurPage(axis){
        let page = 0
        switch(axis){
        case 0:
            page = root.axis0CurPage;
            break
        case 1:
            page = root.axis1CurPage;
            break
        case 2:
            page = root.axis2CurPage;
            break
        case 3:
            page = root.axis3CurPage;
            break
        case 4:
            page = root.axis4CurPage;
            break
        case 5:
            page = root.axis5CurPage;
            break
        case 6:
            page = root.axis6CurPage;
            break
        case 7:
            page = root.axis7CurPage;
            break
        default:
            break
        }
        return page;
    }
    function setCurPage(axis, page){
        switch(axis){
        case 0:
            root.axis0CurPage = page
            break
        case 1:
            root.axis1CurPage = page
            break
        case 2:
            root.axis2CurPage = page
            break
        case 3:
            root.axis3CurPage = page
            break
        case 4:
            root.axis4CurPage = page
            break
        case 5:
            root.axis5CurPage = page
            break
        case 6:
            root.axis6CurPage = page
            break
        case 7:
            root.axis7CurPage = page
            break
        default:
            break
        }
    }
    function initCalcValue(page) {
        switch(page){
        case 1:
            for(let i = 0; i < 3; i++){
                root.page1CalcFirstDisp[i] = false
                root.page1CalcValue[i] = 0
                root.page1BackupValue[i] = 0
            }
            break
        case 2:
            break
        case 3:
            for(let i = 0; i < 3; i++){
                root.page3CalcFirstDisp[i] = false
                root.page3CalcValue[i] = 0
                root.page3BackupValue[i] = 0
            }
            break
        case 4:
            for(let i = 0; i < 2; i++){
                root.page4CalcFirstDisp[i] = false
                root.page4CalcValue[i] = 0
                root.page4BackupValue[i] = 0
            }
            break
        default:
            break
        }
    }
    function initCalcUpDownbtnStatus() {
        root.page1CalcUpDownbtn1Clicked = false
        root.page1CalcUpDownbtn2Clicked = false
        root.page1CalcUpDownbtn3Clicked = false
        root.page3CalcUpDownbtn1Clicked = false
        root.page3CalcUpDownbtn2Clicked = false
        root.page3CalcUpDownbtn3Clicked = false
        root.page4CalcUpDownbtn1Clicked = false
        root.page4CalcUpDownbtn2Clicked = false
    }

    // ImageScrollBar{
    //     id: drvParamSimpleScroll
    //     x:940
    //     y:70
    //     height: 384
    //     jogScrollOn:root.jogScrollOn
    //     topButtonPressImage:root.scrollTopButtonPressImage
    //     topButtonReleaseImage:root.scrollTopButtonReleaseImage
    //     botomButtonPressImage:root.scrollBotomButtonPressImage
    //     botomButtonReleaseImage:root.scrollBotomButtonReleaseImage
    //     pageLabal: (root.getCurPage(root.axisIndex) + 1) + "/" + root.totalPages
    //     onScrollUpClicked: {
    //         //console.log("org_switch onScrollUpClicked:")
    //         let cur_page = root.getCurPage(root.axisIndex)
    //         if(cur_page > 0 && cur_page < 4){
    //             cur_page--
    //             pageModel.startPageScroll()
    //             root.initCalcValue(cur_page + 1)
    //             root.judgeUpdatePage(cur_page + 1);
    //         }else{
    //             cur_page = 3
    //             root.judgeUpdatePage(cur_page + 1);
    //         }
    //         root.setCurPage(root.axisIndex, cur_page)
    //         root.initCalcUpDownbtnStatus()
    //     }
    //     onScrollDownClicked: {
    //         //console.log("org_switch onScrollDownClicked:")
    //         let cur_page = root.getCurPage(root.axisIndex)
    //         if(cur_page >= 0 && cur_page < 3){
    //             cur_page++
    //             pageModel.startPageScroll()
    //             root.initCalcValue(cur_page + 1)
    //             root.judgeUpdatePage(cur_page + 1);
    //         }else{
    //             cur_page = 0
    //             root.judgeUpdatePage(cur_page + 1);
    //         }
    //         root.setCurPage(root.axisIndex, cur_page)
    //         root.initCalcUpDownbtnStatus()
    //     }
    // } //换页按钮
    Item {
        id: drvParamSimpleScroll
        width: 500
        height: 60
        x: 258   // 필요 시 위치 조정
        y: 454
        property alias jogScrollOn: root.jogScrollOn

        Rectangle {
            anchors.fill: parent
            radius: 30
            color: "#eaeaea"
            border.color: "#999999"
        }

        Image {
            id: leftButton
            width: 60
            height: 60
            source: mouseLeft.pressed ? root.scrollTopButtonPressImage : root.scrollTopButtonReleaseImage
            MouseArea {
                id: mouseLeft
                anchors.fill: parent
                onClicked: {
                    let cur_page = root.getCurPage(root.axisIndex)
                    if (cur_page > 0 && cur_page < 4) {
                        cur_page--
                        pageModel.startPageScroll()
                        root.initCalcValue(cur_page + 1)
                        root.judgeUpdatePage(cur_page + 1)
                    } else {
                        cur_page = 3
                        root.judgeUpdatePage(cur_page + 1)
                    }
                    root.setCurPage(root.axisIndex, cur_page)
                    root.initCalcUpDownbtnStatus()
                }
            }
        }

        Image {
            id: rightButton
            width: 60
            height: 60
            anchors.right: parent.right
            source: mouseRight.pressed ? root.scrollBotomButtonPressImage : root.scrollBotomButtonReleaseImage
            MouseArea {
                id: mouseRight
                anchors.fill: parent
                onClicked: {
                    let cur_page = root.getCurPage(root.axisIndex)
                    if (cur_page >= 0 && cur_page < 3) {
                        cur_page++
                        pageModel.startPageScroll()
                        root.initCalcValue(cur_page + 1)
                        root.judgeUpdatePage(cur_page + 1)
                    } else {
                        cur_page = 0
                        root.judgeUpdatePage(cur_page + 1)
                    }
                    root.setCurPage(root.axisIndex, cur_page)
                    root.initCalcUpDownbtnStatus()
                }
            }
        }

        Text {
            anchors.centerIn: parent
            text: (root.getCurPage(root.axisIndex) + 1) + " / " + root.totalPages
            font.pixelSize: 20
            color: "black"
        }
    }
    Item{
        id:stack_drv1
        width: 940
        height: 460
        y:2
        Item{
            id:simplePage1
            visible:((root.getCurPage(root.axisIndex) + 1) === 1)
            Rectangle{
                width: 760
                height: 56
                y:5
                color: "#eaeaea"
                border.color: "#999999"
                Text{
                    width: 150
                    height: 56
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    font.pixelSize: 24
                    font.bold: true
                    color: "#cc3333"
                    text: resourceManager.label(43,207,resourceManager.language)
                }//标题
                Row{
                    spacing: 4
                    x:170
                    anchors.verticalCenter: parent.verticalCenter
                    Rectangle{
                        width: 138
                        height: 45
                        color: (pageModel.dataLoaded === true) ? ((root.axisDataList[0].page1Mode === 0) ? "#cc3333" : "#cccccc"):"#cccccc"
                        border.color: "#999999"
                        anchors.verticalCenter: parent.verticalCenter
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color:resourceManager.rgb(43,208)
                            text:resourceManager.label(43,208,resourceManager.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                page1modeToggleIndex = 0
                                pageModel.modbusSetPageMode(1, root.axisIndex, page1modeToggleIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }//自动调协
                    Rectangle{
                        width: 138
                        height: 45
                        color: (pageModel.dataLoaded === true) ? ((root.axisDataList[0].page1Mode === 1) ? "#cc3333" : "#cccccc"):"#cccccc"
                        border.color: "#999999"
                        anchors.verticalCenter: parent.verticalCenter
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color:resourceManager.rgb(43,209)
                            text:resourceManager.label(43,209,resourceManager.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                page1modeToggleIndex = 1
                                pageModel.modbusSetPageMode(1, root.axisIndex, page1modeToggleIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }//半自动调协
                    Rectangle{
                        width: 138
                        height: 45
                        color: (pageModel.dataLoaded === true) ? ((root.axisDataList[0].page1Mode === 2) ? "#cc3333" : "#cccccc"):"#cccccc"
                        border.color: "#999999"
                        anchors.verticalCenter: parent.verticalCenter
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            font.bold: true
                            color:resourceManager.rgb(43,210)
                            text:resourceManager.label(43,210,resourceManager.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                page1modeToggleIndex = 2
                                pageModel.modbusSetPageMode(1, root.axisIndex, page1modeToggleIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }//手动调协
                }//调谐
                Rectangle{
                    width: 110
                    height: 50
                    x:820
                    y:2
                    border.color: "#999999"
                    color: save_mouse1.pressed ? "#cc3333" : "#cccccc"
                    radius: 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: save_mouse1.pressed ? 23 : 22
                        font.bold: true
                        color: resourceManager.rgb(43,1)
                        text: resourceManager.label(43,1,resourceManager.language)
                    }
                    MouseArea{
                        id:save_mouse1
                        anchors.fill: parent
                        onClicked: {
                            if(isManual){
                                root.remindPopupOpen()
                            }else{
                                remind_save_forbid.open()
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//保存按钮
            }//调谐选择栏

            Rectangle{
                width: 930
                height: 390
                y:60
                color: "#ffffff"
                border.color: "#999999"
                //标题
                Text{
                    x:202
                    y:46
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,211)
                    text: resourceManager.label(43,211,resourceManager.language)
                }
                Text{
                    x:345
                    y:46
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,212)
                    text: resourceManager.label(43,212,resourceManager.language)
                }
                Text{
                    x:536
                    y:46
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,213)
                    text: resourceManager.label(43,213,resourceManager.language)
                }
                //一行
                Text{
                    x:10
                    y:95
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,214)
                    text: resourceManager.label(43,214,resourceManager.language)
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:82
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg1DefValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1DefValue1)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1DefValue1
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    y:82
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg1SetValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue1)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue1
                            }
                        }
                    }
                }
                Image {
                    x:450
                    y:85
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page1Colmun1DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page1CalcUpDownbtn1Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            if (root.page1CalcFirstDisp[0] === false){
                                root.page1CalcValue[0] = root.axisDataList[root.axisIndex].page1SetValue1
                                root.page1CalcFirstDisp[0] = true
                            }
                            if (root.page1CalcValue[0] >= 2){
                                root.page1CalcValue[0]--
                            }
                            root.page1CalcUpDownbtn1Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page1Colmun1Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    y:82
                    max: "40"
                    min: "1"
                    value: {
                        //console.log("page1CalcFirstDisp:"+root.page1CalcFirstDisp[0], "axisIndex:"+root.axisIndex, "page1CalcValue:"+root.page1CalcValue[0], "btnClicked:"+root.page1CalcUpDownbtn1Clicked)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return ""
                            } else {
                                return (root.page1CalcUpDownbtn1Clicked === true) ? root.page1CalcValue[0]:
                                    (page1Colmun1DownMouseArea.pressed === true || page1Colmun1UpMouseArea.pressed === true) ? page1Colmun1Value.value :
                                    (page1Colmun1CancelBtnMouseArea.pressed === true) ? pg1SetValue1.text :
                                    (pageModel.axisDataLoaded === false) ? "" :
                                        root.page1CalcFirstDisp[0] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue1) : root.page1CalcValue[0]
                            }
                        } else {
                            return (page1Colmun1DownMouseArea.pressed === true || page1Colmun1UpMouseArea.pressed === true) ? root.page1CalcValue[0] :
                                (page1Colmun1CancelBtnMouseArea.pressed === true) ? pg1SetValue1.text :
                                    root.page1CalcFirstDisp[0] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue1) : root.page1CalcValue[0]
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(1, 1, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page1CalcValue[0] = parseInt(newValue)
                        root.page1CalcFirstDisp[0] = true
                    }
                }
                Image {
                    x:636
                    y:85
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page1Colmun1UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page1CalcUpDownbtn1Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            if (root.page1CalcFirstDisp[0] === false){
                                root.page1CalcValue[0] = root.axisDataList[root.axisIndex].page1SetValue1
                                root.page1CalcFirstDisp[0] = true
                            }
                            if (root.page1CalcValue[0] <= 39){
                                root.page1CalcValue[0]++
                            }
                            root.page1CalcUpDownbtn1Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    y:82
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page1BackupValue[0] = root.axisDataList[root.axisIndex].page1SetValue1
                                pageModel.modbusSetCalcValue(1, 1, root.axisIndex,
                                    (root.page1CalcFirstDisp[0] === true ? root.page1CalcValue[0] : root.page1BackupValue[0]),
                                    (root.page1CalcFirstDisp[0] === true ? parseFloat(root.page1CalcValue[0]) : parseFloat(root.page1BackupValue[0])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePage(1, 1, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    y:82
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page1Colmun1CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page1CalcValue[0] = pg1SetValue1.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                //二行
                Text{
                    x:10
                    y:200
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,215)
                    text: resourceManager.label(43,215,resourceManager.language)
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:182
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg1DefValue2
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1DefValue2)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1DefValue2
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    y:182
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg1SetValue2
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue2)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue2
                            }
                        }
                    }
                }
                Image {
                    x:450
                    y:185
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page1Colmun2DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page1CalcUpDownbtn2Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            if (root.page1CalcFirstDisp[1] === false){
                                root.page1CalcValue[1] = root.axisDataList[root.axisIndex].page1SetValue2
                                root.page1CalcFirstDisp[1] = true
                            }
                            if (root.page1CalcValue[1] >= 2){
                                root.page1CalcValue[1]--
                            }
                            root.page1CalcUpDownbtn2Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page1Colmun2Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    y:182
                    max: "40"
                    min: "1"
                    value: {
                        //console.log("axisDataLoaded:"+pageModel.axisDataLoaded, "page1CalcFirstDisp:"+root.page1CalcFirstDisp[1], "axisIndex:"+root.axisIndex, "page1CalcValue:"+root.page1CalcValue[1], "btnClicked:"+root.page1CalcUpDownbtn2Clicked)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return ""
                            } else {
                                return (root.page1CalcUpDownbtn2Clicked === true) ? root.page1CalcValue[1]:
                                    (page1Colmun2DownMouseArea.pressed === true || page1Colmun2UpMouseArea.pressed === true) ? page1Colmun2Value.value :
                                    (page1Colmun2CancelBtnMouseArea.pressed === true) ? pg1SetValue2.text :
                                    (pageModel.axisDataLoaded === false) ? "" :
                                        root.page1CalcFirstDisp[1] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue2) : root.page1CalcValue[1]
                            }
                        } else {
                            return (page1Colmun2DownMouseArea.pressed === true || page1Colmun2UpMouseArea.pressed === true) ? root.page1CalcValue[1] :
                                (page1Colmun2CancelBtnMouseArea.pressed === true) ? pg1SetValue2.text :
                                    root.page1CalcFirstDisp[1] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue2) : root.page1CalcValue[1]
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(1, 2, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page1CalcValue[1] = parseInt(newValue)
                        root.page1CalcFirstDisp[1] = true
                    }
                }
                Image {
                    x:636
                    y:185
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page1Colmun2UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page1CalcUpDownbtn2Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            if (root.page1CalcFirstDisp[1] === false){
                                root.page1CalcValue[1] = root.axisDataList[root.axisIndex].page1SetValue2
                                root.page1CalcFirstDisp[1] = true
                            }
                            if (root.page1CalcValue[1] <= 39){
                                root.page1CalcValue[1]++
                            }
                            root.page1CalcUpDownbtn2Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    y:182
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page1BackupValue[1] = root.axisDataList[root.axisIndex].page1SetValue2
                                pageModel.modbusSetCalcValue(1, 2, root.axisIndex,
                                    (root.page1CalcFirstDisp[1] === true ? root.page1CalcValue[1] : root.page1BackupValue[1]),
                                    (root.page1CalcFirstDisp[1] === true ? parseFloat(root.page1CalcValue[1]) : parseFloat(root.page1BackupValue[1])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePage(1, 2, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    y:182
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page1Colmun2CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page1CalcValue[1] = pg1SetValue2.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                //三行
                Text{
                    x:10
                    y:300
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,216)
                    text: resourceManager.label(43,216,resourceManager.language)
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:282
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg1DefValue3
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1DefValue3)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1DefValue3
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    y:282
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg1SetValue3
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : (root.axisDataList[root.axisIndex].page1SetValue3)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue3
                            }
                        }
                    }
                }
                Image {
                    x:450
                    y:285
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page1Colmun3DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page1CalcUpDownbtn3Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            let page1Value3
                            if (root.page1CalcFirstDisp[2] === false){
                                page1Value3 = root.axisDataList[root.axisIndex].page1SetValue3
                                root.page1CalcValue[2] = parseFloat(page1Value3).toFixed(1)
                                root.page1CalcFirstDisp[2] = true
                            }
                            page1Value3 = root.page1CalcValue[2]
                            if (page1Value3 >= 1){
                                page1Value3--;
                                root.page1CalcValue[2] = parseFloat(page1Value3).toFixed(1)
                            }
                            root.page1CalcUpDownbtn3Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page1Colmun3Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    y:282
                    max: "9999"
                    min: "0"
                    value: {
                        //console.log("axisDataLoaded:"+pageModel.axisDataLoaded,"page1CalcFirstDisp:"+root.page1CalcFirstDisp[2], "axisIndex:"+root.axisIndex, "page1CalcValue:"+root.page1CalcValue[2], "btnClicked:"+root.page1CalcUpDownbtn3Clicked)
                        let page1Value3
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return ""
                            } else {
                                if (root.page1CalcUpDownbtn3Clicked === true){
                                    page1Value3 = root.page1CalcValue[2]
                                    return parseFloat(page1Value3).toFixed(1)
                                } else {
                                    if (page3Colmun1DownMouseArea.pressed === true || page3Colmun1UpMouseArea.pressed === true){
                                        page1Value3 = page1Colmun3Value.value
                                        return parseFloat(page1Value3).toFixed(1)
                                    } else {
                                        if (page3Colmun1CancelBtnMouseArea.pressed === true){
                                            return pg1SetValue3.text
                                        } else {
                                            if (pageModel.axisDataLoaded === false){
                                                return ""
                                            } else {
                                                if (root.page1CalcFirstDisp[2] === false){
                                                    if (root.axisIndex === -1){
                                                        return ""
                                                    } else {
                                                        return root.axisDataList[root.axisIndex].page1SetValue3
                                                    }
                                                } else {
                                                    page1Value3 = root.page1CalcValue[2]
                                                    return parseFloat(page1Value3).toFixed(1)
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            return (page1Colmun3DownMouseArea.pressed === true || page1Colmun3UpMouseArea.pressed === true) ? root.page1CalcValue[2] :
                                (page1Colmun3CancelBtnMouseArea.pressed === true) ? pg1SetValue3.text :
                                    root.page1CalcFirstDisp[2] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page1SetValue3) : root.page1CalcValue[2]
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(1, 3, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page1CalcValue[2] = parseFloat(newValue).toFixed(1)
                        root.page1CalcFirstDisp[2] = true
                    }
                }
                Image {
                    x:636
                    y:285
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page1Colmun3UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page1CalcUpDownbtn3Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            let page1Value3
                            if (root.page1CalcFirstDisp[2] === false){
                                page1Value3 = root.axisDataList[root.axisIndex].page1SetValue3
                                root.page1CalcValue[2] = parseFloat(page1Value3).toFixed(1)
                                root.page1CalcFirstDisp[2] = true
                            }
                            page1Value3 = root.page1CalcValue[2]
                            page1Value3++;
                            root.page1CalcValue[2] = parseFloat(page1Value3).toFixed(1)
                            root.page1CalcUpDownbtn3Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    y:282
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page1BackupValue[2] = root.axisDataList[root.axisIndex].page1SetValue3
                                pageModel.modbusSetCalcValue(1, 3, root.axisIndex,
                                    (root.page1CalcFirstDisp[2] === true ? root.page1CalcValue[2] : root.page1BackupValue[2]),
                                    (root.page1CalcFirstDisp[2] === true ? parseFloat(root.page1CalcValue[2]) : parseFloat(root.page1BackupValue[2])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePage(1, 3, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    y:282
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 24
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page1Colmun3CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page1CalcValue[2] = pg1SetValue3.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
            }
        }//第一页

        Item{
            id:simplePage2
            visible:((root.getCurPage(root.axisIndex) + 1) === 2)
            Rectangle{
                width: 760
                height: 56
                y:5
                color: "#eaeaea"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    font.pixelSize: 24
                    font.bold: true
                    color: "#cc3333"
                    text: resourceManager.label(43,219,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:820
                    y:2
                    border.color: "#999999"
                    color: save_mouse2.pressed ? "#cc3333" : "#cccccc"
                    radius: 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: save_mouse2.pressed ? 23 : 22
                        font.bold: true
                        color: resourceManager.rgb(43,1)
                        text: resourceManager.label(43,1,resourceManager.language)
                    }
                    MouseArea{
                        id:save_mouse2
                        anchors.fill: parent
                        onClicked: {
                            if(isManual){
                                root.remindPopupOpen()
                            }else{
                                remind_save_forbid.open()
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//保存按钮
            }
            Rectangle{
                width: 930
                height: 80
                y:60
                color: "#ffffff"
                border.color: "#999999"
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,220)
                    text: resourceManager.label(43,220,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:210
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 5
                    color: (root.axisIndex === -1) ? "#999999" : (root.axisDataList[root.axisIndex].page2Mode === 0) ? "#cc3333" : "#cccccc"
                    border.color: "#999999"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            page2modeToggleIndex = 0
                            pageModel.modbusSetPageMode(2, root.axisIndex, page2modeToggleIndex);
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,221)
                        text: resourceManager.label(43,221,resourceManager.language)
                    }
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:350
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 5
                    color: (root.axisIndex === -1) ? "#999999" : (root.axisDataList[root.axisIndex].page2Mode === 1) ? "#cc3333" : "#cccccc"
                    border.color: "#999999"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            page2modeToggleIndex = 1
                            pageModel.modbusSetPageMode(2, root.axisIndex, page2modeToggleIndex);
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,222)
                        text: resourceManager.label(43,222,resourceManager.language)
                    }
                }
            }//自适应滤波
            Rectangle{
                width: 930
                height: 80
                y:139
                color: "#ffffff"
                border.color: "#999999"
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,223)
                    text: resourceManager.label(43,223,resourceManager.language)
                }
                Text{
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 650
                    font.pixelSize: 20
                    color: resourceManager.rgb(43,270)
                    text: resourceManager.label(43,270,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:290
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg2AnyValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue1)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue1
                            }
                        }
                    }
                }
                Text{
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 350
                    font.pixelSize: 20
                    color: resourceManager.rgb(43,271)
                    text: resourceManager.label(43,271,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:586
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg2AnyValue2
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue2)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue2
                            }
                        }
                    }
                }
            }//共振频率
            Rectangle{
                width: 930
                height: 80
                y:218
                color: "#ffffff"
                border.color: "#999999"
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,224)
                    text: resourceManager.label(43,224,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:290
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg2AnyValue3
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue3)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue3
                            }
                        }
                    }
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:586
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg2AnyValue4
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue4)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue4
                            }
                        }
                    }
                }
            }//衰减量

            //数字监控
            Rectangle{
                width: 760
                height: 56
                y:318
                color: "#eaeaea"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    font.pixelSize: 24
                    font.bold: true
                    color: "#cc3333"
                    text: resourceManager.label(43,225,resourceManager.language)
                }
            }
            Rectangle{
                width: 930
                height: 80
                y:373
                color: "#ffffff"
                border.color: "#999999"
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,227)
                    text: resourceManager.label(43,227,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:190
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg2AnyValue5
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue5)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue5
                            }
                        }
                    }
                }
                Text{
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 350
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,228)
                    text: resourceManager.label(43,228,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:586
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg2AnyValue6
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue6)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page2AnyValue6
                            }
                        }
                    }
                }
            }
        }//第二页

        Item{
            id:simplePage3
            visible:((root.getCurPage(root.axisIndex) + 1) === 3)
            Rectangle{
                width: 760
                height: 56
                y:5
                color: "#eaeaea"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    font.pixelSize: 24
                    font.bold: true
                    color: "#cc3333"
                    text: resourceManager.label(43,229,resourceManager.language)
                }
                Rectangle{
                    width: 140
                    height: 40
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    color: (page3modeToggleIndex === 1) ? "#cc3333" : "#cccccc"
                    border.color: "#999999"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if (page3modeToggleIndex === 0){
                                page3modeToggleIndex = 1
                                pageModel.modbusSetPageMode(3, root.axisIndex, page3modeToggleIndex);
                            } else {
                                page3modeToggleIndex = 0
                                pageModel.modbusSetPageMode(3, root.axisIndex, page3modeToggleIndex);
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        font.bold: true
                        color: resourceManager.rgb(43,230)
                        text: resourceManager.label(43,230,resourceManager.language)
                    }
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:820
                    y:2
                    border.width: 2
                    border.color: "#999999"
                    color: save_mouse3.pressed ? "#cc3333" : "#cccccc"
                    radius: 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: save_mouse3.pressed ? 23 : 22
                        font.bold: true
                        color: resourceManager.rgb(43,1)
                        text: resourceManager.label(43,1,resourceManager.language)
                    }
                    MouseArea{
                        id:save_mouse3
                        anchors.fill: parent
                        onClicked: {
                            if(isManual){
                                root.remindPopupOpen()
                            }else{
                                remind_save_forbid.open()
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//保存按钮
            }

            Rectangle{
                width: 930
                height: 390
                y:60
                color: "#ffffff"
                border.color: "#999999"
                //标题
                Text{
                    x:198
                    y:46
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,211)
                    text: resourceManager.label(43,211,resourceManager.language)
                }
                Text{
                    x:340
                    y:46
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,212)
                    text: resourceManager.label(43,212,resourceManager.language)
                }
                Text{
                    x:532
                    y:46
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,213)
                    text: resourceManager.label(43,213,resourceManager.language)
                }
                //一行
                Text{
                    anchors.right: parent.right
                    y:100
                    anchors.rightMargin: 770
                    font.pixelSize: 22
                    font.bold: true
                    text: "0 : "
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:82
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg3DefValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3DefValue1)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3DefValue1
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    y:82
                    radius: 8
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg3SetValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue1)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue1
                            }
                        }
                    }
                }
                Image {
                    x:450
                    y:85
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page3Colmun1DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page3CalcUpDownbtn1Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            if (root.page3CalcFirstDisp[0] === false){
                                let page3Value1 = root.axisDataList[root.axisIndex].page3SetValue1
                                root.page3CalcValue[0] = parseFloat(page3Value1).toFixed(1)
                                root.page3CalcFirstDisp[0] = true
                            }
                            if (root.page3CalcValue[0] >= 1){
                                root.page3CalcValue[0]--
                            }
                            root.page3CalcUpDownbtn1Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page3Colmun1Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    y:82
                    max: "9999"
                    min: "0"
                    value: {
                        //console.log("page3CalcFirstDisp:"+root.page3CalcFirstDisp[0], "axisIndex:"+root.axisIndex, "page3CalcValue:"+root.page3CalcValue[0], "btnClicked:"+root.page3CalcUpDownbtn1Clicked)
                        let page3Value1
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return ""
                            } else {
                                if (pageModel.axisDataLoaded === false){
                                    return ""
                                } else {
                                    if (root.page3CalcUpDownbtn1Clicked === true){
                                        page3Value1 = root.page3CalcValue[0]
                                        return parseFloat(page3Value1).toFixed(1)
                                    } else {
                                        if (page3Colmun1DownMouseArea.pressed === true || page3Colmun1UpMouseArea.pressed === true){
                                            page3Value1 = page3Colmun1Value.value
                                            return parseFloat(page3Value1).toFixed(1)
                                        } else {
                                            if (page3Colmun1CancelBtnMouseArea.pressed === true){
                                                return pg3SetValue1.text
                                            } else {
                                                if (root.page3CalcFirstDisp[0] === false){
                                                    if (root.axisIndex === -1){
                                                        return ""
                                                    } else {
                                                        return root.axisDataList[root.axisIndex].page3SetValue1
                                                    }
                                                } else {
                                                    page3Value1 = root.page3CalcValue[0]
                                                    return parseFloat(page3Value1).toFixed(1)
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            if (page3Colmun1DownMouseArea.pressed === true || page3Colmun1UpMouseArea.pressed === true){
                                page3Value1 = root.page3CalcValue[0]
                                return parseFloat(page3Value1).toFixed(1)
                            } else {
                                if (page3Colmun1CancelBtnMouseArea.pressed === true){
                                    return pg3SetValue1.text
                                } else {
                                    if (root.page3CalcFirstDisp[0] === false){
                                        if (root.axisIndex === -1){
                                            return ""
                                        } else {
                                            return root.axisDataList[root.axisIndex].page3SetValue1
                                        }
                                    } else {
                                        page3Value1 = root.page3CalcValue[0]
                                        return parseFloat(page3Value1).toFixed(1)
                                    }
                                }
                            }
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(3, 1, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page3CalcValue[0] = parseFloat(newValue).toFixed(1)
                        root.page3CalcFirstDisp[0] = true
                    }
                }
                Image {
                    x:636
                    y:85
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page3Colmun1UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page3CalcUpDownbtn1Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            if (root.page3CalcFirstDisp[0] === false){
                                let page3Value1 = root.axisDataList[root.axisIndex].page3SetValue1
                                root.page3CalcValue[0] = parseFloat(page3Value1).toFixed(1)
                                root.page3CalcFirstDisp[0] = true
                            }
                            root.page3CalcValue[0]++
                            root.page3CalcUpDownbtn1Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    y:82
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page3BackupValue[0] = root.axisDataList[root.axisIndex].page3SetValue1
                                pageModel.modbusSetCalcValue(3, 1, root.axisIndex,
                                    (root.page3CalcFirstDisp[0] === true ? root.page3CalcValue[0] : root.page3BackupValue[0]),
                                    (root.page3CalcFirstDisp[0] === true ? parseFloat(root.page3CalcValue[0]) : parseFloat(root.page3BackupValue[0])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePage(3, 1, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    y:82
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page3Colmun1CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page3CalcValue[0] = pg3SetValue1.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                //二行
                Text{
                    anchors.right: parent.right
                    anchors.rightMargin: 770
                    y:200
                    font.pixelSize: 22
                    font.bold: true
                    text: "1 : "
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:182
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg3DefValue2
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3DefValue2)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3DefValue2
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    y:182
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg3SetValue2
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue2)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue2
                            }
                        }
                    }
                }
                Image {
                    x:450
                    y:185
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page3Colmun2DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page3CalcUpDownbtn2Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            if (root.page3CalcFirstDisp[1] === false){
                                let page3Value2 = root.axisDataList[root.axisIndex].page3SetValue2
                                root.page3CalcValue[1] = parseFloat(page3Value2).toFixed(1)
                                root.page3CalcFirstDisp[1] = true
                            }
                            if (root.page3CalcValue[1] >= 1){
                                root.page3CalcValue[1]--
                            }
                            root.page3CalcUpDownbtn2Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page3Colmun2Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    y:182
                    max: "9999"
                    min: "0"
                    value: {
                        //console.log("page3CalcFirstDisp:"+root.page3CalcFirstDisp[1], "axisIndex:"+root.axisIndex, "page3CalcValue:"+root.page3CalcValue[1], "btnClicked:"+root.page3CalcUpDownbtn2Clicked)
                        let page3Value2
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return ""
                            } else {
                                if (pageModel.axisDataLoaded === false){
                                    return ""
                                } else {
                                    if (root.page3CalcUpDownbtn2Clicked === true){
                                        page3Value2 = root.page3CalcValue[1]
                                        return parseFloat(page3Value2).toFixed(1)
                                    } else {
                                        if (page3Colmun2DownMouseArea.pressed === true || page3Colmun2UpMouseArea.pressed === true){
                                            page3Value2 = page3Colmun2Value.value
                                            return parseFloat(page3Value2).toFixed(1)
                                        } else {
                                            if (page3Colmun2CancelBtnMouseArea.pressed === true){
                                                return pg3SetValue2.text
                                            } else {
                                                if (root.page3CalcFirstDisp[1] === false){
                                                    if (root.axisIndex === -1){
                                                        return ""
                                                    } else {
                                                        return root.axisDataList[root.axisIndex].page3SetValue2
                                                    }
                                                } else {
                                                    page3Value2 = root.page3CalcValue[1]
                                                    return parseFloat(page3Value2).toFixed(1)
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            if (page3Colmun2DownMouseArea.pressed === true || page3Colmun2UpMouseArea.pressed === true){
                                page3Value2 = root.page3CalcValue[1]
                                return parseFloat(page3Value2).toFixed(1)
                            } else {
                                if (page3Colmun2CancelBtnMouseArea.pressed === true){
                                    return pg3SetValue2.text
                                } else {
                                    if (root.page3CalcFirstDisp[1] === false){
                                        if (root.axisIndex === -1){
                                            return ""
                                        } else {
                                            return root.axisDataList[root.axisIndex].page3SetValue2
                                        }
                                    } else {
                                        page3Value2 = root.page3CalcValue[1]
                                        return parseFloat(page3Value2).toFixed(1)
                                    }
                                }
                            }
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(3, 2, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page3CalcValue[1] = parseFloat(newValue).toFixed(1)
                        root.page3CalcFirstDisp[1] = true
                    }
                }
                Image {
                    x:636
                    y:185
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page3Colmun2UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page3CalcUpDownbtn2Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            if (root.page3CalcFirstDisp[1] === false){
                                let page3Value2 = root.axisDataList[root.axisIndex].page3SetValue2
                                root.page3CalcValue[1] = parseFloat(page3Value2).toFixed(1)
                                root.page3CalcFirstDisp[1] = true
                            }
                            root.page3CalcValue[1]++
                            root.page3CalcUpDownbtn2Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    y:182
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page3BackupValue[1] = root.axisDataList[root.axisIndex].page3SetValue2
                                pageModel.modbusSetCalcValue(3, 2, root.axisIndex,
                                    (root.page3CalcFirstDisp[1] === true ? root.page3CalcValue[1] : root.page3BackupValue[1]),
                                    (root.page3CalcFirstDisp[1] === true ? parseFloat(root.page3CalcValue[1]) : parseFloat(root.page3BackupValue[1])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePage(3, 2, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    y:182
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page3Colmun2CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page3CalcValue[1] = pg3SetValue2.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                //三行
                Text{
                    anchors.right: parent.right
                    anchors.rightMargin: 770
                    y:300
                    font.bold: true
                    font.pixelSize: 22
                    color: resourceManager.rgb(43,231)
                    text: resourceManager.label(43,231,resourceManager.language)
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:282
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg3DefValue3
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3DefValue3)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3DefValue3
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    y:282
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg3SetValue3
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue3)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue3
                            }
                        }
                    }
                }
                Image {
                    x:450
                    y:285
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page3Colmun3DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page3CalcUpDownbtn3Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            if (root.page3CalcFirstDisp[2] === false){
                                root.page3CalcValue[2] = root.axisDataList[root.axisIndex].page3SetValue3
                                root.page3CalcFirstDisp[2] = true
                            }
                            if (root.page3CalcValue[2] >= 1){
                                root.page3CalcValue[2]--
                            }
                            root.page3CalcUpDownbtn3Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page3Colmun3Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    y:282
                    max: "9999"
                    min: "0"
                    value: {
                        //console.log("page3CalcFirstDisp:"+root.page3CalcFirstDisp[2], "axisIndex:"+root.axisIndex, "page3CalcValue:"+root.page3CalcValue[2], "btnClicked:"+root.page3CalcUpDownbtn3Clicked)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return "";
                            } else {
                                return (root.page3CalcUpDownbtn3Clicked === true) ? root.page3CalcValue[2]:
                                    (page3Colmun3DownMouseArea.pressed === true || page3Colmun3UpMouseArea.pressed === true) ? page3Colmun3Value.value :
                                    (page3Colmun3CancelBtnMouseArea.pressed === true) ? pg3SetValue3.text :
                                    (pageModel.axisDataLoaded === false) ? "" :
                                        root.page3CalcFirstDisp[2] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue3) : root.page3CalcValue[2]
                            }
                        } else {
                            return (page3Colmun3DownMouseArea.pressed === true || page3Colmun3UpMouseArea.pressed === true) ? root.page3CalcValue[2] :
                                (page3Colmun3CancelBtnMouseArea.pressed === true) ? pg3SetValue3.text :
                                    root.page3CalcFirstDisp[2] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page3SetValue3) : root.page3CalcValue[2]
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(3, 3, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page3CalcValue[2] = parseInt(newValue)
                        root.page3CalcFirstDisp[2] = true
                    }
                }
                Image {
                    x:636
                    y:285
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page3Colmun3UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page3CalcUpDownbtn3Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            if (root.page3CalcFirstDisp[2] === false){
                                root.page3CalcValue[2] = root.axisDataList[root.axisIndex].page3SetValue3
                                root.page3CalcFirstDisp[2] = true
                            }
                            root.page3CalcValue[2]++
                            root.page3CalcUpDownbtn3Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    y:282
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page3BackupValue[2] = root.axisDataList[root.axisIndex].page3SetValue3
                                pageModel.modbusSetCalcValue(3, 3, root.axisIndex,
                                    (root.page3CalcFirstDisp[2] === true ? root.page3CalcValue[2] : root.page3BackupValue[2]),
                                    (root.page3CalcFirstDisp[2] === true ? parseFloat(root.page3CalcValue[2]) : parseFloat(root.page3BackupValue[2])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePage(3, 3, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    y:282
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page3Colmun3CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page3CalcValue[2] = pg3SetValue3.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
            }
        }//第三页

        Item{
            id:simplePage4
            visible:((root.getCurPage(root.axisIndex) + 1) === 4)
            Rectangle{
                width: 760
                height: 56
                y:5
                color: "#eaeaea"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    font.pixelSize: 24
                    font.bold: true
                    color: "#cc3333"
                    text: resourceManager.label(43,232,resourceManager.language)
                }
                Rectangle{
                    width: 110
                    height: 50
                    x:820
                    y:2
                    border.width: 2
                    border.color: "#999999"
                    color: save_mouse4.pressed ? "#cc3333" : "#cccccc"
                    radius: 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: save_mouse4.pressed ? 23 : 22
                        font.bold: true
                        color: resourceManager.rgb(43,1)
                        text: resourceManager.label(43,1,resourceManager.language)
                    }
                    MouseArea{
                        id:save_mouse4
                        anchors.fill: parent
                        onClicked: {
                            if(isManual){
                                root.remindPopupOpen()
                            }else{
                                remind_save_forbid.open()
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//保存按钮
            }

            Rectangle{
                width: 930
                height: 212
                y:60
                color: "#ffffff"
                border.color: "#999999"
                //最大扭矩变化量
                Text{
                    anchors.right: parent.right
                    anchors.rightMargin: 765
                    y:34
                    font.pixelSize: 20
                    font.bold: true
                    text: resourceManager.label(43,276,resourceManager.language)
                    color: resourceManager.rgb(43,276)
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:15
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg4AnyValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((updateBtnMouseArea.pressed === true) ? "0" : (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4AnyValue1)
                                }
                            } else {
                                return (updateBtnMouseArea.pressed === true) ? "0" : (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4AnyValue1
                            }
                        }
                    }
                }
                Image {
                    x:310
                    y:15
                    source: (updateBtnMouseArea.pressed === true) ? resourceManager.imagePath + "/26_machine_p/a260_01.png" : resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,272)
                        text: resourceManager.label(43,272,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:updateBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                //parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                                //pg4AnyValue1.text = "0";
                            }
                            onReleased: {
                                //parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                                // let result = pageDrvMonitorModel.getRealTimePeakTorqueData(root.axisIndex)
                                // if (result < 0){
                                //     console.log("getPeakTorqueData result:"+result)
                                //     pg4AnyValue1.text = ""
                                // } else {
                                //     pg4AnyValue1.text = pageModel.modbusGetMaxTorqueValue(root.axisIndex, result);
                                // }
                            }
                            onClicked: {
                                // NOP
                            }
                        }
                    }
                }

                //冲突判定扭矩变化量 충돌 판정 토크 변화량
                Text{
                    x:198
                    y:92
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,211)
                    text: resourceManager.label(43,211,resourceManager.language)
                }
                Text{
                    x:340
                    y:92
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,212)
                    text: resourceManager.label(43,212,resourceManager.language)
                }
                Text{
                    x:532
                    y:92
                    font.pixelSize: 20
                    font.bold: true
                    color: resourceManager.rgb(43,213)
                    text: resourceManager.label(43,213,resourceManager.language)
                }

                Text{
                    anchors.right: parent.right
                    anchors.rightMargin: 780
                    y:150
                    font.pixelSize: 20
                    font.bold: true
                    text: resourceManager.label(43,277,resourceManager.language)
                    color: resourceManager.rgb(43,277)
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    y:132
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg4DefValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4DefValue1)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4DefValue1
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    y:132
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg4SetValue1
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue1)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue1
                            }
                        }
                    }
                }
                Image {
                    x:450
                    y:135
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page4Colmun1DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page4CalcUpDownbtn1Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            if (root.page4CalcFirstDisp[0] === false){
                                root.page4CalcValue[0] = root.axisDataList[root.axisIndex].page4SetValue1
                                root.page4CalcFirstDisp[0] = true
                            }
                            if (root.page4CalcValue[0] >= 1){
                                root.page4CalcValue[0]--
                            }
                            root.page4CalcUpDownbtn1Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page4Colmun1Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    y:132
                    max: "9999"
                    min: "0"
                    value: {
                        //console.log("page4CalcFirstDisp:"+root.page4CalcFirstDisp[0], "axisIndex:"+root.axisIndex, "page4CalcValue:"+root.page4CalcValue[0], "btnClicked:"+root.page4CalcUpDownbtn1Clicked)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return ""
                            } else {
                                return (root.page4CalcUpDownbtn1Clicked === true) ? root.page4CalcValue[0]:
                                    (page4Colmun1DownMouseArea.pressed === true || page4Colmun1UpMouseArea.pressed === true) ? page4Colmun1Value.value :
                                    (page4Colmun1CancelBtnMouseArea.pressed === true) ? pg4SetValue1.text :
                                    (pageModel.axisDataLoaded === false) ? "" :
                                        root.page4CalcFirstDisp[0] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue1) : root.page4CalcValue[0]
                            }
                        } else {
                            return (page4Colmun1DownMouseArea.pressed === true || page4Colmun1UpMouseArea.pressed === true) ? root.page4CalcValue[0] :
                                (page4Colmun1CancelBtnMouseArea.pressed === true) ? pg4SetValue1.text :
                                    root.page4CalcFirstDisp[0] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue1) : root.page4CalcValue[0]
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(4, 1, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page4CalcValue[0] = parseInt(newValue)
                        root.page4CalcFirstDisp[0] = true
                    }
                }
                Image {
                    x:636
                    y:135
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page4Colmun1UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page4CalcUpDownbtn1Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            if (root.page4CalcFirstDisp[0] === false){
                                root.page4CalcValue[0] = root.axisDataList[root.axisIndex].page4SetValue1
                                root.page4CalcFirstDisp[0] = true
                            }
                            root.page4CalcValue[0]++
                            root.page4CalcUpDownbtn1Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    y:132
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page4BackupValue[0] = root.axisDataList[root.axisIndex].page4SetValue1
                                pageModel.modbusSetCalcValue(4, 1, root.axisIndex,
                                    (root.page4CalcFirstDisp[0] === true ? root.page4CalcValue[0] : root.page4BackupValue[0]),
                                    (root.page4CalcFirstDisp[0] === true ? parseFloat(root.page4CalcValue[0]) : parseFloat(root.page4BackupValue[0])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePage(4, 1, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    y:132
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page4Colmun1CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page4CalcValue[0] = pg4SetValue1.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
            }
            //节能设定
            Rectangle{
                width: 760
                height: 56
                y:285
                color: "#eaeaea"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    font.pixelSize: 24
                    font.bold: true
                    color: "#cc3333"
                    text: resourceManager.label(43,233,resourceManager.language)
                }
            }

            Rectangle{
                width: 930
                height: 108
                y:340
                color: "#ffffff"
                border.color: "#999999"
                Text{
                    anchors.right: parent.right
                    anchors.rightMargin: 795
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 19
                    font.bold: true
                    text: resourceManager.label(43,278,resourceManager.language)
                    color: resourceManager.rgb(43,278)
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:170
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg4DefValue2
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue2)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue2
                            }
                        }
                    }
                }
                Rectangle{
                    width: 130
                    height: 50
                    x:310
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text {
                        id:pg4SetValue2
                        visible: {
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                return pageModel.axisDataLoaded
                            } else {
                                return true
                            }
                        }
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color:"black"
                        text:{
                            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                                if (pageModel.pageScrolled === false){
                                    return ""
                                } else {
                                    return (pageModel.axisDataLoaded === false) ? "" : ((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue2)
                                }
                            } else {
                                return (root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue2
                            }
                        }
                    }
                }
                Image {
                    x:450
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/26_machine_p/d260_05.png"
                    MouseArea{
                        id:page4Colmun2DownMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_06.png"
                            root.page4CalcUpDownbtn2Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_05.png"
                        }
                        onClicked: {
                            if (root.page4CalcFirstDisp[1] === false){
                                root.page4CalcValue[1] = root.axisDataList[root.axisIndex].page4SetValue2
                                root.page4CalcFirstDisp[1] = true
                            }
                            if (root.page4CalcValue[1] >= 1){
                                root.page4CalcValue[1]--
                            }
                            root.page4CalcUpDownbtn2Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                LimitNumericEditBox{
                    id:page4Colmun2Value
                    textVisible: {
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            return pageModel.axisDataLoaded
                        } else {
                            return true
                        }
                    }
                    width: 130
                    height: 50
                    x:502
                    anchors.verticalCenter: parent.verticalCenter
                    max: "9999"
                    min: "0"
                    value: {
                        //console.log("page4CalcFirstDisp:"+root.page4CalcFirstDisp[1], "axisIndex:"+root.axisIndex, "page4CalcValue:"+root.page4CalcValue[1], "btnClicked:"+root.page4CalcUpDownbtn2Clicked)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            if (pageModel.pageScrolled === false){
                                return ""
                            } else {
                                return (root.page4CalcUpDownbtn2Clicked === true) ? root.page4CalcValue[1]:
                                    (page4Colmun2DownMouseArea.pressed === true || page4Colmun2UpMouseArea.pressed === true) ? page4Colmun2Value.value :
                                    (page4Colmun2CancelBtnMouseArea.pressed === true) ? pg4SetValue2.text :
                                    (pageModel.axisDataLoaded === false) ? "" :
                                        root.page4CalcFirstDisp[1] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue2) : root.page4CalcValue[1]
                            }
                        } else {
                            return (page4Colmun2DownMouseArea.pressed === true || page4Colmun2UpMouseArea.pressed === true) ? root.page4CalcValue[1] :
                                (page4Colmun2CancelBtnMouseArea.pressed === true) ? pg4SetValue2.text :
                                    root.page4CalcFirstDisp[1] === false ?((root.axisIndex === -1) ? "" : root.axisDataList[root.axisIndex].page4SetValue2) : root.page4CalcValue[1]
                        }
                    }
                    color: "#ffffff"
                    borderColor:"#999999"
                    fontPixelSize: 22
                    keyLayoutType: root.getKeyType(4, 2, root.axisIndex)
                    onEnter: {
                        value = newValue;
                        root.page4CalcValue[1] = parseInt(newValue)
                        root.page4CalcFirstDisp[1] = true
                    }
                }
                Image {
                    x:636
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/26_machine_p/d260_03.png"
                    MouseArea{
                        id:page4Colmun2UpMouseArea
                        anchors.fill: parent
                        onPressed: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_04.png"
                            root.page4CalcUpDownbtn2Clicked = false;
                        }
                        onReleased: {
                            parent.source = resourceManager.imagePath + "/26_machine_p/d260_03.png"
                        }
                        onClicked: {
                            if (root.page4CalcFirstDisp[1] === false){
                                root.page4CalcValue[1] = root.axisDataList[root.axisIndex].page4SetValue2
                                root.page4CalcFirstDisp[1] = true
                            }
                            root.page4CalcValue[1]++
                            root.page4CalcUpDownbtn2Clicked = true;
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    x:690
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,217)
                        text: resourceManager.label(43,217,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page4BackupValue[1] = root.axisDataList[root.axisIndex].page4SetValue2
                                pageModel.modbusSetCalcValue(4, 2, root.axisIndex,
                                    (root.page4CalcFirstDisp[1] === true ? root.page4CalcValue[1] : root.page4BackupValue[1]),
                                    (root.page4CalcFirstDisp[1] === true ? parseFloat(root.page4CalcValue[1]) : parseFloat(root.page4BackupValue[1])))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                root.updatePageHR(4, 2, root.axisIndex);
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
                Image {
                    x:804
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/26_machine_p/a260_00.png"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: resourceManager.rgb(43,218)
                        text: resourceManager.label(43,218,resourceManager.language)
                    }
                    Rectangle{
                        anchors.centerIn: parent
                        width: 94
                        height: 50
                        radius: 8
                        color: "transparent"
                        MouseArea{
                            id:page4Colmun2CancelBtnMouseArea
                            anchors.fill: parent
                            onPressed: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_01.png"
                            }
                            onReleased: {
                                parent.parent.source = resourceManager.imagePath + "/26_machine_p/a260_00.png"
                            }
                            onClicked: {
                                root.page4CalcValue[1] = pg4SetValue2.text
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
            }
        }//第四页
    }//主要内容画面4张

    Remind_Forbid_Dialog{
        id:remind_save_forbid
        anchors.centerIn: parent
        message: resourceManager.label(43,279,resourceManager.language)
    }//非手动禁止参数保存弹窗
}
