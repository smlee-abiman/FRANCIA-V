import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import "../../Widget/ComboBox"

import Enums 1.0            // Enums.FunctionControllerSetUser

Item {
    width: parent.width
    height: parent.height

    property bool pageActive: false
    property var viewModel: functionViewModel.controllerSettingsView.systemSettingView
    property var axisState: viewModel.axisState


    Column{
        x:15
        y:15
        spacing: 15
        Repeater{
            model: 7
            Row{
                Text{
                    width: 100
                    height: 50
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 20
                    color: resourceManager.rgb(22, 29+index)
                    text: resourceManager.label(22, 29+index, resourceManager.language)
                }
                StringComboBox_style1{
                    id: porogram_select
                    width: 167
                    height: 50
                    enabled: index != 0
                    bgRectBorderWidth: 2
                    bgPopupBorderWidth:2
                    popupDelegateNum: 2
                    resourceGroupId: 42
                    resourceStartIndex: 110
                    bgRectColor: "#c1d4f4"
                    bgIndicatorColor: "#8FAADC"
                    currentIndex: axisState[index]
                    onPopupIndexClick: {
//                        console.log(":::::::::::::::::::::",curIndex)
                        viewModel.setAxisState(index,curIndex)
                    }
                }
            }
        }
    }

    Rectangle{
        x:880
        y:-50
        width: 120
        height: 50
        radius: 8
        border.width: 2
        border.color: "#898989"
        color: mouse_confirm.pressed ? "yellow" : "#C8C8C9"
        Text{
            anchors.centerIn: parent
            font.pixelSize: 22
            text: resourceManager.label(41, 7, resourceManager.language)
        }
        MouseArea{
            id:mouse_confirm
            anchors.fill: parent
            onClicked: viewModel.saveAxisState()
        }
    }//保存
}
