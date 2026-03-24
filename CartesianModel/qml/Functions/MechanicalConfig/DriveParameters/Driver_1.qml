import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../../Components/RemindDialog"
import "../../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Driver_1.qml
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
    width: parent.width
    height: parent.height
    property var viewModel: functionViewModel.mechanicalParamView.drvParamView.drvPrmView
    property int page: viewModel.pageNum
    property int page_max: viewModel.pageMaxNum
    property int axisPageNum: viewModel.axisPageNum
    property int axisPage_max: viewModel.axisPageMaxNum
    property bool pageActive: false
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length

    // このページだけ軸名称・軸データの並びが正常
    property var axisNameIndex: [0, 1, 2, 3, 4, 5, 6, 7]

    property int axisNum1: (1 <= numAxis ? axisOrder[0] : 0)
    property int axisNum2: (2 <= numAxis ? axisOrder[1] : 0)
    property int axisNum3: (3 <= numAxis ? axisOrder[2] : 0)
    property int axisNum4: (4 <= numAxis ? axisOrder[3] : 0)
    property int axisNum5: (5 <= numAxis ? axisOrder[4] : 0)
    property int axisNum6: (6 <= numAxis ? axisOrder[5] : 0)
    property int axisNum7: (7 <= numAxis ? axisOrder[6] : 0)
    property int axisNum8: (8 <= numAxis ? axisOrder[7] : 0)
    property bool isManual: commonViewModel.manipulateMode === 1

    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.FunctionMechanicalParamDrvParam)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }

    Component.onDestruction: viewModel.deactivate()

    Row{
        x:712
        y:22
        spacing: 10
        Rectangle{
            width: 100
            height: 40
            border.width: 2
            border.color: "#999999"
            color: save_mouse.pressed ? "#cc3333" : "#cccccc"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: save_mouse.pressed ? 20 : 19
                font.bold: true
                color: resourceManager.rgb(43,42)
                text: resourceManager.label(43,42,resourceManager.language)
            }
            MouseArea{
                id:save_mouse
                anchors.fill: parent
                onClicked: {
                    if(isManual){
                        remind_save4_5.remindindex = 4;
                        remind_save4_5.open()
                    }else{
                        remind_save_forbid.open()
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            width: 100
            height: 40
            border.width: 2
            border.color: "#999999"
            color: save_mouse1.pressed ? "#cc3333" : "#cccccc"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: save_mouse1.pressed ? 20 : 19
                font.bold: true
                color: resourceManager.rgb(43,1)
                text: resourceManager.label(43,1,resourceManager.language)
            }
            MouseArea{
                id:save_mouse1
                anchors.fill: parent
                onClicked: {
                    if(isManual){
                        remind_save4_5.remindindex = 5;
                        remind_save4_5.open()
                    }else{
                        remind_save_forbid.open()
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }//保存按钮
    Text {
        x:12
        y:28
        font.pixelSize: 22
        font.bold: true
        color: resourceManager.dataColor(18, 54)
        text: viewModel.modelName
    }

    //标题栏
    RowLayout {
        x:8
        y:70
        spacing: 1
        Rectangle{
            width:196
            height: 47
            color: "#999999"
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 1 <= numAxis : 6 <= numAxis)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                font.bold: true
                color: resourceManager.rgb(22, (axisPageNum == 0 ? axisNameIndex[axisNum1] : axisNameIndex[axisNum6]))
                text: resourceManager.label(22,(axisPageNum == 0 ? axisNameIndex[axisNum1] : axisNameIndex[axisNum6]),resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 2 <= numAxis : 7 <= numAxis)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                font.bold: true
                color: resourceManager.rgb(22, (axisPageNum == 0 ? axisNameIndex[axisNum2] : axisNameIndex[axisNum7]))
                text: resourceManager.label(22,(axisPageNum == 0 ? axisNameIndex[axisNum2] : axisNameIndex[axisNum7]),resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 3 <= numAxis : 8 <= numAxis)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                font.bold: true
                color: resourceManager.rgb(22, (axisPageNum == 0 ? axisNameIndex[axisNum3] : axisNameIndex[axisNum8]))
                text: resourceManager.label(22,(axisPageNum == 0 ? axisNameIndex[axisNum3] : axisNameIndex[axisNum8]),resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 4 <= numAxis : false)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                font.bold: true
                color: resourceManager.rgb(22,axisNameIndex[axisNum4])
                text: resourceManager.label(22,axisNameIndex[axisNum4],resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 5 <= numAxis : false)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 24
                font.bold: true
                color: resourceManager.rgb(22,axisNameIndex[axisNum5])
                text: resourceManager.label(22,axisNameIndex[axisNum5],resourceManager.language)
            }
        }
    }

    Image {
        visible: 6 <= numAxis
        x:960
        y:76
        source: resourceManager.imagePath + "/26_machine_p/a260_12.png"
        MouseArea{
            anchors.fill: parent
            onPressed: {
                parent.source = resourceManager.imagePath + "/26_machine_p/a260_13.png"
            }
            onReleased: {
                parent.source = resourceManager.imagePath + "/26_machine_p/a260_12.png"
            }
            onClicked: {
                if(axisPageNum < axisPage_max-1){
                    viewModel.axisPageNum ++
                }else{
                    viewModel.axisPageNum = 0
                }
            }
        }
    }//换页按钮

    //page
    Item{
        id:mec_page
        width: 940
        height: 422
        y:118
        property int pageIndex: page

        Column{
            x:8
            spacing: 1
            Repeater{
                model: 7
                Row{
                    property int rowIndex: index
                    property var dataAxis1: viewModel.getData(mec_page.pageIndex, rowIndex, 0)
                    visible: (dataAxis1.index != -1 ? true : false)
                    spacing: 1
                    enabled: isManual
                    Rectangle{
                        width:196
                        height: 59
                        color: "#999999"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.bold: true
                            font.pixelSize: lineCount != 2 ? 17 : 20
                            color: resourceManager.rgb(43,46+(mec_page.pageIndex*14)+(index*2)) + "\n" + resourceManager.rgb(43,47+(mec_page.pageIndex*14)+(index*2))
                            text: resourceManager.label(43,46+(mec_page.pageIndex*14)+(index*2),resourceManager.language) + "\n" + resourceManager.label(43,47+(mec_page.pageIndex*14)+(index*2),resourceManager.language)
                        }
                    }
                    Repeater{
                        model: 5
                        LimitNumericEditBox{
                            property int axisNum: (axisPageNum*5)+index
                            property int axisIndex: (axisNum < numAxis ? axisOrder[axisNum] : 0)
                            property var data: viewModel.getData(mec_page.pageIndex, rowIndex, axisIndex)
                            visible: ((data.index != -1 ? true : false) && axisNum < numAxis)
                            width:144
                            height: 59
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (data.isChange ? resourceManager.dataColor(18, 56) : resourceManager.dataColor(18, 55))
                            keyLayoutType: data.tenkeyType
                            max: (data.index != -1 ? data.max : "0")
                            min: (data.index != -1 ? data.min : "0")
                            value: (data.index != -1 ? resourceManager.formatData(data.value, data.format, data.scale, false) : "0")

                            onEnter: {
                                viewModel.setValue(rowIndex, axisIndex, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                    }
                }
            }
        }
    }

Item {
    id: horizontalScrollBar
    width: 500
    height: 60
    x: 258   // 필요 시 위치 조정
    y: 514

    Rectangle {

        width: parent.width
        height: parent.height
        radius: 30
        color: "#eaeaea"
        border.color: "#999999"
    }

    // 왼쪽 (이전 페이지) 버튼
    Image {
        id: leftButton
        width: 60
        height: 60
        source: mouseLeft.pressed
            ? resourceManager.imagePath + "/26_machine_p/a260_09.png"
            : resourceManager.imagePath + "/26_machine_p/a260_07.png"
        MouseArea {
            id: mouseLeft
            anchors.fill: parent
            onClicked: pageBack()
        }
    }

    // 오른쪽 (다음 페이지) 버튼
    Image {
        id: rightButton
        width: 60
        height: 60
        anchors.right: parent.right
        source: mouseRight.pressed
            ? resourceManager.imagePath + "/26_machine_p/a260_10.png"
            : resourceManager.imagePath + "/26_machine_p/a260_08.png"
        MouseArea {
            id: mouseRight
            anchors.fill: parent
            onClicked: pageNext()
        }
    }

    // 페이지 정보 표시
    Text {
        anchors.centerIn: parent
        font.pixelSize: 20
        color: "black"
        text: (page + 1) + " / " + page_max
    }
}

    Remind_save{
        id:remind_save4_5
        remindtype: viewModel.saveStatus
    }

    Remind_Forbid_Dialog{
        id:remind_save_forbid
        anchors.centerIn: parent
        message: resourceManager.label(43,279,resourceManager.language)
    }//非手动禁止参数保存弹窗

    function pageNext(){
        if(0 <= page && page+1 < page_max){
           viewModel.pageNum++
        }else{
            viewModel.pageNum=0
        }
    }

    function pageBack(){
        if(0 < page && page < page_max){
           viewModel.pageNum--
        }else{
            viewModel.pageNum = page_max-1
        }
    }

/***********************************************************************/
//    Connections{
//        target:phyKeyManager

//        onJogScrollDownChanged:{
//            if(pageActive){
//                //console.log("ManualIO: received jogScrollDown:" + jogScrollDown)
//                pageNext();
//            }
//        }

//        onJogScrollUpChanged:{
//            if(pageActive){
//                //console.log("ManualIO: received jogScrollUp:" + jogScrollUp)
//                pageBack();
//            }
//        }
//    }
//    MouseArea {
//        anchors.fill: parent
//        propagateComposedEvents: true
//        onPressed: mouse.accepted = false
//        onReleased: mouse.accepted = false
//        onClicked: mouse.accepted = false
//        onWheel: {
//            if(!REAL_TARGET) {
//                if(wheel.angleDelta.y > 0) {
//                    //console.log("mouse wheel up")
//                    phyKeyManager.decrementEmulatedJogCount()
//                } else if(wheel.angleDelta.y < 0) {
//                    //console.log("mouse wheel down")
//                    phyKeyManager.incrementEmulatedJogCount()
//                }
//            }
//        }
//    }


}
