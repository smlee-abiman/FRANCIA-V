import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Program/PosSet/PosSetUtils.js" as Utils
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_4_5.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: SH.HONG
**************************************************************************************/
Rectangle {
    property bool pageActive: false
    color: "transparent"

    QtObject {
        id: local
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[3].subMenuList[4]
        property var selectList: menuModel.selectList
        property int page: 0
        property var axisOrder: commonViewModel.axisOrder
        property int numAxis: axisOrder.length
        property int maxPageNo: Math.ceil(numAxis / numAxisPerPage)
        property int numAxisPerPage: 5
    }

    onPageActiveChanged: {
        if (pageActive) {
            local.menuModel.activate()
        } else {
            local.menuModel.deactivate()
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

//    ImageScrollBar{
//        height: 333
//        anchors.right: parent.right
//        anchors.rightMargin: -4
//        visible: local.maxPageNo >= 2
//        topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
//        topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
//        botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
//        botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
//        pageLabal: local.page+1 + "/" + local.maxPageNo
//        onScrollUpClicked: local.page = Math.max(local.page - 1, 0)
//        onScrollDownClicked: local.page = Math.min(local.page + 1, local.maxPageNo - 1)
//    }
    Component{
        id:listdelegate_7
        Rectangle{
            property int axisType: local.axisOrder[index]
            property bool select: local.selectList[axisType]
            x: 1
            width: 280
            height: 60
            color: "#eaeaea"
            border.color: "#999999"
            CheckBox{
                width: 100
                height: 36
                x:14
                anchors.verticalCenter: parent.verticalCenter
                checked: parent.select
                indicator:Image{
                    scale: 1.3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
                }

                contentItem: Text{
                    readonly property int axisLabelIndex: Utils.axisLabelIndex(axisType)
                    text: resourceManager.label(22, axisLabelIndex, language.value)
                    color: resourceManager.rgb(22, axisLabelIndex)
                    font.pixelSize: 17
                    font.bold: true
                    leftPadding: 30
                }
                onClicked: {
                    local.menuModel.selectList[axisType] = checked
                    checked = Qt.binding(function() { return parent.select })
                }
            }
        }
    }
    Rectangle{
        width: 280
        height: 32
        x:26
        y:10
        color: "#cc3333"
        border.color: "#cc3333"
        Text{
            anchors.centerIn: parent
            font.pixelSize: 19
            text: resourceManager.label(41,530, language.value)
            font.bold: true
            color: "white"
        }
    }//标题栏
    ListView{
        width: 281
        height:310
        x:25
        y:44
        property int pg: local.page
        spacing: 2
        clip: true
        model: local.axisOrder.length
        delegate: listdelegate_7
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg * local.numAxisPerPage, ListView.Beginning)
        }
    }
}
