import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
import "../../Widget"
import "PosSetUtils.js" as Utils
/*************************************************************************************
* Title: UI/UX of AEK
* File: Pos_Adjust.qml
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
    id: posAdjustPage
    width: parent.width
    height: parent.height

    property bool pageActive: false
    property var pageModel
    property var pointList: pageModel.pointList //point数据列表
    property var axisOperateViewmodel: manualViewModel.panelModel[1]
    property var axisDataList: axisOperateViewmodel.axisInfo.axisDataList //轴data
    property int mainpage: 0
    property int subpage: 0 //当前point页码 0~...
    property int subpageMax: 0
    property int pointNumber: pageModel.numPoint //point数目 0~
    property int axisPageNo: 1 //轴切换页码[6~8轴 2页]
    property int selectedPointIndex: -1 //point选择索引
    property int maxAxisPageNo: Math.ceil(numAxis / axisNumPerPage)
    readonly property int axisNumPerPage: 5
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property int axisModelNum: getAxisModelNum(axisPageNo)
    property bool dataLoaded: pageModel.dataLoaded
    property var pointSettableList: [0, 0, 0, 0, 0, 0, 0, 0]
    property bool isManual: commonViewModel.manipulateMode === 1

    function updatePointSettableList(){
        for(let i = 0; i < pointNumber ; i++){
            let pointIndex = pageModel.getPointNo(i/6, i % 6)
            pointSettableList[i] = pageModel.getPointSettable(pointIndex)
        }
        pointSettableListChanged()
    }
    function getAxisModelNum(pageNo){
        let modelNum = numAxis - (pageNo - 1) * axisNumPerPage
        if (modelNum > axisNumPerPage) {
            modelNum = axisNumPerPage
        }
        return modelNum
    }
    function nextAxisPage() {
        if (axisPageNo >= maxAxisPageNo) {
            axisPageNo = 1
        } else {
            axisPageNo++
        }
    }
    function prevAxisPage() {
        if (axisPageNo <= 1) {
            axisPageNo = maxAxisPageNo
        } else {
            axisPageNo--
        }
    }
    onPageActiveChanged: {
        if (pageActive) {
            updatePointSettableList()
        }else{
            selectedPointIndex = -1
        }
    }
    Component.onCompleted: {
        subpageMax = Math.ceil(pointNumber / 6) - 1
        updatePointSettableList()
    }
    QtObject{
        id:local
        property string tenkeyDigits: ""  //键盘显示数字
        property int language: 0
        property int currentPage: 0
        function pageUp() {
            if(subpage == 0){
                subpage = subpageMax
            }else{
                subpage = Math.max(subpage-1, 0)
            }
            pointListView.positionViewAtIndex(subpage * 6, ListView.Beginning)
        }
        function pageDown() {
            if(subpage == subpageMax){
               subpage = 0
            }else{
                subpage = Math.min(subpage+1, subpageMax)
            }
            pointListView.positionViewAtIndex(subpage * 6, ListView.Beginning)
        }
    }
    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    Rectangle{
        // id : axisName,currentPositionName
        width: parent.width - 144 // 930
        height: 100
        anchors.top: parent.top
        anchors.topMargin: 78 // PosSetPage.pos_tab2 에 있는 이미지 height 만큼 마진
        anchors.left: parent.left
        anchors.leftMargin: 30
        //현재위치 배경 shhong
        Rectangle{
            id : posnowaxis
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: parent.top
            anchors.topMargin: -60
            width: parent.width
            height: parent.height + 70
            radius: 10
            color: "#eaeaea"
            border.color: "#cccccc"
        }
        Column{
            id: aixsTitle
            spacing: -1
            Rectangle{
                // 축명칭
                width: 155
                height: 50
                color: "#999999"
                border.color: "white"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    // leftPadding: 5
                    color: "black"
                    font.pixelSize: 25
                    font.bold: true
                    text: resourceManager.label(36,120,local.language)
                }
            }
            Rectangle{
                // 현재위치
                width: 155
                height: 50
                color: "#cccccc"
                border.color: "white"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    // leftPadding: 5
                    color: "black"
                    font.pixelSize: 25
                    font.bold: true
                    text: resourceManager.label(36,121,local.language)
                }
            }
        }
        Column{
            spacing: -1
            anchors.left: aixsTitle.right
            Row{
                spacing: -1
                Repeater{
                    model : axisModelNum
                    Rectangle{
                        // 축이름 (z,x,y...)
                        id: currentPositionRect
                        property int axisIndex: ((axisPageNo - 1) * axisNumPerPage) + index
                        property int axisType: axisOrder[axisIndex]
                        width: 137
                        height: 50
                        color: "#999999"
                        border.color: "white"
                        enabled: isManual && mainpage == 0
                        Text {
                            readonly property int axisLabelIndex: Utils.axisLabelIndex(currentPositionRect.axisType)
                            anchors.centerIn: parent
                            font.pixelSize: 25
                            font.bold : true
                            text: resourceManager.label(22, axisLabelIndex, local.language)
                            color: "white"
                        }
                        MouseArea{
                            anchors.fill: parent
                            onPressed: parent.color = "#cc3333"
                            onReleased: parent.color = "#999999"
                            onClicked: {
                                if (selectedPointIndex != -1) {
                                    if (mainpage == 0) {
                                        pageModel.setPosition(selectedPointIndex,currentPositionRect.axisType,
                                                              parseInt((axisDataList[currentPositionRect.axisType].position)*100),false)
                                    } else {
                                        return
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Row{
                spacing: -1
                Repeater{
                    model: axisModelNum
                    Rectangle{
                        // 현재 축위치
                        id: axisposition
                        property int axisIndex: ((axisPageNo - 1) * axisNumPerPage) + index
                        property int axisType: axisOrder[axisIndex]
                        width: 137
                        height: 50
                        color: "#cccccc"
                        border.color: "white"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 25
                            font.bold: true
                            color: "#FFFFFF"
                            text: axisDataList[axisposition.axisType].position
                        }
                    }
                }
            }
        }
        Image {
            // TODO : 옵션축 추가시에 작업 필요함 당분간 미사용.
            x:775
            anchors.verticalCenter: parent.verticalCenter
            scale: 0.8
            visible: maxAxisPageNo >= 2
            source: nextAxisPageButtonMouseArea.pressed ?  resourceManager.imagePath + "/13_POINT_set/d130_04.png" : resourceManager.imagePath + "/13_POINT_set/d130_03.png"
            MouseArea{
                id: nextAxisPageButtonMouseArea
                anchors.fill: parent
                onClicked: nextAxisPage()
            }
        }

        // 오른쪽 상단 버튼
        Rectangle{
            // 축위치 축속도
            width: 380
            height: 60
            anchors.right: parent.right
//            anchors.rightMargin: 16
            y: -60
            color: "transparent"
            Row{
                spacing: 5
                //move step 활성화시 아래 right 마진은 전부 삭제
                anchors.right: parent.right
                anchors.rightMargin: 21     // 필요하면 16 같은 값
                anchors.verticalCenter: parent.verticalCenter
                //move step 기능 개발시 활성화
//                Rectangle{
//                    width: 120
//                    height: 45
//                    color:  "#cccccc"
//                    border.color: "#FFFFFF"
//                    Text {
//                        anchors.centerIn: parent
//                        font.pixelSize: but_mouse3.pressed ? 19:20
//                        font.bold: true
//                        color: "#FFFFFF"
//                        text: "MOVE"
//                    }
//                    MouseArea{
//                        id:but_mouse3
//                        anchors.fill: parent
//                        onPressed: {
//                            parent.color = "#cc3333"
//                            pageModel.moveSelectdPoint(selectedPointIndex)
//                        }

//                        onReleased: {
//                            pageModel.moveSelectdPoint(selectedPointIndex, true)
//                            parent.color = "#cccccc"
//                        }
//                    }
//                }

                Rectangle{
                    width: 120
                    height: 45
                    // radius: 10
                    color: mainpage == 0 || but_mouse1.pressed ? "#cc3333" : "#cccccc"
                    border.color: "#FFFFFF"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: but_mouse1.pressed ? 19:20
                        font.bold: true
                        color: "#FFFFFF"
                        text: resourceManager.label(36,118,local.language)
                    }
                    MouseArea{
                        id:but_mouse1
                        anchors.fill: parent
                        onClicked: {
                            mainpage = 0
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Rectangle{
                    width: 120
                    height: 45
                    // radius: 10
                    color: mainpage == 1 || but_mouse2.pressed ? "#cc3333" : "#cccccc"
                    border.color: "#FFFFFF"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: but_mouse2.pressed ? 19:20
                        font.bold: true
                        color: "#FFFFFF"
                        text: resourceManager.label(36,119,local.language)
                    }
                    MouseArea{
                        id : but_mouse2
                        anchors.fill: parent
                        onClicked: {
                            mainpage = 1
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }
        }
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        // 각 위치값들
        Rectangle{
            id : positionaxis
            z: -1
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: parent.top
            anchors.topMargin: -60
            width: parent.width
            height: parent.height + 70
            radius: 15
            color: "#eaeaea"
            border.color: "#cccccc"
        }
        Component{
            id: listDelegate
            Rectangle{
                width: parent.width - 64
                height: 60
                color: "transparent"
                Rectangle{
                    id: pointNameRect
                    width: 155
                    height: 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: isManual
                    color: selectedPointIndex === index ? "#cc3333" : "#666666"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        // leftPadding: 4
                        // lineHeight: 0.7
                        // MODIFIED : TextWrap by IJ.YI
                        wrapMode: Text.WordWrap
                        function getLabelIndex(pointNo){
                            let ret = 0
                            if ((pointNo >= 200) && (pointNo <= 207)) {
                                ret = pointNo - 200 + 110
                            } else {
                                ret = 30 + pointNo
                            }
                            return ret
                        }//获取point信息索引
                        font.pixelSize: 22
                        font.bold: true
                        text: resourceManager.label(36, getLabelIndex(pageModel.getPointNo(subpage, index % 6)),local.language)
                        //                   color: resourceManager.rgb(36, getLabelIndex(pageModel.getPointNo(subpage, index % 6)))
                        color: "white"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if (selectedPointIndex === index) {
                                selectedPointIndex = -1
                            } else {
                                selectedPointIndex = index
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//point名称
                Row{
                    id: dataInput
                    anchors.left: pointNameRect.right
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2
                    property int pointRowIndex: index //行索引
                    Repeater{
                        model: axisModelNum
                        LimitNumericEditBox {
                            id:pos_sav1
                            width: 135
                            height: 60
                            property int shift: (mainpage === 0) ? 2 : 0 //位置or速度
                            property int axisColumnIndex: axisPageNo == 2 ? index + 5 :  index //列索引
                            property int axisType: axisOrder[axisColumnIndex]
                            property var data: (mainpage === 0) ? pointList[dataInput.pointRowIndex].positionList[axisType] : pointList[dataInput.pointRowIndex].sppedList[axisType]
                            property bool disable: (data.style === 0 || data.style === 255) //point 轴禁用
                            property int pointSettable: pointSettableList[dataInput.pointRowIndex]//输入框限制属性
                            property bool isPointSettable: (pointSettable & (0x80 >> axisType)) != 0 //输入框限制属性分配到对应输入框 on/off
                            property bool rowSelected: (selectedPointIndex === dataInput.pointRowIndex)

                            fontPixelSize: 25
                            keyLayoutType: 7
                            enablePressAndHoldClearKey: true
                            enabled: disable ? false :  isPointSettable //先受位置设定pen_set显示格式限制，显示格式未设定为0再受模式控制
                            color:!enabled ? "#cccccc" : opened ? "#cc3333" : "white"
                            // ✅ 테두리: 선택된 행이면 두께 3, 색 #cc3333
                            borderColor: (!enabled) ? "#eaeaea"
                                       : (opened ? "#cc3333"
                                       : (rowSelected ? "#cc3333" : "#eaeaea"))
                            borderWidth: (rowSelected && enabled && !opened) ? 3 : 1
                            textColor: "black" //disable ? "#9FA0A0" : isPointSettable ? "white" : "black"
                            value: resourceManager.formatData(data.value, data.style, shift, true)
                            onReady: {
                                if(mainpage == 0){
                                    numberkpad2LayoutType = 0
                                    let pointNo = pointList[dataInput.pointRowIndex].pointNo
                                    pointno = dataInput.pointRowIndex
                                    axisno = axisType
                                    max = commonViewModel.formatPointMaxValue(axisType, pointNo)
                                    min = commonViewModel.formatPointMinValue(axisType, pointNo)
                                    if(isManual){
                                        isNumberKpad2 = false
                                        setDialogPos(36,120,dataInput.pointRowIndex%8,axisColumnIndex%5,22,115)
                                    }else{
                                        isNumberKpad2 = true
                                        setDialogPos(36,150,dataInput.pointRowIndex%8,axisColumnIndex%5,53,115)
                                    }
                                }else{
                                    numberkpad2LayoutType = 1
                                    max = "100"
                                    min = "1"
                                    if(isManual){
                                        isNumberKpad2 = false
                                        setDialogPos(36,120,dataInput.pointRowIndex%8,axisColumnIndex%5,22,115)
                                    }else{
                                        isNumberKpad2 = true
                                        numberkpad2LayoutType = 1
                                        setDialogPos(36,150,dataInput.pointRowIndex%8,axisColumnIndex%5,53,115)
                                    }
                                }
                            }
                            onEnter: {
                                let edittedValue = parseFloat(newValue)
                                if(mainpage == 0){
                                    if (edittedValue !== Utils.INVALID_POSITION_VALUE) {
                                        edittedValue = (edittedValue * 100).toFixed(2)
                                    }
                                    let result = pageModel.setPosition(dataInput.pointRowIndex, axisType, edittedValue, false)
                                    if (result === 0){
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                    }
                                }else{
                                    let result = pageModel.setSpeed(dataInput.pointRowIndex, axisType, edittedValue, false)
                                    if (result === 0){
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                    }
                                }
                            }
                        }
                    }
                }//point输入框
            }
        }

        //표의 사이즈 및 위치 조정
        ListView {
            id:pointListView
            y:130
            width: parent.width
            height: (subpage == subpageMax && ((pointNumber % 6) > 0)) ? (60 * (pointNumber % 6) + (pointNumber % 6) - 1) : 360 //사이즈 조정 shhong
            clip: true
            model: pageModel.pointList
            delegate: listDelegate
            interactive: false
            spacing:1
        }
        // 프리 토글 버튼 (스크롤바 위쪽)
        // MODIFIED : 프리 토글 버튼 추가 by IJ.YI
        Rectangle {
            id: freeToggleBtn
            anchors.left: parent.right
            anchors.leftMargin: 0
            y: -57
            width: 75
            height: 60
            radius: 5
            color: commonViewModel.freeOperate ? "#cc3333" : "#cccccc"
            border.color: "#999999"
            border.width: 1
            Text {
                anchors.centerIn: parent
                text: resourceManager.label(31, 26, local.language)
                font.pixelSize: 25
                font.bold: true
                color: commonViewModel.freeOperate ? "#ffffff" : "#000000"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    commonViewModel.modbusFreeControl(!commonViewModel.freeOperate)
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }

        ImageScrollBar{

            anchors.left: parent.right
            anchors.leftMargin: 15
            y:50
            height: 422
            jogScrollOn: false
            topButtonPressImage: resourceManager.imagePath + "/07_AUTO/a070_17.png"
            topButtonReleaseImage: resourceManager.imagePath + "/07_AUTO/a070_15.png"
            botomButtonPressImage: resourceManager.imagePath + "/07_AUTO/a070_18.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/07_AUTO/a070_16.png"
            pageLabelColor: "black"
            pageLabal: (subpage + 1)  + "/" + (subpageMax + 1)
            onScrollUpClicked: local.pageUp()
            onScrollDownClicked: local.pageDown()
            //스크롤바 배경 shhong
            Rectangle {
                width: 70
                height: 450
                color: "#eaeaea"
                border.color: "#cccccc"
                radius: 35
                z: -1
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: -7
            }
        }//切换按钮
    }

}
