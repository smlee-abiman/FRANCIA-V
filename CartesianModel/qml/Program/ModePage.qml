import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import TeachingModePageModel 1.0
import TeachingModeData 1.0
import Enums 1.0
import "../Widget"
import "../Components/RemindDialog"
/*************************************************************************************
* Title: UI/UX of AEK
* File: ModePage.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.29
* Worker: IJ.YI
**************************************************************************************/
Item {
    id: modePage
    property bool pageActive:false
    property var pageModel: teachingViewModel.pageModel[0]
    property int modeNumPerPage: pageModel.modeNumPerPage()
    property int currentHelpIndex: currentModeList[0].keyNo - 1
    property int curModeCnt: currentModeList.length
    property bool dataLoaded: pageModel.dataLoaded
    property int currentPage: pageModel.currentPage
    property int modelNum: getModelNum(currentPage)
    property var currentModeList: pageModel.currentModeList
    property int numPages: Math.ceil(curModeCnt / modeNumPerPage)
    property bool isInauto: commonViewModel.manipulateMode === 0 || commonViewModel.manipulateMode === 2
    property string dirName: ""
    property int modeSelectedIndex: -1

    function getModelNum(pageIndex){
        let ret = modeNumPerPage
        let start = pageIndex * modeNumPerPage
        let remain = curModeCnt - start
        if (remain < modeNumPerPage) {
            ret = remain
        }
        return ret
    }

    Timer{
        id:timer
        function setTimerout(cb, delayTime){
            timer.interval = delayTime
            timer.repeat = false
            timer.triggered.connect(cb)
            timer.triggered.connect(function release(){
                timer.triggered.disconnect(cb)
                timer.triggered.disconnect(release)
            })
            timer.start()
        }
    }

    Component.onDestruction: pageModel.deactivate()

    onPageActiveChanged: {
        if (pageActive) {
            pageModel.startPageChanging()
            commonViewModel.pageChange(Enums.TeachingMode)
            pageModel.activate()
//            modeimgArea.resetAnimation()
        } else {
            pageModel.deactivate()
        }
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "10"
        readonly property string modeOnColor: resourceManager.dataColor(groupNo, 1)
    }

    Rectangle{
        width: 410
        height: 70
        radius: 35
        anchors.left: parent.left
        anchors.leftMargin: 40
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        color: "#cccccc"
        border.color: "#FFFFFF"

        // 1) 탭바는 뒤에서 페이지 인덱스만 관리
        TabBar {
            id: mode_tab
            z: 0
            anchors.fill: parent
            anchors.margins: 0
            background: Rectangle {
                radius: 35
                color: "#eaeaea"
                border.color: "#cccccc"
            }
            currentIndex: currentPage

            // 페이지 바꾸는 함수는 그대로 유지
            function prevPage(){
                let page = currentPage
                if ((page < numPages) && (page > 0)) {
                    page--
                } else {
                    page = numPages - 1
                }
                pageModel.currentPage = page
            }
            function nextPage(){
                let page = currentPage
                if ((page < (numPages - 1)) && (page >= 0)) {
                    page++
                } else {
                    page = 0
                }
                pageModel.currentPage = page
            }

            // 여기서는 실제 탭 버튼을 만들 필요가 없으니까
            // TabButton은 0개로 둬도 됨. (원하면 dummy 하나 둘 수 있음)
        }

        // 2) ← 화살표 (탭바 바깥에서 가장 위에)
        Image {
            id: modePrev
            z: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            width: 58
            height: 58
            visible: (modePage.numPages >= 2)
            source: modePrevMouse.pressed
                    ? resourceManager.imagePath + "/10_MODE/a100_01.png"
                    : resourceManager.imagePath + "/10_MODE/a100_02.png"
            MouseArea {
                id: modePrevMouse
                anchors.fill: parent
                onClicked: {
                    mode_tab.prevPage()
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }

        // 3) → 화살표
        Image {
            id: modeNext
            z: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            width: 58
            height: 58
            visible: (modePage.numPages >= 2)
            source: modeNextMouse.pressed
                    ? resourceManager.imagePath + "/10_MODE/a100_03.png"
                    : resourceManager.imagePath + "/10_MODE/a100_04.png"
            MouseArea {
                id: modeNextMouse
                anchors.fill: parent
                onClicked: {
                    mode_tab.nextPage()
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }

        // 4) 가운데 페이지 표시
        Text {
            id: modePageIndicator
            z: 5
            anchors.centerIn: parent
            visible : (modePage.curModeCnt > 0)
            font.pixelSize: 22
            color: "black"
            text: (mode_tab.currentIndex + 1) + " / " + modePage.numPages
        }
    }


    Item{
        // 모드 목록
        x : 30
        y : 20
        width: 430
        height: 484
        Rectangle{
            width: parent.width
            height: parent.height
            color: "transparent"
            border.color: "#cccccc"
            border.width: 2
            radius: 10
        }

        Column{
            x:20
            y:20
            spacing: 5
            Repeater{
                // teachingmodepagemodel.h 에서 MODE_NUM_PER_PAGE 에서 설정함
                model: modePage.modelNum
                Rectangle{
                    // 전체 큰 목록
                    id: modeItem
                    property int itemIndex: currentPage * modeNumPerPage + index
                    property var modeData: currentModeList[itemIndex]
                    width: 390
                    height: 70
                    radius: 15
                    color: modePage.modeSelectedIndex === itemIndex ? "#cc3333" : "#cccccc"
                    border.color: "#FFFFFF"
                    Rectangle{
                        // 모드 설명 클릭
                        width: 230
                        height: parent.height
                        color: "transparent"
                        MouseArea{
                            width: parent.width -10
                            height: parent.height -5
                            anchors.centerIn: parent
                            onClicked: {
                                let keyIndex = modeData.keyNo - 1
                                currentHelpIndex = keyIndex
                                modePage.modeSelectedIndex = itemIndex
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }

                    Column{
                        // 모드 약어
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 15
                        Text{
                            width: 190
                            height: 18
                            verticalAlignment: Text.AlignVCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            font.pixelSize: 20
                            font.bold: true
                            color: "black"
                            text: modeData.label1.name
                        }
                        Text{
                            // 모드이름
                            width: 190
                            height: 18
                            verticalAlignment: Text.AlignVCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            wrapMode: Text.WordWrap
                            font.pixelSize: 20
                            font.bold: true
                            color: "black"
                            text: modeData.label.name
                        }
                    }


                    ModeButton{
                        // 모드 on/off 버튼
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 6
                        visible: modePage.dataLoaded
                        count: modeData.status
                        enabled: !isInauto
                        use_te1: resourceManager.label(35, 1, resourceManager.language)
                        use_te2: resourceManager.label(35, 2, resourceManager.language)
                        use_te1_color: penSet.modeOnColor
                        onToggleButtonClick: {
                            let currentValue = modeData.status
                            var hrIndex = pageModel.getKeyNo(itemIndex)
                            var keyNo = isOnOff ? modeData.key_on : modeData.key_off
                            pageModel.modbusUpdateModeStatus(isOnOff,hrIndex);
                            commonViewModel.modbusMemblemSwControl(true, keyNo)
                            timer.setTimerout(function trigger(){commonViewModel.modbusMemblemSwControl(false, keyNo)},150)
                            operationRecord.recordSetMode(hrIndex - 1, currentValue, isOnOff)
                        }
                    }
                    Rectangle{
                        width: 153
                        height: 50
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: 10
                        anchors.bottomMargin: 8
                        radius: 30
                        visible: isInauto
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                mode_remind_dialog.open()
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
            }
        }
    }

//우측 모드 설명 및 애니메이션
    Rectangle{
        id : descriptionArea
        property var modeData: pageModel.modeDataList[currentHelpIndex]
        x : 500
        y : 20
        width: 450
        height: 559
        color: "transparent"
        border.color: "#cccccc"
        border.width: 2
        radius: 10
        Rectangle{
            id : moderightbg
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            width: 410
            height: 519
            color: "#eaeaea"

        Rectangle{
            id : modetitleName
            width: parent.width
            height: 50
            y: 10
            color: "transparent"
            Text {
                id: modeTextred
                visible: (modePage.curModeCnt > 0) ? true : false
                anchors.horizontalCenter: parent.horizontalCenter
                font.bold: true
                font.pixelSize: 25
                color: "#000000"
                text: descriptionArea.modeData.label.name
            }
            Rectangle{
                id : modetitlebg
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                width: 390
                height: 2
                color: "#cccccc"
            }
        }
        Rectangle{
            id : modedescription
            width: parent.width - 15
            height: 180
            anchors.top: modetitleName.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            color: "transparent"
            Text {
                id : descriptionText
                width: parent.width
                height: parent.height
                verticalAlignment: Text.AlignTop
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.Wrap
                font.pixelSize: 20
                color: descriptionArea.modeData.description.color
                text: descriptionArea.modeData.description.name
            }
        }
        // ADD : support animation behavior By.IJ.YI 25.07.29
        // ADD : support animation behavior By.IJ.YI 25.07.29
        Rectangle{
            id : img_ani
            width: parent.width
            height: 278
            anchors.top: modedescription.bottom
            color: "transparent"

            property int frameIndex : 0
            property int fileCount: 0

            // ✅ 핵심: 현재 선택된 modeData에 따라 폴더명이 자동 갱신되게 바인딩
            property string dirName: (descriptionArea.modeData && descriptionArea.modeData.label1)
                                     ? descriptionArea.modeData.label1.name
                                     : ""

            function getfileCount() {
                fileCount = pageModel.getFrameCount(1, dirName)
                // qDebug 찍고 싶으면 아래 활성화
                // console.log("qml: dirName=", dirName, "fileCount=", fileCount)
            }

            function imgPath(index) {
                var s = index.toString()
                while (s.length < 3)
                    s = "0" + s
                return resourceManager.imagePath + "/37_Mode_Animation/" + dirName + "/md" + s + ".png"
            }

            // ✅ dirName이 바뀌는 순간(= MDS 같은 다른 모드 선택) 프레임/카운트 리셋
            onDirNameChanged: {
                frameIndex = 0
                getfileCount()
            }

            Timer {
                id : ani_tm
                interval: 120
                repeat: true
                running: modePage.pageActive
                onTriggered: {
                    if (img_ani.fileCount <= 0) return

                    if (img_ani.frameIndex >= img_ani.fileCount) {
                        img_ani.frameIndex = 0
                    } else {
                        img_ani.frameIndex += 1
                    }
                }
            }

            Image {
                id : animation
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                width: 370
                height: 258
                fillMode: Image.PreserveAspectFit

                // ✅ dirName/fileCount 없으면 빈 소스로(이전 이미지 “잔상” 방지)
                source: (img_ani.dirName === "" || img_ani.fileCount <= 0)
                        ? ""
                        : img_ani.imgPath(img_ani.frameIndex)
            }

            Component.onCompleted: {
                // 최초 진입 시에도 카운트 계산
                getfileCount()
            }
        }
    }
}
    Remind_Forbid_Dialog{
        id:mode_remind_dialog
        x:60
        y:120
        message: resourceManager.label(35,67,resourceManager.language)
        modal: true
    }//非手动状态时模式切换禁止窗口

    /***********************************************************************/
    //    Connections {
    //        target: phyKeyManager
    //        enabled: modePage.pageActive
    //        onJogScrollDownChanged: mode_tab.nextPage()
    //        onJogScrollUpChanged: mode_tab.prevPage()
    //    }//Jog switch page unuse

    //    MouseArea {
    //        anchors.fill: parent
    //        propagateComposedEvents: true
    //        onPressed: mouse.accepted = false
    //        onReleased: mouse.accepted = false
    //        onClicked: mouse.accepted = false
    //        onWheel: {
    //            if (!REAL_TARGET) {
    //                if(wheel.angleDelta.y > 0) {
    //                    phyKeyManager.decrementEmulatedJogCount()
    //                } else if(wheel.angleDelta.y < 0) {
    //                    phyKeyManager.incrementEmulatedJogCount()
    //                }
    //            }
    //        }
    //    }// pc mouse unuse
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
