import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: LimitNumericEditBox.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.16
* Worker: SH.HONG
**************************************************************************************/
Item {
    id: root
    width: 85
    height: 40

    property string min: "0"
    property string max: "9999"
    property bool noLimit: false
    property string value: "0"
    property string defaultValue: ""
    property string unit: ""
    property int horizontalAlignment: Text.AlignHCenter
    property int fontPixelSize: 18
    property int textMaxLength: 12
    property int dialogDisplayX: 500
    property int dialogDisplayY: 100
    property int keyLayoutType: 0
    property int textrightMargin: 0
    property string textColor: "black"
    property string tenkeyTextColor: "black"
    property string color: "white"
    property int rectRadius:0
    property string borderColor:"#999999"
    property int borderWidth:2
    property bool opened: false
    property bool enablePressAndHoldClearKey: false
    property bool textVisible:true
    property bool isNumberKpad2: false
    property int numberkpad2LayoutType: 0
    property bool clicklimit: false
    property int pointno: 0
    property int axisno: 0

    signal enter(string newValue);
    signal open();
    signal close();
    signal ready();
    signal clickfail();

    function setDialogPos(startPosX,startPosY,rowIndexs,columnIndexs,rowspaces,columnspaces){
        let posX = startPosX + columnIndexs *  columnspaces
        let posY = startPosY + rowIndexs * rowspaces
        dialogDisplayX = posX
        dialogDisplayY = posY
    }

    Rectangle {
        width: root.width
        height: root.height
        radius: root.rectRadius
        color: root.color
        border.width: root.borderWidth
        border.color: root.borderColor

        Text{
            visible:root.textVisible
            anchors.fill: parent
            anchors.rightMargin: root.textrightMargin
            horizontalAlignment: root.horizontalAlignment
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: root.fontPixelSize
            color: textColor
            text: root.value + ((root.unit.length > 0) ? root.unit : "")
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                phyKeyManager.ctrlBuzzer()
                if(dialogConnection.createObjects !== null){
                    //console.log("dialogConnection.createObjects is exists");
                    return;
                }
                if(clicklimit){
                    root.clickfail();
                    return;
                }
                root.ready();
                var component = Qt.createComponent("Number_Keyboard.qml");
                if (component.status === Component.Ready) {
                    var dialog = component.createObject(parent,{popupType: 1});
                    dialog.textMaxLength = root.textMaxLength;
                    dialog.x = root.dialogDisplayX;
                    dialog.y = root.dialogDisplayY;
                    dialog.keyLayoutType = root.keyLayoutType;
                    dialog.digits = value;
                    dialog.max = max;
                    dialog.min = min;
                    dialog.pointno = pointno
                    dialog.axisno = axisno
                    dialog.noLimit = noLimit;
                    dialog.valueColor = tenkeyTextColor;
                    dialog.enablePressAndHoldClearKey = root.enablePressAndHoldClearKey;
                    dialog.isnumberkpad2 = isNumberKpad2
                    dialog.numberkpad2LayoutType = numberkpad2LayoutType
                    dialog.numberkpad2enter.connect(dialogConnection.onNumberkpad2enter)
                    dialogConnection.createObjects = component;
                    dialogConnection.target = dialog;
                    opened = true
                    root.open();
                    dialog.open();
                } else if (component.status === Component.Error) {
                    //console.log(component.errorString());
                    component.destroy();
                } else{
                    if(component !== null)
                        component.destroy();
                }
            }
        }

        Connections {
            id: dialogConnection
            property var createObjects: null
            onVisibleChanged: {
                if(!target.visible) {
                    if(typeof target.digits !== 'undefined'){
                        if(target.isEnterkeyPressd){
                            root.enter(target.digits);
                        }
                    }

                    if(createObjects !== null){
                        createObjects.destroy();
                        createObjects = null;
                        opened = false
                        root.close();
                        target.destroy();
                        target = null;
                    }
                }
            }
        // for numberkpad2enter() signal in Number_Keyboard.qml, connect at instance creation.
            function onNumberkpad2enter() {
                root.enter(target.digits);
            }
        }
    }
}
