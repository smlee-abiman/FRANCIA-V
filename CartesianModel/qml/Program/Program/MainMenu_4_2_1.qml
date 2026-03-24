import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4_2_1.qml
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
//调用
Rectangle{
    id:transfer
    width: parent.width
    height: parent.height
    color: "transparent"

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false
    function setMenuIndex(index) {
        local.menuModel.menuIndex = index
    }
    function clearSelect() {
        local.menuModel.clearItems()
    }

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4    // ProgramMenu4
        readonly property var menu42Model: menu4Model.subMenuList[1]    // ProgramMenu4_2
        readonly property var menuModel: menu42Model.subMenuList[0]     // ProgramMenu4_2_1
        readonly property int menuIndex: menuModel.menuIndex
        readonly property bool condition: (menuIndex === 1)
        readonly property int programNo: menuModel.programNo
        readonly property bool dialogOpend: menuModel.openProgramSelectionDialog

        readonly property var itemList: menuModel.itemList
        readonly property int numOfItems: itemList.length
        readonly property int numOfItemsParPage: 3
        readonly property int numOfPages: Math.ceil(numOfItems / numOfItemsParPage)
        property int currentPage: 0

        function initialize() { currentPage = 0 }
        function prevPage() { currentPage = Math.max(currentPage - 1, 0); }
        function nextPage() { currentPage = Math.min(currentPage + 1, numOfPages - 1); }

        onItemListChanged: {
            currentPage = 0
        }
    }

    onPageActiveChanged: {
        if (pageActive) {
            local.menuModel.activate()
        } else {
            setMenuIndex(0)
            local.menuModel.deactivate()
            local.initialize()
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

    Rectangle{
        width: parent.width- 20
        height: 50
        anchors.top: parent.top
        anchors.topMargin: -2
        anchors.right: parent.right
        anchors.rightMargin: 10
        border.color: "#999999"
        color: "#cccccc"
        Rectangle{
            width: 1
            height: 46
            x:130
            anchors.verticalCenter: parent.verticalCenter
            color: "#999999"
        }//分隔线
        Image {
            x:8
            anchors.verticalCenter: parent.verticalCenter
            source: local.menuModel.menuIndex === 0 ? resourceManager.imagePath + "/16_program/a160_74.png" : resourceManager.imagePath + "/16_program/a160_75.png"
            Text{
                height: parent.height
                x: local.menuModel.menuIndex === 0 ? 60 : 8
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 15
                font.bold: true
                text: local.menuModel.menuIndex === 0 ? resourceManager.label(41,541, language.value) : resourceManager.label(41,542, language.value)
                color:  local.menuModel.menuIndex === 0 ? "black" : "white"
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(local.menuModel.menuIndex === 0){
                        setMenuIndex(1)
                    }else{
                        setMenuIndex(0)
                    }
                }
            }
        }// 有条件 or 无条件
        Row{
            x:136
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10
            Text{
                width: 70
                height: 50
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 18
                wrapMode: Text.WrapAnywhere
                color:resourceManager.rgb(41, 540)
                text: resourceManager.label(41,540, language.value)
            }
            Rectangle{
                width: 170
                height: 40
                anchors.verticalCenter: parent.verticalCenter
                border.color: "#999999"
                color: local.dialogOpend ? "#999999" : "white"
                Text{
                    readonly property int resourceIndex: 37 + (local.programNo - 1) // program no ... 1base.
                    visible: (1 <= local.programNo)
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 6
                    font.pixelSize: 18
                    wrapMode:Text.WordWrap
                    color: "black"
                    text: resourceManager.label(41, resourceIndex, language.value)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        local.menuModel.openProgramSelectionDialog = true
                    }
                }
            }
        }
    }
    Rectangle {
        width: 50
        height: 342
        color: "#eaeaea"
        border.color: "#999999"
        anchors.right: parent.right
        anchors.rightMargin: 10
        y: 51
        visible: local.condition
    ImageScrollBar{
        anchors.fill: parent
        anchors.margins: 4
        visible: local.condition
        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
        pageLabal: (local.currentPage + 1) + "/" + local.numOfPages
        onScrollUpClicked: { local.prevPage() }
        onScrollDownClicked:{ local.nextPage() }
    }
    }
    Item {
        width:371
        height: 354
        y:47
        clip: true
        visible: local.condition
        Rectangle{
            width: 100
            height: 36
            x:10
            y:3
            color: "#cc3333"
            border.color: "#cc3333"
            visible: local.currentPage == 0
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 18
                font.bold: true
                color: "white"
                text: resourceManager.label(41, 663, language.value)
            }
        }
        Column{
            x: 10
            y: (local.currentPage == 0) ? 42 : 4
            spacing: 2
            Repeater{
                model: local.numOfItemsParPage
                Rectangle{
                    id:base
                    readonly property int itemIndex: local.currentPage * local.numOfItemsParPage + index
                    readonly property var item: (itemIndex < local.numOfItems) ? local.itemList[itemIndex] : local.itemList[0]
                    width: 358
                    height: itemIndex == 0 ? 77 : 115
                    border.color: "#999999"
                    color: "#eaeaea"
                    CheckBox{
                        width: 34
                        height: 34
                        x:4
                        anchors.verticalCenter: parent.verticalCenter
                        indicator: Rectangle{
                            width: parent.width
                            height: parent.height
                            border.width: 2
                            border.color: "#999999"
                            color: base.item.optional ? "white" : "#cccccc"
                            Image {
                                anchors.centerIn: parent
                                visible: base.item.selectState
                                source: base.item.optional ? resourceManager.imagePath + "/16_program/a160_80.png" : resourceManager.imagePath + "/16_program/a160_80.png"
                            }
                        }
                        onClicked: {
                            if(base.item.optional){
                                if(base.item.selectState){
                                    local.menuModel.deselectItem(base.itemIndex)
                                }else{
                                    local.menuModel.selectItem(base.itemIndex)
                                }
                            }
                        }
                    }

                    Rectangle{
                        width:90
                        height: 44
                        x:46
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -32
                        color: base.item.selectState ? "#cc3333" : "#cccccc"
                        border.color: "#999999"
                        visible: base.itemIndex != 0
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: mouse_xxx.pressed ? 19 : 20
                            font.bold: true
                            visible: base.item.selectState
                            text: base.item.relation === "and" ? "and" : "or"
                            color: "white"
                        }
                        MouseArea{
                            id:mouse_xxx
                            anchors.fill: parent
                            onClicked: {
                                base.item.relation = (base.item.relation === "and") ? "or" : "and"
                            }
                        }
                    }// relation("and", "or").

                    Rectangle{
                        width: 305
                        height: 58
                        x:46
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: base.itemIndex == 0 ? 0 : 25
                        border.color: base.item.selectState ? "#999999" : "#999999"
                        color: local.menuModel.editIndex === base.itemIndex ? "#999999" : base.item.selectState ? "white" : "#cccccc"
                        Text{
                            anchors.fill: parent
                            wrapMode: Text.WrapAnywhere
                            maximumLineCount: 2
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            leftPadding: 5

                            fontSizeMode: Text.Fit
                            minimumPixelSize: 18
                            font.pixelSize: 20
                            text: {
                                if ((base.item.symbol === "") && (base.item.ioAddress === -1)) {
                                    return ""
                                }
                                let str1 = base.item.symbol
                                let str2 = ""
                                let str3 = base.item.opeStr
                                let str4 = (base.item.opeStr === "") ? (base.item.value === 0) ? "OFF" : "ON"   // value is logical(ON/OFF).
                                                                     : base.item.value  // value is a number.
                                if (base.item.ioAddress !== -1) {
                                    if (str1 === "") {
                                        str1 = resourceManager.symbolAt(base.item.ioAddress)
                                    }
                                    str2 = resourceManager.getMessageStringValue("1", base.item.ioAddress, language.value)
                                }
                                return `${str1}(${str2})${str3}${str4}`
                            }
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(base.item.selectState){
                                    // memo:
                                    // When the editIndex is changed,
                                    // the variable selection dialog will open. (in Program.qml)
                                    local.menuModel.editIndex = base.itemIndex
                                }
                            }
                        }
                    }// variable.
                }
            }
        }
    }
}
