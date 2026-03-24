import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Label_Dia.qml
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
Dialog {
    id: root
    width: 640
    height: 320
    focus: true
    closePolicy: Popup.NoAutoClose

    signal selectLabel(var selectLabelNo)

    property var selectedProgram: null
    property bool isAllAssignedLabels: false

    QtObject {
        id: local
        readonly property int numOfLabelsPerPage: 25
        readonly property int maxNumOfLabels: 100
        readonly property var teachingviewModel: teachingViewModel.pageModel[6]
        readonly property var assignedLabels: teachingviewModel.ncpFile.labelStorage.assignedLabels
        readonly property var unUsedLabels: (selectedProgram === null) ? null : selectedProgram.unUsedLabels
        property var targetLabels: []
        property int page: 0
        property int selectLabelNo: -1
        property int selectLabelIndex: -1
        property int addLabelNo: -1

        function insertLabel(newLabel) {
            let size = targetLabels.length;
            if (size <= 0) {
                // list is empty.
                // add to end of list.
                targetLabels.push(newLabel)
                selectLabelIndex = 0
            } else {
                // search insert index.
                let insertIndex = -1
                for (let i = 0; i < size; i++) {
                    let label = local.targetLabels[i];
                    if (newLabel.no < label.no) {
                        insertIndex = i
                        break
                    }
                }
                if (insertIndex === -1) {
                    // new label no is largest.
                    // add to end of list.
                    targetLabels.push(newLabel)
                    selectLabelIndex = targetLabels.length - 1
                } else {
                    // insert into list.
                    targetLabels.splice(insertIndex, 0, newLabel)
                    selectLabelIndex = insertIndex
                }
            }
            targetLabelsChanged()   // notify update targetLabels.
        }
        function isDuplicateLabel(labelNo) {
            for (const label of targetLabels) {
                if (labelNo === label.no) {
                    return true
                }
            }
            return false
        }
        function getLabelsAssignedToProgram() {
            let labelsAssignedToProgram = []
            for (let i=0; i < local.assignedLabels.length; i++) {
                let label = assignedLabels[i]
                if (label.programNo === selectedProgram.no) {
                    labelsAssignedToProgram.push(label)
                    if (labelsAssignedToProgram.length === maxNumOfLabels) {
                        break
                    }
                }
            }
            return labelsAssignedToProgram
        }
    }

    onOpened: {
        local.page = 0
        local.selectLabelNo = -1
        local.selectLabelIndex = -1
        local.addLabelNo = -1
        local.targetLabels = (isAllAssignedLabels) ?
                                local.getLabelsAssignedToProgram() :
                                local.unUsedLabels.slice(0, local.maxNumOfLabels)
        labelView.positionViewAtIndex(0, GridView.Beginning)
    }

    background: Rectangle{
        width: 640
        height: 320
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 640
            height: 50
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                font.bold: true
                color: resourceManager.rgb(41, 667)
                text: resourceManager.label(41, 667, resourceManager.language)
            } //title
        }
        ImageButton{
            x:585
            y:9
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
            downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
            onClicked: {
                root.close()
            }
        } //close button
    }
    //right menu
    Rectangle {
        width: 110
        height: 200
        x:516
        y:48
        color: "#ffffff"
        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            spacing:4
            //add button
            Rectangle {
                width: 100
                height: 42
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    id:add_te
                    anchors.centerIn: parent
                    font.pixelSize: 22
                    color: resourceManager.rgb(41,607)
                    text: resourceManager.label(41,607, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.source = resourceManager.imagePath + "/08_Monitor/d080_25.png"
                        add_te.font.pixelSize = 24
                    }
                    onReleased: {
                        parent.source = resourceManager.imagePath + "/08_Monitor/d080_24.png"
                        add_te.font.pixelSize = 22
                    }
                    onClicked: {
                        if (local.maxNumOfLabels <= local.targetLabels.length) {
                            console.warn("the maximum number of labels has been reached.")
                            return
                        }
                        if (local.addLabelNo !== -1) {
                            console.warn("allready added.")
                            return
                        }
                        if (teachingviewModel.ncpFile.isEmptyFreeLabels()) {
                            console.warn("no empty label number.")
                            return
                        }
                        numberKeyboardLoader.active = false
                        numberKeyboardLoader.active = true
                    }
                }
            }
            //determine button
            Rectangle {
                width: 100
                height: 42
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    id:determine_te
                    anchors.centerIn: parent
                    font.pixelSize: 22
                    color: resourceManager.rgb(41,292)
                    text: resourceManager.label(41, 292, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        parent.source = resourceManager.imagePath + "/08_Monitor/d080_25" + ".png"
                        determine_te.font.pixelSize = 24
                    }
                    onReleased: {
                        parent.source = resourceManager.imagePath + "/08_Monitor/d080_24" + ".png"
                        determine_te.font.pixelSize = 22
                    }
                    onClicked: {
                        if (local.selectLabelNo > 0) {
                            root.selectLabel(local.selectLabelNo)
                            root.close()
                        }
                    }
                }
            }
        }

        //scroll
        ImageScrollBar{
            id: scroll
            height: 156
            anchors.horizontalCenter: parent.horizontalCenter
            y:112
            visible: (2 <= maxPage)
            property int maxPage: {
                let ret = Math.ceil((local.targetLabels.length / local.numOfLabelsPerPage))
                let max = (local.maxNumOfLabels / local.numOfLabelsPerPage)
                return (ret > max) ? max : ret
            }
            topButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_43.png"
            topButtonPressImage: resourceManager.imagePath + "/16_program/a160_44.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/16_program/a160_45.png"
            botomButtonPressImage: resourceManager.imagePath + "/16_program/a160_46.png"
            pageLabal: (local.page + 1) + "/" + maxPage
            onScrollUpClicked: {
                local.page = Math.max(local.page - 1, 0)
                labelView.positionViewAtIndex(local.page * local.numOfLabelsPerPage, GridView.Beginning)
            }
            onScrollDownClicked: {
                local.page = Math.min(local.page + 1, maxPage - 1)
                labelView.positionViewAtIndex(local.page * local.numOfLabelsPerPage, GridView.Beginning)
            }
        }
    }

    //label
    Item {
        width: 516
        height: 262
        x:-4
        y:40
        Component {
            id:listDelegate
            Rectangle {
                width: 95
                height: 42
                color: (local.selectLabelIndex === index) ? "#cc3333" : "white"
                border.color: "#999999"
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    text: "L" + ('000' + local.targetLabels[index].no).slice(-3)
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        local.selectLabelNo = local.targetLabels[index].no
                        local.selectLabelIndex = index
                    }
                }
            }
        }

        GridView {
            id:labelView
            width: 515
            height: getLabelViewHeight(cellHeight, local.page, model)
            x: 8
            y: 10
            cellWidth: 102
            cellHeight: 50
            interactive: false
            clip: true
            model: Math.min(local.targetLabels.length, local.maxNumOfLabels)
            flow: GridView.FlowLeftToRight
            delegate: listDelegate
            Component.onCompleted: {
                positionViewAtIndex(local.page * local.numOfLabelsPerPage, GridView.Beginning)
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
            x: root.x + 210
            y: root.y
            min: 1
            max: 999
            isEmptyPermit: true

            onClosed: {
                if (isEnterkeyPressd) {
                    isEnterkeyPressd = false

                    let newLabel = null
                    if (digits === "") {
                        newLabel = (isAllAssignedLabels) ?
                                        teachingviewModel.ncpFile.findFreeLabel() :
                                        teachingviewModel.ncpFile.findFreeLabel(local.targetLabels);
                    } else {
                        let newLabelNo = Number(digits)
                        if (local.isDuplicateLabel(newLabelNo)) {
                            errorDialog.errKinds = 0
                            errorDialog.open()
                            return
                        }
                        if (!teachingviewModel.ncpFile.isExistFreeLabels(newLabelNo)) {
                            errorDialog.errKinds = 1
                            errorDialog.open()
                            return
                        }
                        newLabel = teachingviewModel.ncpFile.labelAt(newLabelNo)
                    }

                    if (newLabel === null) {
                        // memo: This case is not possible. It's a bug.
                        console.error("failed to add new label.")
                        return
                    }

                    local.selectLabelNo = newLabel.no
                    local.addLabelNo = local.selectLabelNo
                    local.insertLabel(newLabel)     // memo: update local.selectLabelIndex

                    local.page = Math.ceil((local.selectLabelIndex + 1) / local.numOfLabelsPerPage) - 1
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    labelView.positionViewAtIndex(local.page * local.numOfLabelsPerPage, GridView.Beginning)
                }
            }
        }
    }

    //error dialog
    ConfirmPopup {
        id: errorDialog
        property int errKinds: 0
        property int resourseIndex: errKinds > 0 ? 666 : 665
        color: resourceManager.rgb(41, resourseIndex)
        text: resourceManager.label(41, resourseIndex, resourceManager.language)
        isBtnVisible: false
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

    function getLabelViewHeight(cellHeight, page, modelLength)
    {
        //check last page
        if((page + 1) === Math.ceil((modelLength / local.numOfLabelsPerPage))) {
            //check not last cell
            if((modelLength % local.numOfLabelsPerPage) !== 0) {
                //cellHeight * number of line
                return cellHeight * (Math.ceil((modelLength % local.numOfLabelsPerPage) / 5))
            } else {
                return 246
            }
        } else {
            return 246
        }
    }
}
