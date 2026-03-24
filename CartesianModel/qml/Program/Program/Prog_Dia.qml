import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Prog_Dia.qml
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
Dialog {
    id:root
    width: 685
    height: 410
    focus: true
    closePolicy: Popup.NoAutoClose

    property bool isSelectionOnly: false
    signal selectProgram(var selectProgram)

    QtObject {
        id: local
        readonly property var teachingviewModel: teachingViewModel.pageModel[6]
        property bool isDeleteMode: false
        property var currentPageIndexs: [0, 0, 0]
    }

    onOpened: {
        commonViewModel.pageChange(Enums.TeachingProgram, 1);
        progview.positionViewAtIndex(0, GridView.Beginning)
        prog_tab.currentIndex = 0
        prog.page = 0
        local.currentPageIndexs = [0, 0, 0]
    }

    onClosed: {
        commonViewModel.pageChange(Enums.TeachingProgram, 0);
    }

    background: Rectangle{
        id:bac
        width: 685
        height: 410
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 685
            height: 50
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                color: resourceManager.rgb(41, 644)
                text: resourceManager.label(41, 644, language.value)
            }
        }
        ImageButton{
            x:610
            y:9
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
            downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
            onClicked: {
                local.isDeleteMode = false
                root.close()
            }
        }
    }

    Rectangle {
        id: prog
        property int page: 0
        width: 672
        height: 310
        x:-5
        y:84
        color: "transparent"

        //prog button
        Component {
            id: listdelegate
            Item {
                width: progview.cellWidth
                height: progview.cellHeight
                Rectangle{
                    anchors.fill: parent
                    anchors.margins: 3
                    color: "#eaeaea"
                    border.color: "#999999"
                    Text{
                        anchors.verticalCenter: parent.verticalCenter
                        x:3
                        font.pixelSize: 18
                        color: "black"
                        text: ("000" + model.modelData.no).slice(-3)
                    }
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 36
                        font.pixelSize: 16

                        wrapMode: Text.WordWrap
                        //fontSizeMode: Text.Fit
                        color: "black"
                        text:resourceManager.label(41, model.modelData.no + 36, resourceManager.language)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressed:{
                            //do not delete in case of progNo.1
                            if ((local.isDeleteMode) &&
                                    (model.modelData.no === teachingviewModel.ncpFile.mainProgram.no)) {
                                parent.color = "#eaeaea"
                            } else {
                                parent.color = "#cc3333"
                            }
                        }
                        onReleased: {
                            parent.color = "#eaeaea"
                        }
                        onClicked: {
                            if(local.isDeleteMode) {
                                //do not delete in case of progNo.1
                                if(model.modelData.no !== teachingviewModel.ncpFile.mainProgram.no) {
                                    deleteDialog.progNo = model.modelData.no
                                    deleteDialog.open()
                                }
                            } else {
                                root.selectProgram(model.modelData)
                                root.close();
                            }
                        }
                    }
                }
            }
        }

        //prog data
        GridView{
            id: progview
            property int pg: prog.page
            width: getProgViewWidth(cellWidth, pg, model)
            height: 294
            x: 6
            y: 6
            cellWidth: 176
            cellHeight: 49
            interactive: false
            clip: true
            flow: GridView.FlowTopToBottom
            model: {
                switch (prog_tab.currentIndex) {
                case 0:
                    return teachingviewModel.ncpFile.userPrograms
                case 1:
                    return teachingviewModel.ncpFile.manualPrograms
                case 2:
                    return teachingviewModel.ncpFile.systemPrograms
                }
            }
            delegate: listdelegate
            Component.onCompleted: {
                positionViewAtIndex(pg * 18, GridView.Beginning)
            }
        }

        //right menu
        Rectangle {
            width: 128
            height: 349
            x:545
            y:-38
            color: "#ffffff"
            Rectangle {
                width: 20
                height: 340
                color: "#ffffff"
                anchors.left: parent.left
            }
            Column{
                visible: !root.isSelectionOnly
                anchors.horizontalCenter: parent.horizontalCenter
                y:15
                spacing: 20
                Rectangle{
                    width: 100
                    height: 45
                    border.width: 2
                    border.color: "#999999"
                    color: add_mouse.pressed ? "#cc3333" : "#cccccc"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: add_mouse.pressed ? 21 : 22
                        color: resourceManager.rgb(41,607)
                        text: resourceManager.label(41,607,resourceManager.language)
                    }
                    MouseArea{
                        id:add_mouse
                        anchors.fill: parent
                        onClicked: {
                            local.isDeleteMode = false
                            numberKeyboardLoader.active = false
                            numberKeyboardLoader.active = true
                        }
                    }
                }//新建按钮
                Rectangle{
                    width: 100
                    height: 45
                    border.width: 2
                    border.color: "#999999"
                    color: (delete_mouse.pressed || local.isDeleteMode) ? "#cc3333" : "#cccccc"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: delete_mouse.pressed ? 23 : 22
                        color: resourceManager.rgb(41, 608)
                        text: resourceManager.label(41, 608,resourceManager.language)
                    }
                    MouseArea{
                        id:delete_mouse
                        anchors.fill: parent
                        onClicked: {
                            local.isDeleteMode = true
                        }
                    }
                }//删除按钮
            }

            //scroll
            ImageScrollBar{
                id: progScroll
                visible: (progview.model.length < 19) ? false : true
                height: 200
                y: 135
                anchors.horizontalCenter: parent.horizontalCenter

                property int maxPage: Math.ceil((progview.model.length / 18))
                topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
                topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
                botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
                botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
                pageLabal: (prog.page + 1) + "/" + maxPage
                onScrollUpClicked: {
                    prog.page = Math.max(prog.page - 1, 0)
                    local.currentPageIndexs[prog_tab.currentIndex] = prog.page
                    progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)
                }
                onScrollDownClicked: {
                    prog.page = Math.min(prog.page + 1, maxPage - 1)
                    local.currentPageIndexs[prog_tab.currentIndex] = prog.page
                    progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)
                }
            }
        }
    }

    //program select tab
    TabBar{
        id:prog_tab
        width: 335
        height: 40
        x:2
        y:42
        z:parent.z+1
        spacing: 10
        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
        }
        TabButton{
            id:tab_1
            height: 36
            anchors.bottom: parent.bottom
            background: Rectangle{
                border.width: 2
                color: (prog_tab.currentIndex === 0 || tab_1.pressed) ? "#cc3333" : "#cccccc"
                border.color: (prog_tab.currentIndex === 0 || tab_1.pressed) ? "#999999" : "#999999"
                // Rectangle{
                //     width: (prog_tab.currentIndex === 0 || tab_1.pressed) ? parent.width -4 : parent.width
                //     height: 2
                //     anchors.horizontalCenter: parent.horizontalCenter
                //     anchors.bottom: parent.bottom
                //     color: (prog_tab.currentIndex === 0 || tab_1.pressed) ? "#cc3333" : "#cccccc"
                // }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: (prog_tab.currentIndex === 0 || tab_1.pressed) ? "white" : "black"
                    text: resourceManager.label(41,604,resourceManager.language)
                    font.pixelSize: 18
                    font.bold: true

                }
            }
            onClicked: {
                local.isDeleteMode = false
                prog_tab.currentIndex = 0
                prog.page = local.currentPageIndexs[prog_tab.currentIndex]
                progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)
            }
        }
        TabButton{
            id:tab_2
            height: 36
            anchors.bottom: parent.bottom
            background: Rectangle{
                border.width: 2
                color: (prog_tab.currentIndex === 1 || tab_2.pressed) ? "#cc3333" : "#cccccc"
                border.color: (prog_tab.currentIndex === 1 || tab_2.pressed) ? "#999999" : "#999999"
                // Rectangle{
                //     width: (prog_tab.currentIndex === 1 || tab_2.pressed) ? parent.width -4 : parent.width
                //     height: 2
                //     anchors.horizontalCenter: parent.horizontalCenter
                //     anchors.bottom: parent.bottom
                //     color: (prog_tab.currentIndex === 1 || tab_2.pressed) ? "#cc3333" : "#cccccc"
                // }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: (prog_tab.currentIndex === 1 || tab_2.pressed) ? "white" : "black"
                    text: resourceManager.label(41,605,resourceManager.language)
                    font.pixelSize: 18
                    font.bold: true

                }
            }
            onClicked: {
                local.isDeleteMode = false
                prog_tab.currentIndex = 1
                prog.page = local.currentPageIndexs[prog_tab.currentIndex]
                progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)
            }
        }
        TabButton{
            id:tab_3
            height: 36
            anchors.bottom: parent.bottom
            background: Rectangle{
                border.width: 2
                color: (prog_tab.currentIndex === 2 || tab_3.pressed) ? "#cc3333" : "#cccccc"
                border.color: (prog_tab.currentIndex === 2 || tab_3.pressed) ? "#999999" : "#999999"
                // Rectangle{
                //     width: (prog_tab.currentIndex === 2 || tab_3.pressed) ? parent.width -4 : parent.width
                //     height: 2
                //     anchors.horizontalCenter: parent.horizontalCenter
                //     anchors.bottom: parent.bottom
                //     color: (prog_tab.currentIndex === 2 || tab_3.pressed) ? "#cc3333" : "#cccccc"
                // }
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: (prog_tab.currentIndex === 2 || tab_3.pressed) ? "white" : "black"
                    text: resourceManager.label(41,606,resourceManager.language)
                    font.pixelSize: 18
                    font.bold: true

                }
            }
            onClicked: {
                local.isDeleteMode = false
                prog_tab.currentIndex = 2
                prog.page = local.currentPageIndexs[prog_tab.currentIndex]
                progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)
            }
        }
    }

    //numeric keypad
    Loader {
        id: numberKeyboardLoader
        active: false
        onLoaded: item.open()
        sourceComponent: Number_Keyboard {
            id: numKeyboard
            visible: false
            x: root.x + 450
            y: root.y

            onOpened: {
                if(prog_tab.currentIndex === 0) {
                    min = 1
                    max = 99
                } else if(prog_tab.currentIndex === 1) {
                    min = 178
                    max = 249
                } else if(prog_tab.currentIndex === 2) {
                    min = 100
                    max = 255
                }
                digits = 0
            }

            onClosed: {
                let numKeyboardDigits = Number(digits)

                if(isEnterkeyPressd) {
                    isEnterkeyPressd = false
                    if(prog_tab.currentIndex === 0) {
                        if(teachingviewModel.ncpFile.checkExistUserPrograms(numKeyboardDigits)) {
                            errorDialog.errKinds = 0
                            errorDialog.open()
                            return
                        }
                        teachingviewModel.createNcpProgram(numKeyboardDigits)
                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)

                    } else if(prog_tab.currentIndex === 1) {
                        if(teachingviewModel.ncpFile.checkExistManualPrograms(numKeyboardDigits)) {
                            errorDialog.errKinds = 0
                            errorDialog.open()
                            return
                        }
                        teachingviewModel.createNcpProgram(numKeyboardDigits)
                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)

                    } else if(prog_tab.currentIndex === 2) {
                        if((numKeyboardDigits < 250 || numKeyboardDigits > 255) && (numKeyboardDigits < 100 || numKeyboardDigits > 177)) {
                            errorDialog.errKinds = 1
                            errorDialog.open()
                            return
                        } else if(teachingviewModel.ncpFile.checkExistSystemPrograms(numKeyboardDigits)) {
                            errorDialog.errKinds = 0
                            errorDialog.open()
                            return
                        }
                        teachingviewModel.createNcpProgram(numKeyboardDigits)
                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)
                    }
                }
            }
        }
    }

    //error dialog
    ConfirmPopup {
        id: errorDialog
        property int errKinds: 0
        property int resourseIndex: errKinds > 0 ? 641 : 640
        color: resourceManager.rgb(41, resourseIndex)
        text: resourceManager.label(41, resourseIndex, resourceManager.language)
        isBtnVisible: false
    }

    //delete confirm dialog
    ConfirmPopup {
        id: deleteDialog
        color: resourceManager.rgb(41, 642)
        text: resourceManager.label(41, 642, resourceManager.language)
        isBtnVisible: true
        property int progNo: 0

        onAccepted: {
            //change to progNo.1 if the currently selected program is deleted
            if (teachingviewModel.selectedProgramNo === progNo) {
                root.selectProgram(teachingviewModel.ncpFile.mainProgram)
            }

            teachingviewModel.deleteNcpProgram(progNo)

            if (prog.page === progScroll.maxPage) {
                if ((progview.model.length % 18) === 0) {
                    prog.page = Math.max(prog.page - 1, 0)
                    local.currentPageIndexs[prog_tab.currentIndex] = prog.page
                }
            }
            progview.positionViewAtIndex(prog.page * 18, GridView.Beginning)
        }

        onClosed: {
            local.isDeleteMode = false
        }
    }

    function setDlgPoint(dlgX ,dlgY)
     {
         //设置窗口拖拽不能超过父窗口
         if(root.x + dlgX < 0){
             root.x = 0
         }else if(root.x + dlgX > root.parent.width - root.width){
             root.x = root.parent.width - root.width
         }else{
             root.x = root.x + dlgX
         }

         if(root.y + dlgY < 0){
             root.y = 0
         }else if(root.y + dlgY > root.parent.height - root.height){
             root.y = root.parent.height - root.height
         }else{
             root.y = root.y + dlgY
         }
     }

    function getProgViewWidth(cellWidth, pg, progData)
    {
        //check last page
        if((pg + 1) === Math.ceil((progData.length / 18))) {
            //check not last cell
            if((progData.length % 18) !== 0) {
                //cellWidth * number of column
                return cellWidth * (Math.ceil((progData.length % 18) / 6))
            } else {
                return 526
            }
        } else {
            return 526
        }
    }
}
