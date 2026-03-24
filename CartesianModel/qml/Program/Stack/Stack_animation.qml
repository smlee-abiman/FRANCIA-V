import QtQuick 2.12
import QtQml 2.3
/*************************************************************************************
* Title: UI/UX of AEK
* File: Stack_animation.qml
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
Rectangle{
    id: root
    width: 442
    height: 270
    color: "transparent"
    property bool pageActive1: false
    property bool pageActiveTransition: false
    property var xLine: "" //坐标系x轴对应轴号
    property var yLine: ""//坐标系y轴对应轴号
    property var zLine: ""//坐标系z轴对应轴号
    property int currentShow: -1
    property bool restartShow: false
    property int stackType: 0 //0:xzy; 1:zxy; 2:yxz; 3:yzx; 4:zyx; 5:xyz
    property var showIndexType0:[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39]
    property var showIndexType1:[0,1,2,3,4,20,21,22,23,24,5,6,7,8,9,25,26,27,28,29,10,11,12,13,14,30,31,32,33,34,15,16,17,18,19,35,36,37,38,39]
    property var showIndexType2:[0,20,1,21,2,22,3,23,4,24,5,25,6,26,7,27,8,28,9,29,10,30,11,31,12,32,13,33,14,34,15,35,16,36,17,37,18,38,19,39]
    property var showIndexType3:[0,5,10,15,1,6,11,16,2,7,12,17,3,8,13,18,4,9,14,19,20,25,30,35,21,26,31,36,22,27,32,37,23,28,33,38,24,29,34,39]
    property var showIndexType4:[0,5,10,15,20,25,30,35,1,6,11,16,21,26,31,36,2,7,12,17,22,27,32,37,3,8,13,18,23,28,33,38,4,9,14,19,24,29,34,39]
    property var showIndexType5:[0,20,5,25,10,30,15,35,1,21,6,26,11,31,16,36,2,22,7,27,12,32,17,37,3,23,8,28,13,33,18,38,4,24,9,29,14,34,19,39]

    signal restartshowStack()
    Image {
        x:-2
        y:1
        scale: 0.96
        source: resourceManager.imagePath + "/14_N_palletize/a140_01.png"
        Text{
            x:396
            y:132
            font.pixelSize: 16
            text: xLine
        }
        Text{
            x:22
            y:228
            font.pixelSize: 16
            text: yLine
        }
        Text{
            x:133
            y:4
            font.pixelSize: 16
            text: zLine
        }
    }

    Timer{
        id:stack_timer
        running: false
        interval: 200
        repeat: true
        onTriggered: {
            currentShow = Math.min(40, currentShow+1)
        }
    }
    Timer{
        id:stack_Transition
        running: false
        interval: 100
        repeat: false
        onTriggered: {
            pageActiveTransition = false
        }
    }

    onPageActive1Changed: {
        if(pageActive1){
            restartshowStack()
            pageActiveTransition = true
        }else{
            stack_Transition.running = true
        }
    }

    onCurrentShowChanged: {
        if(currentShow == 40){
            stack_timer.running = false
            currentShow = -1
            restartShow = 0
        }
    }
    onRestartShowChanged: {
        if(restartShow){
            currentShow = -1
            stack_timer.running = true
        }
    }
    onRestartshowStack: {
        restartShow = 1
        restartShow = 0
    }

    Column{
        id:first_floor
        x:114
        y:96
        spacing: -20
        Repeater{
            model: 4
            Row{
                id:xx
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -23 * index
                property int columnIndex: index
                Repeater{
                    model: 5
                    Image {
                        id:rect1
                        property int imageIndex: index + (xx.columnIndex * 5)
                        property bool isshowed: false

                        opacity: {
                            switch(stackType){
                            case 0:
                                return (showIndexType0[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 1:
                                return (showIndexType1[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 2:
                                return (showIndexType2[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 3:
                                return (showIndexType3[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 4:
                                return (showIndexType4[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 5:
                                return (showIndexType5[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 6:
                                return (showIndexType6[currentShow] === imageIndex) || isshowed ? 1 : 0
                            }
                        }
                        onOpacityChanged: {
                            if(opacity == 1){
                                isshowed = 1
                            }
                        }
                        source: resourceManager.imagePath + "/14_N_palletize/d140_16.png"
                        Connections {
                            target: root
                            enabled: pageActiveTransition
                            onRestartShowChanged:{
                                if(restartShow){
                                    rect1.isshowed = 0
                                }
                            }
                            onPageActive1Changed: {
                                if(!pageActive1){
                                    rect1.isshowed = 0
                                    currentShow = -1
                                    restartShow = 0
                                    stack_timer.running = false
                                }
                            }
                        }
                    }
                }
            }
        }
    }//1层
    Column{
        id:second_floor
        x:114
        y:60
        spacing: -20
        Repeater{
            model: 4
            Row{
                id: xxx
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -23 * index
                property int columnIndex: index
                Repeater{
                    model: 5
                    Image {
                        id:rect2
                        property int imageIndex: index + (xxx.columnIndex * 5) + 20
                        property bool isshowed: false

                        opacity: {
                            switch(stackType){
                            case 0:
                                return (showIndexType0[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 1:
                                return (showIndexType1[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 2:
                                return (showIndexType2[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 3:
                                return (showIndexType3[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 4:
                                return (showIndexType4[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 5:
                                return (showIndexType5[currentShow] === imageIndex) || isshowed ? 1 : 0
                            case 6:
                                return (showIndexType6[currentShow] === imageIndex) || isshowed ? 1 : 0
                            }
                        }
                        onOpacityChanged: {
                            if(opacity == 1){
                                isshowed = 1
                            }
                        }
                        source: resourceManager.imagePath + "/14_N_palletize/d140_16.png"
                        Connections {
                            target: root
                            enabled: pageActiveTransition
                            onRestartShowChanged:{
                                if(restartShow){
                                    rect2.isshowed = 0
                                }
                            }
                            onPageActive1Changed: {
                                if(!pageActive1){
                                    rect2.isshowed = 0
                                    currentShow = -1
                                    restartShow = 0
                                    stack_timer.running = false
                                }
                            }
                        }
                    }
                }
            }
        }
    }//2层
}
