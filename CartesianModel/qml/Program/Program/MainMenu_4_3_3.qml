import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4_3_3.qml
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
//寄存器+1
Item{
    id: root
    width: parent.width
    height: parent.height

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4    // ProgramMenu4
        readonly property var menu43Model: menu4Model.subMenuList[2]    // ProgramMenu4_3
        readonly property var menuModel: menu43Model.subMenuList[2]     // ProgramMenu4_3_3
        readonly property var countType3Addr: menuModel.countType3Addr
        readonly property int selectedCountType3Addr: menuModel.selectedCountType3Addr

        readonly property int numOfItems: countType3Addr.length
        readonly property int numOfItemsParPage: 8
        readonly property int numOfPages: Math.ceil(numOfItems / numOfItemsParPage)
        property int currentPage: 0

        onSelectedCountType3AddrChanged: {
            let index = countType3Addr.indexOf(selectedCountType3Addr)
            gotoPage(index)
        }

        function initialize() {
            currentPage = 0
            menuModel.selectedCountType3Addr = -1
        }
        function prevPage() {
            currentPage = Math.max(currentPage - 1, 0);
        }
        function nextPage() {
            currentPage = Math.min(currentPage + 1, numOfPages - 1);
        }
        function gotoPage(index) {
            if (index < 0) { return }
            let page = Math.floor(index / numOfItemsParPage);
            currentPage = Math.min(page, numOfPages - 1);
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

    Rectangle {
        width: 50
        height: 390
        color: "#eaeaea"
        border.color: "#999999"
        anchors.right: parent.right
        anchors.rightMargin: 35
        y: 3
    ImageScrollBar{
        anchors.fill: parent
        anchors.margins: 4
        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
        pageLabal: (local.currentPage + 1) + "/" + local.numOfPages
        onScrollUpClicked: { local.prevPage() }
        onScrollDownClicked:{ local.nextPage() }
    }
    }
    //count
    Component{
        id:listdelegate_3_3
        Rectangle{
            x:1
            width: 310
            height: 47
            readonly property int ioAddress: local.countType3Addr[index]
            readonly property bool isSelected: (ioAddress === local.menuModel.selectedCountType3Addr)
            color: isSelected ? "#cccccc" : "#cccccc"
            border.color: "#999999"
            CheckBox{
                width: parent.width
                height: 36
                x:8
                anchors.verticalCenter: parent.verticalCenter
                indicator:Image{
                    scale: 1.1
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    source: isSelected ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                }
                contentItem: Text{
                    text: resourceManager.symbolAt(ioAddress) + " " + resourceManager.getMessageStringValue("1", ioAddress, language.value)
                    font.pixelSize: 17
                    leftPadding: 25
                    wrapMode: Text.WordWrap

                    verticalAlignment: Text.AlignVCenter
                }
                onClicked:{
                    local.menuModel.selectedCountType3Addr = isSelected ? -1 : ioAddress
                }
            }
        }
    }
    ListView{
        width: 311
        height: 390
        x:12
        y:3
        readonly property int pg: local.currentPage
        clip: true
        spacing: 2
        interactive: false
        model: local.numOfItems
        delegate: listdelegate_3_3
        onPgChanged: {
            positionViewAtIndex(pg * local.numOfItemsParPage, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * local.numOfItemsParPage, ListView.Beginning)
        }
    }
}
