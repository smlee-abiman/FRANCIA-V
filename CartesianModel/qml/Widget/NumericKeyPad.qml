import QtQuick 2.12
import QtQml 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: NumericKeyPad.qml
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
Rectangle{
    id: root
    width: 325
    height: 338
    color: "transparent"
    property string inputText: ""
    property int textMaxLength: 0
    property int keyLayoutType: 0
    property var maxValue: 0
    property var minValue: 0
    property bool noLimit: false
    property string textColor: "black"
    property bool enablePressAndHoldClearKey: false
    readonly property string pressAndHoldText: "****.**"
    readonly property string returnedTextWhenPressAndHold: "2164260864" // 0x81000000
    property bool isEmptyPermit: false
    property int pointno: 0
    property int axisno: 0
    property var pospageModel: teachingViewModel.pageModel[1]
    property var axisDataList: commonViewModel.axisInfo.axisDataList //轴data

    signal enter(string newValue);
    signal close();

    onEnter: {
        local.isFirstInput = true
    }

    QtObject {
        id: local
        property bool isFirstInput: true
        property var visibleTable: [
            [   //Type 0, decimal point:x, sign change:x, jog:x
             true, true, true, true,
             true, true, true, true,
             true, true, true, false,
             true, false, false, true
            ],
            [   //Type 1
             true, true, true, true,
             true, true, true, true,
             true, true, true, true,
             true, true, true, true
            ],
            [   //Type 2
             true, true, true, true,
             true, true, true, true,
             true, true, true, false,
             true, false, true, true
            ],
            [   //Type 3
             true, true, true, true,
             true, true, true, true,
             true, true, true, false,
             true, false, false, true
            ],
            [   //Type 4
             true, true, true, true,
             true, true, true, true,
             true, true, true, false,
             true, true, false, true
            ],
            [   //Type 5
             true, true, true, true,
             true, true, true, true,
             true, true, true, true,
             true, true, false, true
            ],
            [   //Type 6
             true, true, true, true,
             true, true, true, true,
             true, true, true, false,
             true, true, true, true
            ],
            [   //Type 7
             true, true, true, true,
             true, true, true, true,
             true, true, true, true,
             true, true, true, true
            ]
        ]
    }

    function clearInputTextWhenFirstInput() {
        if (local.isFirstInput) {
            root.inputText = ""
            local.isFirstInput = false;
        }
    }

    Text {
        y:25
        x:-18
        text: inputText
        width: 320
        height: 68
        color: textColor
        font.pointSize: 24
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
    }//当前值显示

    Grid {
        id: gridNum
        topPadding: 90
        leftPadding: 12
        columns: 4
        columnSpacing: 14
        rowSpacing: 10
        Repeater{
            model: ["7", "8", "9", "C", "4", "5", "6", "DEL", "1", "2", "3", "MEM", "0", ".", "±", "EN"]

            Rectangle{
                width: 65
                height: 62
                radius: 5
                border.color: "#898989"
                color:mouse_keybad.pressed ? "#CC3333" : "#CCCCCC"
                enabled: local.visibleTable[root.keyLayoutType][index]
                opacity: local.visibleTable[root.keyLayoutType][index]
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: mouse_keybad.pressed ? 28 : 26
                    font.bold: true
                    lineHeight: 0.8
                    color: mouse_keybad.pressed ? "white" : "#999999"
                    text: modelData
                }
                MouseArea{
                    id: mouse_keybad
                    anchors.fill: parent
                    property bool isPressAndHold: false
                    pressAndHoldInterval: 2000
                    onPressed: isPressAndHold = false
                    onClicked: {
                        if ((modelData !== "C") && (modelData !== "EN")) {
                            if (inputText === root.pressAndHoldText) {
                                inputText = "";
                            }
                        }
                        var psNum;
                        switch( modelData ) {
                        case "C":
                            if (isPressAndHold) {
                                break
                            }
                            inputText = "";
                            break;

                        case "DEL":
                            local.isFirstInput = false
                            if( inputText.length > 0 ) {
                                inputText = inputText.slice( 0, -1 );
                                // 开头符号删除
                                if(( inputText.length === 1 ) && ( inputText.indexOf( "-" ) === 0 )) {
                                    inputText = inputText.slice( 0, -1 );
                                }
                            }
                            break;

                        case "MEM":
                            let positiondata = parseInt((axisDataList[axisno].position)*100)
                            if(positiondata >= minValue && positiondata <= maxValue){
                                pospageModel.setPosition(pointno, axisno, positiondata, false)
                                root.close()
                            }
                            break;

                        case ".":
                            clearInputTextWhenFirstInput()
                            if(( inputText.length < textMaxLength ) && ( inputText.indexOf( "." ) === -1 )) {
                                if( inputText.length === 0 ){
                                    inputText += "0";
                                }
                                inputText += modelData;
                            }
                            break;

                        case "±":
                            if( inputText.length <= 0 ) {
                                break;
                            }
                            psNum = parseFloat( inputText );
                            if(psNum === 0){
                                //何もしない
                            } else {
                                local.isFirstInput = false
                                if( psNum < 0 ){
                                    inputText = inputText.slice( 1, inputText.length );
                                } else {
                                    inputText = ( "-" + inputText );
                                }
                            }
                            break;

                        case "EN":
                            if( inputText.length <= 0 ) {
                                if (isEmptyPermit) {
                                    root.enter(inputText);
                                }
                                break;
                            }

                            if( keyLayoutType !== 3 ) {
                                if( Number( inputText ) === 0 ) {
                                    inputText = "0";
                                }
                            }

                            if(noLimit){
                                if (inputText === root.pressAndHoldText) {
                                    root.enter(root.returnedTextWhenPressAndHold);
                                } else {
                                    root.enter(inputText);
                                }
                                inputText = "";
                            }else{
                                if (inputText === root.pressAndHoldText) {
                                    root.enter(root.returnedTextWhenPressAndHold);
                                    inputText = "";
                                } else if(((maxValue === pressAndHoldText) || (Number(inputText) <=　maxValue)) && (Number(inputText) >= minValue) ){
                                    root.enter(inputText);
                                    inputText = "";
                                } else {
                                    return
                                }
                            }
                            break;

                        default:
                            clearInputTextWhenFirstInput()
                            if( keyLayoutType === 3 ) {
                                if( inputText.length >= textMaxLength ) {
                                    break;
                                }
                                inputText += modelData;

                            } else {
                                var tempText = inputText;
                                var topChar = inputText.slice( 0, 1 );
                                if( topChar === "-" ){
                                    tempText = inputText.slice( 1, inputText.length );
                                }

                                if( tempText.length >= textMaxLength ) {
                                    break;
                                }

                                if(( inputText.indexOf( "0" ) === 0 ) && ( inputText.indexOf( "." ) === -1 )) {
                                    if( modelData == "0" ) {
                                        break;
                                    }
                                    inputText = inputText.replace( "0", "" );
                                }
                                inputText += modelData;
                            }
                        }
                    }
                    onPressAndHold: {
                        if (modelData === "C") {
                            if (root.enablePressAndHoldClearKey) {
                                inputText = root.pressAndHoldText;
                                isPressAndHold = true
                            }
                        }
                    }
                }
            }
        }
    }//主键盘
}
