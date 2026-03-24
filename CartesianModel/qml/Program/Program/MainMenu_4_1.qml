import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4_1.qml
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
    id: root
    width: parent.width
    height: parent.height
    color: "transparent"
    border.width: 2
    border.color: "#999999"
    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false

    function clearSelect() {
        local.menuModel.clearItems()
    }

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4    // ProgramMenu4
        readonly property var menuModel: menu4Model.subMenuList[0]      // ProgramMenu4_1

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
            local.menuModel.deactivate()
            local.initialize()
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

    Rectangle{
        x: 16
        width: 391
        height: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        border.color: "#999999"
        color: "#eaeaea"
        Row{
            x:10
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10
            Text{
                width: 120
                height: 40
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                wrapMode: Text.WordWrap
                font.pixelSize: 18
                font.bold: true
                color: resourceManager.rgb(41, 535)
                text: resourceManager.label(41, 535, language.value)
            }
            Rectangle{
                width: 120
                height: 40
                anchors.verticalCenter: parent.verticalCenter
                border.color: "#999999"
                color: "#ffffff"
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 18
                    font.bold: true
                    text: local.menuModel.overTime
                }
            }
        }
    }

Rectangle {
    width: 50
    height: 340
    color: "#eaeaea"
    border.color: "#999999"
    anchors.right: parent.right
    anchors.rightMargin: 23
    y: 50
    ImageScrollBar {
        anchors.fill: parent
        anchors.margins: 4
        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
        pageLabal: (local.currentPage + 1) + "/" + local.numOfPages

        onScrollUpClicked: { local.prevPage() }
        onScrollDownClicked: { local.nextPage() }
    }
}


    Item {
        width: 360
        height: 391
        x: 12
        y: 2
        clip: true
        Rectangle{
            width: 100
            height: 40
            color: "#cc3333"
            border.color: "#cc3333"
            visible: (local.currentPage == 0)
            x:4
            y:6
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
            x:4
            y: (local.currentPage == 0) ? 48 : 34
            spacing: 5
            Repeater{
                model: local.numOfItemsParPage
                Rectangle{
                    id:base
                    readonly property int itemIndex: local.currentPage * local.numOfItemsParPage + index
                    readonly property var item: (itemIndex < local.numOfItems) ? local.itemList[itemIndex] : local.itemList[0]
                    width: 340
                    height: itemIndex == 0 ? 75 : 115
                    border.color: "#999999"
                    color: "#eaeaea"
                    CheckBox{
                        width: 30
                        height: 30
                        x:6
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
                        height: 40
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
                            font.pixelSize: mouse_xx.pressed ? 19 : 20
                            font.bold: true
                            visible: base.item.selectState
                            color: base.item.selectState ? "white" : "black"
                            text: base.item.relation === "and" ? "and" : "or"
                        }
                        MouseArea{
                            id:mouse_xx
                            anchors.fill: parent
                            onClicked: {
                                base.item.relation = (base.item.relation === "and") ? "or" : "and"
                            }
                        }
                    }// relation("and", "or").

                    Rectangle{
                        width: 258
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
                                let str4 = (base.item.opeStr === "") ? (base.item.value === 0) ? "  OFF" : "  ON"   // value is logical(ON/OFF).
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
