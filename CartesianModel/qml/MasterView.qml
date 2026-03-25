import QtQuick 2.12
import QtQml 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import Enums 1.0
import "./Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MasterView.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.06.17
* Worker: IJ.YI
**************************************************************************************/
Item {
    id: root
    visible: true
    width: 1024
    height: 768

    property bool isOperation: true
    property bool stateScale: false
    property int progMaxTime:!REAL_TARGET ? 3000 : 13000
    property bool isStartupScreenNum: false
    property var progressCounter: splashScreenCounter
    property int progressCount: splashScreenCounter.count
    property bool ioPageOpened: false

    // ADD : loading image random load
    property int splashIndex : 1

    Component.onCompleted: {
        splashIndex = Math.floor(Math.random() * 3) + 1
    }

    Timer {
        id: refreshTimer
        interval: 16
        running: true
        repeat: true
    }

    Rectangle {
        id: centerAngle
        width: parent.width
        height: parent.height

        // 0~100
        property int progress: 0

        // 100% 되면 바깥(상위)에서 메인화면 전환하도록 신호로 빼두는 게 안전함
        signal finished()

        Image {
            id: splashBg
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop

            property int tryIndex: 0
            property string requestedSource: resourceManager.imagePath + "/loading.png"

            source: requestedSource

            onStatusChanged: {
                if (status === Image.Error) {
                    if (tryIndex === 0) {
                        tryIndex = 1
                        requestedSource = "file:///C:/Users/SMLEE/Downloads/francia-v0.0-master/CartesianModel/images/loading.png"
                        source = requestedSource
                    } else if (tryIndex === 1) {
                        tryIndex = 2
                        requestedSource = ":/images/loading.png"
                        source = requestedSource
                    }
                }
            }
        }

        // DEBUG: 경로 표시 (최상단 레이어, 검정색 텍스트로 흰색 배경 대비)
        Rectangle {
            anchors.centerIn: parent
            width: 460
            height: 100
            color: "black"
            opacity: 0.7
            z: 1000
            visible: false
        }

        Text {
            anchors.centerIn: parent
            text: "Image path: " + resourceManager.imagePath + "/loading.png\nStatus: " + splashBg.status
            color: "white"
            font.pixelSize: 16
            z: 1001
            visible: false
        }

        // DEBUG: 절대 경로로 테스트 이미지
        Image {
            id: testImage
            source: "file:///C:/Users/SMLEE/Downloads/francia-v0.0-master/CartesianModel/images/test_loading.png"
            anchors.top: parent.top
            anchors.left: parent.left
            width: 100
            height: 100
            z: 1002
            visible: false
        }

        Text {
            anchors.top: testImage.bottom
            anchors.left: parent.left
            text: "Test Image Status: " + testImage.status
            color: "red"
            font.pixelSize: 14
            z: 1003
            visible: false
        }

//        Text {
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.bottom: parent.bottom
//            anchors.bottomMargin: 70     // 부모 바닥에서 70px 위
//            text: centerAngle.progress + " %"
//            font.pixelSize: 30
//            font.bold: true
//            color: "black"
//        }

        ProgressBar {
            id: progressBar
            width: parent.width * 0.4
            height: 10   // 원하는 두께
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 280
            from: 0
            to: 100
            value: centerAngle.progress

            background: Rectangle {
                anchors.fill: parent
                radius: height / 2
                color: "#ffffff"          // 바(빈 부분) 색
                border.color: "#999999"   // 테두리 필요 없으면 이 줄 삭제
            }

            contentItem: Item {
                anchors.fill: parent

                Rectangle {
                    height: parent.height
                    width: progressBar.visualPosition * parent.width
                    radius: height / 2
                    color: "#cc3333"      // 진행(채워지는) 색
                }
            }
        }

        // 15초 = 150ms * 100step
        Timer {
            id: progressTimer
            interval: 150
            repeat: true
            running: true

            onTriggered: {
                if (centerAngle.progress < 100) {
                    centerAngle.progress += 1
                } else {
                    stop()
                    centerAngle.finished()   // 여기서 메인 화면 전환 트리거
                }
            }
        }

        Component.onCompleted: {
            centerAngle.progress = 0
            progressTimer.start()
        }
        Loader {
            id: subMainView
            active: false
            asynchronous: true
            visible: false
            width: parent.width
            height: 698
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            source: "qrc:/qml/Operation.qml"
        }
        Loader {
            id: top_Tab
            active: false
            asynchronous: true
            visible: false
            width: parent.width
            height: 70
            anchors.top: parent.top
            anchors.left: parent.left
            source: "qrc:/qml/TopTab.qml"
            Connections{
                target: top_Tab.item
                onBackMenu:{
                    subMainView.item.backDefaultMenu()
                }
            }
        }//上方标签

///-----------------------------------------------------------------
    }

    Loader {
        id:io_dialog
        function open(){
            openDialog()
        }
        function close(){
            if (item != null) {
                item.close()
            }
        }
        function openDialog(){
            item.x = 0
            item.y = 70
            item.open()
        } // 펑션 기능 팝업 위치
        active: true
        source: "qrc:/qml/Manual/IO_operate.qml"
    }
///-----------------------------------------------------------------
    Connections{
        target:phyKeyManager
        onMemblemSwKey03PressChanged:{
            if(phyKeyManager.memblemSwKey03Press){
                commonViewModel.selectPSaxis()
            }
        }

        onMemblemSwKey04PressChanged:{
            if(phyKeyManager.memblemSwKey04Press){
                if(!ioPageOpened){
                    io_dialog.open()
                    ioPageOpened = true
                }else{
                    io_dialog.close()
                    ioPageOpened = false
                }
            }
        }
        onMemblemSwKey17PressChanged:{
            if(phyKeyManager.memblemSwKey17Press){
                subMainView.item.backDefaultMenu()
            }
        }
    }


    onProgressCountChanged: {
        if (root.isStartupScreenNum === false){
            let modbusConnectStatus = modbusManager.getModbusConnectStatus()
            if (modbusConnectStatus === 2){
                commonViewModel.changeMainScreenId(Enums.MainScreenIdStartup)
                root.isStartupScreenNum = true
            }
        }

        if (progressCounter.elapsedTime >= root.progMaxTime) {
            if ((subMainView.status == Loader.Ready) && (top_Tab.status == Loader.Ready)) {
                progressCounter.stop()
                subMainView.visible = true
                top_Tab.visible = true
                splashBg.visible = false
                commonViewModel.permissionTimeUpdate = true
                operationRecord.recordOpePowerOn()
                commonViewModel.bootCompleted = true
            }
        }
    }

    Connections{
        target: refreshTimer
        onTriggered: {
            progressCounter.start(root.progMaxTime)
            if (!subMainView.active) {
                subMainView.active = true
            }
            if (!top_Tab.active) {
                top_Tab.active = true
            }
            enabled = false
        }
    }
}
