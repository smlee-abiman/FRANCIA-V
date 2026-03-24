import QtQml 2.12
import QtQuick 2.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: File_1.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.23
* Worker: SH.HONG
**************************************************************************************/
Item {
    id: setupChgPage
    property int page: 1
    property bool pageActive: false
    property bool jogScrollOn:false
    property var buttonText:[
        resourceManager.label(51,10,local.language),
        resourceManager.label(51,11,local.language),
        resourceManager.label(51,13,local.language),
        resourceManager.label(51,14,local.language),
        resourceManager.label(51,12,local.language)
    ]
    property var buttonTextColor:[
        resourceManager.rgb(51,10),
        resourceManager.rgb(51,11),
        resourceManager.rgb(51,13),
        resourceManager.rgb(51,14),
        resourceManager.rgb(51,12)
    ]
    property int fileNumMax: setupChangeViewModel.getSetupMngFileNumMax()
    property var setupChgFileList:setupChangeViewModel.setupChangeViewFileList
    property int fileListNum:setupChangeViewModel.fileListNum
    property int totalPage:(setupChgPage.fileListNum / 7 === 0) ? 1 : (setupChgPage.fileListNum % 7 === 0) ? setupChgPage.fileListNum / 7 : (setupChgPage.fileListNum / 7) + 1
    property bool listSelect:false
    property string readConfirm:resourceManager.label(51,16,local.language)
    property string eraseConfirm:resourceManager.label(51,17,local.language)
    property string overWriteConfirm:resourceManager.label(51,23,local.language)
    property string compare1Confirm:resourceManager.label(51,24,local.language)
    property string compare2Confirm:resourceManager.label(51,25,local.language)
    property string numberStr:resourceManager.label(51,5,local.language)
    property string processingMsg:resourceManager.label(51,28,local.language)
    property string processFailMsg:resourceManager.label(51,29,local.language)
    property string processSuccessMsg:resourceManager.label(51,30,local.language)
    property string confirmFailMsg:resourceManager.label(51,45,local.language)
    property string fileNameIllegalMsg:resourceManager.label(51,46,local.language)
    property string underOrderSetUpDataMsg:resourceManager.label(51,53,local.language)
    property bool isAlreadyCmdDispSetting:false
    property var readCmdDispSetting:[]
    property var saveCmdDispSetting:[]
    property var eraseCmdDispSetting:[]
    property var writeCmdDispSetting:[]
    property var compareCmdDispSetting:[]
    property bool sortFileNumToggle:false               // 按数字顺序
    property bool sortFileNameToggle:false              // 按字母顺序
    property bool sortFileUpdateTimeToggle:false        // 最早/最新的文件修改日期

    property bool isUpdateList:true
    property bool isFtpFileAcquired:setupChangeViewModel.isFtpFileAcquired
    property string orderingMsg:resourceManager.label(22,11,local.language)
    property bool isManual: commonViewModel.manipulateMode === 1
    property bool isReqPagePrepare:false
    property bool isOrderingData:false
    property string commentText: ""
    property bool activeFromVtk:false
    property var pospageModel: teachingViewModel.pageModel[1]

    signal openVirtualKeyboard()

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

    function getCmdBtnDispSettings(){
        for(let i = 0; i < 5; i++){
            setupChgPage.readCmdDispSetting[i]       = setupChangeViewModel.getCmdButtonDispSetting(0, i)
            setupChgPage.saveCmdDispSetting[i]       = setupChangeViewModel.getCmdButtonDispSetting(1, i)
            setupChgPage.eraseCmdDispSetting[i]      = setupChangeViewModel.getCmdButtonDispSetting(2, i)
            setupChgPage.writeCmdDispSetting[i]      = setupChangeViewModel.getCmdButtonDispSetting(3, i)
            setupChgPage.compareCmdDispSetting[i]    = setupChangeViewModel.getCmdButtonDispSetting(4, i)
        }
    }

    function setCmdBtnDispSettings(){
        for(let i = 0; i < 5; i++){
            btnListRepeater.itemAt(i).visibleReadButton = ((setupChgPage.readCmdDispSetting[commonViewModel.userLevel] === 1) ? true : false)
            btnListRepeater.itemAt(i).visibleSaveButton = ((setupChgPage.saveCmdDispSetting[commonViewModel.userLevel] === 1) ? true : false)
            btnListRepeater.itemAt(i).visibleWriteButton = ((setupChgPage.writeCmdDispSetting[commonViewModel.userLevel] === 1) ? true : false)
            btnListRepeater.itemAt(i).visibleCompareButton = ((setupChgPage.compareCmdDispSetting[commonViewModel.userLevel] === 1) ? true : false)
            btnListRepeater.itemAt(i).visibleEraseButton = ((setupChgPage.eraseCmdDispSetting[commonViewModel.userLevel] === 1) ? true : false)
        }
    }
    function preparePageInit(){
        preparePageInitTimer.running = true;
        preparePageInitTimer.repeat = true;
    }
    function prepareSetupData(){
        prepareSetupDataTimer.running = true;
        prepareSetupDataTimer.repeat = true;
    }
    function getSetupData(){
        // msg open
        var msgStr = setupChgPage.underOrderSetUpDataMsg
        confirmPopup.text = msgStr
        confirmPopup.isBtnVisible = false
        confirmPopup.isCloseBtnVisible = false
        confirmPopup.open()
        // 段取換ファイル再取り寄せ
        setupChgPage.isOrderingData = true
        setupChangeViewModel.getFtpFileInfoAndData();
    }
    function openSaveDialog(){
        saveItem.commentText = setupChgPage.commentText
        saveItem.openItem()
        bgModal.visible = true
    }
// 파일 자동선택함수 260112 shhong
    function selectCurrentFileFromTopTab() {
        var targetName = commonViewModel.curFileName
        if (!targetName || setupChgPage.fileListNum <= 0)
            return

        for (var i = 0; i < setupChgPage.fileListNum; i++) {
            if (setupChgPage.setupChgFileList[i].fileSetupName === targetName) {

                // 1) 해당 파일이 있는 페이지로 이동
                setupChgPage.page = Math.floor(i / 7) + 1

                // 2) 페이지 이동/리스트 정렬 반영 후 선택 적용
                Qt.callLater(function() {
                    setupChgList.currentIndex = i % 7
                    setupChgPage.listSelect = true
                })

                return
            }
        }
    }
    //Component.onCompleted:{
    onPageActiveChanged: {
        console.debug("File_1 onPageActiveChanged pageActive:"+pageActive, "isReqPagePrepare:"+setupChgPage.isReqPagePrepare, "activeFromVtk:"+setupChgPage.activeFromVtk)
        if (pageActive) {
            if (setupChgPage.activeFromVtk === false){
                if(REAL_TARGET) {
                    if (setupChgPage.isReqPagePrepare === true){
                        setupChgPage.pageInitProcess()
                    } else {
                        setupChangeViewModel.activate()
                        setupChgPage.preparePageInit();
                    }
                } else {
                    setupChgPage.pageInitProcess()
                }
            } else {
                setupChgPage.activeFromVtk = false;
            }
        } else {
            //setupChangeViewModel.timerStop(0)
            setupChangeViewModel.deactivate();
        }
    }

    function pageInitProcess(){
        if (setupChgPage.isAlreadyCmdDispSetting === false){
            setupChgPage.getCmdBtnDispSettings()
            setupChgPage.isAlreadyCmdDispSetting = true
        }

        setupChgPage.setCmdBtnDispSettings()

        if(!REAL_TARGET) {
            setupChangeViewModel.startFtpCommunication()
        } else {
            if (externalMemoryViewModel.isNeedUpdateSetupData === true
                || setupChangeViewModel.isNeedUpdateSetupData === true){
                setupChgPage.prepareSetupData();
            } else {
                setupChangeViewModel.getFtpFileData();
            }
        }

        // ✅ 여기 추가 (리스트 준비 후 자동 선택)
        selectCurrentFileFromTopTab()
    }

    function orderingPopupProcess(){
        var msgStr = (setupChgPage.orderingMsg).toString()
        messagePopup.openOrderingPopup(msgStr, false, false)
    }

    Timer {
        id:preparePageInitTimer
        interval: 100
        running: false
        repeat: false
        onTriggered:{
            if (setupChgPage.isReqPagePrepare === true){
                preparePageInitTimer.running = false;
                preparePageInitTimer.repeat = false;
                setupChgPage.pageInitProcess()
            }
        }
    }

    Timer {
        id:prepareSetupDataTimer
        interval: 100
        running: false
        repeat: false
        onTriggered:{
            prepareSetupDataTimer.running = false;
            prepareSetupDataTimer.repeat = false;
            setupChgPage.getSetupData()
        }
    }

    //当前档案 현재 파일
    Row{
        anchors.right: parent.right
        y:-45
        anchors.rightMargin: 10
        height: 45
        spacing: 4
        Text{
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 20
            text: resourceManager.label(51,18, local.language)
            color: resourceManager.rgb(51,18)
        }
        Rectangle{
            width: 100
            height: 45
            color: "#cccccc"
            border.color: "#999999"
            Text{
                anchors.centerIn: parent
                font.pixelSize: 22
                text: (setupChgPage.isOrderingData === true) ? 0 : setupChgPage.fileListNum
            }
        }
    }

    // 타이틀바. 메뉴바.
Rectangle {
    x: 10
    y: -2
    width: 801
    height: 40    // 높이는 필요 시 조절
    color: "#999999"
    Text {
        x:27
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 22
        text: resourceManager.label(51,4, local.language)
        color: "white"
    }
    Text {
        id:fileNoTitle
        x:130
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 22
        text: resourceManager.label(51,5, local.language)
        color: "white"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if(setupChgPage.sortFileNumToggle === false){
                    setupChgPage.isUpdateList = false
                    setupChangeViewModel.sortFileNumber(1)
                    setupChgPage.sortFileNumToggle = true
                } else {
                    setupChgPage.isUpdateList = false
                    setupChangeViewModel.sortFileNumber(0)
                    setupChgPage.sortFileNumToggle = false
                }
            }
//            onPressed: {
//                fileNoTitle.color = setupChangeViewModel.getPushedFileContentDispColor()
//            }
//            onReleased: {
//                fileNoTitle.color = resourceManager.rgb(51,5)
//            }
        }
    }
    Text {
        id:fileTimeTitle
        x:290
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 22
        text: resourceManager.label(51,6, local.language)
        color: "white"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if(setupChgPage.sortFileUpdateTimeToggle === false){
                    setupChgPage.isUpdateList = false
                    setupChangeViewModel.sortFileUpdateTime(1)
                    setupChgPage.sortFileUpdateTimeToggle = true
                } else {
                    setupChgPage.isUpdateList = false
                    setupChangeViewModel.sortFileUpdateTime(0)
                    setupChgPage.sortFileUpdateTimeToggle = false
                }
            }
//            onPressed: {
//                fileTimeTitle.color = setupChangeViewModel.getPushedFileContentDispColor()
//            }
//            onReleased: {
//                fileTimeTitle.color = resourceManager.rgb(51,6)
//            }
        }
    }
    Text {
        id:fileCommentTitle
        x:530
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 22
        text: resourceManager.label(51,7, local.language)
        color: "white"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if(setupChgPage.sortFileNameToggle === false){
                    setupChgPage.isUpdateList = false
                    setupChangeViewModel.sortFileName(1)
                    setupChgPage.sortFileNameToggle = true
                } else {
                    setupChgPage.isUpdateList = false
                    setupChangeViewModel.sortFileName(0)
                    setupChgPage.sortFileNameToggle = false
                }
            }
//            onPressed: {
//                fileCommentTitle.color = setupChangeViewModel.getPushedFileContentDispColor()
//            }
//            onReleased: {
//                fileCommentTitle.color = resourceManager.rgb(51,7)
//            }
        }
    }
}

    Component{
        id:listdelegate
        Rectangle{
            width: 750
            height: 54
            border.color: "#898989"
            color: (setupChgList.currentIndex >= 0 && setupChgPage.listSelect === true) ?
                    ((setupChgList.currentIndex === index) ? "#cc3333" : (index%2 == 0) ? "#ffffff": "#cccccc") :
                     (index%2 == 0) ? "#ffffff": "#cccccc"

            Row{
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    id:fileAttrText
                    width: 100
                    height: 54
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 24
                    text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
                            (setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileAttribute === "2" ? resourceManager.label(51,8,local.language) : resourceManager.label(51,9,local.language))) : ""
                    color: (setupChgPage.listSelect === true)
                        ? ((index === setupChgList.currentIndex)
                            ? setupChangeViewModel.getSelectedFileContentDispColor()
                            : setupChangeViewModel.getFileContentDispColor())
                        : setupChangeViewModel.getFileContentDispColor()
                }
                Text{
                    id:fileNumText
                    width: 100
                    height: 54
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 24
                    text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
                            setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileNumber) : ""
                    color: (setupChgPage.listSelect === true)
                        ? ((index === setupChgList.currentIndex)
                            ? setupChangeViewModel.getSelectedFileContentDispColor()
                            : setupChangeViewModel.getFileContentDispColor())
                        : setupChangeViewModel.getFileContentDispColor()
                }
                Text{
                    id:fileUpdateTimeText
                    width: 230
                    height: 54
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 24
                    text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
                            setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileUpdateTime) : ""
                    color: (setupChgPage.listSelect === true)
                        ? ((index === setupChgList.currentIndex)
                            ? setupChangeViewModel.getSelectedFileContentDispColor()
                            : setupChangeViewModel.getFileContentDispColor())
                        : setupChangeViewModel.getFileContentDispColor()
                }
                Text{
                    id:fileSetupNameText
                    width: 320
                    height: 54
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 10
                    wrapMode: Text.Wrap
                    lineHeight: 0.8
                    font.pixelSize: lineCount >= 2 ? 22 : 24
                    text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
                            setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileSetupName) : ""
                    color: (setupChgPage.listSelect === true)
                        ? ((index === setupChgList.currentIndex)
                            ? setupChangeViewModel.getSelectedFileContentDispColor()
                            : setupChangeViewModel.getFileContentDispColor())
                        : setupChangeViewModel.getFileContentDispColor()
                }
            }

//            Text{
//                id:fileAttrText
//                x:30
//                anchors.verticalCenter: parent.verticalCenter
//                font.pixelSize: 24
//                //text:setupChgPage.setupChgFileList[index].fileAttribute
//                text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
//                        (setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileAttribute === "2" ? resourceManager.label(51,8,local.language) : resourceManager.label(51,9,local.language))) : ""
//                color: (setupChgPage.listSelect === true)
//                    ? ((index === setupChgList.currentIndex)
//                        ? setupChangeViewModel.getSelectedFileContentDispColor()
//                        : setupChangeViewModel.getFileContentDispColor())
//                    : setupChangeViewModel.getFileContentDispColor()
//            }
//            Text{
//                id:fileNumText
//                x:160
//                anchors.verticalCenter: parent.verticalCenter
//                font.pixelSize: 24
//                text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
//                        setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileNumber) : ""
//                color: (setupChgPage.listSelect === true)
//                    ? ((index === setupChgList.currentIndex)
//                        ? setupChangeViewModel.getSelectedFileContentDispColor()
//                        : setupChangeViewModel.getFileContentDispColor())
//                    : setupChangeViewModel.getFileContentDispColor()
//            }
//            Text{
//                id:fileUpdateTimeText
//                x:280
//                anchors.verticalCenter: parent.verticalCenter
//                font.pixelSize: 24
//                text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
//                        setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileUpdateTime) : ""
//                color: (setupChgPage.listSelect === true)
//                    ? ((index === setupChgList.currentIndex)
//                        ? setupChangeViewModel.getSelectedFileContentDispColor()
//                        : setupChangeViewModel.getFileContentDispColor())
//                    : setupChangeViewModel.getFileContentDispColor()
//            }
//            Text{
//                id:fileSetupNameText
//                x:550
//                anchors.verticalCenter: parent.verticalCenter
//                font.pixelSize: 24
//                text: (setupChgPage.isOrderingData === true) ? "" : (setupChgPage.isUpdateList === true) ? ((((setupChgPage.page - 1) * 7 + index) >= setupChgPage.fileListNum) ? "":
//                        setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + index].fileSetupName) : ""
//                color: (setupChgPage.listSelect === true)
//                    ? ((index === setupChgList.currentIndex)
//                        ? setupChangeViewModel.getSelectedFileContentDispColor()
//                        : setupChangeViewModel.getFileContentDispColor())
//                    : setupChangeViewModel.getFileContentDispColor()
//            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if (((setupChgPage.page - 1) * 7 + index) < setupChgPage.fileListNum){
                        setupChgPage.listSelect = true
                        setupChgList.currentIndex = index
                    }
                }
            }
        }
    }
    ListView{
        id:setupChgList
        property int pg: page
        x:10
        y:40 - ((pg >= setupChgPage.totalPage) ? 0 : 0)
        width: 750
        height: 420
        clip: true
        spacing: 6
        //model: setupChgPage.fileNumMax
        model: 7
        delegate: listdelegate
        interactive: false
        onPgChanged: {
            positionViewAtIndex((pg-1)*7,ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex((pg-1)*7,ListView.Beginning)
        }
        onCurrentItemChanged:{
            if (setupChgPage.listSelect === true){
                //console.log("currentIndex:"+setupChgList.currentIndex, "fileAttribute:"+setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileAttribute,
                //    "fileNumber:"+setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber,
                //    "fileUpdateTime:"+setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileUpdateTime,
                //    "fileSetupName:"+setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName)
            }
        }
    }

    //换页按钮
    Rectangle {
        x: 759
        y: 40
        width: 52
        height: 414
        color: "#eaeaea"
        border.color: "#999999"
    ImageScrollBar{
        id:setupChgPageScroll
        anchors.fill: parent
        anchors.margins: 5   // 상하좌우 5픽셀 여백
        jogScrollOn: setupChgPage.jogScrollOn
        topButtonPressImage: resourceManager.imagePath + "/28_USB/a280_09" + ".png"
        topButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_07" + ".png"
        botomButtonPressImage: resourceManager.imagePath + "/28_USB/a280_10" + ".png"
        botomButtonReleaseImage: resourceManager.imagePath + "/28_USB/a280_08" + ".png"
        pageLabal: page + "/" + setupChgPage.totalPage
        onScrollUpClicked: {
            if (setupChgPage.page > 1) {
                setupChgPage.page--
                setupChgPage.listSelect = false
            } else {
                setupChgPage.page = setupChgPage.totalPage
                setupChgPage.listSelect = false
            }
        }
        onScrollDownClicked: {
            if (setupChgPage.page >= setupChgPage.totalPage) {
                setupChgPage.page = 1
                setupChgPage.listSelect = false
            } else {
                setupChgPage.page++
                setupChgPage.listSelect = false
            }
        }
    }
    }
    //우측
    Rectangle{
        id:rightButton
        width: 164
        height: 414
        x:830
        y:40
        color: "#eaeaea"
        border.color: "#999999"
        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20  // 위에서 20픽셀 시작
            spacing: 20            // 버튼 간격 20픽셀
            Repeater{
                id: btnListRepeater
                model: 5
                delegate: ImageButton{
                    id:imgbtn
                    property bool visibleReadButton:true
                    property bool visibleSaveButton:true
                    property bool visibleWriteButton:true
                    property bool visibleCompareButton:true
                    property bool visibleEraseButton:true
                    enabled: isManual
                    visible:{
                        switch(index){
                        case 0:
                            return imgbtn.visibleReadButton
                        case 1:
                            return imgbtn.visibleSaveButton
                        case 2:
                            return imgbtn.visibleWriteButton
                        case 3:
                            return imgbtn.visibleCompareButton
                        case 4:
                            return imgbtn.visibleEraseButton
                        default:
                            return true;
                        }
                    }
                    upImageSource: resourceManager.imagePath + "/28_USB/a280_11.png"
                    downImageSource: resourceManager.imagePath + "/28_USB/a280_12.png"
                    fontPixelSize:18
                    text:setupChgPage.buttonText[index]
                    textColor: "#666666"
                    enableBtnTextColorChg:true
                    btnPressColor:setupChangeViewModel.getCommandPressColor()
                    onClicked:{
                        if (setupChgPage.listSelect === true){
                            switch(index){
                            case 0://读取
                                confirmPopup.buttonIndex = index
                                var msgStr = (setupChgPage.readConfirm).toString()
                                    .replace("[no]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber)
                                    .replace("[file]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName);
                                confirmPopup.text = msgStr
                                confirmPopup.isBtnVisible = true
                                confirmPopup.listIndex = (setupChgPage.page - 1) * 7 + setupChgList.currentIndex
                                confirmPopup.isCloseBtnVisible = true
                                confirmPopup.open()
                                break
                            case 1://另存
                                saveItem.open(index, setupChangeViewModel.searchFreeFileNumber("0"), setupChgPage.buttonText[index])
                                setupChgPage.activeFromVtk = true
                                bgModal.visible = true
                                break
                            case 2://覆盖
                                confirmPopup.buttonIndex = index
                                var msgStr = (setupChgPage.overWriteConfirm).toString()
                                    .replace("[no]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber)
                                    .replace("[file]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName);
                                confirmPopup.text = msgStr
                                confirmPopup.isBtnVisible = true
                                confirmPopup.listIndex = (setupChgPage.page - 1) * 7 + setupChgList.currentIndex
                                confirmPopup.isCloseBtnVisible = true
                                confirmPopup.open()
                                break
                            case 3://比较
                                confirmPopup.buttonIndex = index
                                var msgStr = (setupChgPage.compare2Confirm).toString()
                                    .replace("[no]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber)
                                    .replace("[file]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName);
                                confirmPopup.text = msgStr
                                confirmPopup.isBtnVisible = true
                                confirmPopup.listIndex = (setupChgPage.page - 1) * 7 + setupChgList.currentIndex
                                confirmPopup.isCloseBtnVisible = true
                                confirmPopup.open()
                                break
                            case 4://删除
                                confirmPopup.buttonIndex = index
                                var msgStr = (setupChgPage.eraseConfirm).toString()
                                    .replace("[no]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber)
                                    .replace("[file]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName);
                                confirmPopup.text = msgStr
                                confirmPopup.isBtnVisible = true
                                confirmPopup.listIndex = (setupChgPage.page - 1) * 7 + setupChgList.currentIndex
                                confirmPopup.isCloseBtnVisible = true
                                confirmPopup.open()
                                break
                            default:
                                break
                            }
                        } else {
                            switch(index){
                            case 1:
                                saveItem.open(index, setupChangeViewModel.searchFreeFileNumber("0"), setupChgPage.buttonText[index])
                                setupChgPage.activeFromVtk = true
                                bgModal.visible = true
                                break
                            default:
                                break
                            }
                        }
                    }
                }
            }
        }
    }

    ComparePopup{
        id: compare_popup
        anchors.centerIn: parent
        viewModel:setupChangeViewModel
    }

    FileConfirmPopup{
        id:confirmPopup
        viewModel:setupChangeViewModel
        tabIndex:0
        onProcessingDialogOpen:{
            if (setupChgPage.pageActive === true){
                console.debug("confirmPopup onProcessingDialogOpen currentIndex:"+setupChgList.currentIndex, "btnIndex:"+btnIndex, "buttonText:"+setupChgPage.buttonText[btnIndex])
                var msgStr = (setupChgPage.processingMsg).toString()
                    .replace("[no]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber)
                    .replace("[file]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName)
                    .replace("[cmd]", setupChgPage.buttonText[btnIndex]);
                processingPopup.text = msgStr
                processingPopup.isBtnVisible = false
                processingPopup.buttonIndex = btnIndex
                processingPopup.isCloseBtnVisible = true
                processingPopup.open()
            }
        }
        onDialogClose:{
        }
        onReOrderSetupDataListFinishNotify:{
            console.debug("confirmPopup onReOrderSetupDataListFinishNotify isReOrderSetupDataListFinish:"+isReOrderSetupDataListFinish)
            if (isReOrderSetupDataListFinish === true){
                setupChangeViewModel.getFtpFileData(true);
            }
        }
    }

    BackGroundModal{
        id:bgModal
        visible:false
    }

    Loader {
        id:saveItem
        anchors.fill: parent
        property bool activeByRequest: false
        property string text: ""
        property string freeNumber: ""
        property int buttonIndex:-1
        property string commentText:""
        function open(buttonIndex, freeNumber, text){
            saveItem.buttonIndex = buttonIndex
            saveItem.freeNumber = freeNumber
            saveItem.text = text
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openItem()
            }
        }
        function openItem() {
            item.buttonIndex = saveItem.buttonIndex
            item.freeNumber = saveItem.freeNumber
            item.text = saveItem.text
            item.commentText = saveItem.commentText
            item.visible = true
        }
        function closeItem() {
            if (active){
                item.visible = false
                item.commentText = ""
                bgModal.visible = false;
            }
        }
        active: false
        asynchronous: true
        sourceComponent: FileSave {
            viewModel:setupChangeViewModel
            onItemClose:{
                if (setupChgPage.pageActive === true){
                    item.visible = false
                    item.commentText = ""
                    bgModal.visible = false;
                }
            }
            onProcessingSaveItemOpen:{
                if (setupChgPage.pageActive === true){
                    console.debug("saveItem onProcessingSaveItemOpen btnIndex:"+btnIndex, "buttonText:"+setupChgPage.buttonText[btnIndex], "fileNumber:"+fileNumber, "fileName:"+fileName)
                    var msgStr = (setupChgPage.processingMsg).toString()
                        .replace("[no]", fileNumber)
                        .replace("[file]", fileName)
                        .replace("[cmd]", setupChgPage.buttonText[btnIndex]);
                    processingPopup.text = msgStr
                    processingPopup.isBtnVisible = false
                    processingPopup.buttonIndex = btnIndex
                    processingPopup.saveFileNumber = fileNumber
                    processingPopup.saveFileName = fileName
                    processingPopup.isCloseBtnVisible = true
                    processingPopup.open()
                }
            }
            onEnactFailItemOpen:{
                if (setupChgPage.pageActive === true){
                    console.debug("saveItem onEnactFailItemOpen btnIndex:"+btnIndex, "buttonText:"+setupChgPage.buttonText[btnIndex], "fileNumber:"+fileNumber, "fileName:"+fileName, "errCode:"+errCode)
                    var msgStr = (setupChgPage.processFailMsg).toString()
                        .replace("[no]", fileNumber)
                        .replace("[file]", fileName)
                        .replace("[cmd]", setupChgPage.buttonText[btnIndex])
                        .replace("[error_no]", errCode);
                    messagePopup.openProcessFinishPopup(msgStr, false, btnIndex, true)
                }
            }
            onConfirmFailItemOpen:{
                if (setupChgPage.pageActive === true){
                    console.debug("saveItem onConfirmFailDialogOpen btnVisible:"+btnVisible, "fileNumber:"+fileNumber)
                    //var msgStr = (setupChgPage.confirmFailMsg).toString()
                    //    .replace("[no]", fileNumber);
                    var msgStr = (setupChgPage.confirmFailMsg).toString();
                    messagePopup.openCheckFailPopup(msgStr, btnVisible, true)
                }
            }
            onCautionFileNameIllegalItemOpen:{
                if (setupChgPage.pageActive === true){
                    console.debug("saveItem onCautionFileNameIllegalDialogOpen btnIndex:"+btnIndex, "buttonText:"+setupChgPage.buttonText[btnIndex], "fileName:"+fileName)
                    var msgStr = (setupChgPage.fileNameIllegalMsg).toString()
                    messagePopup.openProcessFinishPopup(msgStr, false, btnIndex, true)
                }
            }
            onOpenVirtualKeyboard:{
                if (setupChgPage.pageActive === true){
                    item.visible = false
                    setupChgPage.openVirtualKeyboard()
                }
            }
        }
        onLoaded: {
            if (activeByRequest) {
                openItem()
                activeByRequest = false
            }
        }
    }

    FileConfirmPopup{
        id:processingPopup
        viewModel:setupChangeViewModel
        tabIndex:0
        onEnactSuccessDialogOpen:{
            if (setupChgPage.pageActive === true){
                console.debug("processingPopup onEnactSuccessDialogOpen currentIndex:"+setupChgList.currentIndex, "btnIndex:"+btnIndex, "buttonText:"+setupChgPage.buttonText[btnIndex])
                console.debug("processingPopup onEnactSuccessDialogOpen fileNumber:"+fileNumber, "fileName:"+fileName)
                if (btnIndex === 0){
                    pospageModel.fileSwitchUpdatePoint()
                }
                if (btnIndex === 1){
                    var msgStr = (setupChgPage.processSuccessMsg).toString()
                        .replace("[no]", fileNumber)
                        .replace("[file]", fileName)
                        .replace("[cmd]", setupChgPage.buttonText[btnIndex]);
                    messagePopup.openProcessFinishPopup(msgStr, false, btnIndex, true)
                } else {
                    var msgStr = (setupChgPage.processSuccessMsg).toString()
                        .replace("[no]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber)
                        .replace("[file]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName)
                        .replace("[cmd]", setupChgPage.buttonText[btnIndex]);
                    messagePopup.openProcessFinishPopup(msgStr, false, btnIndex, true)
                }
            }
        }
        onEnactFailDialogOpen:{
            if (setupChgPage.pageActive === true){
                console.debug("processingPopup onEnactSuccessDialogOpen currentIndex:"+setupChgList.currentIndex, "btnIndex:"+btnIndex, "buttonText:"+setupChgPage.buttonText[btnIndex])
                console.debug("processingPopup onEnactSuccessDialogOpen fileNumber:"+fileNumber, "fileName:"+fileName, "errCode:"+errCode)
                if (btnIndex === 1){
                    var msgStr = (setupChgPage.processFailMsg).toString()
                        .replace("[no]", fileNumber)
                        .replace("[file]", fileName)
                        .replace("[cmd]", setupChgPage.buttonText[btnIndex])
                        .replace("[error_no]", errCode);
                    messagePopup.openProcessFinishPopup(msgStr, false, btnIndex, true)
                } else {
                    var msgStr = (setupChgPage.processFailMsg).toString()
                        .replace("[no]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber)
                        .replace("[file]", setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName)
                        .replace("[cmd]", setupChgPage.buttonText[btnIndex])
                        .replace("[error_no]", errCode);
                    messagePopup.openProcessFinishPopup(msgStr, false, btnIndex, true)
                }
            }
        }
        onDialogClose:{
            if (setupChgPage.pageActive === true){
                saveItem.closeItem();
            }
        }
        onCompareDiscordFailDialogOpen:{
            if (setupChgPage.pageActive === true){
                console.debug("processingPopup onCompareDiscordFailDialogOpen factors:"+factors)
                compare_popup.fileNumber = setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileNumber
                compare_popup.fileName = setupChgPage.setupChgFileList[(setupChgPage.page - 1) * 7 + setupChgList.currentIndex].fileSetupName
                if (factors <= 0){
                    compare_popup.compareResult = true
                } else {
                    compare_popup.compareResult = false
                }
                compare_popup.open();
            }
        }
    }

    Item {
        id: messagePopup
        anchors.centerIn: parent
        function _openPopup(stateName, msgStr, btnVisible, btnIndex, closeBtnVisible) {
            if (_inner_confirmPopup.opened) {
                // _inner_confirmPopup.dialogClose()    // todo: need to call ?
                _inner_confirmPopup.close()
            }
            state = stateName
            _inner_confirmPopup.text = msgStr
            _inner_confirmPopup.isBtnVisible = btnVisible
            _inner_confirmPopup.buttonIndex = btnIndex
            _inner_confirmPopup.isCloseBtnVisible = closeBtnVisible
            _inner_confirmPopup.open()
        }
        function openProcessFinishPopup(msgStr, btnVisible, btnIndex, closeBtnVisible) {
            _openPopup("processFinishPopup", msgStr, btnVisible, btnIndex, closeBtnVisible)
        }
        function openCheckFailPopup(msgStr, btnVisible, closeBtnVisible) {
            _openPopup("checkFailPopup", msgStr, btnVisible, -1, closeBtnVisible)
        }
        function openOrderingPopup(msgStr, btnVisible, closeBtnVisible) {
            _openPopup("orderingPopup", msgStr, btnVisible, -1, closeBtnVisible)
        }
        FileConfirmPopup {
            id: _inner_confirmPopup
            tabIndex: 0
            viewModel: setupChangeViewModel
            onDialogClose: {
                saveItem.closeItem();
            }
            // note: The behavior of FileConfirmPopup is set by messageDialog.states.
        }
        states: [
            State {
                name: "processFinishPopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose:{
                        console.debug("processFinishPopup onDialogClose pageActive:"+setupChgPage.pageActive)
                        if (setupChgPage.pageActive === true){
                            setupChgPage.listSelect = false
                            setupChgPage.commentText = "";
                        }
                    }
                }
            },
            State {
                name: "checkFailPopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose:{
                        console.debug("CheckFailPopup onDialogClose()")
                        setupChgPage.commentText = "";
                    }
                }
            },
            State {
                name: "orderingPopup"
                PropertyChanges {
                    target: _inner_confirmPopup
                    onDialogClose: {
                        console.debug("orderingPopup onDialogClose pageActive:"+setupChgPage.pageActive)
                        if (setupChgPage.pageActive === true){
                            setupChgPage.isOrderingData = false;
                            setupChgPage.commentText = "";
                        }
                    }
                }
            }
        ]
    }

/***********************************************************************/
    Connections {
        target:commonViewModel
        onUserLevelChanged:{
            console.debug("onUserLevelChanged userlevel:"+userLevel)
            setupChgPage.setCmdBtnDispSettings()
        }
    }

    Connections {
        target:setupChangeViewModel
        onFileListNumChanged:{
            console.debug("File_1 setupChangeViewModel pageActive:"+setupChgPage.pageActive, "fileListNum:"+fileListNum, "page:"+setupChgPage.page)
            if (setupChgPage.pageActive === true){
                if ((fileListNum % 7) === 0 && setupChgPage.page > (fileListNum / 7)){
                    setupChgPageScroll.scrollUpClicked();
                }
            }
        }
        onUpdateListChanged:{
            console.debug("File_1 onUpdateListChanged updateList:"+updateList, "pageActive:"+setupChgPage.pageActive)
            if (setupChgPage.pageActive === true){
                setupChgPage.isUpdateList = updateList;
            }
        }
        onIsFtpFileAcquiredChanged:{
            console.debug("File_1 onIsFtpFileAcquiredChanged isFtpFileAcquired:"+isFtpFileAcquired, "pageActive:"+setupChgPage.pageActive, "isReqPagePrepare:"+setupChgPage.isReqPagePrepare)
            if(setupChgPage.pageActive === true){
                if (isFtpFileAcquired === true){
                    setupChgPage.isReqPagePrepare = true
                    setupChgPage.isOrderingData = false
                    selectCurrentFileFromTopTab()   // ✅ 이거 추가 필요
                } else {
                    setupChgPage.orderingPopupProcess()
                }
            }
        }
    }
//    MouseArea {
//        anchors.fill: parent
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
}
