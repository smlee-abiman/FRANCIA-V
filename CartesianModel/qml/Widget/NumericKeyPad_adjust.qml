import QtQuick 2.12
import QtQml 2.12

Rectangle{
    id: root
    width: 325
    height: 160
    color: "transparent"
    property string inputText: ""
    property string textColor: "black"
    property var inputvalue: parseFloat(inputText)
    property var maxValue: 0
    property var minValue: 0
    property bool noLimit: false
    property int keyLayoutType: 0

    signal enter(string newValue);

    QtObject {
        id: local
        property var visibleTable: [
            [   //Type 0,
             true, true, true,
             true, true, true
            ],
            [   //Type 1
             false, true, true,
             false, true, true
            ]
        ]
    }
    //主键盘
    Grid {
        anchors.centerIn: parent
        columns: keyLayoutType ? 2 : 3
        columnSpacing: 16
        rows: 2
        rowSpacing: 24

        Repeater{
            model: ["+0.1", "+1", "+10", "-0.1", "-1", "-10",]
            Rectangle{
                width: 90
                height: 52
                color: "#3065AF"
                radius: 6
                visible: local.visibleTable[keyLayoutType][index]
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 30
                    font.bold: true
                    color: mouse.pressed ? "yellow" : "white"
                    text: modelData
                }
                MouseArea{
                    id: mouse
                    anchors.fill: parent
                    onPressed: parent.color = "#32A4D0"
                    onReleased: parent.color = "#3065AF"
                    onClicked: {
                        switch( modelData ) {
                        case "+0.1":
                            var value = inputvalue + 0.1
                            if(noLimit){
                                root.enter(value.toString())
                            }else if(value >=  maxValue){
                                root.enter(maxValue)
                            }else{
                                root.enter(value.toString())
                            }
                            break;
                        case "+1":
                            var value1 = inputvalue + 1
                            if(noLimit){
                                root.enter(value1.toString())
                            }else if(value1 >=  maxValue){
                                root.enter(maxValue)
                            }else{
                                root.enter(value1.toString())
                            }
                            break;
                        case "+10":
                            var value2 = inputvalue + 10
                            if(noLimit){
                                root.enter(value2.toString())
                            }else if(value2 >=  maxValue){
                                root.enter(maxValue)
                            }else{
                                root.enter(value2.toString())
                            }
                            break;
                        case "-0.1":
                            var value3 = inputvalue - 0.1
                            if(value < 0){
                                root.enter("0")
                            }else if(noLimit){
                                root.enter(value3.toString())
                            }else if(value3 < minValue){
                                root.enter(minValue)
                            }else{
                                root.enter(value3.toString())
                            }
                            break;
                        case "-1":
                            var value4 = inputvalue - 1
                            if(value < 0){
                                root.enter("0")
                            }else if(noLimit){
                                root.enter(value4.toString())
                            }else if(value4 < minValue){
                                root.enter(minValue)
                            }else{
                                root.enter(value4.toString())
                            }
                            break;
                        case "-10":
                            var value5 = inputvalue - 10
                            if(value < 0){
                                root.enter("0")
                            }else if(noLimit){
                                root.enter(value5.toString())
                            }else if(value5 < minValue){
                                root.enter(minValue)
                            }else{
                                root.enter(value5.toString())
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}


