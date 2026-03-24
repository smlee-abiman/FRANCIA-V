import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4.qml
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
    id: root
    width: 370
    height: 511

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4
        readonly property int menuIndex: menu4Model.menuIndex
        readonly property bool isSelectedMenu41: (menuIndex === 0)
        readonly property bool isSelectedMenu42: (menuIndex === 1)
        readonly property bool isSelectedMenu43: (menuIndex === 2)
        property int language: 0
    }

    onPageActiveChanged: {
        if (pageActive) {
            wait.active = true
            judge.active = true
            counter.active = true
        }
    }

    Language { id: language }

    Rectangle{
        width: 430
        height: 85
        color: "#999999"
        Row{
            anchors.verticalCenter: parent.verticalCenter
            x:31
            spacing: 2
            // wait button.
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(local.isSelectedMenu41) ? resourceManager.imagePath + "/16_program/a160_61.png" : resourceManager.imagePath + "/16_program/a160_60.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_61.png"
                    onClicked: {
                        clear_select()
                        local.menu4Model.menuIndex = 0
                    }
                }
                Text{
                    width: 90
                    height: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: lineCount != 1 ? 16 : 18
                    font.bold: true

                    color:"black"
                    text: resourceManager.label(41,23, language.value)
                }
            }
            // judge button.
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(local.isSelectedMenu42) ? resourceManager.imagePath + "/16_program/a160_63.png" : resourceManager.imagePath + "/16_program/a160_62.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_63.png"
                    onClicked:{
                        local.menu4Model.menuIndex = 1
                        clear_select()
                        judge.setMenuIndex(0)
                    }
                }
                Text{
                    width: 90
                    height: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: lineCount != 1 ? 16 : 18
                    font.bold: true

                    color:"black"
                    text: resourceManager.label(41,24, language.value)
                }
            }
            // counter button.
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(local.isSelectedMenu43) ? resourceManager.imagePath + "/16_program/a160_65.png" : resourceManager.imagePath + "/16_program/a160_64.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_65.png"
                    onClicked: {
                        local.menu4Model.menuIndex = 2
                        clear_select()
                        counter.setMenuIndex(0)
                    }
                }
                Text{
                    width: 90
                    height: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: lineCount != 1 ? 16 : 18
                    font.bold: true

                    color:"black"
                    text: resourceManager.label(41,25, language.value)
                }
            }
        }
    }//菜单栏

    StackLayout{
        width: parent.width
        height: 453
        y:85
        currentIndex: local.menuIndex

        Loader {
            id: wait
            property bool isActive: root.pageActive && local.isSelectedMenu41

            function clearSelect(){
                if (item)
                    item.clearSelect()
            }

            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_1 {
                pageActive: wait.isActive
            }
        }//等待

        Loader {
            id: judge
            property bool isActive: root.pageActive && local.isSelectedMenu42

            function clearSelect(){
                if (item)
                    item.clearSelect()
            }
            function setMenuIndex(index){
                if (item)
                    item.setMenuIndex(index)
            }

            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_2 {
                pageActive: judge.isActive
            }
        }//判断

        Loader {
            id: counter
            property bool isActive: root.pageActive && local.isSelectedMenu43

            function setMenuIndex(index)
            {
                if (item)
                    item.setMenuIndex(index)
            }

            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_3 {
                pageActive: counter.isActive
            }
        }//计数器
    }
    function clear_select(){
        wait.clearSelect()
        judge.clearSelect()
    }
}
