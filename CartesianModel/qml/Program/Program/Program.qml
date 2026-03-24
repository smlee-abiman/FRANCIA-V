import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0            // Enums.
import "../../Widget"
import "../../Components"
import "../.."

/*************************************************************************************
* Title: UI/UX of AEK
* File: Program.qml
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
/**********************程序窗口****************************/
Item {
    id:pro
    width: 1018
    height: 670
    x: 2.5
    y: 21
    property var teachingviewModel: teachingViewModel.pageModel[6]
    property var menu1_4Model: teachingviewModel.progMenu1.subMenuList[3]
    property var menu4Model: teachingviewModel.progMenu4   // ProgramMenu4
    property var menu41Model: menu4Model.subMenuList[0]    // ProgramMenu4_1
    property var menu42Model: menu4Model.subMenuList[1]    // ProgramMenu4_2
    property var menu421Model: menu42Model.subMenuList[0]  // ProgramMenu4_2_1
    property var menu422Model: menu42Model.subMenuList[1]  // ProgramMenu4_2_2
    property var menu423Model: menu42Model.subMenuList[2]  // ProgramMenu4_2_3
    property var menu5_2Model: teachingviewModel.progMenu5.subMenuList[1]
    property bool expand: false
    property int progCommandclick: -1   //当前程序显示主窗口指令程序栏点击索引
    property int progLabelclick: -1     //当前程序显示主窗口标签栏点击索引
    property bool progLabelDiaType: false
    property int progviewPage: teachingviewModel.ncp.progviewPage
    property bool pen: false
    property bool pageActive: false
    property bool modify: true
    property bool isPopupDisp:false
    property int curProgSaveStatus:-3
    property string compileErrMsg:resourceManager.label(41,648,language.value)
    property int compileErrorStep:teachingviewModel.compileErrorStep
    property bool isInauto: commonViewModel.manipulateMode === 0 || commonViewModel.manipulateMode === 2
    property var stepOperate: teachingviewModel.step
    property int selectPrgBtnIndex: 1 // ADD : button Index By. IJ.YI 25.07.19
    signal programClose()

    QtObject {
        id: local
        readonly property int runningStepNo: teachingviewModel.runningStepNo
        readonly property bool autoDrive: (commonViewModel.manipulateMode === 2)
        property bool realTimeStep: false
        property int processResult:0
        readonly property var ncpFile: teachingviewModel.ncpFile
        property var selectedProgram: ncpFile.mainProgram
        readonly property int wizardMenuIndex: teachingviewModel.Mainmenu_Index
        readonly property int numOfSteps: (selectedProgram === null) ? 0 : selectedProgram.steps.length
        readonly property int limitNumOfSteps: 255
        readonly property int numOfItemsParPage: 7
        readonly property int numOfPages: {
            let page = Math.ceil(numOfSteps / numOfItemsParPage)
            return Math.min(limitNumOfPages, page)
        }
        readonly property int limitNumOfPages: Math.ceil(limitNumOfSteps/numOfItemsParPage)
        property int currentPage: 0
        property int currentPageFirstRow: 0
        property int curStatusCode:-1
        property string curDetailInfo:""
        property bool ncpStepListUpdate: teachingviewModel.ncpStepListUpdate

        readonly property int topPopupDialog: commonViewModel.topPopupDialog
        readonly property bool isNcpFileLoaded: teachingviewModel.isNcpFileLoaded

        property bool dataLoaded: teachingviewModel.dataLoaded

        onNcpFileChanged: {
            initialize()
        }//NCP 文件改变

        onRealTimeStepChanged: {
            if (realTimeStep) {
                // memo: move to current running step no.
                let stepNo = runningStepNoInSelectedProgram()
                pro.progCommandclick = stepNo;
                pro.progLabelclick = stepNo;
                (stepNo === -1) ? goToPage(0) : goToPage(stepNo);
            } else {
                // memo:pro.progCommandclick stays the same.
                pro.progLabelclick = -1;
            }
        }//实时step变化

        onRunningStepNoChanged: {
            if (!realTimeStep) { return }
            if ((runningStepNo < 0) || (local.numOfSteps <= runningStepNo) || (local.limitNumOfSteps <= runningStepNo)) {
                return
            }
            pro.progCommandclick = runningStepNo
            pro.progLabelclick = runningStepNo
            goToPage(runningStepNo)
        }
        onSelectedProgramChanged: {
            if (selectedProgram === null) {
                return
            }
            teachingviewModel.selectedProgramNo = selectedProgram.no
            let stepNo = -1;
            if (realTimeStep) {
                stepNo = runningStepNoInSelectedProgram();
            }
            pro.progCommandclick = stepNo;
            pro.progLabelclick = stepNo;
            (stepNo === -1) ? goToPage(0) : goToPage(stepNo);
        }
        onTopPopupDialogChanged: {
            controlNcpFileLoadingDialog()
        }
        onIsNcpFileLoadedChanged: {
            controlNcpFileLoadingDialog()
        }

        function startAsyncCreationOfDialog() {
            prog_dia_loader.active = true
            label_dia_loader.active = true
            variable_dia_loader.active = true
        }
        function initialize() {
            modify = true
            realTimeStep = false
            selectedProgram = ncpFile.mainProgram
            currentPage = 0
        }
        function prevPage() {
            currentPage = Math.max(currentPage - 1, 0);
            currentPageFirstRow = currentPage * 9
            progCommandclick = progCommandclick % 9 + currentPageFirstRow
            proglist.positionViewAtIndex(currentPage * numOfItemsParPage, ListView.Beginning)
        }
        function nextPage() {
            currentPage = Math.min(currentPage + 1, numOfPages - 1);
            currentPageFirstRow = currentPage * 9
            progCommandclick = progCommandclick % 9 + currentPageFirstRow
            proglist.positionViewAtIndex(currentPage * numOfItemsParPage, ListView.Beginning)
        }
        function prevRow() {
            currentPageFirstRow = Math.max(currentPageFirstRow - 1,0);
            currentPage = currentPageFirstRow / 9
            if(progCommandclick == (currentPageFirstRow + 9)){
                progCommandclick = currentPageFirstRow + 8
            }
            proglist.positionViewAtIndex(currentPageFirstRow, ListView.Beginning)
        }
        function nextRow() {
            currentPageFirstRow = Math.min(currentPageFirstRow + 1,numOfSteps - 9)
            currentPage = currentPageFirstRow / 9
            if(progCommandclick == currentPageFirstRow -1){
                progCommandclick = currentPageFirstRow
            }
            proglist.positionViewAtIndex(currentPageFirstRow, ListView.Beginning)
        }
        function goToPage(stepIndex) {
            currentPage = Math.floor(stepIndex / numOfItemsParPage);
            currentPage = Math.min(currentPage, numOfPages - 1);
            currentPage = Math.max(currentPage, 0);
            proglist.positionViewAtIndex(currentPage * numOfItemsParPage, ListView.Beginning)
        }

        function assignLabelNo(labelNo) {
            // No need to check pro.progLabelclick.
            teachingviewModel.assignLabelNo(selectedProgram, pro.progLabelclick, labelNo)
            pro.progLabelclick = -1
        }
        function releaseLabel() {
            // No need to check pro.progLabelclick.
            teachingviewModel.releaseLabel(selectedProgram, pro.progLabelclick)
            pro.progLabelclick = -1
        }
        function replaceNcpCommandByCommandLine(commandLine) {
            if (commandLine === "") { return }
            // No need to check pro.progCommandclick.
            if (teachingviewModel.replaceNcpCommandByCommandLine(selectedProgram, pro.progCommandclick, commandLine.trim()) !== 0) {
                // replace failed.
                return
            }
        }

        function copySelectedStep() {
            // No need to check pro.progCommandclick.
            teachingviewModel.copyNcpStepToTemp(selectedProgram, pro.progCommandclick)
        }
        function pasteCopiedStep() {
            if (limitNumOfSteps <= numOfSteps) {
                console.log(`The number of steps exceeds the limit(${limitNumOfSteps})."`)
                return
            }
            // No need to check pro.progCommandclick.
            if (teachingviewModel.pasteNcpStepFromTemp(selectedProgram, pro.progCommandclick) !== 0) {
                // paste failed.
                return
            }
            pro.progCommandclick++
            goToPage(pro.progCommandclick)
        }
        function removeSelectedStep() {
            // No need to check pro.progCommandclick.
            teachingviewModel.removeNcpStepInNcpProgram(selectedProgram, pro.progCommandclick)
            goToPage(pro.progCommandclick)
            pro.progCommandclick = -1
        }
        function moveUpSelectedStep() {
            // No need to check pro.progCommandclick.
            teachingviewModel.moveBackOneStepInNcpProgram(selectedProgram, pro.progCommandclick)
            pro.progCommandclick--
            goToPage(pro.progCommandclick)
        }
        function moveDownSelectedStep() {
            // No need to check pro.progCommandclick.
            teachingviewModel.moveForwardOneStepInNcpProgram(selectedProgram, pro.progCommandclick)
            pro.progCommandclick++
            goToPage(pro.progCommandclick)
        }
        function updateModbusData() {
            if (teachingviewModel.updateModbusData() !== 0) {
                // update failed.
                return
            }
        }
        function replaceNcpCommandByWizard() {
            if (pro.progCommandclick === -1) { return; }
            if (teachingviewModel.replaceNcpCommand(selectedProgram, pro.progCommandclick) !== 0) {
                // replace failed.
                return
            }
        }
        function insertNcpStepByWizard() {
            if (limitNumOfSteps <= numOfSteps) {
                console.log(`The number of steps exceeds the limit(${limitNumOfSteps})."`)
                return
            }
            let insertLineIndex = pro.progCommandclick;
            let nextLineIndex = insertLineIndex + 1;
            if (pro.progCommandclick === -1) {
                if (numOfSteps <= 0) {
                    insertLineIndex = 0;
                    nextLineIndex = 0;
                } else {
                    insertLineIndex = numOfSteps - 1;
                    nextLineIndex = insertLineIndex + 1;
                }
            }
            if (teachingviewModel.insertNcpCommand(selectedProgram, insertLineIndex) !== 0) {
                // insert failed.
                return
            }
            pro.progCommandclick = nextLineIndex;
            goToPage(pro.progCommandclick)
        }
        function getCompileDetailInfo(statusCode){
            local.curStatusCode = statusCode
            switch(statusCode){
            case 148:
                local.curDetailInfo = resourceManager.label(41,650,language.value)
                break
            case 149:
                local.curDetailInfo = resourceManager.label(41,651,language.value)
                break
            case 150:
                local.curDetailInfo = resourceManager.label(41,652,language.value)
                break
            case 151:
                local.curDetailInfo = resourceManager.label(41,653,language.value)
                break
            case 152:
                local.curDetailInfo = resourceManager.label(41,654,language.value)
                break
            case 153:
                local.curDetailInfo = resourceManager.label(41,655,language.value)
                break
            case 154:
                local.curDetailInfo = resourceManager.label(41,656,language.value)
                break
            case 155:
                local.curDetailInfo = resourceManager.label(41,657,language.value)
                break
            case 156:
                local.curDetailInfo = resourceManager.label(41,658,language.value)
                break
            case 157:
                local.curDetailInfo = resourceManager.label(41,659,language.value)
                break
            case 158:
                local.curDetailInfo = resourceManager.label(41,660,language.value)
                break
            default:
                break
            }
        }
        function processPopup(progSaveStatus){
            pro.curProgSaveStatus = progSaveStatus;
            if (pro.curProgSaveStatus >= -2){
                if (pro.isPopupDisp === true) {
                    confirmPopup.close()
                    pro.isPopupDisp = false
                }
                let isPopup = true;
                switch(pro.curProgSaveStatus){
                case -2:
                    confirmPopup.text = resourceManager.label(41,664,language.value)
                    confirmPopup.color = resourceManager.rgb(41,664)
                    confirmPopup.isBtnVisible = false
                    confirmPopup.isDetailBtnVisible = false
                    break;
                case -1:
                    confirmPopup.text = resourceManager.label(41,664,language.value)
                    confirmPopup.color = resourceManager.rgb(41,664)
                    confirmPopup.isBtnVisible = false
                    confirmPopup.isDetailBtnVisible = false
                    break;
                case 0:
                    isPopup = false;
                    break;
                case 1:
                    confirmPopup.text = resourceManager.label(41,646,language.value)
                    confirmPopup.color = resourceManager.rgb(41,646)
                    confirmPopup.isBtnVisible = false
                    confirmPopup.isDetailBtnVisible = false
                    break;
                case 2:
                    confirmPopup.text = resourceManager.label(41,647,language.value)
                    confirmPopup.color = resourceManager.rgb(41,647)
                    confirmPopup.isBtnVisible = false
                    confirmPopup.isDetailBtnVisible = false
                    break;
                case 3:
                    // Compile Success
                    isPopup = false;
                    break;
                case 4:
                    var msgStr = (pro.compileErrMsg).toString()
                        .replace("[code]", local.curStatusCode)
                        .replace("[detail]", local.curDetailInfo);
                    confirmPopup.text = msgStr
                    confirmPopup.color = resourceManager.rgb(41,648)
                    confirmPopup.isBtnVisible = false
                    confirmPopup.isDetailBtnVisible = true
                    confirmPopup.detailBtnResourceGroup = 41
                    confirmPopup.detailBtnResourceIndex = 649
                    break;
                default:
                    isPopup = false;
                    break;
                }
                if (isPopup === true){
                    confirmPopup.open()
                    pro.isPopupDisp = true
                }
            }
        }
        function getCompileErrorStepInfo(errLineNo){
            let errorProgram = ncpFile.getProgramFromCompileErrorStepNoForQml(errLineNo)
            if (errorProgram === null) {
                console.warn(`not found program, errLineNo:${errLineNo}`)
                return
            }
            local.selectedProgram = errorProgram
            let errorStepNo = ncpFile.getNcpStepNoInProgramFromCompileErrorStepNo(errLineNo)
            if ((errorStepNo < 0) || (local.numOfSteps <= errorStepNo) || (local.limitNumOfSteps <= errorStepNo)) {
                console.warn(`out of range, errLineNo:${errLineNo}, errorStepNo:${errorStepNo}`)
                errorStepNo = 0
            }
            pro.progCommandclick = errorStepNo
            goToPage(errorStepNo)
        }

        function runningStepNoInSelectedProgram() {
            let len = teachingviewModel.runningProgramNos.length;
            for (let i = 0; i < len; i++) {
                let runningProgramNo = teachingviewModel.runningProgramNos[i];
                if (selectedProgram.no === runningProgramNo) {
                    return teachingviewModel.runningProgramSteps[i];
                }
            }
            return -1;
        }
        function controlNcpFileLoadingDialog() {
            if (isNcpFileLoaded) {
                closeNcpFileLoadingDialog()
                return
            }
            // loading ncp file.
            let isFileDialogOpened = commonViewModel.isPopupDialogOpened(Enums.PopupDialogStatusFile)
            if (isFileDialogOpened) {
                return
            }
            openNcpFileLoadingDialog()
        }
        function openNcpFileLoadingDialog() {
            if (pro.pageActive) {
                ncpFileLoadingDialog.open()
            }
        }
        function closeNcpFileLoadingDialog() {
            if (pro.pageActive) {
                ncpFileLoadingDialog.close()
            }
        }
    }

    Component.onCompleted: {
        // console.log("Program.qml Component.onCompleted()")  // todo: debug.
        teachingviewModel.selectMenu1()
        local.startAsyncCreationOfDialog()
        local.initialize()
    }
    Component.onDestruction: {
        // console.log("Program Component.onDestruction()") // todo: debug.
        teachingviewModel.deactivate()
    }
    onExpandChanged: {
        if (expand) {
            commonViewModel.pageChange(Enums.TeachingProgram, 3);
        } else {
            commonViewModel.pageChange(Enums.TeachingProgram, 0);
        }
    }

    onPageActiveChanged: {
        if (pageActive) {
            teachingviewModel.startPageChanging()
            commonViewModel.pageChange(Enums.TeachingProgram, 0);
            teachingviewModel.activate()
            if (!local.isNcpFileLoaded) {
                local.openNcpFileLoadingDialog()
            }
            componentLoader.loadComponent(local.wizardMenuIndex)
        } else {
            componentLoader.unloadAllComponents()
            teachingviewModel.deactivate()
            local.initialize()
            phyKeyManager. setStepOperate(false)
        }
    }

    onStepOperateChanged: {
        if(stepOperate){
            phyKeyManager. setStepOperate(true)
        }else{
            phyKeyManager. setStepOperate(false)
        }
    }

    ComponentLoader {
        id: componentLoader
        loadComponentList: [
            mainmenu1,
            mainmenu2,
            mainmenu3,
            mainmenu4,
            mainmenu5
        ]
    }

    Language { id: language }

    Rectangle{
        width: parent.width
        height: parent.height
        border.width: 2
        border.color: "white"
        color: "#ffffff"
        radius: 6
    }//background

/********************************left area*******************************/
    ImageButton{
        x:5
        y:5
        upImageSource: resourceManager.imagePath + "/16_program/a160_03" + ".png"
        downImageSource: resourceManager.imagePath + "/16_program/a160_04" + ".png"
        onClicked: {
            if (teachingviewModel.ncpFile.edited) {
                teachingviewModel.requestOpenSaveProgramDialog()
            } else {
                pro.programClose()
            }
        }
    }//close button

    Row{
        x:58
        y:6
        spacing: 6
        Text{
            width: 80
            height: 45
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            color: resourceManager.rgb(41, 2)
            text: resourceManager.label(41,2, language.value)
        }

        Rectangle{
            id:progte
            width: 370
            height: 45
            anchors.verticalCenter: parent.verticalCenter
            border.width: 2
            border.color: "#999999"
            color: "white"
            radius: 6
            Text{
                readonly property int resourceIndex: (local.selectedProgram === null) ? 37 : 37 + (local.selectedProgram.no - 1) // program no ... 1base.
                id:progte_te
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: 6
                font.pixelSize: 18
                color: "black"
                text: (local.selectedProgram === null) ? "" : ("000" + local.selectedProgram.no).slice(-3) + " " + resourceManager.label(41, resourceIndex, language.value)
            } // selected program name text.
            MouseArea{
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: (stepEditButton.visible) ? stepEditButton.left : parent.right
                }
                onPressed: parent.color = "#cc3333"
                onReleased: {
                    parent.color = "white"
                }
                onClicked: {
                    prog_dia_loader.openProgramSelectionDialog = true
                }
            }

            Item {
                id: stepEditButton
                width: 45
                height: parent.height
                anchors.right: parent.right
                visible: (commonViewModel.manipulateMode !== 2) && (progCommandclick != -1)
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/16_program/a160_47.png"
                }
                MouseArea {
                    anchors.fill: parent
                    pressAndHoldInterval: 2000
                    onPressAndHold: {
                        let step = local.selectedProgram.steps[progCommandclick]
                        oneLineEditDialogLoader.defaultText = step.command.toCode()
                        oneLineEditDialogLoader.lineText = ""
                        oneLineEditDialogLoader.open()
                    }
                    onPressed: {
                        if (!oneLineEditDialogLoader.active) {
                            oneLineEditDialogLoader.active = true
                        }
                    }
                }
                Loader {
                    id: oneLineEditDialogLoader
                    active: false
                    asynchronous: true
                    property string defaultText: ""
                    property string lineText: ""
                    property bool activeByRequest: false
                    function open() {
                        if (!active) {
                            activeByRequest = true
                            asynchronous = false
                            active = true
                        } else if (status === Loader.Ready) {
                            dialogOpen()
                        }
                    }
                    function dialogOpen() {
                        item.defaultText = defaultText
                        item.lineText = lineText
                        item.open()
                    }
                    onLoaded: {
                        item.textMaxLength = 1000
                        if (activeByRequest) {
                            dialogOpen()
                        }
                    }
                    source: "qrc:/qml/Widget/OneLineEditDialog.qml"
                    Connections {
                        target: oneLineEditDialogLoader.item
                        onOpened: {
                            commonViewModel.pageChange(Enums.TeachingProgram, 2);
                        }
                        onClosed: {
                            commonViewModel.pageChange(Enums.TeachingProgram, 0);
                        }
                        onEnter: {
                            local.replaceNcpCommandByCommandLine(newValue)
                        }
                    }
                }
            }// step edit button.
        }// selected program name area.

        ImageButton{
            anchors.verticalCenter: parent.verticalCenter
            upImageSource: expand ? resourceManager.imagePath + "/16_program/a160_07.png" : resourceManager.imagePath + "/16_program/a160_05.png"
            downImageSource: expand ? resourceManager.imagePath + "/16_program/a160_08.png" : resourceManager.imagePath + "/16_program/a160_06.png"
            onClicked: expand = !expand
        }// expand button.
    }//program No. display area

/**************************command display area**************************/
    Row{
        width: proglist.width
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 4
        anchors.topMargin: 55
        spacing: -1
        Rectangle{
            width: 45
            height: 34
            border.color: "#999999"
            color: "#cccccc"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 18
                color: resourceManager.rgb(41, 31)
                text: resourceManager.label(41,31, language.value)
            }
        }
        Rectangle{
            width: 55
            height: 34
            border.color: "#999999"
            color: "#cccccc"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 18
                color: resourceManager.rgb(41, 32)
                text: resourceManager.label(41,32, language.value)
            }
        }
        Rectangle{
            width: expand ? 859 : 410
            height: 34
            border.color: "#999999"
            color: "#cccccc"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 18
                color: resourceManager.rgb(41, 33)
                text: resourceManager.label(41,33, language.value)
            }
        }
    }//command title

    Component{
        id:listdelegate
        Row{
            visible: (index < local.limitNumOfSteps)
            width: proglist.width
            spacing: -1
            function isCurrentStep(index, clickedIndex) {
                return (clickedIndex === index)
            }

            Rectangle{
                id:step
                width: 45
                height: 75
                border.color: "#999999"
                color: isCurrentStep(index, progCommandclick)  ? "#cc3333" : "#cccccc"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 22
                    text: index+1
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (local.realTimeStep) { return }
                        // console.log("line No." + index)  // todo: debug.
                        progCommandclick = (progCommandclick == index) ? -1 : index
                        teachingviewModel.selectWizardMenuByStep(modelData)
                    }
                }
            }//line No.
            Rectangle{
                id:label
                width: 55
                height: 75
                border.color: "#999999"
                color: isCurrentStep(index, progLabelclick) ? "#cc3333" : "#ffffff"
                Text{
                    id:label_te
                    anchors.fill: parent
                    font.pixelSize: 20
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: (modelData.labelNo === -1) ? "" : "L" + ('000' + modelData.labelNo).slice(-3)
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (local.realTimeStep) { return }
                        progLabelclick = index
                        if (modelData.labelNo === -1) {
                            label_dia_loader.openLabelSelectionDialog = true
                        } else {
                            delete_dia.deleteLabelNo = modelData.labelNo
                            delete_dia.open()
                        }
                        teachingviewModel.selectWizardMenuByStep(modelData)
                    }
                }
            }//label No.
            Rectangle{
                id:command
                width: expand ? 859 : 410
                height: 75
                border.color: "#999999"
                color: isCurrentStep(index, progCommandclick)  ? "#cc3333" : "#ffffff"
                Rectangle{
                    width: 106
                    height: 75
                    color: isCurrentStep(index, progCommandclick)  ? "#cc3333" :"#ffffff"
                    border.color: "#999999"
                    Text {
                        function commandName(step, language, ncpStepListUpdate){
                            //return "[" + step.commandName() + "]"
                            return step.commandName()
                        }
                        anchors.fill: parent
                        wrapMode: Text.WordWrap
                        font.pixelSize: 20

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        maximumLineCount: 2
                        elide: Text.ElideRight
                        text: (modelData.command === null) ? "" : commandName(modelData, language.value, local.ncpStepListUpdate)
                    }
                }
                Text {
                    id: commandParameterText
                    function toTranslatedString(data, language, ncpStepListUpdate){
                        return teachingviewModel.toTranslatedString(data, language)
                    }
                    width: expand ? 769 : 320
                    height: parent.height
                    x:106
                    wrapMode: Text.WordWrap
                    font.pixelSize: 20

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 2
                    maximumLineCount: 2
                    elide: Text.ElideRight
                    text: (modelData.command === null) ? "" : toTranslatedString(modelData, language.value, local.ncpStepListUpdate)
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (local.realTimeStep) { return }
                        // console.log("line No." + index)  // todo: debug.
                        progCommandclick = (progCommandclick == index) ? -1 : index
                        if(progCommandclick == index){
                            teachingviewModel.selectWizardMenuByStep(modelData)
                        }
                    }
                }
            }//command.
        }
    }//step list component.

    ListView{
        id:proglist
        width: expand ? 956 : 508
        height: {
            const delegateHeigth = 75
            const numOfValidItems = Math.min(local.numOfSteps - (local.currentPage * local.numOfItemsParPage), local.numOfItemsParPage)
            return (delegateHeigth * numOfValidItems) + (spacing * (numOfValidItems - 1))
        }
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 4
        anchors.topMargin: 88
        property int pg: local.currentPage
        readonly property int maxpage: local.numOfPages
        property int pageIndex: (pg == maxpage-1 && pg !=0) ? (pg-1)*9 + (teachingviewModel.ncp.currentProgrows - pg*9) : pg*9

        spacing: -1
        clip: true
        interactive: false
        model: (local.selectedProgram === null) ? 0 : local.selectedProgram.steps
        delegate: listdelegate
    }// step list view.

    // Scrollbar that scrolls 10 line at a time
    Column{
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: expand ? 960 : 511
        anchors.topMargin: 55
        spacing: 446
        Rectangle{
            width: 53
            height: 53
            color: "transparent"
            Image {
                anchors.centerIn: parent
                source: up_mouse2.pressed ? resourceManager.imagePath + "/16_program/a160_40.png" : resourceManager.imagePath + "/16_program/a160_39.png"
            }
            MouseArea{
                id: up_mouse2
                anchors.fill: parent
                onClicked: local.prevPage()
            }
        }
        Rectangle{
            width: 53
            height: 53
            color: "transparent"
            Image {
                anchors.centerIn: parent
                source: down_mouse2.pressed ? resourceManager.imagePath + "/16_program/a160_42.png" : resourceManager.imagePath + "/16_program/a160_41.png"
            }
            MouseArea{
                id: down_mouse2
                anchors.fill: parent
                onClicked: local.nextPage()
            }
        }
    }
    // Scrollbar that scrolls 1 line at a time
    Column{
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: expand ? 960 : 511
        anchors.topMargin: 106
        spacing: 344
        Rectangle{
            width: 53
            height: 53
            color: "transparent"
            Image {
                anchors.centerIn: parent
                source: up_mouse1.pressed ? resourceManager.imagePath + "/16_program/a160_44.png" : resourceManager.imagePath + "/16_program/a160_43.png"
            }
            MouseArea{
                id: up_mouse1
                anchors.fill: parent
                onClicked: local.prevRow()
            }
        }
        Rectangle{
            width: 53
            height: 53
            color: "transparent"
            Image {
                anchors.centerIn: parent
                source: down_mouse1.pressed ? resourceManager.imagePath + "/16_program/a160_46.png" : resourceManager.imagePath + "/16_program/a160_45.png"
            }
            MouseArea{
                id: down_mouse1
                anchors.fill: parent
                onClicked: local.nextRow()
            }
        }
    }

    Rectangle{
        width: expand ? 1009 : 560
        height: 554
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 4
        anchors.topMargin: 55
        color: "transparent"
        border.width: 2
        border.color: "#999999"
        // boundary line for section separation
        Rectangle{
            width: 1
            height: 554
            anchors.right: parent.right
            anchors.rightMargin: 52
            color: "#999999"
        }
    }

    // auto run visibel 1cycle btn
    Row{
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 65
        anchors.topMargin: 2
        visible: expand && commonViewModel.manipulateMode === 2
        spacing: 5
        Rectangle{
            width: 130
            height: 50
            radius: 10
            border.color: "#999999"
            border.width: 1
            color: (cycle1_mosue.pressed || teachingviewModel.oneCycle) ? "#CC3333" : "#CCCCCC"
            Image {
                x : 10
                anchors.verticalCenter: parent.verticalCenter
                source: cycle1_mosue.pressed ? resourceManager.imagePath + "/16_program/a160_36.png" : resourceManager.imagePath + "/16_program/a160_35.png"
            }
            Text{
                x : 46
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 20
                font.bold: true
                color: cycle1_mosue.pressed ? "#FFFFFF" : "#999999"
                text: resourceManager.label(41,669, language.value);
            }
            MouseArea{
                id: cycle1_mosue
                anchors.fill: parent
                onPressed: commonViewModel.modbusMemblemSwControl(true, 241);
                onReleased: commonViewModel.modbusMemblemSwControl(false, 241);
            }
        }
        Rectangle{
            width: 130
            height: 50
            radius: 10
            border.color: "#999999"
            border.width: 1
            color: (cycle_mosue.pressed || teachingviewModel.cycle) ? "#CC3333" : "#CCCCCC"
            Image {
                x : 10
                anchors.verticalCenter: parent.verticalCenter
                source: cycle_mosue.pressed ? resourceManager.imagePath + "/16_program/a160_38.png" : resourceManager.imagePath + "/16_program/a160_37.png"
            }
            Text{
                x:46
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 20
                font.bold: true
                color: cycle_mosue.pressed ? "#FFFFFF" : "#999999"
                text: resourceManager.label(41,670, language.value);
            }
            MouseArea{
                id: cycle_mosue
                anchors.fill: parent
                onPressed: commonViewModel.modbusMemblemSwControl(true, 240);
                onReleased: commonViewModel.modbusMemblemSwControl(false, 240);
            }
        }
    }//1循环全循环

/******************************************right area****************************************/
    // side eexpand right area top button
Rectangle {
    width: 430
    height: 60
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.topMargin: 6
    anchors.rightMargin: 13
    color: "transparent"
    visible: !expand
    enabled: modify
    Row {
        spacing: 10
        width: parent.width
        height: 60
        anchors.right: parent.right
        // btn1
        Rectangle {
            width: 78
            height: 60
            radius: 5
            color: 1 === selectPrgBtnIndex ? "#CC3333" : "#CCCCCC"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                font.bold: true
                color: 1 === selectPrgBtnIndex ? "#FFFFFF" : "#666666"
                text: resourceManager.label(41, 8, language.value)
            }
            MouseArea{
                id : prg_io_btn
                anchors.fill: parent
                onClicked: {
                    teachingviewModel.selectMenu1()
                    selectPrgBtnIndex = 1
                }
            }
        }
        // btn2
        Rectangle {
            width: 78
            height: 60
            radius: 5
            color: 2 === selectPrgBtnIndex ? "#CC3333" : "#CCCCCC"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                font.bold: true
                color: 2 === selectPrgBtnIndex ? "#FFFFFF" : "#666666"
                text: resourceManager.label(41, 9, language.value)
            }
            MouseArea{
                id : prg_servo_btn
                anchors.fill: parent
                onClicked: {
                    teachingviewModel.selectMenu2()
                    selectPrgBtnIndex = 2
                }
            }
        }
        // btn 3
        Rectangle {
            width: 78
            height: 60
            radius: 5
            color: 3 === selectPrgBtnIndex ? "#CC3333" : "#CCCCCC"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                font.bold: true
                color: 3 === selectPrgBtnIndex ? "#FFFFFF" : "#666666"
                text : resourceManager.label(41,10, language.value)
            }
            MouseArea {
                id : prg_delay_btn
                anchors.fill: parent
                onClicked: {
                    teachingviewModel.selectMenu3()
                    selectPrgBtnIndex = 3
                }
            }
        }
        // btn 4
        Rectangle{
            width: 78
            height: 60
            radius: 5
            color: 4 === selectPrgBtnIndex ? "#CC3333" : "#CCCCCC"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                font.bold: true
                color: 4 === selectPrgBtnIndex ? "#FFFFFF" : "#666666"
                text: resourceManager.label(41,11, language.value)
            }
            MouseArea{
                id :  prg_process_btn
                anchors.fill: parent
                onClicked: {
                    teachingviewModel.selectMenu4()
                    selectPrgBtnIndex = 4
                }
            }
        }
        // btn 5
        Rectangle {
            width: 78
            height: 60
            radius: 5
            color: 5 === selectPrgBtnIndex ? "#CC3333" : "#CCCCCC"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 18
                font.bold: true
                color: 5 === selectPrgBtnIndex ? "#FFFFFF" : "#666666"
                text: resourceManager.label(41,12, language.value)
            }
            MouseArea{
                id : prg_sporder_btn
                anchors.fill: parent
                onClicked: {
                    teachingviewModel.selectMenu5()
                    selectPrgBtnIndex = 5
                }
            }
        }
    }



    Rectangle {
        anchors.fill: parent
        opacity: 0.3
        visible: commonViewModel.manipulateMode === 2 && !modify
        color: "#cccccc"
    }
}

//오른쪽 메뉴중 내용부분
    StackLayout {
        id:secondmenu
        width: 430
        height: 596
        x:575
        y:71
        clip: true
        visible: !expand
        enabled: modify
        currentIndex: local.wizardMenuIndex

        Loader {
            id:mainmenu1
            property bool isActive: pro.pageActive && (local.wizardMenuIndex == 0)
            active: false
            asynchronous: !isActive
            source: "qrc:/qml/Program/Program/MainMenu_1.qml"
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if(item != null){
                    item.pageActive = isActive;
                }
            }
        }
        Loader {
            id:mainmenu2
            property bool isActive: pro.pageActive && (local.wizardMenuIndex == 1)
            active: false
            asynchronous: !isActive
            source: "qrc:/qml/Program/Program/MainMenu_2.qml"
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if(item != null){
                    item.pageActive = isActive;
                }
            }
        }
        Loader{
            id:mainmenu3
            property bool isActive: pro.pageActive && (local.wizardMenuIndex == 2)
            active: false
            asynchronous: !isActive
            source: "qrc:/qml/Program/Program/MainMenu_3.qml"
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if(item != null){
                    item.pageActive = isActive;
                }
            }
        }
        Loader{
            id:mainmenu4
            property bool isActive: pro.pageActive && (local.wizardMenuIndex == 3)
            active: false
            asynchronous: !isActive
            source: "qrc:/qml/Program/Program/MainMenu_4.qml"
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if(item != null){
                    item.pageActive = isActive;
                }
            }
        }
        Loader {
            id:mainmenu5
            property bool isActive: pro.pageActive && (local.wizardMenuIndex == 4)
            active: false
            asynchronous: !isActive
            source: "qrc:/qml/Program/Program/MainMenu_5.qml"
            onLoaded: {
                item.pageActive = isActive;
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if(item != null){
                    item.pageActive = isActive;
                }
            }
        }
    }//2级菜单

    Rectangle{
        anchors.fill: secondmenu
        opacity: 0.3
        visible: commonViewModel.manipulateMode === 2 && !modify && !expand
        color: "#cccccc"
    }//auto display

/**************************************bottom menu area************************************/
    // bottom left button 1~8 background box
    Rectangle{
        width: 1016
        height: 47
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 4
        anchors.bottomMargin: 14
        border.color: "white"
        // Bottom left button 1~4
        Row{
            spacing: 10
            visible: commonViewModel.manipulateMode !== 2
            Repeater{
                model: 4
                Rectangle{
                    width: 116
                    height: 47
                    color: "transparent"
                    enabled: {
                        switch (index) {
                        case 0: return (progCommandclick !== -1)
                        case 1: return (progCommandclick !== -1)
                        case 2: return (progCommandclick !== -1)
                        case 3: return true
                        }
                    }
                    Image {
                        id: left_btn_bg_press
                        visible: mouse_leftbut.pressed
                        source: resourceManager.imagePath + "/16_program/a160_09.png"
                    }
                    Image {
                        id: left_btn_bg
                        visible: !mouse_leftbut.pressed
                        source: resourceManager.imagePath + "/16_program/a160_10.png"
                    }
                    Image {
                        id : left_btn_icon
                        x:6
                        anchors.verticalCenter: parent.verticalCenter
                        source:{
                            switch (index) {
                            case 0 :
                                return mouse_leftbut.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_12.png" :
                                            resourceManager.imagePath + "/16_program/a160_11.png" ;
                            case 1 :
                                return mouse_leftbut.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_14.png" :
                                            resourceManager.imagePath + "/16_program/a160_13.png" ;

                            case 2 :
                                return mouse_leftbut.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_16.png" :
                                            resourceManager.imagePath + "/16_program/a160_15.png" ;
                            case 3 :
                                return mouse_leftbut.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_18.png" :
                                            resourceManager.imagePath + "/16_program/a160_17.png" ;
                            }
                        }
                    }
                    Text{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 40
                        wrapMode: Text.WordWrap

                        font.pixelSize: 20
                        font.bold: true
                        // orign prog color resourceManager.rgb(41, 3~7)
                        color : {
                            switch (index) {
                            case 0 :
                                return mouse_leftbut.pressed ? "#FFFFFF" : "#666666" ;
                            case 1 :
                                return mouse_leftbut.pressed ? "#FFFFFF" : "#666666" ;
                            case 2 :
                                return mouse_leftbut.pressed ? "#FFFFFF" : "#666666" ;
                            case 3 :
                                return mouse_leftbut.pressed ? "#FFFFFF" : "#666666" ;
                            }
                        }

                        text: {
                            switch (index) {
                            case 0: return resourceManager.label(41,3, language.value);
                            case 1: return resourceManager.label(41,4, language.value);
                            case 2: return resourceManager.label(41,5, language.value);
                            case 3: return resourceManager.label(41,7, language.value);
                            }
                        }
                    }
                    MouseArea{
                        id:mouse_leftbut
                        anchors.fill: parent
                        onClicked: {
                            switch (index) {
                            case 0: local.copySelectedStep(); break;
                            case 1: local.pasteCopiedStep(); break;
                            case 2: deleteProg_dia.open(); break;
                            case 3: teachingviewModel.requestOpenSaveProgramDialog(false); break;
                            }
                        }
                    }
                }
            }
        }//left function.

        // bottom right button (Buttons that are only visible in automatic mode)
        Row{
            x:504
            spacing: 1
            Repeater{
                model: 3
                Rectangle{
                    width: 126
                    height: 47
                    color: "transparent"
                    visible: {
                        switch (index) {
                        case 0: return commonViewModel.manipulateMode === 2 && expand
                        case 1: return commonViewModel.manipulateMode === 2 && expand
                        case 2: return commonViewModel.manipulateMode === 2
                        }
                    }
                    Image {
                        id : crop_btn_bg_press
                        visible: mouse_rightbut.pressed
                        source: resourceManager.imagePath + "/16_program/a160_09.png"
                    }
                    Image {
                        id : crop_btn_bg
                        visible: !mouse_rightbut.pressed
                        source: resourceManager.imagePath + "/16_program/a160_10.png"
                    }

                    Image {
                        x:6
                        anchors.verticalCenter: parent.verticalCenter
                        source:{
                            switch (index) {
                            case 0 :
                                return mouse_rightbut.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_28.png" :
                                            resourceManager.imagePath + "/16_program/a160_27.png" ;
                            case 1 :
                                return mouse_rightbut.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_30.png" :
                                            resourceManager.imagePath + "/16_program/a160_29.png" ;
                            case 2 :
                                return modify ?
                                            (mouse_rightbut.pressed ?
                                                 resourceManager.imagePath + "/16_program/a160_32.png" :
                                                 resourceManager.imagePath + "/16_program/a160_31.png" ) :
                                            (mouse_rightbut.pressed ?
                                                 resourceManager.imagePath + "/16_program/a160_34.png" :
                                                 resourceManager.imagePath + "/16_program/a160_33.png") ;
                            }
                        }
                    }
                    Text{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 40
                        wrapMode: Text.WordWrap

                        font.pixelSize: 20
                        font.bold: true
                        // orign prog color resourceManager.rgb(41, 28~30)
                        color: {
                            switch (index) {
                            case 0 :
                                return mouse_rightbut.pressed ? "#FFFFFF" : "#666666" ;
                            case 1 :
                                return mouse_rightbut.pressed ? "#FFFFFF" : "#666666" ;
                            case 2 :
                                return mouse_rightbut.pressed ? "#FFFFFF" : "#666666" ;
                            case 3 :
                                return mouse_rightbut.pressed ? "#FFFFFF" : "#666666" ;
                            }
                        }

                        text: {
                            switch (index) {
                            case 0: return resourceManager.label(41,671, language.value);
                            case 1: return resourceManager.label(41,672, language.value);
                            case 2: return modify ? resourceManager.label(41,35, language.value) : resourceManager.label(41,34, language.value);
                            }
                        }
                    }
                    MouseArea{
                        id:mouse_rightbut
                        anchors.fill: parent
                        onPressed: {
                            switch (index) {
                            case 0:
                                if(teachingviewModel.step){
                                    commonViewModel.modbusMemblemSwControl(true, 243);
                                }else{
                                    commonViewModel.modbusMemblemSwControl(true, 244);
                                }
                                break;
                            case 1: commonViewModel.modbusMemblemSwControl(true, 245); break;
                            }
                        }
                        onReleased: {
                            switch (index) {
                            case 0:
                                if(teachingviewModel.step){
                                    commonViewModel.modbusMemblemSwControl(true, 243);
                                }else{
                                    commonViewModel.modbusMemblemSwControl(false, 244);
                                }
                                break;
                            case 1: commonViewModel.modbusMemblemSwControl(false, 245); break;
                            }
                        }
                        onClicked: {
                            switch (index) {
                            case 0:
                                break
                            case 1:
                                break
                            case 2: modify ? modify = 0 : modify = 1
                                local.realTimeStep = !local.realTimeStep
                                break;
                            }
                        }
                    }
                }
            }// real time button.(auto drive only.)

            // buttom right button 5~8
            Repeater{
                model: 4
                Rectangle{
                    width: 126
                    height: 47
                    color: "transparent"
                    visible: {
                        switch (index) {
                        case 0: return commonViewModel.manipulateMode !== 2
                        case 1: return commonViewModel.manipulateMode !== 2
                        case 2: return !expand
                        case 3: return !expand && commonViewModel.manipulateMode !== 2
                        }
                    }
                    enabled: {
                        switch (index) {
                        case 0: return ((progCommandclick !== -1) && (0 < progCommandclick))
                        case 1: return ((progCommandclick !== -1) && (progCommandclick < local.numOfSteps - 1))
                        case 2: return ((progCommandclick !== -1) || (commonViewModel.manipulateMode === 2))
                        case 3: return true
                        }
                    }
                    property bool selected: false
                    // ADD : Add code to change the button icon and background image By. IJ.YI 25.07.17
                    Image {
                        id: right_bg_press
                        visible: {
                            switch (index) {
                            case 0: return commonViewModel.manipulateMode !== 2
                            case 1: return commonViewModel.manipulateMode !== 2
                            case 2: return !expand
                            case 3: return !expand && commonViewModel.manipulateMode !== 2
                            default: return false
                            }
                        }
                        enabled: {
                            switch (index) {
                            case 0: return ((progCommandclick !== -1) && (0 < progCommandclick))
                            case 1: return ((progCommandclick !== -1) && (progCommandclick < local.numOfSteps - 1))
                            case 2: return ((progCommandclick !== -1) || (commonViewModel.manipulateMode === 2))
                            case 3: return true
                            default: return false
                            }
                        }
                        source: {
                            const isVisible = (() => {
                                switch (index) {
                                case 0: return commonViewModel.manipulateMode !== 2
                                case 1: return commonViewModel.manipulateMode !== 2
                                case 2: return !expand
                                case 3: return !expand && commonViewModel.manipulateMode !== 2
                                default: return false
                                }
                            })();
                            const isEnabled = (() => {
                                switch (index) {
                                case 0: return ((progCommandclick !== -1) && (0 < progCommandclick))
                                case 1: return ((progCommandclick !== -1) && (progCommandclick < local.numOfSteps - 1))
                                case 2: return ((progCommandclick !== -1) || (commonViewModel.manipulateMode === 2))
                                case 3: return true
                                default: return false
                                }
                            })();
                            if (isVisible && isEnabled && mouse_rightbut1.pressed) {
                                return resourceManager.imagePath + "/16_program/a160_09.png"
                            } else {
                                return resourceManager.imagePath + "/16_program/a160_10.png"
                            }
                        }
                    }

                    // bottom right icon image
                    Image {
                        x:6
                        z:1
                        anchors.verticalCenter: parent.verticalCenter
                        source:{
                            switch (index) {
                            case 0 :
                                return mouse_rightbut1.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_20.png" :
                                            resourceManager.imagePath + "/16_program/a160_19.png" ;
                            case 1 :
                                return mouse_rightbut1.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_22.png" :
                                            resourceManager.imagePath + "/16_program/a160_21.png" ;
                            case 2 :
                                return mouse_rightbut1.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_24.png" :
                                            resourceManager.imagePath + "/16_program/a160_23.png" ;
                            case 3 :
                                return mouse_rightbut1.pressed ?
                                            resourceManager.imagePath + "/16_program/a160_26.png" :
                                            resourceManager.imagePath + "/16_program/a160_25.png" ;
                            }
                        }
                    }
                    Text{
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 40
                        wrapMode: Text.WordWrap

                        font.pixelSize: mouse_rightbut1.pressed  ? 21 : 20
                        font.bold: true
                        // orign prog color resourceManager.rgb(41, 28~30,6)
                        color: {
                            switch (index) {
                            case 0 :
                                return mouse_rightbut1.pressed ? "#FFFFFF" : "#666666" ;
                            case 1 :
                                return mouse_rightbut1.pressed ? "#FFFFFF" : "#666666" ;
                            case 2 :
                                return mouse_rightbut1.pressed ? "#FFFFFF" : "#666666" ;
                            case 3 :
                                return mouse_rightbut1.pressed ? "#FFFFFF" : "#666666" ;
                            }
                        }

                        text: {
                            switch (index) {
                            case 0: return resourceManager.label(41,28, language.value);
                            case 1: return resourceManager.label(41,29, language.value);
                            case 2: return resourceManager.label(41,30, language.value);
                            case 3: return resourceManager.label(41,6, language.value);
                            }
                        }
                    }
                    MouseArea{
                        id:mouse_rightbut1
                        anchors.fill: parent
                        onClicked: {
                            switch (index) {
                            case 0: local.moveUpSelectedStep(); break;
                            case 1: local.moveDownSelectedStep(); break;
                            case 2:
                                local.updateModbusData()
                                if (!local.autoDrive) {
                                    local.replaceNcpCommandByWizard()
                                }
                                break
                            case 3:
                                local.updateModbusData()
                                if (!local.autoDrive) {
                                    local.insertNcpStepByWizard()
                                }
                                break
                            }
                        }
                    }
                }
            }// move down(0)/up(1), edit(2), insert(3).
        }// right function.

        // bottom auto state load prog no.
        Rectangle{
            width: 500
            height: 47
            radius: 5
            border.color: "white"
            color: "#CCCCCC"
            visible: (commonViewModel.manipulateMode == 2) || (phyKeyManager.selectSwMode == 1)
            Rectangle{
                width: parent.width-6
                height: 1
                anchors.centerIn: parent
                color: "white"
            }
            // program no.
            Text{
                x:8
                height: 23
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                color: "black"
                text: "Program No:"
            }
            Row {
                x: 100
                spacing: 15
                visible: local.dataLoaded
                Repeater {
                    model: teachingviewModel.runningProgramNos.length
                    Text {
                        height: 23
                        font.pixelSize: 20
                        verticalAlignment: Text.AlignVCenter
                        text: ('000' + teachingviewModel.runningProgramNos[index]).slice(-3)
                    }
                }
            }
            // step no.
            Text{
                x:8; y:24
                height: 23
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                color: "black"
                text: "Step No:"
            }
            Row{
                x:100; y:24
                height: 23
                spacing: 15
                visible: local.dataLoaded
                Repeater {
                    model: teachingviewModel.runningProgramSteps.length
                    Text {
                        height: 23
                        font.pixelSize: 20
                        verticalAlignment: Text.AlignVCenter
                        text: ('000' + teachingviewModel.runningProgramSteps[index]).slice(-3)
                    }
                }
            }
        }// running program no/step area.
    }//function button area.

/*******************************************popup dialog************************************/
    Loader {
        id: prog_dia_loader
        active: false
        asynchronous: true
        width: parent.width
        height: parent.height

        onLoaded: {
            if (!asynchronous) {
                item.open()
            }
        }

        property bool openProgramSelectionDialog: false
        readonly property bool menu421_openProgramSelectionDialog: menu421Model.openProgramSelectionDialog
        readonly property bool menu423_openProgramSelectionDialog: menu423Model.openProgramSelectionDialog

        onOpenProgramSelectionDialogChanged: {
            if (openProgramSelectionDialog) {
                asynchronous = false
                if (status === Loader.Ready) {
                    item.open()
                }
            }
        }

        onMenu421_openProgramSelectionDialogChanged: {
            openProgramSelectionDialog = menu421_openProgramSelectionDialog // notify onOpenProgramSelectionDialogChanged().
        }

        onMenu423_openProgramSelectionDialogChanged: {
            openProgramSelectionDialog = menu423_openProgramSelectionDialog // notify onOpenProgramSelectionDialogChanged().
        }

        sourceComponent: Prog_Dia {
            id: prog_select
            anchors.centerIn: parent
            modal: true
            isSelectionOnly: (menu421_openProgramSelectionDialog || menu423_openProgramSelectionDialog)
            onSelectProgram: {
                if (menu421_openProgramSelectionDialog) {
                    menu421Model.setProgramNo(selectProgram.no)
                } else if (menu423_openProgramSelectionDialog) {
                    menu423Model.setProgramNo(menu423Model.selectedIndex, selectProgram.no)
                } else if (openProgramSelectionDialog) {
                    // open Prog Dia directly.
                    local.selectedProgram = selectProgram
                }
            }
            onClosed: {
                openProgramSelectionDialog = false
                menu421Model.openProgramSelectionDialog = false
                menu423Model.openProgramSelectionDialog = false
            }
        }//程序选择窗口
    }

    Loader {
        id: label_dia_loader
        active: false
        asynchronous: true
        width: parent.width
        height: parent.height

        onLoaded: {
            if (!asynchronous) {
                item.open()
            }
        }

        property bool openLabelSelectionDialog: false
        readonly property bool menu422_openLabelSelectionDialog: menu422Model.openLabelSelectionDialog

        onOpenLabelSelectionDialogChanged: {
            if (openLabelSelectionDialog) {
                asynchronous = false
                if (status === Loader.Ready) {
                    item.isAllAssignedLabels = menu422_openLabelSelectionDialog
                    item.open()
                }
            }
        }

        onMenu422_openLabelSelectionDialogChanged: {
            openLabelSelectionDialog = menu422_openLabelSelectionDialog     // notify onOpenLabelSelectionDialogChanged().
        }

        sourceComponent: Label_Dia {
            id: label_select
            anchors.centerIn: parent
            modal: true
            selectedProgram: local.selectedProgram
            onSelectLabel: {
                if (menu422_openLabelSelectionDialog) {
                    menu422Model.setLabelNo(selectLabelNo)
                } else if (openLabelSelectionDialog) {
                    local.assignLabelNo(selectLabelNo)
                }
            }
            onClosed: {
                openLabelSelectionDialog = false
                menu422Model.openLabelSelectionDialog = false
                progLabelclick = -1
            }
        }
    }

    Delete_Dia{
        id:delete_dia
        anchors.centerIn: parent
        modal: true
        onSelectDetermine: {
            local.releaseLabel()
        }
        onClosed: {
            progLabelclick = -1
        }
    }//删除操作提示窗口

    Loader {
        id: variable_dia_loader
        active: false
        asynchronous: true
        width: parent.width
        height: parent.height

        onLoaded: {
            if (!asynchronous) {
                item.open()
            }
        }

        property int editIndex: -1
        readonly property int menu14_editIndex: menu1_4Model.editIndex
        readonly property int menu41_editIndex: menu41Model.editIndex
        readonly property int menu421_editIndex: menu421Model.editIndex
        readonly property int menu422_editIndex: menu422Model.editIndex
        readonly property int menu52_editIndex: pro.menu5_2Model.editIndex

        function enableAllMenuState() {
            for(let i = 0; i < 8; i++)
                variableViewmodel.menu_State[i] = true
            variableViewmodel.menuIndex = 0
        }

        function enableSpecificMenuStateOnly(enableIndex) {
            for(let i = 0; i < 8; i++)
                variableViewmodel.menu_State[i] = (i === enableIndex)
            variableViewmodel.menuIndex = enableIndex
        }

        onEditIndexChanged: {
            if(editIndex >= 0){
                asynchronous = false
                if (status === Loader.Ready) {
                    item.open()
                }
            }
        }

        onMenu14_editIndexChanged: {
            if(menu14_editIndex >= 0){
                enableSpecificMenuStateOnly(2)
                editIndex = menu14_editIndex    // notify onEditIndexChanged().
            }
        }

        onMenu41_editIndexChanged: {
            if(menu41_editIndex >= 0){
                enableAllMenuState()
                editIndex = menu41_editIndex    // notify onEditIndexChanged().
            }
        }

       onMenu421_editIndexChanged: {
           if(menu421_editIndex >= 0){
               enableAllMenuState()
               editIndex = menu421_editIndex    // notify onEditIndexChanged().
           }
       }

       onMenu422_editIndexChanged: {
           if(menu422_editIndex >= 0){
               enableAllMenuState()
               editIndex = menu422_editIndex    // notify onEditIndexChanged().
           }
       }

        onMenu52_editIndexChanged: {
            if(menu52_editIndex >= 0){
                enableSpecificMenuStateOnly(1)
                editIndex = menu52_editIndex    // notify onEditIndexChanged().
            }
        }

        sourceComponent: Variable_Dia{
            id:variable_dia
            x:5
            y:48
            modal: true
            onEnter: {
                switch (local.wizardMenuIndex) {
                case 0:
                    menu1_4Model.setCondition(editIndex, symbol, msg, value)
                    menu1_4Model.setVariableOk(editIndex)
                    break
                case 3:
                    if (menu4Model.menuIndex === 0) {
                        menu41Model.setCondition(editIndex, symbol, operator, value, addr)
                        menu41Model.setVariableOk(editIndex)
                    } else if (menu4Model.menuIndex === 1) {
                        if (menu42Model.menuIndex === 0) {
                            menu421Model.setCondition(editIndex, symbol, operator, value, addr)
                            menu421Model.setVariableOk(editIndex)
                        } else if (menu42Model.menuIndex === 1) {
                            menu422Model.setCondition(editIndex, symbol, operator, value, addr)
                            menu422Model.setVariableOk(editIndex)
                        }
                    }
                    break
                case 4:
                    menu5_2Model.setCondition(editIndex, symbol, operator, msg, value)
                    menu5_2Model.setVariableOk(editIndex)
                    break
                default: // case 1: case 2:
                    // nop.
                    break
                }
            }
            onClosed: {
                editIndex = -1
                menu1_4Model.editIndex = -1
                menu41Model.editIndex = -1
                menu421Model.editIndex = -1
                menu422Model.editIndex = -1
                menu5_2Model.editIndex = -1
            }
        }//变量选择窗口(4-1等待功能使用)
    }

    Dialog{
        id:deleteProg_dia
        width: 600
        height: 310
        anchors.centerIn: parent
        modal: true
        closePolicy: Popup.CloseOnEscape
        background: Rectangle{
            width: 600
            height: 310
            border.width: 2
            border.color: "#ffffff"
            color: "#ffffff"
            Rectangle{
                width: 600
                height: 55
                border.width: 2
                border.color: "#ffffff"
                color: "#999999"
                Row{
                    anchors.centerIn: parent
                    spacing: 5
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: resourceManager.imagePath + "/08_MENU/d082_11.png"
                    }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 24
                    font.bold: true
                    color: resourceManager.rgb(41, 474)
                    text: resourceManager.label(41, 474, language.value)
                }
                }
                ImageButton{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 15
                    scale: 1.5
                    upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
                    onClicked: deleteProg_dia.close()
                }
            }
        }
        Text{
            anchors.centerIn: parent
            font.pixelSize: 30
            color: resourceManager.rgb(41, 643)
            text: resourceManager.label(41, 643, language.value)
        }
        Rectangle{
            width: 110
            height: 44
            radius: 6
            x:460
            y:240
            border.width: 2
            border.color: "#ffffff"
            color: save_mouse.pressed ? "#cc3333" : "#cccccc"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: save_mouse.pressed ? 25 : 24
                color: resourceManager.rgb(41,292)
                text: resourceManager.label(41,292,language.value)
            }
            MouseArea{
                id:save_mouse
                anchors.fill: parent
                onClicked: {
                    local.removeSelectedStep()
                    deleteProg_dia.close()
                }
            }
        }//program selct
    }//화면 팝업창 삭제

    Dialog{
        id:saveProg_dia
        width: 600
        height: 310
        anchors.centerIn: parent
        modal : Qt. WindowModal
        closePolicy: Popup.CloseOnEscape
        onClosed: {
            teachingviewModel.requestCloseSaveProgramDialog()
        }
        background: Rectangle{
            width: 600
            height: 310
            border.width: 2
            border.color: "#ffffff"
            color: "#ffffff"
            Rectangle{
                width: 600
                height: 55
                border.width: 2
                border.color: "#ffffff"
                color: "#999999"
                Row{
                    anchors.centerIn: parent
                    spacing: 5
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: resourceManager.imagePath + "/08_MENU/d082_11.png"
                    }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 24
                    font.bold: true
                    color: resourceManager.rgb(41, 474)
                    text: resourceManager.label(41, 474, language.value)
                }
                }
                ImageButton{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
                    onClicked: saveProg_dia.close()
                }
            }
        }
        Text{
            anchors.centerIn: parent
            font.pixelSize: 30

            color: resourceManager.rgb(41,645)
            text: resourceManager.label(41,645,language.value)
        }

        Row{
            x:350
            y:240
            spacing: 6
            Rectangle{
                width: 110
                height: 44
                border.width: 2
                border.color: "#ffffff"
                color: calcel_mouse1.pressed ? "#cc3333" : "#cccccc"
                radius: 6
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: calcel_mouse1.pressed ? 25 : 24
                    color: resourceManager.rgb(41,293)
                    text: resourceManager.label(41,293,language.value)
                }
                MouseArea{
                    id:calcel_mouse1
                    anchors.fill: parent
                    onClicked: {
                        teachingviewModel.reasonClosingSaveProgramDialog = 0
                        saveProg_dia.close()
                    }
                }
            }
            Rectangle{
                width: 110
                height: 44
                border.width: 2
                border.color: "#ffffff"
                color: save_mouse1.pressed ? "#cc3333" : "#cccccc"
                radius: 6
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: save_mouse1.pressed ? 25 : 24
                    color: resourceManager.rgb(41,292)
                    text: resourceManager.label(41,292,language.value)
                }
                MouseArea{
                    id:save_mouse1
                    anchors.fill: parent
                    onClicked: {
                        teachingviewModel.reasonClosingSaveProgramDialog = 1
                        saveProg_dia.close()
                        local.processResult = teachingviewModel.saveNCPFile()
                    }
                }
            }//确定按钮
        }
    }//保存按钮弹出窗口

    ConfirmPopup{
        id:confirmPopup
        viewModel:pro.teachingviewModel
        onProcessingDialogOpen:{
            // NOP
        }
        onEnactFailDialogOpen:{
            // NOP
        }
        onDialogClose:{
            pro.isPopupDisp = false
        }
        onDetailBtnClick:{
            //console.log("Program confirmPopup onDetailBtnClick:")
            local.getCompileErrorStepInfo(pro.compileErrorStep)
        }
    }

    Loader {
        id: ncpFileLoadingDialog
        width: parent.width
        height: parent.height
        readonly property bool opened: (status === Loader.Ready) && item.opened
        function open() {
            asynchronous = false
            if (status === Loader.Ready) {
                item.open()
            }
        }
        function close() {
            if (status === Loader.Ready) {
                item.close()
            }
            asynchronous = true
        }
        active: pro.pageActive
        asynchronous: true
        visible: (status === Loader.Ready) && pro.pageActive
        onActiveChanged: {
            if (!active) {
                asynchronous = true
            }
        }
        onLoaded: {
            if (!asynchronous) {
                item.open()
            }
            asynchronous = true
        }
        sourceComponent: ConfirmPopup {
            text: resourceManager.label(22, 11, language.value)
            color: resourceManager.rgb(22, 11)
            isBtnVisible: false
            isDetailBtnVisible: false
            onDialogClose: {
                if (opened) {
                    pro.programClose()
                }
            }
        }
    }

/***********************************************************************************************/
    Connections {
        readonly property bool isCloseProgramWhenSaveCanceled: teachingviewModel.isCloseProgramWhenSaveCanceled
        readonly property int reasonClosingSaveProgramDialog: teachingviewModel.reasonClosingSaveProgramDialog
        target: teachingviewModel
        onOpenSaveProgramDialogChanged: {
            if (openSaveProgramDialog) {
                teachingviewModel.reasonClosingSaveProgramDialog = -1
                saveProg_dia.open()
            } else {
                if (reasonClosingSaveProgramDialog === 0) {
                    if (isCloseProgramWhenSaveCanceled) {
                        pro.programClose()
                        // teachingviewModel.restoreNcpFile()
                    }
                }
            }
        }
    }

    Connections {
        target:pro.teachingviewModel
        onCompileErrorStepChanged:{
            //console.log("Program onCompileErrorStepChanged compileErrorStep:"+compileErrorStep, "pageActive:"+pro.pageActive)
            if (pro.pageActive === true){
            }
        }
        onProgSaveStatusChanged:{
            //console.log("Program onProgSaveStatusChanged progSaveStatus:"+progSaveStatus, "pageActive:"+pro.pageActive)
            if (pro.pageActive === true){
                local.processPopup(progSaveStatus);
            }
        }
        onCompileStatusCodeChanged:{
            //console.log("Program onCompileStatusCodeChanged compileStatusCode:"+compileStatusCode, "pageActive:"+pro.pageActive)
            if (pro.pageActive === true){
                local.getCompileDetailInfo(compileStatusCode);
            }
        }
    }

    Connections {
        target: phyKeyManager
        enabled: pageActive && (1 < local.numOfPages) && !teachingviewModel.step
        onJogScrollUpChanged: local.prevRow()
        onJogScrollDownChanged: local.nextRow()
    }

    Connections {
        target: phyKeyManager
        enabled: pageActive && teachingviewModel.step && isInauto
        onJogScrollDownChanged: {
            commonViewModel.modbusMemblemSwControl(true, 245)
            commonViewModel.modbusMemblemSwControl(false, 245)
        }
    }

//    MouseArea {
//        anchors.fill: parent
//        enabled: pageActive && (1 < local.numOfPages)
//        propagateComposedEvents: true
//        onPressed: mouse.accepted = false
//        onReleased: mouse.accepted = false
//        onClicked: mouse.accepted = false
//        onWheel: {
//            if(!REAL_TARGET) {
//                if(wheel.angleDelta.y > 0) {
//                    //console.log("mouse wheel up")
//                    phyKeyManager.decrementEmulatedJogCount()
//                } else if(wheel.angleDelta.y < 0) {
//                    //console.log("mouse wheel down")
//                    phyKeyManager.incrementEmulatedJogCount()
//                }
//            }
//        }
//    }
//    Connections {
//        target: phyKeyManager
//        enabled: pageActive && (1 < local.numOfPages)
//        onJogScrollUpChanged: local.prevPage()
//        onJogScrollDownChanged: local.nextPage()
//    }
}
