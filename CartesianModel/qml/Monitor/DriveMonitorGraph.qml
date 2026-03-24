import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: DriveMonitorGraph.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: IJ.YI
**************************************************************************************/
Item {
    id:root
    width: parent.width
    height: parent.height

    property var pageModel:monitorViewModel.drvMonitorView
    property bool pageActive: false
    property bool graphMode:false       // false:通常 true:3軸表示
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length

    property int curveCurPage : 1
    property int curveTotalPages : (root.numAxis <= 5) ? 1 : root.numAxis / 5 + 1
    property int curveThreeAxisPage : 1
    property int curveThreeAxisTotalPages : (root.numAxis <= 3) ? 1 : (( root.numAxis % 3 === 0) ? root.numAxis / 3 : root.numAxis / 3 + 1)
    property int axisCount: pageModel.getAxisCount()
    property int onePageColumnMax:5
    property int threeAxisOnePageColumnMax:3
    property bool curveJogScrollOn:false
    property int oneAxisTimerSettingToggle:0
    property int threeAxis1TimerSettingToggle:0
    property int threeAxis2TimerSettingToggle:0
    property int threeAxis3TimerSettingToggle:0
    property var timerValue:["0.2s", "0.5s", "1.0s", "2.0s"]
    property var drawIndexMax:[21, 51, 101, 201]
    property var speedPeakData:pageModel.axisSpeedPeak
    property var torquePeakData:pageModel.axisTorquePeak
    property var rotationMaxData:pageModel.axisRotationMax
    property bool isRTGraphDraw:pageModel.isRTGraphDraw
    property int selectedAxisIndex: pageModel.curAxisIndex
    property int oneAxisModeIndex: selectedAxisIndex % onePageColumnMax
    property int threeAxisModeIndex: selectedAxisIndex % threeAxisOnePageColumnMax
    property bool dispMode: false

    property int scrollTotalPages:(root.graphMode === true) ? root.curveThreeAxisTotalPages : root.curveTotalPages
    property int scrollCurPage:(root.graphMode === true) ? root.curveThreeAxisPage : root.curveCurPage

    signal scrollPageDirect(int page)

    onPageActiveChanged: {
        if (root.graphMode === false){
            pageModel.setGraphDrawMode(0)
        } else {
            pageModel.setGraphDrawMode(1)
        }
        if (root.graphMode === false){
            pageModel.setTimerValue(0, root.oneAxisTimerSettingToggle);
        } else {
            pageModel.setTimerValue(1, root.threeAxis1TimerSettingToggle);
            pageModel.setTimerValue(2, root.threeAxis2TimerSettingToggle);
            pageModel.setTimerValue(3, root.threeAxis3TimerSettingToggle);
        }
    }

    function getDivisionX(axisTimer){
        switch(axisTimer){
        case 0:
            return 20;
        case 1:
            return 50;
        case 2:
            return 100;
        case 3:
            return 200;
        default:
            return 20;
        }
    }

    function getStepValue(axisTimer){
        switch(axisTimer){
        case 0:
            return 1;
        case 1:
            return 1;
        case 2:
            return 1;
        case 3:
            return 1;
        default:
            return 1;
        }
    }

    function setAxisTopIndex(page){
        if (root.graphMode === false){
            switch(page){
            case 1:
                pageModel.setCurAxis(0);
                break
            case 2:
                pageModel.setCurAxis(5);
                break
            default:
                pageModel.setCurAxis(0);
                break
            }
        } else {
            switch(page){
            case 1:
                pageModel.setCurAxis(0);
                break
            case 2:
                pageModel.setCurAxis(3);
                break
            case 3:
                pageModel.setCurAxis(6);
                break
            default:
                pageModel.setCurAxis(0);
                break
            }
        }
    }

    Timer{
        id:graphDrawTimer
        interval: 50
        running: false
        repeat: false
        onTriggered: {
            //console.log("graphDrawTimer graphMode:"+root.graphMode, "oneAxisTimerSettingToggle:"+root.oneAxisTimerSettingToggle, "threeAxis1TimerSettingToggle:"+root.threeAxis1TimerSettingToggle)
        }
    }

/******************************left area axis select*******************************************/
    function changeSelectedAxis(index){
        var axisIndex;
        if (root.graphMode === false){
            // 1軸
            axisIndex = ((root.curveCurPage - 1) * root.onePageColumnMax + index);
            pageModel.setCurAxis(axisIndex);
        }
        else {
            // 3軸
            axisIndex = ((root.curveThreeAxisPage - 1) * root.threeAxisOnePageColumnMax + index);
            pageModel.setCurAxis(axisIndex);
        }
    }
    // add bg color

    Rectangle {
        width: parent.width
        height: parent.height - 60
        anchors.top: parent.top
        anchors.topMargin: 60
        color: "#FFFFFF"
        border.width: 2
        border.color: "#CCCCCC"
    }

    Rectangle{
        id:oneAxisModeSelectDisp
        width: 110
        height: 490
        radius: 15
        x: 6
        y: 100
        color: "#EAEAEA"
        border.color: "#999999"
        border.width: 2
        visible:!(root.graphMode)
        Column{
            anchors.centerIn: parent
            spacing: 50
            Repeater{
                model: (root.numAxis < 5) ? root.numAxis % 5 : 5
                Rectangle{
                    id:axis_select_5axis
                    width: 100
                    height: 50
                    radius: 10
                    border.width: 2
                    border.color: oneAxisModeIndex == index ? "#FFFFFF" : "#999999"
                    color: oneAxisModeIndex == index ? "#CC3333" : "#CCCCCC"
                    visible:{
                        if (((root.curveCurPage - 1) * 5 + index + 1) <= root.numAxis){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    property int axisNum: root.axisOrder[(root.curveCurPage-1)*5+index]

                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        font.bold: true
                        color: oneAxisModeIndex == index ? "#FFFFFF" : "#999999"
                        text: resourceManager.label(22, axis_select_5axis.axisNum, resourceManager.language)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            changeSelectedAxis(index)
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }
        }
    }//left axis select [5 axis mode]
    Rectangle{
        id:threeAxisModeSelectDisp
        width: 110
        height: 490
        radius: 15
        x: 6
        y: 100
        color: "#EAEAEA"
        border.color: "#999999"
        visible:root.graphMode
        Column{
            anchors.centerIn: parent
            spacing: 50
            Repeater{
                model: (root.numAxis < 3) ? root.numAxis % 3 : 3
                Rectangle{
                    id:axis_select_3axis
                    width: 100
                    height: 50
                    radius: 10
                    border.width: 2
                    border.color: oneAxisModeIndex == index ? "#FFFFFF" : "#999999"
                    color: oneAxisModeIndex == index ? "#CC3333" : "#CCCCCC"
                    visible:{
                        if (((root.curveCurPage - 1) * 5 + index + 1) <= root.numAxis){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    property int axisNum: root.axisOrder[(root.curveThreeAxisPage-1)*3+index]

                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        font.bold: true
                        color: oneAxisModeIndex == index ? "#FFFFFF" : "#999999"
                        text: resourceManager.label(22, axis_select_3axis.axisNum, resourceManager.language)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            changeSelectedAxis(index)
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }
        }
    }//left axis select [3 axis mode]
/******************************middle area curve show*******************************************/

    DriveMonitorGraphLDraw{
        id: oneAxisGraph
        visible:!(root.graphMode)
        anchors.top: oneAxisModeSelectDisp.top
        anchors.topMargin:10
        anchors.left: oneAxisModeSelectDisp.right
        anchors.leftMargin:20
        xDivisions: root.getDivisionX(root.oneAxisTimerSettingToggle)
        minY: -400
        maxY: 400
        speedMinY: -((rotationMaxData[0]/6)*7)
        speedMaxY: ((rotationMaxData[0]/6)*7)
        torqueMinY: -466
        torqueMaxY: 466
        step: root.getStepValue(root.oneAxisTimerSettingToggle)
        xGridLineStep: 9
        yGridLineStep: 7
        areaWidth:652
        areaHeight:448
        bgColor:"#EFEFEE"
        borderColor:"#BABABA"
        borderWidth:1
        property real defaultMinY: -60
        property real defaultMaxY: 60
        property real defaultXGridLineStep: 9
        property real defaultYGridLineStep: 7
        graphMode:root.graphMode
        timerSettingToggle: root.oneAxisTimerSettingToggle
        axisIndex: selectedAxisIndex
    }

    DriveMonitorGraphMDraw{
        id: threeAxisFirstGraph
        visible:root.graphMode
        anchors.top: oneAxisModeSelectDisp.top
        anchors.topMargin:10
        anchors.left: oneAxisModeSelectDisp.right
        anchors.leftMargin:20
        xDivisions: root.getDivisionX(root.threeAxis1TimerSettingToggle)
        minY: -400
        maxY: 400
        speedMinY: -(rotationMaxData[0])
        speedMaxY: rotationMaxData[0]
        torqueMinY: -400
        torqueMaxY: 400
        step: root.getStepValue(root.threeAxis1TimerSettingToggle)
        xGridLineStep: 9
        yGridLineStep: 7
        areaWidth:652
        areaHeight:150
        bgColor:"#EFEFEE"
        borderColor:"#BABABA"
        borderWidth:1
        timerSetVisible:true
        property real defaultMinY: -60
        property real defaultMaxY: 60
        property real defaultXGridLineStep: 9
        property real defaultYGridLineStep: 7
        speedPeakData:(root.graphMode === true) ? root.speedPeakData[0] : 0
        torquePeakData:(root.graphMode === true) ? root.torquePeakData[0] : 0
        timerSettingToggle: root.threeAxis1TimerSettingToggle
    }
    DriveMonitorGraphMDraw{
        id: threeAxisSecondGraph
        visible: {
            if(root.graphMode === true){
                if (root.curveThreeAxisPage < root.curveThreeAxisTotalPages){
                    return true;
                } else {
                    if ((root.numAxis % 3) === 2 || (root.numAxis % 3) === 0){
                        return true;
                    } else {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        anchors.top: threeAxisFirstGraph.bottom
        anchors.topMargin:13
        anchors.left: oneAxisModeSelectDisp.right
        anchors.leftMargin:20
        xDivisions: root.getDivisionX(root.threeAxis2TimerSettingToggle)
        minY: -400
        maxY: 400
        speedMinY: -(rotationMaxData[1])
        speedMaxY: rotationMaxData[1]
        torqueMinY: -400
        torqueMaxY: 400
        step: root.getStepValue(root.threeAxis2TimerSettingToggle)
        xGridLineStep: 9
        yGridLineStep: 7
        areaWidth:652
        areaHeight:150
        bgColor:"#EFEFEE"
        borderColor:"#BABABA"
        borderWidth:1
        timerSetVisible:true
        property real defaultMinY: -60
        property real defaultMaxY: 60
        property real defaultXGridLineStep: 9
        property real defaultYGridLineStep: 7
        speedPeakData:(root.graphMode === true) ? root.speedPeakData[1] : 0
        torquePeakData:(root.graphMode === true) ? root.torquePeakData[1] : 0
        timerSettingToggle: root.threeAxis2TimerSettingToggle
    }
    DriveMonitorGraphMDraw{
        id: threeAxisThirdGraph
        visible: {
            if(root.graphMode === true){
                if (root.curveThreeAxisPage < root.curveThreeAxisTotalPages){
                    return true;
                } else {
                    if ((root.numAxis % 3) === 0){
                        return true;
                    } else {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        anchors.top: threeAxisSecondGraph.bottom
        anchors.topMargin:13
        anchors.left: oneAxisModeSelectDisp.right
        anchors.leftMargin:20
        xDivisions: root.getDivisionX(root.threeAxis3TimerSettingToggle)
        minY: -400
        maxY: 400
        speedMinY: -(rotationMaxData[2])
        speedMaxY: rotationMaxData[2]
        torqueMinY: -400
        torqueMaxY: 400
        step: root.getStepValue(root.threeAxis3TimerSettingToggle)
        xGridLineStep: 9
        yGridLineStep: 7
        areaWidth:652
        areaHeight:150
        bgColor:"#EFEFEE"
        borderColor:"#BABABA"
        borderWidth:1
        timerSetVisible:true
        property real defaultMinY: -60
        property real defaultMaxY: 60
        property real defaultXGridLineStep: 9
        property real defaultYGridLineStep: 7
        speedPeakData:(root.graphMode === true) ? root.speedPeakData[2] : 0
        torquePeakData:(root.graphMode === true) ? root.torquePeakData[2] : 0
        timerSettingToggle: root.threeAxis3TimerSettingToggle
    }

    function changeDispAxisNum(){
        var selectedAxisIndex = root.selectedAxisIndex;
        if (root.graphMode === false){
            // 1軸->3軸
            root.graphMode = true;
            pageModel.setGraphDrawMode(1);
            switch(selectedAxisIndex % 3){
            case 0:
                root.threeAxis1TimerSettingToggle = root.oneAxisTimerSettingToggle;
                pageModel.setTimerValue(1, root.threeAxis1TimerSettingToggle);
                break;
            case 1:
                root.threeAxis2TimerSettingToggle = root.oneAxisTimerSettingToggle;
                pageModel.setTimerValue(2, root.threeAxis2TimerSettingToggle);
                break;
            case 2:
                root.threeAxis3TimerSettingToggle = root.oneAxisTimerSettingToggle;
                pageModel.setTimerValue(3, root.threeAxis3TimerSettingToggle);
                break;
            }

            if(selectedAxisIndex < 3){
                scrollPageDirect(1);
            }
            else if(selectedAxisIndex < 6){
                scrollPageDirect(2);
            }
            else{
                scrollPageDirect(3);
            }
            pageModel.setCurAxis(selectedAxisIndex);
        }
        else {
            // 3軸->1軸
            root.graphMode = false;
            pageModel.setGraphDrawMode(0);
            if(root.threeAxisModeIndex == 0){
                root.oneAxisTimerSettingToggle = root.threeAxis1TimerSettingToggle;
            }
            else if(root.threeAxisModeIndex == 1){
                root.oneAxisTimerSettingToggle = root.threeAxis2TimerSettingToggle;
            }
            else{
                root.oneAxisTimerSettingToggle = root.threeAxis3TimerSettingToggle;
            }
            pageModel.setTimerValue(0, root.oneAxisTimerSettingToggle);

            if (selectedAxisIndex < 5){
                root.curveCurPage = 1;
            } else{
                root.curveCurPage = 2;
            }
            pageModel.setCurAxis(selectedAxisIndex);
        }
    }

    SimpleImageToggleButton{
        id:axisButton
        anchors.top:(root.graphMode) ? threeAxisFirstGraph.top : oneAxisGraph.top
        anchors.topMargin:0
        anchors.right:(root.graphMode) ? threeAxisFirstGraph.right : oneAxisGraph.right
        anchors.rightMargin:0
        offImage:resourceManager.imagePath + "/29_AMP_moni/d290_00" + ".png"
        onImage:resourceManager.imagePath + "/29_AMP_moni/d290_02" + ".png"
        on: dispMode
        onPressed: {
            axisButton.offImage = resourceManager.imagePath + "/29_AMP_moni/d290_01" + ".png"
            axisButton.onImage = resourceManager.imagePath + "/29_AMP_moni/d290_03" + ".png"
        }
        onReleased: {
            axisButton.offImage = resourceManager.imagePath + "/29_AMP_moni/d290_00" + ".png"
            axisButton.onImage = resourceManager.imagePath + "/29_AMP_moni/d290_02" + ".png"
        }
        onClicked:{
            console.debug("axisButton onClicked axisButton on:", axisButton.on)
            dispMode = !dispMode
            changeDispAxisNum();
        }
    }
    SimpleImageToggleButton{
        id:axisButton2
        visible: {
            if(root.graphMode === true){
                if (root.curveThreeAxisPage < root.curveThreeAxisTotalPages){
                    return true;
                } else {
                    if ((root.numAxis % 3) === 2 || (root.numAxis % 3) === 0){
                        return true;
                    } else {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        anchors.top:threeAxisSecondGraph.top
        anchors.topMargin:0
        anchors.right:threeAxisSecondGraph.right
        anchors.rightMargin:0
        offImage: resourceManager.imagePath + "/29_AMP_moni/d290_00" + ".png"
        onImage: resourceManager.imagePath + "/29_AMP_moni/d290_02" + ".png"
        on: dispMode
        onPressed: {
            axisButton2.offImage = resourceManager.imagePath + "/29_AMP_moni/d290_01" + ".png"
            axisButton2.onImage = resourceManager.imagePath + "/29_AMP_moni/d290_03" + ".png"
        }
        onReleased: {
            axisButton2.offImage = resourceManager.imagePath + "/29_AMP_moni/d290_00" + ".png"
            axisButton2.onImage = resourceManager.imagePath + "/29_AMP_moni/d290_02" + ".png"
        }
        onClicked:{
            console.debug("axisButton2 onClicked axisButton on:", axisButton.on)
            dispMode = !dispMode
            changeDispAxisNum();
        }
    }
    SimpleImageToggleButton{
        id:axisButton3
        visible: {
            if(root.graphMode === true){
                if (root.curveThreeAxisPage < root.curveThreeAxisTotalPages){
                    return true;
                } else {
                    if ((root.numAxis % 3) === 0){
                        return true;
                    } else {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        anchors.top:threeAxisThirdGraph.top
        anchors.topMargin:0
        anchors.right:threeAxisThirdGraph.right
        anchors.rightMargin:0
        offImage: resourceManager.imagePath + "/29_AMP_moni/d290_00" + ".png"
        onImage: resourceManager.imagePath + "/29_AMP_moni/d290_02" + ".png"
        on: dispMode
        onPressed: {
            axisButton3.offImage = resourceManager.imagePath + "/29_AMP_moni/d290_01" + ".png"
            axisButton3.onImage = resourceManager.imagePath + "/29_AMP_moni/d290_03" + ".png"
        }
        onReleased: {
            axisButton3.offImage = resourceManager.imagePath + "/29_AMP_moni/d290_00" + ".png"
            axisButton3.onImage = resourceManager.imagePath + "/29_AMP_moni/d290_02" + ".png"
        }
        onClicked:{
            console.debug("axisButton3 onClicked axisButton on:", axisButton.on)
            dispMode = !dispMode
            changeDispAxisNum();
        }
    }

    //刻度1
    Column{
        x:116
        y:148
        spacing: 173
        visible: !(root.graphMode)
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "400"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "[%]0"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "-400"
        }
    }
    Column{
        x:770
        y: 148
        spacing: 173
        visible: !(root.graphMode)
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: rotationMaxData[0].toString()
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "0[rpm]"
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "-" + rotationMaxData[0].toString()
        }
    }
    Row{
        x:144
        y:572
        spacing: 306
        visible: !(root.graphMode)&&root.oneAxisTimerSettingToggle === 0
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.1"
        }
        Text{
            font.pixelSize: 12
            text: "0.2"
        }
    }
    Row{
        x:144
        y:572
        spacing: 305
        visible: !(root.graphMode)&&root.oneAxisTimerSettingToggle === 1
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.25"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
    }
    Row{
        x:144
        y:572
        spacing: 310
        visible: !(root.graphMode)&&root.oneAxisTimerSettingToggle === 2
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
    }
    Row{
        x:144
        y:572
        spacing: 314
        visible: !(root.graphMode)&&root.oneAxisTimerSettingToggle === 3
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
        Text{
            font.pixelSize: 12
            text: "2"
        }
    }

    //刻度2
    Column{
        x:116
        y:120
        spacing: 53
        visible: root.graphMode
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "400"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "[%]0"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "-400"
        }
    }
    Column{
        x:770
        y:120
        spacing: 53
        visible: root.graphMode
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: rotationMaxData[0].toString()
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "0[rpm]"
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "-" + rotationMaxData[0].toString()
        }
    }
    Row{
        x:144
        y:262
        spacing: 307
        visible: root.graphMode&&root.threeAxis1TimerSettingToggle === 0
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.1"
        }
        Text{
            font.pixelSize: 12
            text: "0.2"
        }
    }
    Row{
        x:144
        y:262
        spacing: 305
        visible: root.graphMode&&root.threeAxis1TimerSettingToggle === 1
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.25"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
    }
    Row{
        x:144
        y:262
        spacing: 310
        visible: root.graphMode&&root.threeAxis1TimerSettingToggle === 2
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
    }
    Row{
        x:144
        y:262
        spacing: 314
        visible: root.graphMode&&root.threeAxis1TimerSettingToggle === 3
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
        Text{
            font.pixelSize: 12
            text: "2"
        }
    }
    Column{
        x:116
        y:284
        spacing: 53
        visible: root.graphMode
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "400"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "[%]0"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "-400"
        }
    }
    Column{
        x:770
        y:284
        spacing: 53
        visible: root.graphMode
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: rotationMaxData[1].toString()
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "0[rpm]"
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "-" + rotationMaxData[1].toString()
        }
    }
    Row{
        x:144
        y:424
        spacing: 307
        visible: root.graphMode&&root.threeAxis2TimerSettingToggle === 0
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.1"
        }
        Text{
            font.pixelSize: 12
            text: "0.2"
        }
    }
    Row{
        x:144
        y:424
        spacing: 305
        visible: root.graphMode&&root.threeAxis2TimerSettingToggle === 1
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.25"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
    }
    Row{
        x:144
        y:424
        spacing: 310
        visible: root.graphMode&&root.threeAxis2TimerSettingToggle === 2
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
    }
    Row{
        x:144
        y:424
        spacing: 314
        visible: root.graphMode&&root.threeAxis2TimerSettingToggle === 3
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
        Text{
            font.pixelSize: 12
            text: "2"
        }
    }
    Column{
        x:116
        y:448
        spacing: 53
        visible: root.graphMode && threeAxisThirdGraph.visible == true
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "400"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "[%]0"
        }
        Text{
            font.pixelSize: 12
            anchors.right: parent.right
            text: "-400"
        }
    }
    Column{
        x:770
        y:448
        spacing: 53
        visible: root.graphMode && threeAxisThirdGraph.visible == true
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: rotationMaxData[2].toString()
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "0[rpm]"
        }
        Text{
            font.pixelSize: 12
            anchors.left: parent.left
            text: "-" + rotationMaxData[2].toString()
        }
    }
    Row{
        x:144
        y:587
        spacing: 307
        visible: root.graphMode&&root.threeAxis3TimerSettingToggle === 0 && threeAxisThirdGraph.visible == true
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.1"
        }
        Text{
            font.pixelSize: 12
            text: "0.2"
        }
    }
    Row{
        x:144
        y:587
        spacing: 305
        visible: root.graphMode&&root.threeAxis3TimerSettingToggle === 1 && threeAxisThirdGraph.visible == true
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.25"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
    }
    Row{
        x:144
        y:587
        spacing: 310
        visible: root.graphMode&&root.threeAxis3TimerSettingToggle === 2 && threeAxisThirdGraph.visible == true
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "0.5"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
    }
    Row{
        x:144
        y:587
        spacing: 314
        visible: root.graphMode&&root.threeAxis3TimerSettingToggle === 3 &&  threeAxisThirdGraph.visible == true
        Text{
            font.pixelSize: 12
            text: "0"
        }
        Text{
            font.pixelSize: 12
            text: "1"
        }
        Text{
            font.pixelSize: 12
            text: "2"
        }
    }

/******************************right area*******************************************/
    onScrollPageDirect:{
        if (root.graphMode){
            root.curveThreeAxisPage = page
            root.setAxisTopIndex(root.curveThreeAxisPage)
        } else {
            root.curveCurPage = page
            root.setAxisTopIndex(root.curveCurPage)
        }
    }

    Rectangle{
        id: rightbox
        width: 140
        height: 485
        x:815
        y:100
        radius: 15
        color: "#EAEAEA"
        border.color: "#999999"
        border.width: 2
        Column{
            anchors.horizontalCenter: rightbox.horizontalCenter
            y:20
            spacing: 15
            Rectangle{
                width: 100
                height: 50
                radius: 10
                border.width: 2
                border.color: start_mouse.pressed ? "#FFFFFF" : "#999999"
                color: start_mouse.pressed ? "#CC3333" : "#CCCCCC"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    font.bold: true
                    color: start_mouse.pressed ? "#FFFFFF" : "#999999"
                    text: resourceManager.label(33, 33, resourceManager.language)
                }
                MouseArea{
                    id: start_mouse
                    anchors.fill: parent
                    onClicked: {
                        pageModel.ctrlRTGraphDraw(true);
                        operationRecord.recordStartDrvMonitor()
                        operationRecord.recordOpeDrvMonitorStart()
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//start button
            Rectangle{
                width: 100
                height: 50
                radius: 10
                border.width: 2
                border.color: stop_mouse.pressed ? "#FFFFFF" : "#999999"
                color: stop_mouse.pressed ? "#CC3333" : "#CCCCCC"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    font.bold: true
                    color: stop_mouse.pressed ? "#FFFFFF" : "#999999"
                    text: resourceManager.label(33, 41, resourceManager.language)
                }
                MouseArea{
                    id: stop_mouse
                    anchors.fill: parent
                    onClicked: {
                        pageModel.ctrlRTGraphDraw(false);
                        operationRecord.recordStopDrvMonitor()
                        operationRecord.recordOpeDrvMonitorStop()
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//stop button
        }//start stop buttons

        Rectangle{
            anchors.horizontalCenter: parent.horizontalCenter
            y:150
            width: 130
            height: 40
            color: "transparent"
            border.color: "#898989"
            visible: !(root.graphMode)
            Rectangle{
                width: 60
                height: 30
                anchors.centerIn: parent
                color: "#FFFFFF"
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 16
                    color:"black"
                    text:root.timerValue[root.oneAxisTimerSettingToggle]
                }
            }
            Rectangle{
                width: 32
                height: 32
                x:4
                anchors.verticalCenter: parent.verticalCenter
                radius: 16
                border.width: 2
                border.color: "#999999"
                color: sub_mouse.pressed ? "#CC3333" : "#CCCCCC"
                Rectangle{
                    width: 24
                    height: 5
                    anchors.centerIn: parent
                    color: sub_mouse.pressed ? "#FFFFFF" : "#666666"
                }
                MouseArea{
                    id:sub_mouse
                    anchors.fill: parent
                    onClicked: {
                        if (root.oneAxisTimerSettingToggle <= 0){
                            root.oneAxisTimerSettingToggle = 0
                        } else {
                            root.oneAxisTimerSettingToggle--
                            pageModel.setTimerValue(0, root.oneAxisTimerSettingToggle)
                        }
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }// -
            Rectangle{
                width: 32
                height: 32
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 4
                radius: 16
                border.width: 2
                border.color: "#999999"
                color: add_mouse.pressed ? "#CC3333" : "#CCCCCC"
                Rectangle{
                    width: 24
                    height: 5
                    anchors.centerIn: parent
                    color: add_mouse.pressed ? "#FFFFFF" : "#666666"
                }
                Rectangle{
                    width: 5
                    height: 24
                    anchors.centerIn: parent
                    color: add_mouse.pressed ? "#FFFFFF" : "#666666"
                }
                MouseArea{
                    id:add_mouse
                    anchors.fill: parent
                    onClicked: {
                        if (root.oneAxisTimerSettingToggle >= 3){
                            root.oneAxisTimerSettingToggle = 3
                        } else {
                            root.oneAxisTimerSettingToggle++
                            pageModel.setTimerValue(0, root.oneAxisTimerSettingToggle)
                        }
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }// +
        }//axis curve time set [1AXIS]

        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            y:150
            spacing: 8
            Rectangle{
                width: 130
                height: 40
                color: "transparent"
                border.color: "#898989"
                visible:root.graphMode
                Rectangle{
                    width: 60
                    height: 30
                    anchors.centerIn: parent
                    color: "#FFFFFF"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 16
                        color:"#000000"
                        text:root.timerValue[root.threeAxis1TimerSettingToggle]
                    }
                }
                Rectangle{
                    width: 32
                    height: 32
                    x:4
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 16
                    border.width: 2
                    border.color: "#999999"
                    color: sub_mouse1.pressed ? "#CC3333" : "#CCCCCC"
                    Rectangle{
                        width: 24
                        height: 5
                        anchors.centerIn: parent
                        color: sub_mouse1.pressed ? "#FFFFFF" : "#666666"
                    }
                    MouseArea{
                        id:sub_mouse1
                        anchors.fill: parent
                        onClicked: {
                            if (root.threeAxis1TimerSettingToggle <= 0){
                                root.threeAxis1TimerSettingToggle = 0
                            } else {
                                root.threeAxis1TimerSettingToggle--
                                pageModel.setTimerValue(1, root.threeAxis1TimerSettingToggle)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }// -
                Rectangle{
                    width: 32
                    height: 32
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 4
                    radius: 16
                    border.width: 2
                    border.color: "#999999"
                    color: add_mouse1.pressed ? "#CC3333" : "#CCCCCC"
                    Rectangle{
                        width: 24
                        height: 5
                        anchors.centerIn: parent
                        color: add_mouse1.pressed ? "#FFFFFF" : "#666666"
                    }
                    Rectangle{
                        width: 5
                        height: 24
                        anchors.centerIn: parent
                        color: add_mouse1.pressed ? "#FFFFFF" : "#666666"
                    }
                    MouseArea{
                        id:add_mouse1
                        anchors.fill: parent
                        onClicked: {
                            if (root.threeAxis1TimerSettingToggle >= 3){
                                root.threeAxis1TimerSettingToggle = 3
                            } else {
                                root.threeAxis1TimerSettingToggle++
                                pageModel.setTimerValue(1, root.threeAxis1TimerSettingToggle)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }// +
            }//AXIS 1
            Rectangle{
                width: 130
                height: 40
                color: "transparent"
                border.color: "#999999"
                visible:root.graphMode
                Rectangle{
                    width: 60
                    height: 35
                    anchors.centerIn: parent
                    color: "#FFFFFF"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 16
                        color:"#000000"
                        text:root.timerValue[root.threeAxis2TimerSettingToggle]
                    }
                }
                Rectangle{
                    width: 32
                    height: 32
                    x:4
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 16
                    border.width: 2
                    border.color: "#999999"
                    color: sub_mouse2.pressed ? "#CC3333" : "#CCCCCC"
                    Rectangle{
                        width: 24
                        height: 5
                        anchors.centerIn: parent
                        color: sub_mouse2.pressed ? "#FFFFFF" : "#666666"
                    }
                    MouseArea{
                        id:sub_mouse2
                        anchors.fill: parent
                        onClicked: {
                            if (root.threeAxis2TimerSettingToggle <= 0){
                                root.threeAxis2TimerSettingToggle = 0
                            } else {
                                root.threeAxis2TimerSettingToggle--
                                pageModel.setTimerValue(2, root.threeAxis2TimerSettingToggle)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }// -
                Rectangle{
                    width: 32
                    height: 32
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 4
                    radius: 16
                    border.width: 2
                    border.color: "#999999"
                    color: add_mouse2.pressed ? "#CC3333" : "#CCCCCC"
                    Rectangle{
                        width: 24
                        height: 5
                        anchors.centerIn: parent
                        color: add_mouse2.pressed ? "#FFFFFF" : "#666666"
                    }
                    Rectangle{
                        width: 5
                        height: 24
                        anchors.centerIn: parent
                        color: add_mouse2.pressed ? "#FFFFFF" : "#666666"
                    }
                    MouseArea{
                        id:add_mouse2
                        anchors.fill: parent
                        onClicked: {
                            if (root.threeAxis2TimerSettingToggle >= 3){
                                root.threeAxis2TimerSettingToggle = 3
                            } else {
                                root.threeAxis2TimerSettingToggle++
                                pageModel.setTimerValue(2, root.threeAxis2TimerSettingToggle)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }// +
            }//AXIS 2
            Rectangle{
                width: 130
                height: 40
                color: "transparent"
                border.color: "#898989"
                visible: {
                    if(root.graphMode === true){
                        if (root.curveThreeAxisPage < root.curveThreeAxisTotalPages){
                            return true;
                        } else {
                            if ((root.numAxis % 3) === 0){
                                return true;
                            } else {
                                return false;
                            }
                        }
                    } else {
                        return false;
                    }
                }
                Rectangle{
                    width: 60
                    height: 35
                    anchors.centerIn: parent
                    color: "#FFFFFF"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 16
                        color:"#000000"
                        text:root.timerValue[root.threeAxis3TimerSettingToggle]
                    }
                }
                Rectangle{
                    width: 32
                    height: 32
                    x:4
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 16
                    border.width: 2
                    border.color: "#999999"
                    color: sub_mouse3.pressed ? "#CC3333" : "#CCCCCC"
                    Rectangle{
                        width: 24
                        height: 5
                        anchors.centerIn: parent
                        color: sub_mouse3.pressed ? "#FFFFFF" : "#666666"
                    }
                    MouseArea{
                        id:sub_mouse3
                        anchors.fill: parent
                        onClicked: {
                            if (root.threeAxis3TimerSettingToggle <= 0){
                                root.threeAxis3TimerSettingToggle = 0
                            } else {
                                root.threeAxis3TimerSettingToggle--
                                pageModel.setTimerValue(3, root.threeAxis3TimerSettingToggle)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }// -
                Rectangle{
                    width: 32
                    height: 32
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 4
                    radius: 16
                    border.width: 2
                    border.color: "#999999"
                    color: add_mouse3.pressed ? "#CC3333" : "#CCCCCC"
                    Rectangle{
                        width: 24
                        height: 5
                        anchors.centerIn: parent
                        color: add_mouse3.pressed ? "#FFFFFF" : "#666666"
                    }
                    Rectangle{
                        width: 5
                        height: 24
                        anchors.centerIn: parent
                        color: add_mouse3.pressed ? "#FFFFFF" : "#666666"
                    }
                    MouseArea{
                        id:add_mouse3
                        anchors.fill: parent
                        onClicked: {
                            if (root.threeAxis3TimerSettingToggle >= 3){
                                root.threeAxis3TimerSettingToggle = 3
                            } else {
                                root.threeAxis3TimerSettingToggle++
                                pageModel.setTimerValue(3, root.threeAxis3TimerSettingToggle)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }// +
            }//AXIS 3
        }//axis curve time set [3AXIS]

        // "Change the `ImageScrollBar` from a vertical layout to a horizontal layout."
        Rectangle {
            width: 130
            height: 50
            radius: 25
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
            color: "#FFFFFF"
            border.color: "#999999"
            Row {
                spacing: 1
                leftPadding: 5
                anchors.verticalCenter: parent.verticalCenter
                // up
                Rectangle {
                    width: 40
                    height: 40
                    color: "transparent"
                    Image {
                        id : upscroll
                        anchors.centerIn: parent
                        source: resourceManager.imagePath + "/08_Monitor/a080_09.png"
                        MouseArea {
                            id : upScroll_mouse
                            anchors.fill: parent
                            onClicked: {
                                if (root.graphMode){
                                    if (root.curveThreeAxisPage > 1) {
                                        root.curveThreeAxisPage--
                                    } else {
                                        root.curveThreeAxisPage = root.curveThreeAxisTotalPages
                                    }
                                    root.setAxisTopIndex(root.curveThreeAxisPage)
                                } else {
                                    if (root.curveCurPage > 1) {
                                        root.curveCurPage--
                                    } else {
                                        root.curveCurPage = root.curveTotalPages
                                    }
                                    root.setAxisTopIndex(root.curveCurPage)
                                }
                            }
                        }
                    }
                }
                // page
                Rectangle {
                    width: 40
                    height: 40
                    color: "transparent"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        font.bold: true
                        color: "#000000"
                        text: root.scrollCurPage + "/" + root.scrollTotalPages
                    }
                }
                // down
                Rectangle {
                    width: 40
                    height: 40
                    color: "transparent"
                    Image {
                        id : downscroll
                        anchors.right: parent.right
                        anchors.rightMargin: -1
                        anchors.verticalCenter: parent.verticalCenter
                        source: resourceManager.imagePath + "/08_Monitor/a080_10.png"
                    }
                    MouseArea {
                        id : downscroll_mouse
                        anchors.fill: parent
                        onClicked: {
                            if (root.graphMode){
                                if (root.curveThreeAxisPage >= root.curveThreeAxisTotalPages) {
                                    root.curveThreeAxisPage = 1
                                } else {
                                    root.curveThreeAxisPage++
                                }
                                root.setAxisTopIndex(root.curveThreeAxisPage)
                            } else {
                                if (root.curveCurPage >= root.curveTotalPages) {
                                    root.curveCurPage = 1
                                } else {
                                    root.curveCurPage++
                                }
                                root.setAxisTopIndex(root.curveCurPage)
                            }
                        }
                    }
                }
            }
        }
    }//right area

/*************************************************************************/
    Connections{
        target:pageModel
        onGraphSpeedDataFirstChanged:{
            if (root.graphMode === true){
                threeAxisFirstGraph.blueLineValues = graphSpeedDataFirst;
                // threeAxisFirstGraph.requestPaintBlue(root.firstBlueGraphDrawIndex)
                threeAxisFirstGraph.requestPaintBlue(0)
                // root.firstBlueGraphDrawIndex++;
                // if (root.firstBlueGraphDrawIndex >= root.drawIndexMax[root.threeAxis1TimerSettingToggle]){
                //     root.firstBlueGraphDrawIndex = 0
                // }
            }
        }
        onGraphTorqueDataFirstChanged:{
            if (root.graphMode === true){
                threeAxisFirstGraph.redLineValues = graphTorqueDataFirst;
                // threeAxisFirstGraph.requestPaintRed(root.firstRedGraphDrawIndex)
                threeAxisFirstGraph.requestPaintRed(0)
                // root.firstRedGraphDrawIndex++;
                // if (root.firstRedGraphDrawIndex >= root.drawIndexMax[root.threeAxis1TimerSettingToggle]){
                //     root.firstRedGraphDrawIndex = 0
                // }
            }
        }
        onGraphSpeedDataSecondChanged:{
            if (root.graphMode === true){
                threeAxisSecondGraph.blueLineValues = graphSpeedDataSecond;
                // threeAxisSecondGraph.requestPaintBlue(root.secondBlueGraphDrawIndex)
                threeAxisSecondGraph.requestPaintBlue(0)
                // root.secondBlueGraphDrawIndex++;
                // if (root.secondBlueGraphDrawIndex >= root.drawIndexMax[root.threeAxis2TimerSettingToggle]){
                //     root.secondBlueGraphDrawIndex = 0
                // }
            }
        }
        onGraphTorqueDataSecondChanged:{
            if (root.graphMode === true){
                threeAxisSecondGraph.redLineValues = graphTorqueDataSecond;
                // threeAxisSecondGraph.requestPaintRed(root.secondRedGraphDrawIndex)
                threeAxisSecondGraph.requestPaintRed(0)
                // root.secondRedGraphDrawIndex++;
                // if (root.secondRedGraphDrawIndex >= root.drawIndexMax[root.threeAxis2TimerSettingToggle]){
                //     root.secondRedGraphDrawIndex = 0
                // }
            }
        }
        onGraphSpeedDataThirdChanged:{
            if (root.graphMode === true){
                threeAxisThirdGraph.blueLineValues = graphSpeedDataThird;
                // threeAxisThirdGraph.requestPaintBlue(root.thirdBlueGraphDrawIndex)
                threeAxisThirdGraph.requestPaintBlue(0)
                // root.thirdBlueGraphDrawIndex++;
                // if (root.thirdBlueGraphDrawIndex >= root.drawIndexMax[root.threeAxis3TimerSettingToggle]){
                //     root.thirdBlueGraphDrawIndex = 0
                // }
            }
        }
        onGraphTorqueDataThirdChanged:{
            if (root.graphMode === true){
                threeAxisThirdGraph.redLineValues = graphTorqueDataThird;
                // threeAxisThirdGraph.requestPaintRed(root.thirdRedGraphDrawIndex)
                threeAxisThirdGraph.requestPaintRed(0)
                // root.thirdRedGraphDrawIndex++;
                // if (root.thirdRedGraphDrawIndex >= root.drawIndexMax[root.threeAxis3TimerSettingToggle]){
                //     root.thirdRedGraphDrawIndex = 0
                // }
            }
        }
    }
}
