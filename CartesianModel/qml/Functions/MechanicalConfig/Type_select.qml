import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../Components/RemindDialog"
import "../../Widget/ComboBox"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Type_select.qml
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
    id: type_select_page
    width: parent.width
    height: parent.height
    property var viewModel: functionViewModel.mechanicalParamView.modelSelectView
    property int page: viewModel.pageNum
    property int page_max: viewModel.pageMaxNum
    property bool pageActive: false
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property int selectedModelIdx: viewModel.selectedModelIdx
    property var axisNameIndex: [0, 1, 2, 3, 4, 5, 6, 7]

    property int axisNum1: (1 <= numAxis ? axisOrder[0] : 0)
    property var data1: viewModel.getData(axisNum1)
    property int axisNum2: (2 <= numAxis ? axisOrder[1] : 0)
    property var data2: viewModel.getData(axisNum2)
    property int axisNum3: (3 <= numAxis ? axisOrder[2] : 0)
    property var data3: viewModel.getData(axisNum3)
    property int axisNum4: (4 <= numAxis ? axisOrder[3] : 0)
    property var data4: viewModel.getData(axisNum4)
    property int axisNum5: (5 <= numAxis ? axisOrder[4] : 0)
    property var data5: viewModel.getData(axisNum5)
    property int axisNum6: (6 <= numAxis ? axisOrder[5] : 0)
    property var data6: viewModel.getData(axisNum6)
    property int axisNum7: (7 <= numAxis ? axisOrder[6] : 0)
    property var data7: viewModel.getData(axisNum7)
    property int axisNum8: (8 <= numAxis ? axisOrder[7] : 0)
    property var data8: viewModel.getData(axisNum8)
    property bool isManual: commonViewModel.manipulateMode === 1

    QtObject {
        id: local
        property int language: 0
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    onSelectedModelIdxChanged:{
        control.currentIndex = selectedModelIdx
    }

    onPageActiveChanged: {
        if (pageActive) {
            viewModel.startPageChanging();
            commonViewModel.pageChange(Enums.FunctionMechanicalParamModelSelect)
            viewModel.activate()
            control.currentIndex = viewModel.selectedModelIdx
        } else {
            viewModel.deactivate()
        }
    }

    Component.onDestruction: viewModel.deactivate()

    StringComboBox_style3{
        id: control
        width: 260
        height: 49
        x:40
        y:20
        bgRectBorderWidth: 2
        bgRectColor: "#ffffff"
        bgIndicatorColor: "#cccccc"
        comboBoxMode: viewModel.modelList
    }

    Rectangle{
        width: 110
        height: 48
        x:828
        y:20
        border.width: 2
        border.color: "#999999"
        color: save_mouse.pressed ? "#cc3333" : "#cccccc"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: save_mouse.pressed ? 25 : 24
            font.bold: true
            color: resourceManager.rgb(43,1)
            text: resourceManager.label(43,1,local.language)
        }
        MouseArea{
            id:save_mouse
            anchors.fill: parent
            onClicked: {
                if(isManual){
                    remind_save1.open()
                }else{
                    remind_save_forbid.open()
                }
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//保存按钮

    //翻页
    Item{
        visible: 6 <= numAxis
        x:920
        y:70
        Image {
            y:0
            source: resourceManager.imagePath + "/26_machine_p/a260_07" + ".png"
            Rectangle{
                width: 56
                height: 56
                anchors.centerIn: parent
                radius: 8
                color: "transparent"
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.parent.source=resourceManager.imagePath + "/26_machine_p/a260_09" + ".png"
                    }
                    onReleased:{
                        parent.parent.source=resourceManager.imagePath + "/26_machine_p/a260_07" + ".png"
                    }
                    onClicked: {
                        pageBack()
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }

        Text {
            x:18
            y:210
            font.pixelSize: 22
            color: resourceManager.dataColor(18, 5)
            text: (page+1) + "/" + page_max
        }

        Image {
            y:352
            source: resourceManager.imagePath + "/26_machine_p/a260_08" + ".png"
            Rectangle{
                width: 56
                height: 56
                anchors.centerIn: parent
                radius: 8
                color: "transparent"
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.parent.source=resourceManager.imagePath + "/26_machine_p/a260_10" + ".png"
                    }
                    onReleased:{
                        parent.parent.source=resourceManager.imagePath + "/26_machine_p/a260_08" + ".png"
                    }
                    onClicked: {
                        pageNext()
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }
    }

    ColumnLayout{
        x:40
        y:80
        spacing: 2
        Row{
            spacing: 2
            Repeater{
                model: 5
                Rectangle{
                    width: 178
                    height: 61
                    color: "#999999"
                    border.color: "#999999"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        font.bold: true
                        color: resourceManager.rgb(43,25+index)
                        text: resourceManager.label(43,25+index,local.language)
                    }
                }
            }
        }//标题

        Row{
            visible: (page == 0 ? 1 <= numAxis : 6 <= numAxis)
            spacing: 2
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    color: resourceManager.rgb(22, (page == 0 ? axisNameIndex[axisNum1] : axisNameIndex[axisNum6]))
                    text: resourceManager.label(22,(page == 0 ? axisNameIndex[axisNum1] : axisNameIndex[axisNum6]),local.language)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.centerIn: parent
                    font.pixelSize: 26
                    color: resourceManager.dataColor(18, 1)
                    text: page == 0 ? data1.makerName : data6.makerName
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.centerIn: parent
                    font.pixelSize: 26
                    color: resourceManager.dataColor(18, 2)
                    text: getCapacityStr((page == 0 ? data1.capacity : data6.capacity))
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.centerIn: parent
                    font.pixelSize: 26
                    color: resourceManager.dataColor(18, 3)
                    text: getEncoderTypeStr((page == 0 ? data1.encoderType : data6.encoderType))
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.centerIn: parent
                    font.pixelSize: 26
                    color: resourceManager.dataColor(18, 4)
                    text: resourceManager.formatData((page == 0 ? data1.stroke : data6.stroke), resourceManager.getPenSetIntValue("18", 30, (page == 0 ? axisNum1 : axisNum6)))
                }
            }
        }//Y轴

        Row{
            visible: (page == 0 ? 2 <= numAxis : 7 <= numAxis)
            spacing: 2
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.rgb(22, (page == 0 ? axisNameIndex[axisNum2] : axisNameIndex[axisNum7]))
                    text: resourceManager.label(22,(page == 0 ? axisNameIndex[axisNum2] : axisNameIndex[axisNum7]),local.language)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 1)
                    text: page == 0 ? data2.makerName : data7.makerName
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 2)
                    text: getCapacityStr((page == 0 ? data2.capacity : data7.capacity))
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 3)
                    text: getEncoderTypeStr((page == 0 ? data2.encoderType : data7.encoderType))
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 4)
                    text: resourceManager.formatData((page == 0 ? data2.stroke : data7.stroke), resourceManager.getPenSetIntValue("18", 30, (page == 0 ? axisNum2 : axisNum7)))
                }
            }
        }//MX轴

        Row{
            visible: (page == 0 ? 3 <= numAxis : 8 <= numAxis)
            spacing: 2
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.rgb(22, (page == 0 ? axisNameIndex[axisNum3] : axisNameIndex[axisNum8]))
                    text: resourceManager.label(22,(page == 0 ? axisNameIndex[axisNum3] : axisNameIndex[axisNum8]),local.language)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 1)
                    text: page == 0 ? data3.makerName : data8.makerName
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 2)
                    text: getCapacityStr((page == 0 ? data3.capacity : data8.capacity))
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 3)
                    text: getEncoderTypeStr((page == 0 ? data3.encoderType : data8.encoderType))
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 4)
                    text: resourceManager.formatData((page == 0 ? data3.stroke : data8.stroke), resourceManager.getPenSetIntValue("18", 30, (page == 0 ? axisNum3 : axisNum8)))
                }
            }
        }//MZ轴

        Row{
            visible: (page == 0 ? 4 <= numAxis : false)
            spacing: 2
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.rgb(22,axisNameIndex[axisNum4])
                    text: resourceManager.label(22,axisNameIndex[axisNum4],local.language)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 1)
                    text: data4.makerName
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 2)
                    text: getCapacityStr(data4.capacity)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 3)
                    text: getEncoderTypeStr(data4.encoderType)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 4)
                    text: resourceManager.formatData(data4.stroke, resourceManager.getPenSetIntValue("18", 30, axisNum4))
                }
            }
        }//SX轴

        Row{
            visible: (page == 0 ? 5 <= numAxis : false)
            spacing: 2
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.rgb(22,axisNameIndex[axisNum5])
                    text: resourceManager.label(22,axisNameIndex[axisNum5],local.language)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 1)
                    text: data5.makerName
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 2)
                    text: getCapacityStr(data5.capacity)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 3)
                    text: getEncoderTypeStr(data5.encoderType)
                }
            }
            Rectangle{
                width: 178
                height: 68
                color: "#cccccc"
                border.color: "#999999"
                border.width: 1
                Text{
                    visible: viewModel.dataLoaded
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22

                    color: resourceManager.dataColor(18, 4)
                    text: resourceManager.formatData(data5.stroke, resourceManager.getPenSetIntValue("18", 30, axisNum5))
                }
            }
        }//SZ轴
    }

    Remind_save{
        id:remind_save1
        remindindex: 1
        remindtype: viewModel.saveStatus
        savevalue: control.currentIndex
    }//参数保存弹窗

    Remind_Forbid_Dialog{
        id:remind_save_forbid
        anchors.centerIn: parent
        message: resourceManager.label(43,279,local.language)
    }//非手动禁止参数保存弹窗

    function getCapacityStr(capacity){
        return resourceManager.label(43,235+capacity,local.language)
    }

    function getEncoderTypeStr(encoderType){
        return resourceManager.label(43,251+encoderType,local.language)
    }

    function pageNext(){
        if(page < page_max-1){
            page ++
        }else{
            page = 0
        }
    }

    function pageBack(){
        if( 0 < page){
            page --
        }else{
            page=page_max-1
        }
    }

/***********************************************************************/
//    Connections{
//        target:phyKeyManager
//        enabled: type_select_page.pageActive
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
