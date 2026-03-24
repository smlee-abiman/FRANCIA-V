import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Language_Dialog.qml
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
Dialog{
    id: language_dialog
    width: 642
    height: 323
    modal: false
    closePolicy: Popup.CloseOnEscape
    background: Rectangle{
        width: 642
        height: 310
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 642
            height: 55
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.centerIn: parent
                font.pixelSize: 26
                color: resourceManager.rgb(57,0)
                text: resourceManager.label(57,0,resourceManager.language)
            }
            ImageButton{
                id:language_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    language_dialog.close()
                }
            }
        }//标题栏
        Rectangle{
            width: 1
            height: 250
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            color: "#999999"
        }//分隔线
    }

    QtObject {
        id: local
        readonly property int languageCount: commonViewModel.languageCount
        readonly property var languageOrder: commonViewModel.languageOrder
        readonly property int language: resourceManager.language
        onLanguageChanged: {
            if ((language < 0) || (languageCount <= language)) {
                return
            }
            languageChecked = [false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false]
            languageChecked[language] = true
            //languageCheckedChanged()
        }
        Component.onCompleted: {
            // 현재 언어 기준으로 체크 상태 한번 초기화
            local.languageChecked = [false, false, false, false, false, false, false, false,
                                     false, false, false, false, false, false, false, false]
            if (local.language >= 0 && local.language < local.languageChecked.length) {
                local.languageChecked[local.language] = true
            }
        }
        property var languageChecked: [
            // language code order.
            false, //  0.Japanese.
            false, //  1.Chinese.
            false, //  2.Chinese.
            false, //  3.Korean.
            false, //  4.English.
            false, //  5.Malaysia.
            false, //  6.Myanmar.
            false, //  7.Vietnam.
            false, //  8.reserve.
            false, //  9.reserve.
            false, // 10.reserve.
            false, // 11.reserve.
            false, // 12.reserve.
            false, // 13.reserve.
            false, // 14.reserve.
            false, // 15.reserve.
        ]
        readonly property var languageTexts: [
            // language code order.
            resourceManager.label(57, 3,resourceManager.language),  //  0.Japanese.
            resourceManager.label(57, 1,resourceManager.language),  //  1.Chinese.
            resourceManager.label(57, 2,resourceManager.language),  //  2.Chinese.
            resourceManager.label(57, 4,resourceManager.language),  //  3.Korean.
            resourceManager.label(57, 5,resourceManager.language),  //  4.English.
            resourceManager.label(57, 6,resourceManager.language),  //  5.Vietnam.
            resourceManager.label(57, 7,resourceManager.language),  //  6.Myanmar.
            resourceManager.label(57, 8,resourceManager.language),  //  7.Vietnam.
            resourceManager.label(57, 9,resourceManager.language),  //  8.mexico.
            resourceManager.label(57,10,resourceManager.language),  //  9.reserve.
            resourceManager.label(57,11,resourceManager.language),  // 10.reserve.
            resourceManager.label(57,12,resourceManager.language),  // 11.reserve.
            resourceManager.label(57,13,resourceManager.language),  // 12.reserve.
            resourceManager.label(57,14,resourceManager.language),  // 13.reserve.
            resourceManager.label(57,15,resourceManager.language),  // 14.reserve.
            resourceManager.label(57,16,resourceManager.language),  // 15.reserve.
        ]
        readonly property var languageTextColors: [
            // language code order.
            resourceManager.rgb(57, 3), //  0.Japanese.
            resourceManager.rgb(57, 1), //  1.Chinese.
            resourceManager.rgb(57, 2), //  2.Chinese.
            resourceManager.rgb(57, 4), //  3.Korean.
            resourceManager.rgb(57, 5), //  4.English.
            resourceManager.rgb(57, 6), //  5.Vietnam.
            resourceManager.rgb(57, 7), //  6.Myanmar.
            resourceManager.rgb(57, 8), //  7.Vietnam.
            resourceManager.rgb(57, 9), //  8.mexico.
            resourceManager.rgb(57,10), //  9.reserve.
            resourceManager.rgb(57,11), // 10.reserve.
            resourceManager.rgb(57,12), // 11.reserve.
            resourceManager.rgb(57,13), // 12.reserve.
            resourceManager.rgb(57,14), // 13.reserve.
            resourceManager.rgb(57,15), // 14.reserve.
            resourceManager.rgb(57,16), // 15.reserve.
        ]
        readonly property var languageIcons: [
            "/36_DIALOG/a360_21.png", //  0.Japanese.
            "/36_DIALOG/a360_22.png", //  1.Chinese.
            "/36_DIALOG/a360_22.png", //  2.Chinese.
            "/36_DIALOG/a360_23.png", //  3.Korean.
            "/36_DIALOG/a360_24.png", //  4.English.
            "/36_DIALOG/a360_25.png", //  5.Vietnam.
            "/36_DIALOG/a360_44.png", //  6.Myanmar.
            "/36_DIALOG/a360_25.png", //  7.Vietnam.
            "/36_DIALOG/a360_26.png", //  8.mexico.
            "/36_DIALOG/a360_47.png", //  9.reserve.
            "/36_DIALOG/a360_48.png", // 10.reserve.
            "/36_DIALOG/a360_49.png", // 11.reserve.
            "/36_DIALOG/a360_50.png", // 12.reserve.
            "/36_DIALOG/a360_51.png", // 13.reserve.
            "/36_DIALOG/a360_52.png", // 14.reserve.
            "/36_DIALOG/a360_53.png", // 15.reserve.
        ]
        function selectLanguage(languageCode) {
            commonViewModel.modbusUpdateLanguageCode(languageCode)
            if(!REAL_TARGET) {
                resourceManager.language = languageCode
                let curPage = commonViewModel.getCurrentPage()
                commonViewModel.updateTitle(curPage);
            }
        }
    }

    Column{
        id: leftPage
        x:10
        y:50
        spacing: 15
        readonly property int baseIndex: (top_bar.languageCurPage === 1) ? 0 : 8
        Repeater {
            model: Math.max(0, Math.min(4, local.languageCount - leftPage.baseIndex))
            Row {
                id: leftPageItem
                readonly property int languageIndex: index + leftPage.baseIndex
                readonly property var languageCode: local.languageOrder[languageIndex]
                CheckBox {
                    id: leftPageCheckBox
                    anchors.verticalCenter: parent.verticalCenter
                    checked: (resourceManager.language === leftPageItem.languageCode)
                    onClicked: {
                        local.selectLanguage(leftPageItem.languageCode)
                        phyKeyManager.ctrlBuzzer()
                    }
                    nextCheckState: function() {
                        return Qt.Checked
                    }
                }
                Text {
                    width: 150
                    height: 40
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    text: local.languageTexts[leftPageItem.languageCode]
                    color: local.languageTextColors[languageCode]
                    font.pixelSize: 15
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            local.selectLanguage(leftPageItem.languageCode)
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    width: 50
                    height: 50
                    source: resourceManager.imagePath + local.languageIcons[leftPageItem.languageCode]
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            local.selectLanguage(leftPageItem.languageCode)
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }
        }
    }

    Column{
        id: rightPage
        x:320
        y:50
        spacing: 20
        readonly property int baseIndex: (top_bar.languageCurPage === 1) ? 4 : 12
        Repeater {
            model: Math.max(0, Math.min(4, local.languageCount - rightPage.baseIndex))
            Row {
                id: rightPageItem
                readonly property int languageIndex: index + rightPage.baseIndex
                readonly property var languageCode: local.languageOrder[languageIndex]
                CheckBox {
                    id: rightPageCheckBox
                    anchors.verticalCenter: parent.verticalCenter
                    checked: (resourceManager.language === rightPageItem.languageCode)
                    onClicked: {
                        local.selectLanguage(rightPageItem.languageCode)
                        phyKeyManager.ctrlBuzzer()
                    }
                    nextCheckState: function() {
                        return Qt.Checked
                    }
                }
                Text {
                    width: 150
                    height: 40
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    text: local.languageTexts[rightPageItem.languageCode]
                    color: local.languageTextColors[languageCode]
                    font.pixelSize: 15
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            local.selectLanguage(rightPageItem.languageCode)
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
                Image {
                    width: 50
                    height: 50
                    source: resourceManager.imagePath + local.languageIcons[rightPageItem.languageCode]
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            local.selectLanguage(rightPageItem.languageCode)
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }
        }
    }

    ImageScrollBar{
        x:580
        y:20
        height: 300
        scale: 0.8
        // visible : top_bar.languagePageCount > 1 ? true : false
        jogScrollOn: false
        topButtonPressImage: resourceManager.imagePath + "/36_DIALOG/a360_03" + ".png"
        topButtonReleaseImage: resourceManager.imagePath + "/36_DIALOG/a360_31" + ".png"
        botomButtonPressImage: resourceManager.imagePath + "/36_DIALOG/a360_04" + ".png"
        botomButtonReleaseImage: resourceManager.imagePath + "/36_DIALOG/a360_32" + ".png"
        pageLabal: top_bar.languageCurPage + "/" + top_bar.languagePageCount
        onScrollUpClicked: {
            top_bar.languageCurPage--
            if (top_bar.languageCurPage <= 0){
                top_bar.languageCurPage = 1
            }
        }
        onScrollDownClicked: {
            top_bar.languageCurPage++
            if (top_bar.languageCurPage > top_bar.languagePageCount){
                top_bar.languageCurPage = top_bar.languagePageCount
            }
        }
    }
}
