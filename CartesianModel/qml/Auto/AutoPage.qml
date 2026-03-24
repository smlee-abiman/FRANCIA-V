import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Enums 1.0
import "../Widget"

/*******************************自动运行画面********************************/
Item {
    id: auto_page
    width: parent.width
    height: 578
    property int select_tab: autoViewModel.selectTab
    property bool pageActive: false

    property bool auto_start_operable: (autoViewModel.oneCycle || autoViewModel.cycle) && !autoViewModel.step && (!autoViewModel.autoRunning || autoViewModel.isRun)
    property bool auto_start_opacity: (autoViewModel.oneCycle || autoViewModel.cycle) && !autoViewModel.step
    property bool auto_pause_operable: (autoViewModel.oneCycle || autoViewModel.cycle) && autoViewModel.autoRunning && !autoViewModel.step && !autoViewModel.isRun
    property bool auto_pause_opacity: (autoViewModel.oneCycle || autoViewModel.cycle) && autoViewModel.autoRunning && !autoViewModel.step
    property bool auto_stop_operable: autoViewModel.oneCycle || autoViewModel.cycle
    property bool step_operable: (autoViewModel.oneCycle || autoViewModel.cycle) && !autoViewModel.autoRunning
    property bool step_operable1: (autoViewModel.oneCycle || autoViewModel.cycle) && autoViewModel.step

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

    onPageActiveChanged: {
        if (pageActive) {
            autoViewModel.activate()
            setPage(auto_left_tab.currentIndex, auto_right_tab.currentIndex);
            productionPage.active = true
            ioPage.active = true
            counterPage.active = true
        } else {
            autoViewModel.deactivate()
        }
    }
    Component.onDestruction: autoViewModel.deactivate()

    Rectangle{
        anchors.fill: parent.Center
        width: 1018
        height: 590
        radius: 6
        border.color: "#898989"
        border.width: 2
        color: "#E5F4F9"
        Rectangle{
            x:1
            y:160
            width: 588
            height: 1
            color: "#898989"
        }
        Rectangle{
            x:589
            y:160
            width: 1
            height: 80
            color: "#898989"
        }
        Rectangle{
            x:589
            y:239
            width: 427
            height: 1
            color: "#898989"
        }
    }//背景
///-------------------------------------------------------------------------
    Row{
        id:auto_row
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 6
        anchors.leftMargin: 20
        spacing: 36
        Rectangle{
            id:auto_cycle1
            width: 130
            height: 150
            color: "transparent"
            Text {
                id: auto_cycle1_te
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                font.pixelSize: 22
                color: resourceManager.rgb(32,6)
                text: resourceManager.label(32,6,local.language)
            }
            Image {
                id: auto_cycle1_img
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenterOffset: auto_cycle1_mouse.pressed
                anchors.bottomMargin: auto_cycle1_mouse.pressed ? 10 : 12
                source: (autoViewModel.oneCycle) ? resourceManager.imagePath + "/07_AUTO/d070_02.png" : resourceManager.imagePath + "/07_AUTO/d070_01.png"
                MouseArea{
                    id:auto_cycle1_mouse
                    anchors.fill: parent
                    onPressed: {
                        commonViewModel.modbusMemblemSwControl(true, 241);
                    }
                    onReleased: {
                        commonViewModel.modbusMemblemSwControl(false, 241);
                    }
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
        }//1循环
        Rectangle{
            id:auto_cycle
            width: 130
            height: 150
            color: "transparent"
            Text {
                id: auto_cycle_te
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                font.pixelSize: 22
                color: resourceManager.rgb(32,7)
                text: resourceManager.label(32,7,local.language)
            }
            Image {
                id: auto_cycle_img
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenterOffset: auto_cycle2_mouse.pressed
                anchors.bottomMargin: auto_cycle2_mouse.pressed ? 10 : 12
                source: (autoViewModel.cycle) ? resourceManager.imagePath + "/07_AUTO/d070_04.png" : resourceManager.imagePath + "/07_AUTO/d070_03.png"
                MouseArea{
                    id:auto_cycle2_mouse
                    anchors.fill: parent
                    onPressed: {
                        commonViewModel.modbusMemblemSwControl(true, 240);
                    }
                    onReleased: {
                        commonViewModel.modbusMemblemSwControl(false, 240);
                    }
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
        }//全循环
        Rectangle{
            id:auto_start
            width: 130
            height: 150
            color: "transparent"
            Text {
                id: auto_start_te
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                font.pixelSize: 22
                color: resourceManager.rgb(32,8)
                text: resourceManager.label(32,8,local.language)
            }
            Image {
                id: auto_start_img
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenterOffset: auto_start_mouse.pressed
                anchors.bottomMargin: auto_start_mouse.pressed ? 18 : 20
                enabled: auto_start_operable || auto_start_mouse.pressed
                opacity: auto_start_opacity ? 1 : 0.4
                source: (autoViewModel.autoRunning && !autoViewModel.isRun) ? resourceManager.imagePath + "/07_AUTO/d070_29.png" : resourceManager.imagePath + "/07_AUTO/d070_28.png"
                MouseArea{
                    id:auto_start_mouse
                    anchors.fill: parent
                    onPressed: commonViewModel.modbusMemblemSwControl(true, 242)
                    onReleased: commonViewModel.modbusMemblemSwControl(false, 242)
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
        }//启动
        Rectangle{
            id:auto_pause
            width: 130
            height: 150
            color: "transparent"
            Text {
                id: auto_pause_te
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                font.pixelSize: 22
                color: resourceManager.rgb(32,9)
                text: resourceManager.label(32,9,local.language)
            }
            Image {
                id: auto_pause_img
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenterOffset: auto_pause_mouse.pressed
                anchors.bottomMargin: auto_pause_mouse.pressed ? 18 : 20
                enabled: auto_pause_operable || auto_pause_mouse.pressed
                opacity: auto_pause_opacity ? 1 : 0.4
                source: autoViewModel.isRun && (autoViewModel.oneCycle || autoViewModel.cycle) ? resourceManager.imagePath + "/07_AUTO/d070_31.png" : resourceManager.imagePath + "/07_AUTO/d070_30.png"
                MouseArea{
                    id:auto_pause_mouse
                    anchors.fill: parent
                    onPressed: commonViewModel.modbusMemblemSwControl(true, 248);
                    onReleased: commonViewModel.modbusMemblemSwControl(false, 248);
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
        }//暂停
        Rectangle{
            id:auto_stop
            width: 130
            height: 150
            color: "transparent"
            Text {
                id: auto_stop_te
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                font.pixelSize: 22
                color: resourceManager.rgb(32,10)
                text: resourceManager.label(32,10,local.language)
            }
            Image {
                id: auto_stop_img
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenterOffset: auto_stop_mouse.pressed
                anchors.bottomMargin: auto_stop_mouse.pressed ? 18 : 20
                enabled: auto_stop_operable || auto_stop_mouse.pressed
                opacity: auto_stop_operable ? 1 : 0.4
                source: auto_stop_mouse.pressed ? resourceManager.imagePath + "/07_AUTO/d070_33.png" : resourceManager.imagePath + "/07_AUTO/d070_32.png"
                MouseArea{
                    id:auto_stop_mouse
                    anchors.fill: parent
                    onPressed: commonViewModel.modbusMemblemSwControl(true, 243);
                    onReleased: commonViewModel.modbusMemblemSwControl(false, 243);
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
        }//停止
        Rectangle{
            id: auto_step
            width: 150
            height: 220
            color: "transparent"
            border.color: "#898989"
            Text {
                id: auto_step_te
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                font.pixelSize: 22
                color: resourceManager.rgb(32,11)
                text: resourceManager.label(32,11,local.language)
            }
            Image {
                id: auto_step_img
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.horizontalCenterOffset: auto_step_img_mouse.pressed
                anchors.topMargin: auto_step_img_mouse.pressed ? 50 : 48
                enabled: step_operable
                opacity: step_operable ? 1 : 0.4
                source: resourceManager.imagePath + (autoViewModel.step ? "/07_AUTO/d070_35.png" : "/07_AUTO/d070_34.png")
                MouseArea{
                    id:auto_step_img_mouse
                    anchors.fill: parent
                    onPressed: commonViewModel.modbusMemblemSwControl(true, 244);
                    onReleased: commonViewModel.modbusMemblemSwControl(false, 244);
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }//step按键
            Column{
                id:auto_step_2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                spacing: 2
                Text {
                    id: auto_step_2_te
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 16
                    color: resourceManager.rgb(32,13)
                    text: resourceManager.label(32,13,local.language)
                }
                Image {
                    id: auto_step_2_img
                    enabled: step_operable1
                    opacity: step_operable1 ? 1 : 0.4
                    source: resourceManager.imagePath + (autoViewModel.isFront ? "/07_AUTO/d070_15.png" : "/07_AUTO/d070_13.png")
                    Rectangle{
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        color: "transparent"
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                commonViewModel.modbusMemblemSwControl(true, 245);
                            }
                            onReleased: {
                                commonViewModel.modbusMemblemSwControl(false, 245);
                            }
                            onClicked: phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }//单步进
        }//步进
    }//上方操作按钮

    Rectangle{
        id: auto_left
        width: 562
        height: 360
        x: 10
        y: 225
        color: "#A9B0F5"
        border.color: "#898989"
        StackLayout{
            width: parent.width
            height: parent.height
            currentIndex: auto_left_tab.currentIndex

            SubAxisPage{
                id: axisPage
                width: parent.width
                height: parent.height
                pageActive: auto_page.pageActive && (auto_left_tab.currentIndex == 0)
            }//轴信息
            Loader{
                id: productionPage
                width: parent.width
                height: parent.height
                source: "qrc:/qml/Auto/SubProductionPage.qml";
                property bool isActive: auto_page.pageActive && (auto_left_tab.currentIndex == 1)
                active: false
                asynchronous: !isActive
                onLoaded: {
                    item.pageActive = isActive;
                }
                onIsActiveChanged: {
                    if(status == Loader.Ready){
                        item.pageActive = isActive;
                    }
                }
            }//生产管理
            Loader{
                id: ioPage
                width: parent.width
                height: parent.height
                source: "qrc:/qml/Auto/SubIoPage.qml";
                property bool isActive: auto_page.pageActive && (auto_left_tab.currentIndex == 2)
                active: false
                asynchronous: !isActive
                onLoaded: {
                    item.pageActive = isActive;
                }
                onIsActiveChanged: {
                    if(status == Loader.Ready){
                        item.pageActive = isActive;
                    }
                }
            }//io信号
        }
    }//自动画面左侧

    TabBar{
        id:auto_left_tab
        width: 562
        height: 56
        anchors.left: auto_left.left
        y:192
        spacing: 10
        background: Rectangle{
            color: "transparent"
        }

        TabButton{
            id:auto_left_tab_1
            height: 56
            background: Rectangle{
                color: (auto_left_tab.currentIndex === 0 || auto_left_tab_1.pressed) ? "#A9B0F5" : "#C9CACA"
                border.color: (auto_left_tab.currentIndex === 0 || auto_left_tab_1.pressed) ? "#898989" : "#C9CACA"
                Rectangle{
                    width: (auto_left_tab.currentIndex  === 0) ? parent.width -2 : parent.width
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: (auto_left_tab.currentIndex === 0 || auto_left_tab_1.pressed) ? "#A9B0F5" : "#898989"
                }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: resourceManager.rgb(32,1)
                    text: resourceManager.label(32,1,local.language)
                    font.pixelSize: auto_left_tab_1.pressed ? 23 : 22
                }
            }
            onClicked: {
                auto_left_tab.currentIndex = 0
                select_tab = 0
                setPage(auto_left_tab.currentIndex, auto_right_tab.currentIndex);
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id:auto_left_tab_2
            height: 56
            background: Rectangle{
                color: (auto_left_tab.currentIndex  === 1 || auto_left_tab_2.pressed) ? "#A9B0F5" : "#C9CACA"
                border.color: (auto_left_tab.currentIndex === 1 || auto_left_tab_2.pressed) ? "#898989" : "#C9CACA"
                Rectangle{
                    width: (auto_left_tab.currentIndex  === 1) ? parent.width -2 : parent.width
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: (auto_left_tab.currentIndex  === 1 || auto_left_tab_2.pressed) ? "#A9B0F5" : "#898989"
                }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: resourceManager.rgb(32,2)
                    text: resourceManager.label(32,2,local.language)
                    font.pixelSize: auto_left_tab_2.pressed ? 23 : 22
                }
            }
            onClicked: {
                auto_left_tab.currentIndex = 1
                select_tab = 1
                setPage(auto_left_tab.currentIndex, auto_right_tab.currentIndex);
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id:auto_left_tab_3
            height: 56
            background: Rectangle{
                color: (auto_left_tab.currentIndex  === 2 || auto_left_tab_3.pressed) ? "#A9B0F5" : "#C9CACA"
                border.color: (auto_left_tab.currentIndex === 2 || auto_left_tab_3.pressed) ? "#898989" : "#C9CACA"
                Rectangle{
                    width: (auto_left_tab.currentIndex  === 2) ? parent.width -2 : parent.width
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: (auto_left_tab.currentIndex  === 2 || auto_left_tab_3.pressed) ? "#A9B0F5" : "#898989"
                }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: resourceManager.rgb(32,3)
                    text: resourceManager.label(32,3,local.language)
                    font.pixelSize: auto_left_tab_3.pressed ? 23 : 22
                }
            }
            onClicked: {
                auto_left_tab.currentIndex = 2
                select_tab = 2
                setPage(auto_left_tab.currentIndex, auto_right_tab.currentIndex);
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//自动左侧子画面切换按钮栏

    Rectangle{
        id: auto_right
        width: 418
        height: 280
        x:590
        y:305
        color: "#A9B0F5"
        border.color: "#898989"
        StackLayout{
            width: parent.width
            height: parent.height
            currentIndex: auto_right_tab.currentIndex

            SubTimerPage{
                id: timerPage
                width: parent.width
                height: parent.height
                pageActive: auto_page.pageActive && (auto_right_tab.currentIndex == 0)
            }//计时器
            Loader{
                id: counterPage
                width: parent.width
                height: parent.height
                source: "qrc:/qml/Auto/SubCounterPage.qml";
                property bool isActive: auto_page.pageActive && (auto_right_tab.currentIndex == 1)
                active: false
                asynchronous: !isActive
                onLoaded: {
                    item.pageActive = isActive;
                }
                onIsActiveChanged: {
                    if(status == Loader.Ready){
                        item.pageActive = isActive;
                    }
                }
            }//io信号
        }
    }//自动画面右侧

    TabBar{
        id:auto_right_tab
        width: 300
        height: 56
        anchors.left: auto_right.left
        y: 272
        spacing: 10
        background: Rectangle{
            color: "transparent"
        }

        TabButton{
            id:auto_right_tab_1
            height: 56
            background: Rectangle{
                color: (auto_right_tab.currentIndex  === 0 || auto_right_tab_1.pressed) ? "#A9B0F5" : "#C9CACA"
                border.color: (auto_right_tab.currentIndex === 0 || auto_right_tab_1.pressed) ? "#898989" : "#C9CACA"
                Rectangle{
                    width: (auto_right_tab.currentIndex === 0) ? parent.width -2 : parent.width
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: (auto_right_tab.currentIndex === 0 || auto_right_tab_1.pressed) ? "#A9B0F5" : "#898989"
                }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: resourceManager.rgb(32,4)
                    text: resourceManager.label(32,4,local.language)
                    font.pixelSize: auto_right_tab_1.pressed ? 23 : 22
                }
            }
            onClicked: {
                auto_right_tab.currentIndex = 0
                select_tab = 3
                setPage(auto_left_tab.currentIndex, auto_right_tab.currentIndex);
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id:auto_right_tab_2
            height: 56
            background: Rectangle{
                color: (auto_right_tab.currentIndex  === 1 || auto_right_tab_2.pressed) ? "#A9B0F5" : "#C9CACA"
                border.color: (auto_right_tab.currentIndex === 1 || auto_right_tab_2.pressed) ? "#898989" : "#C9CACA"
                Rectangle{
                    width: (auto_right_tab.currentIndex  === 1) ? parent.width -2 : parent.width
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: (auto_right_tab.currentIndex === 1 || auto_right_tab_2.pressed) ? "#A9B0F5" : "#898989"
                }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: resourceManager.rgb(32,5)
                    text: resourceManager.label(32,5,local.language)
                    font.pixelSize: auto_right_tab_2.pressed ? 23 : 22
                }
            }
            onClicked: {
                auto_right_tab.currentIndex = 1
                select_tab = 4
                setPage(auto_left_tab.currentIndex, auto_right_tab.currentIndex);
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//自动右侧子画面切换按钮栏

    function setPage(leftTabIdx, rightTabIdx){
        switch(leftTabIdx){
        case 0:
            switch(rightTabIdx){
            case 0:
                if(commonViewModel.getCurrentPage() !== Enums.AutoAxisTimer){
                    autoViewModel.axisView.startPageChanging();
                    autoViewModel.timerView.startPageChanging();
                    commonViewModel.pageChange(Enums.AutoAxisTimer);
                }
                break;
            case 1:
                if(commonViewModel.getCurrentPage() !== Enums.AutoAxisCounter){
                    autoViewModel.axisView.startPageChanging();
                    autoViewModel.counterView.startPageChanging();
                    commonViewModel.pageChange(Enums.AutoAxisCounter);
                }
                break;
            default:
                break;
            }
            break;
        case 1:
            switch(rightTabIdx){
            case 0:
                if(commonViewModel.getCurrentPage() !== Enums.AutoProductMngTimer){
                    autoViewModel.prodView.startPageChanging();
                    autoViewModel.timerView.startPageChanging();
                    commonViewModel.pageChange(Enums.AutoProductMngTimer);
                }
                break;
            case 1:
                if(commonViewModel.getCurrentPage() !== Enums.AutoProductMngCounter){
                    autoViewModel.prodView.startPageChanging();
                    autoViewModel.counterView.startPageChanging();
                    commonViewModel.pageChange(Enums.AutoProductMngCounter);
                }
                break;
            default:
                break;
            }
            break;
        case 2:
            switch(rightTabIdx){
            case 0:
                if(commonViewModel.getCurrentPage() !== Enums.AutoIOTimer){
                    autoViewModel.ioView.startPageChanging();
                    autoViewModel.timerView.startPageChanging();
                    commonViewModel.pageChange(Enums.AutoIOTimer);
                }
                break;
            case 1:
                if(commonViewModel.getCurrentPage() !== Enums.AutoIOCounter){
                    autoViewModel.ioView.startPageChanging();
                    autoViewModel.counterView.startPageChanging();
                    commonViewModel.pageChange(Enums.AutoIOCounter);
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}
