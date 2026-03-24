import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: CounterPage.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.04
* Worker: SH.HONG
**************************************************************************************/
Item {
    id: counter_page
    width: parent.width
    height: parent.height
    property int cc: 0
    property int xx: 0
    readonly property var pageModel: teachingViewModel.pageModel[4]

    QtObject {
        id: penSet
        readonly property string numericTextColorRGB: pageModel.numericTextColorRGB
        readonly property int numOfCountersDisplayed: pageModel.numOfCountersDisplayed
        readonly property int numOfCounterPages: (penSet.numOfCountersDisplayed === 0) ? 0 : ((penSet.numOfCountersDisplayed + 5) / 6)
        readonly property int currentValueDisplayFormatNo: pageModel.currentValueDisplayFormatNo
        readonly property int settingValueDisplayFormatNo: pageModel.settingValueDisplayFormatNo
    }

    function counterPageUp() {
        let page = cc - 1
        cc = Math.max(0, page)
        commonViewModel.pageChange(Enums.TeachingCounter, cc);
    }
    function counterPageDown() {
        let page = cc + 1
        cc = Math.min(penSet.numOfCounterPages - 1, page)
        commonViewModel.pageChange(Enums.TeachingCounter, cc);
    }

    Component.onDestruction: pageModel.deactivate()
    property bool pageActive: false
    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.TeachingCounter)
            pageModel.activate()
        } else {
            pageModel.deactivate()
        }
    }

    ImageScrollBar{
        x:895
        y:60
        height: 420
        topButtonPressImage: resourceManager.imagePath + "/12_COUNTER/a120_03.png"
        topButtonReleaseImage: resourceManager.imagePath + "/12_COUNTER/a120_01.png"
        botomButtonPressImage: resourceManager.imagePath + "/12_COUNTER/a120_04.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/12_COUNTER/a120_02.png"
        pageLabal: (penSet.numOfCounterPages === 0) ? "0/0" : cc + 1  + "/" + penSet.numOfCounterPages
        onScrollUpClicked: counterPageUp()
        onScrollDownClicked: counterPageDown()
            //스크롤바 배경 shhong
        Rectangle {
            width: 70
            height: 446
            color: "#eaeaea"
            border.color: "#cccccc"
            radius: 35
            z: -1
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: -6
        }
    }//切换按钮
        //현재위치 배경 shhong
    Rectangle{
            id : counterbg
            anchors.left: parent.left
            anchors.leftMargin: 19
            anchors.top: parent.top
            anchors.topMargin: 22
            width: parent.width - 154
            height: parent.height - 116
            radius: 10
            color: "#eaeaea"
            border.color: "#cccccc"
        }
Rectangle {
    x : 42
    y : 42
    width: 800
    height: 60
    color: "transparent"

    Row {
        anchors.centerIn: parent
        spacing: 2

        Rectangle {
            width: 200
            height: 60
            color: "#999999"
            border.color: "#999999"

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(39, 1)
                text: resourceManager.label(39, 1, resourceManager.language)
            }
        }

        Rectangle {
            width: 200
            height: 60
            color: "#999999"
            border.color: "#999999"

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(39, 2)
                text: resourceManager.label(39, 2, resourceManager.language)
            }
        }

        Rectangle {
            width: 200
            height: 60
            color: "#999999"
            border.color: "#999999"

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(39, 3)
                text: resourceManager.label(39, 3, resourceManager.language)
            }
        }

        Rectangle {
            width: 200
            height: 60
            color: "#999999"
            border.color: "#999999"

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: "black"
                text: resourceManager.label(39, 4, resourceManager.language)
            }
        }
    }
}


    StackLayout{
        id:stack_count
        width: parent.width-100
        height: 450
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 39
        anchors.topMargin: 106
        currentIndex: cc
        Item{
            Column{

                spacing:4
                Repeater{
                    model: (6 <= penSet.numOfCountersDisplayed) ? 6 : penSet.numOfCountersDisplayed % 6;
                    Rectangle{
                        width: 806
                        height: 60
                        color: "#eaeaea"
                        Rectangle{
                            width: 200
                            height: 60
                            color: "#999999"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 20
                                font.bold: true
                                wrapMode: Text.WordWrap
                                property var index1: pageModel.counterMsgIndex[index]
                                text: resourceManager.label(1, index1, resourceManager.language - 1)
                                color: "white"
                            }
                        }
                        Rectangle{
                        width: 200
                        height: 60
                        x : 202
                        color: "#cccccc"
                        border.color: "#999999"
                        Image {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            source: resourceManager.imagePath + "/12_COUNTER/a120_05.png"
                            MouseArea{
                                anchors.fill: parent
                                onPressed: {
                                    parent.source = resourceManager.imagePath + "/12_COUNTER/a120_06.png"
                                    commonViewModel.modbusMemblemSwControl(true, Enums.CounterKeyResetBase + index)
                                }
                                onReleased: {
                                    parent.source = resourceManager.imagePath + "/12_COUNTER/a120_05.png"
                                    commonViewModel.modbusMemblemSwControl(false, Enums.CounterKeyResetBase + index)
                                }
                                onClicked: phyKeyManager.ctrlBuzzer()
                            }
                        }// reset button.
                        }
                        Rectangle{
                            width: 200
                            height: 60
                            x:404
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#cccccc"
                            border.color: "#999999"
                            Rectangle{
                                width: 150
                                height: 50
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                color: "#999999"
                                border.color: "#999999"
                            Text {
                                anchors.centerIn: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color: (pageModel.currentValueTbl[index] === pageModel.settingValueTbl[index]) ? penSet.numericTextColorRGB : "black"
                                font.pixelSize: 18
                                font.bold: true
                                text: {
                                    let currentValue = pageModel.currentValueTbl[index];
                                    return resourceManager.formatData(currentValue, penSet.currentValueDisplayFormatNo)
                                }
                            }
                            }
                        }// current value.
                        Rectangle{
                            x:606
                            width: 200
                            height: 60
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#cccccc"
                            border.color: "#999999"
                        LimitNumericEditBox{
                            width: 160
                            height: 50
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#ffffff"
                            textColor: "black"
                            borderColor: "#999999"
                            keyLayoutType: 0    // type0
                            min: "0"
                            max: "9999999"
                            value: {
                                let settingValue = pageModel.settingValueTbl[index];
                                return resourceManager.formatData(settingValue, penSet.settingValueDisplayFormatNo)
                            }
                            onEnter: {
                                pageModel.editSettingValue(index, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                            dialogDisplayX: 330
                            dialogDisplayY: 220
                        }// setting value.
                        }
                    }
                }
            }
        }
        Item{
            Column{
                spacing:4
                Repeater{
                    model: (12 <= penSet.numOfCountersDisplayed) ? 6 : penSet.numOfCountersDisplayed % 6;
                    Rectangle{
                        width: 806
                        height: 60
                        color: "#eaeaea"
                        Rectangle{
                            width: 200
                            height: 60
                            color: "#999999"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 20
                                font.bold: true
                                property var index1: pageModel.counterMsgIndex[index + 6]
                                color: "white"
                                text: resourceManager.label(1, index1, resourceManager.language - 1)
                            }
                        }
                        // reset button.
                        Rectangle{
                            width: 200
                            height: 60
                            x:202
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#cccccc"
                            border.color: "#999999"
                        Image {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            source: resourceManager.imagePath + "/12_COUNTER/a120_05.png"
                            MouseArea{
                                anchors.fill: parent
                                onPressed: {
                                    parent.source = resourceManager.imagePath + "/12_COUNTER/a120_06.png"
                                    commonViewModel.modbusMemblemSwControl(true, Enums.CounterKeyResetBase + index)
                                }
                                onReleased: {
                                    parent.source = resourceManager.imagePath + "/12_COUNTER/a120_05.png"
                                    commonViewModel.modbusMemblemSwControl(false, Enums.CounterKeyResetBase + index)
                                }
                                onClicked: phyKeyManager.ctrlBuzzer()
                            }
                        }
                        }
                        // current value.
                        Rectangle{
                            width: 200
                            height: 60
                            x:404
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#cccccc"
                            border.color: "#999999"
                            Rectangle{
                                width: 150
                                height: 50
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                color: "#999999"
                                border.color: "#999999"
                            Text {
                                anchors.centerIn: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color: (pageModel.currentValueTbl[index + 6] === pageModel.settingValueTbl[index + 6]) ? penSet.numericTextColorRGB : "black"
                                font.pixelSize: 18
                                font.bold: true
                                text: {
                                    let currentValue = pageModel.currentValueTbl[index + 6];
                                    return resourceManager.formatData(currentValue, penSet.currentValueDisplayFormatNo)
                                }
                            }
                        }
                        }
                        // setting value.
                        Rectangle{
                            x:606
                            width: 200
                            height: 60
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#cccccc"
                            border.color: "#999999"
                        LimitNumericEditBox{
                            width: 160
                            height: 50
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#ffffff"
                            textColor: "black"
                            borderColor: "#999999"
                            keyLayoutType: 0    // type0
                            min: "0"
                            max: "9999999"
                            value: {
                                let settingValue = pageModel.settingValueTbl[index + 6];
                                return resourceManager.formatData(settingValue, penSet.settingValueDisplayFormatNo)
                            }
                            onEnter: {
                                pageModel.editSettingValue(index + 6, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        }
                    }
                }
            }
        }

        Item{
            Column{
                spacing:4
                Repeater{
                    model: (24 <= penSet.numOfCountersDisplayed) ? 6 : penSet.numOfCountersDisplayed % 6;
                    Rectangle{
                        width: 806
                        height: 60
                        color: "#eaeaea"
                        Rectangle{
                            width: 200
                            height: 60
                            color: "#999999"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                anchors.centerIn: parent
                                font.pixelSize: 18
                                font.bold: true
                                property var index1: pageModel.counterMsgIndex[index + 18]
                                color: "white"
                                text: resourceManager.label(1, index1, resourceManager.language - 1)
                            }
                        }
                        // reset button.
                        Rectangle{
                            width: 200
                            height: 60
                            x : 202
                            color: "#cccccc"
                            border.color: "#999999"
                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: resourceManager.imagePath + "/12_COUNTER/a120_05.png"
                            MouseArea{
                                anchors.fill: parent
                                onPressed: {
                                    parent.source = resourceManager.imagePath + "/12_COUNTER/a120_06.png"
                                    commonViewModel.modbusMemblemSwControl(true, Enums.CounterKeyResetBase + index)
                                }
                                onReleased: {
                                    parent.source = resourceManager.imagePath + "/12_COUNTER/a120_05.png"
                                    commonViewModel.modbusMemblemSwControl(false, Enums.CounterKeyResetBase + index)
                                }
                                onClicked: phyKeyManager.ctrlBuzzer()
                            }
                        }
                        }
                        // current value.
                        Rectangle{
                            width: 200
                            height: 60
                            x:404
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#cccccc"
                            border.color: "#999999"
                            Rectangle{
                                width: 150
                                height: 50
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                color: "#999999"
                                border.color: "#999999"
                            Text {
                                anchors.centerIn: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color: (pageModel.currentValueTbl[index + 18] === pageModel.settingValueTbl[index + 18]) ? penSet.numericTextColorRGB : "black"
                                font.pixelSize: 18
                                font.bold: true
                                text: {
                                    let currentValue = pageModel.currentValueTbl[index + 18];
                                    return resourceManager.formatData(currentValue, penSet.currentValueDisplayFormatNo)
                                }
                            }
                        }
                        }
                        // setting value.
                        Rectangle{
                            x:606
                            width: 200
                            height: 60
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#cccccc"
                            border.color: "#999999"
                        LimitNumericEditBox{
                            width: 160
                            height: 50
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#ffffff"
                            textColor: "black"
                            borderColor: "#999999"
                            keyLayoutType: 0    // type0
                            min: "0"
                            max: "9999999"
                            value: {
                                let settingValue = pageModel.settingValueTbl[index + 18];
                                return resourceManager.formatData(settingValue, penSet.settingValueDisplayFormatNo)
                            }
                            onEnter: {
                                pageModel.editSettingValue(index + 18, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        }
                    }
                }
            }
        }
    }

    // Rectangle{
    //     width: 831
    //     height: 76
    //     border.color: "#bfbfbe"
    //     anchors.bottom: parent.bottom
    //     anchors.left: parent.left
    //     anchors.bottomMargin: -2
    //     anchors.leftMargin: 10
    //     color: "#838384"
    //     radius: 6
    //     Rectangle{
    //         width: 50
    //         height: 10
    //         anchors.right: parent.right
    //         anchors.top: parent.top
    //         color: "#838384"
    //         border.color: "#838384"
    //     }
    //     Rectangle{
    //         width: 50
    //         height: 10
    //         anchors.right: parent.right

    //         anchors.top: parent.top
    //         anchors.rightMargin: 1
    //         anchors.topMargin: 1
    //         color: "#505150"
    //     }

    //     Rectangle{
    //         width: 80
    //         height: parent.height
    //         border.color: "#bfbfbe"
    //         color: "#505150"
    //         Text{
    //             anchors.fill:parent
    //             verticalAlignment: Text.AlignVCenter
    //             horizontalAlignment: Text.AlignLeft
    //             leftPadding: 4
    //             font.pixelSize: 20
    //             lineHeight: 0.7
    //             color: "white"
    //             text: resourceManager.label(39,29, resourceManager.language)
    //         }
    //     }
    //     Label{
    //         width: 726
    //         height: 76
    //         x:81
    //         background: Rectangle{
    //             anchors.fill: parent
    //             color: "transparent"
    //         }

    //         font.pixelSize: 20
    //         wrapMode: Text.WordWrap
    //         lineHeight: 0.8
    //         color: "white"
    //         text: resourceManager.label(39, 30 + xx, resourceManager.language)
    //     }
    // }//下方提示窗口
/***********************************************************************/
//    Connections {
//        target: phyKeyManager
//        enabled: counter_page.pageActive
//        onJogScrollUpChanged:{
//            counterPageUp()
//        }
//        onJogScrollDownChanged:{
//            counterPageDown()
//        }
//    }

//    // jog dial.
//    MouseArea {
//        anchors.fill: parent
//        propagateComposedEvents: true
//        enabled: pageActive
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
