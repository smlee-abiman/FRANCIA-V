import QtQuick 2.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../Widget"
import "../../Components"
import "../../Program/PosSet/PosSetUtils.js" as Utils
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_1_2.qml
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
Rectangle{
    id:menu1_2

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property int page: 0
    property int menuIndex: menuModel.menuIndex
    property bool pageActive: false
    property var menuModel: programViewModel.progMenu1.subMenuList[1]
    property var timerViewModel: teachingViewModel.pageModel[3]
    property var itemList: menuModel.itemList
    property int numItems: menuModel.numItems
    readonly property int numItemsPerPage: 8
    property int maxPageNo: (numItems - 1) / numItemsPerPage

    function prevPage() {
        if (page <= 0) {
            page = 0
        } else {
            page--
        }
    }

    function nextPage() {
        if (page < maxPageNo) {
            page++
        }
    }

    color: "transparent"
    border.width: 2
    border.color: "#999999"

    onPageActiveChanged: {
        if (pageActive) {
            menuModel.activate()
            m_stack.load_page(itemList[menuIndex].programNo)
            timer.running = true
        } else {
            menuModel.menuIndex = 0
            page = 0
            menuModel.deactivate()
            timer.running = false
        }
    }

    onMenuIndexChanged: {
        menuModel.resetUse()
        m_stack.load_page(itemList[menuIndex].programNo)
    }

    Component.onDestruction: menuModel.deactivate()

    Timer{
        id:timer
        running: false
        interval: 100
        repeat: true
        onTriggered: {
            menuModel.modbusGetSignal()
        }
    }

    Language { id: language }

    Rectangle{
        width: 120
        height: parent.height
        color: "#eaeaea"
        border.color: "#999999"
        Component{
            id:listdelegate_2
            Rectangle{
                readonly property int programNo: modelData.programNo
                readonly property int titleIndex: 36 + programNo
                width: 100
                height: 40
                x: 10
                radius: 5
                border.color: "#999999"
                color: menu1_2.menuIndex == index ? "#cc3333" : "#cccccc"
                visible: (programNo !== 0)
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    wrapMode: Text.WordWrap
                    text: resourceManager.label(41,titleIndex, language.value)
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        menuModel.menuIndex = index
                        m_stack.load_page(programNo)
                    }
                }
            }
        }
        ListView{
            id:list1_2
            width: 120
            height: 378
            y:48
            property int pg: menu1_2.page
            spacing: 4
            clip: true
            model: itemList
            delegate: listdelegate_2
            interactive: false
            onPgChanged: {
                positionViewAtIndex(pg * 8, ListView.Beginning)
            }
            Component.onCompleted: {
                positionViewAtIndex(pg * 8, ListView.Beginning)
            }
        }
        Rectangle{
            width: 100
            height: 40
            border.color: "#999999"
            color: "transparent"
            x: 10
            y: 5
            Image{
                id:up1
                anchors.centerIn: parent
                source: resourceManager.imagePath + "/16_program/d161_87.png"
            }
            MouseArea{
                anchors.fill: parent
                onPressed: up1.source = resourceManager.imagePath + "/16_program/d161_89.png"
                onReleased: {
                    prevPage()
                    up1.source = resourceManager.imagePath + "/16_program/d161_87.png"
                }
            }
        }
        Rectangle{
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            width: 100
            height: 40
            x: 10
            border.color: "#B5B5B6"
            color: "transparent"
            Image{
                id:down1
                anchors.centerIn: parent
                source: resourceManager.imagePath + "/16_program/d161_88.png"
            }
            MouseArea{
                anchors.fill: parent
                onPressed: down1.source = resourceManager.imagePath + "/16_program/d161_90.png"
                onReleased: {
                    nextPage()
                    down1.source = resourceManager.imagePath + "/16_program/d161_88.png"
                }
            }
        }
    }//菜单栏

    StackView{
        id:m_stack
        width: 310
        height: 453
        x:118
        clip: true

        function getSignalStatus(value, bit){
            return (value >> bit) & 1
        }
        function getMaxTime(timerIndex){
            let ret
            let digits = timerViewModel.getIntegerDigits(timerIndex)
            switch (digits) {
            case 2:
                ret = "99.99"
                break
            case 3:
                ret = "655.35"
                break
            default:
                console.warn("getMaxTime: Invalid value.", index, digits)
                ret = "655.35"
                break
            }
            return ret
        }
        function getMaxCounter(){
            return "9999999"
        }
        function pushKey(itemIndex, keyIndex){
            let keyNo = menuModel.getKeyNo(itemIndex, keyIndex)
            if (keyNo > 0) {
                commonViewModel.modbusMemblemSwControl(true, keyNo)
            }
        }
        function releaseKey(itemIndex, keyIndex){
            let keyNo = menuModel.getKeyNo(itemIndex, keyIndex)
            if (keyNo > 0) {
                commonViewModel.modbusMemblemSwControl(false, keyNo)
                menuModel.modbusGetSignal()
            }
        }

        Component{
            id:prog123
            Rectangle{
                id:prog123_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                property bool wait: menuItem.wait
                width: 260
                height: 40
                x:20
                y:10
                color: "transparent"
                border.color: "#cccccc"
                Rectangle{
                    width: parent.width
                    height: 40
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x: 20
                        y: 2
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog123_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog123_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,304)
                                text: resourceManager.label(41,304, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,305)
                                text: resourceManager.label(41,305, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 55
                    y:153
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 36
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog123_1.wait ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            text: resourceManager.label(41,303, language.value)
                            color: resourceManager.rgb(41,303)
                            font.pixelSize: 18
                            topPadding: -2
                            leftPadding: 36
                        }
                        onClicked: menuItem.wait = !prog123_1.wait
                    }
                }
                Column{
                    x:10
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 0
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,307)
                                    text: resourceManager.label(41,307, language.value)

                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 0
                                readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 1
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,308)
                                    text: resourceManager.label(41,308, language.value)
                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 1
                                readonly property bool signalStatus: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: signalStatus ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                    }
                }//操作
            }
        }//Prog#123回正

        Component{
            id:prog124
            Rectangle{
                id:prog124_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                property bool wait: menuItem.wait
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog124_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog124_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,309)
                                text: resourceManager.label(41,309, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,310)
                                text: resourceManager.label(41,310, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 55
                    y:153
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 36
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog124_1.wait ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            text: resourceManager.label(41,303, language.value)
                            color: resourceManager.rgb(41,303)
                            font.pixelSize: 18
                            topPadding: -2
                            leftPadding: 36
                        }
                        onClicked: menuItem.wait = !prog124_1.wait
                    }
                }
                Column{
                    x:10
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 0
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,307)
                                    text: resourceManager.label(41,307, language.value)

                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 0
                                readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 1
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,308)
                                    text: resourceManager.label(41,308, language.value)
                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 1
                                readonly property bool signalStatus: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: signalStatus ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                    }
                }//操作
            }
        }//Prog#124侧姿

        Component{
            id:prog151
            Rectangle{
                id:prog151_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog151_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog151_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,311)
                                text: resourceManager.label(41,311, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        time
                                    } else {
                                        timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,312)
                                text: resourceManager.label(41,312, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        ImageButton{
                            readonly property int keyIndex: 0
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,313)
                                text: resourceManager.label(41,313, language.value)

                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                        Image {
                            readonly property int signalBit: 0
                            readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                            anchors.verticalCenter: parent.verticalCenter
                            source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                        }
                        ImageButton{
                            readonly property int keyIndex: 1
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,314)
                                text: resourceManager.label(41,314, language.value)
                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                    }
                }//操作
            }
        }//Prog#151副臂前进气缸

        Component{
            id:prog150
            Rectangle{
                id:prog150_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog150_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog150_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,315)
                                text: resourceManager.label(41,315, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,316)
                                text: resourceManager.label(41,316, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        ImageButton{
                            readonly property int keyIndex: 0
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,313)
                                text: resourceManager.label(41,313, language.value)

                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                        Image {
                            readonly property int signalBit: 0
                            readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                            anchors.verticalCenter: parent.verticalCenter
                            source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                        }
                        ImageButton{
                            readonly property int keyIndex: 1
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,314)
                                text: resourceManager.label(41,314, language.value)
                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                    }
                }//操作
            }
        }//Prog#150副臂后退气缸

        Component{
            id:prog152
            Rectangle{
                id:prog152_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog152_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog152_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,317)
                                text: resourceManager.label(41,317, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,318)
                                text: resourceManager.label(41,318, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 80
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 80
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        ImageButton{
                            readonly property int keyIndex: 0
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,319)
                                text: resourceManager.label(41,319, language.value)

                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                        Image {
                            readonly property int signalBit: 0
                            readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                            anchors.verticalCenter: parent.verticalCenter
                            source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                        }
                        ImageButton{
                            readonly property int keyIndex: 1
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,320)
                                text: resourceManager.label(41,320, language.value)
                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                    }
                }//操作
            }
        }//Prog#152副臂上行气缸

        Component{
            id:prog153
            Rectangle{
                id:prog153_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog153_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog153_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,317)
                                text: resourceManager.label(41,317, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,318)
                                text: resourceManager.label(41,318, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 80
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 80
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        ImageButton{
                            readonly property int keyIndex: 0
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,319)
                                text: resourceManager.label(41,319, language.value)

                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                        Image {
                            readonly property int signalBit: 0
                            readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                            anchors.verticalCenter: parent.verticalCenter
                            source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                        }
                        ImageButton{
                            readonly property int keyIndex: 1
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,320)
                                text: resourceManager.label(41,320, language.value)
                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                    }
                }//操作
            }
        }//Prog#153副臂下行气缸

        Component{
            id:prog121
            Rectangle{
                id:prog121_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                property bool wait: menuItem.wait
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog121_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog121_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,325)
                                text: resourceManager.label(41,325, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,326)
                                text: resourceManager.label(41,326, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 55
                    y:153
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 36
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog121_1.wait ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            text: resourceManager.label(41,303, language.value)
                            color: resourceManager.rgb(41,303)
                            font.pixelSize: 18
                            topPadding: -2
                            leftPadding: 36
                        }
                        onClicked: menuItem.wait = !prog121_1.wait
                    }
                }
                Column{
                    x:10
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 70
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,323)
                            text: resourceManager.label(41,323, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 70
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 0
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,323)
                                    text: resourceManager.label(41,323, language.value)

                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 0
                                readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 1
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,324)
                                    text: resourceManager.label(41,324, language.value)
                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 1
                                readonly property bool signalStatus: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: signalStatus ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                    }
                }//操作
            }
        }//Prog#121回转

        Component{
            id:prog122
            Rectangle{
                id:prog122_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                property bool wait: menuItem.wait
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog122_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog122_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,321)
                                text: resourceManager.label(41,321, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,322)
                                text: resourceManager.label(41,322, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 55
                    y:153
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 36
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog122_1.wait ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            text: resourceManager.label(41,303, language.value)
                            color: resourceManager.rgb(41,303)
                            font.pixelSize: 18
                            topPadding: -2
                            leftPadding: 36
                        }
                        onClicked: menuItem.wait = !prog122_1.wait
                    }
                }
                Column{
                    x:10
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 0
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,323)
                                    text: resourceManager.label(41,323, language.value)

                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 0
                                readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                        Row{
                            ImageButton{
                                readonly property int keyIndex: 1
                                upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                                downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                                Text{
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap

                                    font.pixelSize: 18
                                    color: resourceManager.rgb(41,324)
                                    text: resourceManager.label(41,324, language.value)
                                }
                                onPressed: pushKey(itemIndex, keyIndex)
                                onReleased: releaseKey(itemIndex, keyIndex)
                            }
                            Image {
                                readonly property int signalBit: 1
                                readonly property bool signalStatus: getSignalStatus(menuItem.signal, signalBit)
                                anchors.verticalCenter: parent.verticalCenter
                                source: signalStatus ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                            }
                        }
                    }
                }//操作
            }
        }//Prog#122旋转

        Component{
            id:prog149
            Rectangle{
                id:prog149_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog149_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog149_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,328)
                                text: resourceManager.label(41,328, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,329)
                                text: resourceManager.label(41,329, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 85
                    y:153
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Column{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 5
                        Text{
                            width: 120
                            height: 40
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter

                            wrapMode: Text.WordWrap
                            font.pixelSize: 18
                            color: resourceManager.rgb(41,330)
                            text: resourceManager.label(41,330, language.value)
                        }
                        LimitNumericEditBox {
                            readonly property int timeIndex: 2
                            property int time: menuItem.timeList[timeIndex]
                            property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                            width: 100
                            height: 40
                            anchors.horizontalCenter: parent.horizontalCenter
                            rectRadius: 6
                            borderColor: "#898989"
                            borderWidth: 1
                            fontPixelSize: 22
                            keyLayoutType: isCounterHrAddress ? 0 : 4
                            max: {
                                if (isCounterHrAddress) {
                                    return m_stack.getMaxCounter()
                                } else {
                                    return m_stack.getMaxTime(timeIndex)
                                }
                            }
                            min: "0"
                            value: {
                                if (isCounterHrAddress) {
                                    return time
                                } else {
                                    return timerViewModel.formatSpeed(timeIndex, time)
                                }
                            }
                            onEnter: {
                                if (isCounterHrAddress) {
                                    menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                } else {
                                    menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                }
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                    }
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 10
                        ImageButton{
                            readonly property int keyIndex: 0
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,331)
                                text: resourceManager.label(41,331, language.value)
                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                        Image {
                            readonly property int signalBit: 0
                            readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                            anchors.verticalCenter: parent.verticalCenter
                            source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                        }
                    }
                }//操作
            }
        }//Prog#149喷雾

        Component{
            id:prog147
            Rectangle{
                id:prog147_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Rectangle{
                    width: parent.width
                    height: 55
                    border.color: "#C1C2F4"
                    color: "transparent"
                    CheckBox{
                        width: 80
                        height: 36
                        x:20
                        anchors.verticalCenter: parent.verticalCenter
                        indicator:Image{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            scale: 1.3
                            source: prog147_1.use ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                        }
                        contentItem: Text{
                            color: resourceManager.rgb(41,544)
                            text: resourceManager.label(41,544, language.value)
                            font.pixelSize: 18
                            leftPadding: 30
                            topPadding: -2
                        }
                        onClicked: menuItem.use = !prog147_1.use
                    }
                }
                Rectangle{
                    width: parent.width
                    height: 100
                    y:54
                    border.color: "#C1C2F4"
                    color: "transparent"
                    Row{
                        x:10
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 10
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,332)
                                text: resourceManager.label(41,332, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 0
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                        Column{
                            spacing: 4
                            Text{
                                width: 120
                                height: 40
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                wrapMode: Text.WordWrap
                                font.pixelSize: 18
                                color: resourceManager.rgb(41,333)
                                text: resourceManager.label(41,333, language.value)
                            }
                            LimitNumericEditBox {
                                readonly property int timeIndex: 1
                                property int time: menuItem.timeList[timeIndex]
                                property bool isCounterHrAddress: menuModel.isCounterHrAddress(itemIndex, timeIndex)
                                width: 100
                                height: 40
                                anchors.horizontalCenter: parent.horizontalCenter
                                rectRadius: 6
                                borderColor: "#898989"
                                borderWidth: 1
                                fontPixelSize: 22
                                keyLayoutType: isCounterHrAddress ? 0 : 4
                                max: {
                                    if (isCounterHrAddress) {
                                        return m_stack.getMaxCounter()
                                    } else {
                                        return m_stack.getMaxTime(timeIndex)
                                    }
                                }
                                min: "0"
                                value: {
                                    if (isCounterHrAddress) {
                                        return time
                                    } else {
                                        return timerViewModel.formatSpeed(timeIndex, time)
                                    }
                                }
                                onEnter: {
                                    if (isCounterHrAddress) {
                                        menuModel.setCounter(itemIndex, timeIndex, parseInt(newValue))
                                    } else {
                                        menuModel.setTime(itemIndex, timeIndex, parseFloat(newValue))
                                    }
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    y:355
                    spacing: 10
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 15
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                        Text{
                            font.pixelSize: 20
                            color: resourceManager.rgb(41,306)
                            text: resourceManager.label(41,306, language.value)
                        }
                        Rectangle{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 1
                            color: "#8FAADC"
                        }
                    }
                    Row{
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 10
                        ImageButton{
                            readonly property int keyIndex: 0
                            upImageSource: resourceManager.imagePath + "/16_program/d161_91.png"
                            downImageSource: resourceManager.imagePath + "/16_program/d161_92.png"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WordWrap

                                font.pixelSize: 18
                                color: resourceManager.rgb(41,334)
                                text: resourceManager.label(41,334, language.value)
                            }
                            onPressed: pushKey(itemIndex, keyIndex)
                            onReleased: releaseKey(itemIndex, keyIndex)
                        }
                        Image {
                            readonly property int signalBit: 0
                            readonly property bool status: getSignalStatus(menuItem.signal, signalBit)
                            anchors.verticalCenter: parent.verticalCenter
                            source: status ? resourceManager.imagePath + "/16_program/d161_85.png" : resourceManager.imagePath + "/16_program/d161_86.png"
                        }
                    }
                }//操作
            }
        }//Prog#147输送带

        Component{
            id:prog111
            Rectangle{
                id:prog111_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                CheckBox{
                    width: 100
                    height: 25
                    x:10
                    y:10
                    indicator: Rectangle{
                        width: 25
                        height: 25
                        color: parent.checked ? "yellow" : "white"
                        border.color: "#898989"
                    }
                    contentItem: Text{
                        leftPadding:30
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 18
                        text: resourceManager.label(41, 408, language.value)
                        color: resourceManager.rgb(41, 408)
                    }
                    onClicked: menuItem.use = !prog111_1.use
                }
            }
        }//Prog#111 伺服ON

        Component{
            id:prog112
            Rectangle{
                id:prog112_1
                readonly property int itemIndex: menuModel.menuIndex
                property var menuItem: itemList[itemIndex]
                property bool use: menuItem.use
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                CheckBox{
                    width: 100
                    height: 25
                    x:10
                    y:10
                    indicator: Rectangle{
                        width: 25
                        height: 25
                        color: parent.checked ? "yellow" : "white"
                        border.color: "#898989"
                    }
                    contentItem: Text{
                        leftPadding:30
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 18
                        text: resourceManager.label(41, 409, language.value)
                        color: resourceManager.rgb(41, 409)
                    }
                    onClicked: menuItem.use = !prog112_1.use
                }
            }
        }//Prog#111 伺服OFF

        Component{
            id:prognull
            Rectangle{
                width: m_stack.width
                height: m_stack.height
                color: "transparent"
                border.color: "#B5B5B6"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 40
                    text: "NULL"
                }
            }
        }//空画面

        function load_page(page){
            let component
            switch(page){
            case 121: component = prog121; break;
            case 122: component = prog122; break;
            case 123: component = prog123; break;
            case 124: component = prog124; break;
            case 147: component = prog147; break;
            case 148: component = prog148; break;
            case 149: component = prog149; break;
            case 150: component = prog150; break;
            case 151: component = prog151; break;
            case 152: component = prog152; break;
            case 153: component = prog153; break;
            case 111: component = prog111; break;
            case 112: component = prog112; break;
            default: component = prognull; break;
            }
            if (depth >= 1) {
                m_stack.replace(component,StackView.Immediate);
            } else {
                m_stack.push(component,StackView.Immediate);
            }
        }
    }
}

