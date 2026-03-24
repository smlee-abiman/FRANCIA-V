import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import Enums 1.0
import AlarmInfo 1.0
import "../../Widget"
import "../../Widget/ComboBox"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Alarm_Dialog.qml
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
Dialog  {
    id:alm_dialog
    modal: false
    width: 642
    height: 370
    closePolicy:Popup.NoAutoClose

    signal dialogClose()
    property int alarmCount:commonViewModel.curAlarmCount
    property bool jogScrollOn:false
    property int curPage : 1
    property int totalPages : (alm_dialog.alarmCount <= numItemsPerPage) ? 1 : ((alm_dialog.alarmCount % numItemsPerPage) === 0) ? alm_dialog.alarmCount / numItemsPerPage : alm_dialog.alarmCount / numItemsPerPage + 1
    property int listItemCount:commonViewModel.curAlarmCount
    property var alarmList:commonViewModel.alarmList
    property var isDetail:[false, false, false, false, false, false, false, false, false, false]
    readonly property bool jogDialSwOpe: (commonViewModel.topPopupDialog === Enums.PopupDialogStatusAlarm)
    property int stepVal: 30
    property bool isJogDialSwOpe:false
    property int detailItemIndex: -1
    readonly property int numItemsPerPage: 6
    property int page: 0

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

    background: Rectangle{
        width: 642
        height: 370
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 642
            height: 55
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Row{
                anchors.centerIn: parent
                spacing: 10
                Image {
                    x:200
                    y:2
                    source: resourceManager.imagePath + "/08_MENU/d084_02.png"
                }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 26
                    font.weight: Font.Bold
                    font.bold: true
                    color: (resourceManager.Alm_type === 1) ? resourceManager.rgb(52,4) : resourceManager.rgb(52,0)
                    text: (resourceManager.Alm_type === 1) ? resourceManager.label(52,4,local.language) : resourceManager.label(52,0,local.language)
                }
            }
            ImageButton{
                id:error_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    alm_dialog.dialogClose()
                    alm_dialog.close()
                    phyKeyManager.ctrlBuzzer()
                }
            }//关闭弹窗
        }//标题栏
    }

    //标题栏
Rectangle {
    x: -5
    y: 46
    width: 578
    height: 40
    border.width: 1
    border.color: "#999999"
    color: "#cccccc"  //

    // 텍스트 1
    Text {
        x: 0
        y: 6
        width: 70
        height: 28
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        text: resourceManager.label(52, 1, local.language)
    }

    // 텍스트 2
    Text {
        x: 82
        y: 6
        width: 60
        height: 28
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        text: resourceManager.label(52, 2, local.language)
    }

    // 텍스트 3
    Text {
        x: 275
        y: 6
        width: 130
        height: 28
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        text: resourceManager.label(52, 3, local.language)
    }
}
    ListView{
        id:listview
        width: 580
        height: 260
        y: 90
        x: -5
        property int pg: page
        function calcTopItemIndex(index){
            let pageTopItemIndex = pg * numItemsPerPage
            let itemIndex = index % numItemsPerPage
            let ret = 0
            if (itemIndex <= 1) {
                ret = 0
            } else {
                ret = itemIndex - 1
            }
            ret += pageTopItemIndex
            return ret
        }
        function updateDetailItemStatus(index){
            let topItemIndex
            alm_dialog.isJogDialSwOpe = false
            if (detailItemIndex === index){
                closeDetailItem(index)
            } else {
                topItemIndex = calcTopItemIndex(index)
                listview.positionViewAtIndex(topItemIndex, ListView.Beginning)
                listview.currentIndex = topItemIndex
                detailItemIndex = index
            }
        }
        function closeDetailItem(index){
            let topItemIndex = page * numItemsPerPage
            positionViewAtIndex(topItemIndex, ListView.Beginning)
            currentIndex = topItemIndex;
            detailItemIndex = -1
        }
        spacing: 4
        clip: true
        cacheBuffer:1000
        model: listModelData

        ListModel{
            id:listModelData
            Component.onCompleted: {
                for(var i = 0; i < alm_dialog.alarmCount; i++){
                    listModelData.append({
                                        type:alm_dialog.alarmList[i].type,
                                             NO:alm_dialog.alarmList[i].alarmNumber,
                                             factsColor:alm_dialog.alarmList[i].alarmDetailsColor,
                                        alarmIndex: alm_dialog.alarmList[i].alarmIndex,
                                             valid: true
                                         });
                }
                for (i = 0; i < 14 - alm_dialog.alarmCount; i++) {
                    //console.debug("listModelData.append empty", i)
                    listModelData.append({
                                        type: AlarmInfo.Unknown,
                                             NO:"",
                                             factsColor:"",
                                        alarmIndex: 0,
                                             valid: false
                                         });
                }
            }
        }
        delegate: listdelegate
        interactive: false
        Component.onCompleted: {
            positionViewAtIndex(pg*numItemsPerPage,ListView.Beginning)
            currentIndex = pg*numItemsPerPage
        }
    }
    Component{
        id:listdelegate
        Item{
            id:wrapper
            width: listview.width
            height: 40
            visible: valid
            state: detailItemIndex === index ? "expanded" : ""
            Rectangle{
                id:type1
                width: 80
                height: 40
                color: "#cc3333"
                border.color: "#999999"
                Text{
                    function getTypeString(type, language){
                        let ret = ""
                        if ((type >= AlarmInfo.Axis1) && (type <= AlarmInfo.Axis8)) {
                            ret = resourceManager.label(22, type, language);
                        } else if (type === AlarmInfo.System) {
                            ret = resourceManager.label(50, 10, language)
                        } else if (type === AlarmInfo.Normal) {
                            ret = resourceManager.label(52, 4, language)
                        }
                        return ret
                    }

                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    text: getTypeString(type, local.language)
                }
            }
            Rectangle{
                id:no
                width: 60
                height: 40
                x:84
                color: "#cccccc"
                border.color: "#999999"

                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18
                    text: NO
                    color: "black"
                }
            }
            Rectangle{
                id:descript
                width: 430
                height: 40
                x:148
                color: "#ffffff"
                border.color: "#999999"

                Image{
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 5
                    scale:0.8
                    source: factview.opacity != 0 ? resourceManager.imagePath + "/36_DIALOG/a360_01.png" : resourceManager.imagePath + "/36_DIALOG/a360_02.png"
                }

                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4
                    rightPadding: 30
                    wrapMode: Text.WordWrap
                    font.pixelSize: 16
                    text: resourceManager.getMessageStringValue("12", alarmIndex - 1, local.language)
                    color: "black"
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: parent.color = "#cc3333"
                    onReleased: parent.color = "#ffffff"
                    onClicked: {
                        listview.updateDetailItemStatus(index)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
            Rectangle{
                id:factview
                width: 578
                height: 174
                anchors.top: type1.bottom
                anchors.topMargin: 2
                color: "#ffffff"
                border.color: "#999999"
                opacity: 0
                PropertyAnimation {
                    id: scrollAnimation
                    target: scrollView.contentItem
                    properties: "contentY"
                    duration: 250
                }
                ScrollView {
                    id: scrollView
                    y: 1
                    width: 550
                    height: 174
                    clip: true
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOff
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    TextArea {
                        id: factsDetailArea
                        width: 527
                        enabled: false
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: 16
                        color: factsColor
                        selectedTextColor: factsColor
                        text: resourceManager.getMessageStringValue("12", alarmIndex, local.language)
                        onEditingFinished: {
                        }
                    }
                }
                ImageButton{
                    id: upBtn
                    anchors.right: parent.right
                    y:4
                    scale: 0.6
                    upImageSource: resourceManager.imagePath + "/36_DIALOG/a360_03.png"
                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_03.png"
                    onClicked: {
                        click();
                    }
                    function click(){
                        //console.log("upBtn contentY:"+scrollView.contentItem.contentY, "stepVal:"+stepVal)
                        if(scrollView.contentItem.contentY > 0 ){
                            if(scrollView.contentItem.contentY - stepVal < 0){
                                scrollAnimation.to = 0;
                            }
                            else{
                                scrollAnimation.to = scrollView.contentItem.contentY - stepVal;
                            }
                            //console.log("upBtn scrollAnimation.to:"+scrollAnimation.to)
                            scrollAnimation.start();
                        }
                    }
                }
                ImageButton{
                    id: downBtn
                    anchors.right: parent.right
                    y:130
                    scale:0.6
                    upImageSource: resourceManager.imagePath + "/36_DIALOG/a360_04.png"
                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_04.png"
                    onClicked: {
                        click();
                    }
                    function click(){
                        //console.log("downBtn contentItem.height:"+scrollView.contentItem.height,"contentY:"+scrollView.contentItem.contentY,"stepVal:"+stepVal, "factsDetailArea height:"+factsDetailArea.height)
                        if(factsDetailArea.height - scrollView.contentItem.height <= 0){
                            return;
                        }else if (factsDetailArea.height - scrollView.contentItem.height >= scrollView.contentItem.contentY + stepVal){
                            scrollAnimation.to = scrollView.contentItem.contentY + stepVal;
                        } else{
                            scrollAnimation.to = factsDetailArea.height - scrollView.contentItem.height;
                        }
                        //console.log("downBtn scrollAnimation.to:"+scrollAnimation.to)
                        scrollAnimation.start();
                    }
                }
            }

            //自定义代理
            states: [
                State {
                    name: "expanded";
                    PropertyChanges {
                        target: wrapper
                        height: {
                            return (listview.currentIndex === alm_dialog.listItemCount - 1) ? 300 : 215
                        }
                    }

                    PropertyChanges { target: factview; opacity: 1; }//文本详细信息可见
                }
            ]
            transitions: [
                Transition {
                    NumberAnimation {
                        duration: 200
                        properties: "height,width,anchors.rightMargin,anchors.topMargin,opacity";
                    }
                }
            ]
        }
    }

    ImageScrollBar{
        x:572
        y:56
        height: 325
        scale: 0.8
        visible : alm_dialog.alarmCount > numItemsPerPage ? true : false
        jogScrollOn: alm_dialog.jogScrollOn
        topButtonPressImage: resourceManager.imagePath + "/33_ALM_history/a330_02.png"
        topButtonReleaseImage: resourceManager.imagePath + "/33_ALM_history/a330_00.png"
        botomButtonPressImage: resourceManager.imagePath + "/33_ALM_history/a330_03.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/33_ALM_history/a330_01.png"
        pageLabal: parseInt(page + 1, 10) + "/" + alm_dialog.totalPages
        onScrollUpClicked: {
            //console.log("org_switch onScrollUpClicked:")
            let prevPage = page
            if (page >= 1){
                if (alm_dialog.alarmCount > numItemsPerPage){
                    page = Math.max(page - 1, 0)
                } else {
                    page = 0;
                }
            }
            if (prevPage !== page) {
                listview.closeDetailItem(page*numItemsPerPage)
            }
        }
        onScrollDownClicked: {
            //console.log("org_switch onScrollDownClicked:")
            let prevPage = page
            if (page < 1) {
                if (alm_dialog.alarmCount > numItemsPerPage){
                    page = Math.min(page + 1, 2)
                } else {
                    page = 0;
                }
            }
            if (prevPage !== page) {
                listview.closeDetailItem(page*numItemsPerPage)
            }
        }
    }//换页按钮

/***********************************************************************/
    Connections {
        target:commonViewModel
        onAlarmListStatusChanged:{
            //console.log("Connections commonViewModel alarmListStatus:"+alarmListStatus)
            //console.log("Connections commonViewModel onAlarmListStatusChanged:"+alm_dialog.alarmCount, "curAlarmCount:"+commonViewModel.getCurAlarmCount())
            listModelData.clear();
            alm_dialog.alarmCount = commonViewModel.getCurAlarmCount();
            alm_dialog.listItemCount = commonViewModel.getCurAlarmCount();
            if (alarmListStatus === true){
                for(var i = 0; i < alm_dialog.alarmCount; i++){
                    listModelData.append({
                                            type:alm_dialog.alarmList[i].type,
                                             NO:alm_dialog.alarmList[i].alarmNumber,
                                             factsColor:alm_dialog.alarmList[i].alarmDetailsColor,
                                            alarmIndex: alm_dialog.alarmList[i].alarmIndex,
                                             valid: true
                                         });
                }
                for (i = 0; i < 14 - alm_dialog.alarmCount; i++) {
                    listModelData.append({
                                        type: AlarmInfo.Unknown,
                                             NO:"",
                                             factsColor:"",
                                        alarmIndex: 0,
                                             valid: false
                                         });
                }

            } else {
                for(var j = 0; j < commonViewModel.getCurAlarmCount(); j++){
                    listModelData.append({
                                            type:alm_dialog.alarmList[j].type,
                                             NO:alm_dialog.alarmList[j].alarmNumber,
                                             factsColor:alm_dialog.alarmList[j].alarmDetailsColor,
                                            alarmIndex: alm_dialog.alarmList[j].alarmIndex,
                                             valid: true
                                         });
                }
                for (j = 0; j < 14 - commonViewModel.getCurAlarmCount(); j++) {
                    listModelData.append({
                                        type: AlarmInfo.Unknown,
                                             NO:"",
                                             factsColor:"",
                                        alarmIndex: 0,
                                             valid: false
                                         });
                }

            }
            alm_dialog.alarmCount = commonViewModel.getCurAlarmCount();
            alm_dialog.listItemCount = commonViewModel.getCurAlarmCount();
        }
    }
    Connections{
        target:phyKeyManager
        enabled: alm_dialog.jogDialSwOpe
        onJogDialSwitchInvalidChanged:{
            //console.log("Alm_Dialog onJogDialSwitchInvalidChanged:"+jogDialSwitchInvalid, "currentIndex:"+listview.currentIndex)
            if (listview.currentItem === null || listview.currentIndex < 0){
                console.log("Alm_Dialog onJogDialSwitchInvalidChanged currentItem Object is null currentIndex:"+listview.currentIndex)
                return
            }
            //console.log("Alm_Dialog2 isDetail:"+alm_dialog.isDetail[listview.currentIndex], "currentIndex:"+listview.currentIndex)
            // if (alm_dialog.isDetail[listview.currentIndex] === false){
            //     listview.currentItem.state = "expanded"
            //     alm_dialog.isDetail[listview.currentIndex] = true
            // } else {
            //     listview.currentItem.state = ""
            //     alm_dialog.isDetail[listview.currentIndex] = false
            // }

            //console.log("Alm_Dialog currentIndex:"+listview.currentIndex, "isDetail:"+alm_dialog.isDetail[listview.currentIndex], "state:"+listview.currentItem.state)
            let itemIndex
            if (detailItemIndex == -1) {
                itemIndex = 0
            } else {
                itemIndex = detailItemIndex + 1
            }

            if (itemIndex < alm_dialog.listItemCount) {
                console.debug("page", page, Math.ceil(itemIndex / 6), itemIndex / 6)
                if (itemIndex >= 6){
                    page = Math.min(Math.ceil(itemIndex / 6), 1)
                } else {
                    page = 0;
                }
                alm_dialog.isJogDialSwOpe = true
                listview.updateDetailItemStatus(itemIndex)
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
