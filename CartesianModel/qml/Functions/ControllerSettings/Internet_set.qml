import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"

import Enums 1.0            // Enums.FunctionControllerSetNetwork
/*************************************************************************************
* Title: UI/UX of AEK
* File: Internet_set.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.18
* Worker: SH.HONG
**************************************************************************************/
Item {
    property bool pageActive: false
    property var viewModel: functionViewModel.controllerSettingsView.networkSettingView
    width: parent.width
    height: parent.height

    QtObject {
        id: local
        property var networkSetting1: viewModel.networkSettingTbl[0]
        property var networkSetting2: viewModel.networkSettingTbl[1]
        property var edited1_ip:       [false, false, false, false]
        property var edited1_subnet:   [false, false, false, false]
        property var edited1_modbusId: false
        property var edited2_ip:       [false, false, false, false]
        property var edited2_subnet:   [false, false, false, false]
        property var edited2_modbusId: false
        function isEditedInArray(array) {
            for (let value of array) {
                if (value) {
                    return true;
                }
            }
            return false;
        }
        function isEdited() {
            return (isEditedInArray(edited1_ip) || isEditedInArray(edited1_subnet) || edited1_modbusId ||
                    isEditedInArray(edited2_ip) || isEditedInArray(edited2_subnet) || edited2_modbusId)
        }
        function resetAllEdited() {
            edited1_ip       = [false, false, false, false]
            edited1_subnet   = [false, false, false, false]
            edited1_modbusId = false;
            edited2_ip       = [false, false, false, false]
            edited2_subnet   = [false, false, false, false]
            edited2_modbusId = false;
        }
    }
    QtObject {
        id: penSet
        readonly property int groupNo: 17
        readonly property var editTextColorRGB: resourceManager.dataColor(groupNo, 49)
    }

    Component.onDestruction: viewModel.deactivate()
    onPageActiveChanged: {
        if (pageActive) {
            local.resetAllEdited()
            commonViewModel.pageChange(Enums.FunctionControllerSetNetwork)
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }
    Rectangle{
        width: 110
        height: 48
        x:852
        y:-46
        border.color: "#999999"
        color: save_mouse.pressed ? "#cc3333" : "#cccccc"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: save_mouse.pressed ? 25 : 24
            color: resourceManager.rgb(42,99)
            text: resourceManager.label(42,99, resourceManager.language)
        }
        MouseArea{
            id:save_mouse
            anchors.fill: parent
            onClicked: {
                if (local.isEdited()) {
                    viewModel.saveNetworkSettings()
                    local.resetAllEdited()
                }
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//保存按钮

    //通道1
    Rectangle{
        width: 960
        height: 250
        x:4
        y:4
        color: "#eaeaea"
        border.color: "#999999"
        border.width: 1
        Rectangle{
            width: 150
            height: 250
            color: "#999999"
            border.color: "#999999"
            border.width: 1
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                wrapMode: Text.Wrap
                color: resourceManager.rgb(42,68)
                text: resourceManager.label(42,68,resourceManager.language) + "\n" + "(LAN2)"
            }
        }
        //行分隔线
        Rectangle{
            width: 808
            height: 2
            x:151
            y:62
            color: "white"
        }
        Rectangle{
            width: 808
            height: 2
            x:151
            y:125
            color: "white"
        }
        Rectangle{
            width: 808
            height: 2
            x:151
            y:188
            color: "white"
        }
        //列分隔线
        Rectangle{
            width: 2
            height: 50
            x:326
            y:6
            color: "white"
        }
        Rectangle{
            width: 2
            height: 50
            x:326
            y:70
            color: "white"
        }
        Rectangle{
            width: 2
            height: 50
            x:326
            y:132
            color: "white"
        }
        Rectangle{
            width: 2
            height: 50
            x:326
            y:195
            color: "white"
        }
        //IP地址 : ip address.
        Text {
            x:152
            y:16
            font.pixelSize: 21
            text: resourceManager.label(42,70,resourceManager.language)
            color: resourceManager.rgb(42,70)
        }
        Row {
            x:334
            y:7
            spacing: 4
            Repeater {
                model: local.networkSetting1.ipAddress.length
                Rectangle {
                    width: 94
                    height: 48
                    color: "#999999"
                    border.color: "#999999"
                    border.width: 2
                    radius: 4
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: local.edited1_ip[index] ? penSet.editTextColorRGB : "black"
                        text: local.networkSetting1.ipAddress[index]
                    }
                }
//                LimitNumericEditBox {
//                    width: 94
//                    height: 48
//                    color: "#ffffff"
//                    borderColor: "#999999"
//                    borderWidth: 2
//                    rectRadius: 6
//                    fontPixelSize: 22
//                    min: "0"
//                    max: "255"
//                    keyLayoutType: 0
//                    textColor: local.edited1_ip[index] ? penSet.editTextColorRGB : "black"
//                    value: local.networkSetting1.ipAddress[index]
//                    onEnter: {
//                        if (value === newValue) { return }
//                        local.networkSetting1.editIpAddress(index, Number(newValue))
//                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
//                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
//                        local.edited1_ip[index] = true;
//                        local.edited1_ipChanged();
//                    }
//                }
            }
        }

        //子网掩码 : subnet mask.
        Text {
            x:152
            y:78
            font.pixelSize: 21
            text: resourceManager.label(42,71,resourceManager.language)
            color:resourceManager.rgb(42,71)
        }
        Row {
            x:334
            y:71
            spacing: 4
            Repeater {
                model: local.networkSetting1.subnetMask.length
                Rectangle {
                    width: 94
                    height: 48
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: local.edited1_subnet[index] ? penSet.editTextColorRGB : "black"
                        text: local.networkSetting1.subnetMask[index]
                    }
                }
//                LimitNumericEditBox {
//                    width: 94
//                    height: 48
//                    color: "#ffffff"
//                    borderColor: "#999999"
//                    borderWidth: 2
//                    rectRadius: 6
//                    fontPixelSize: 22
//                    min: "0"
//                    max: "255"
//                    keyLayoutType: 0
//                    textColor: local.edited1_subnet[index] ? penSet.editTextColorRGB : "black"
//                    value: local.networkSetting1.subnetMask[index]
//                    onEnter: {
//                        if (value === newValue) { return }
//                        local.networkSetting1.editSubnetMask(index, Number(newValue))
//                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
//                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
//                        local.edited1_subnet[index] = true;
//                        local.edited1_subnetChanged();
//                    }
//                }
            }
        }

        //MAC地址
        Text {
            x:152
            y:140
            font.pixelSize: 21
            text: resourceManager.label(42,72,resourceManager.language)
            color: resourceManager.rgb(42,72)
        }
        Row {
            x:334
            y:133
            spacing: 4
            Repeater {
                model: local.networkSetting1.macAddress.length
                Rectangle {
                    width: 94
                    height: 48
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        color: local.edited1_subnet[index] ? penSet.editTextColorRGB : "black"
                        text: local.networkSetting1.macAddress[index].toString(16).toUpperCase()
                    }
                }
//                Rectangle {
//                    width: 94
//                    height: 48
//                    color: "#999999"
//                    border.color: "#999999"
//                    border.width: 2
//                    radius: 6
//                    Text {
//                        anchors.centerIn: parent
//                        font.pixelSize: 22
//                        text: local.networkSetting1.macAddress[index].toString(16).toUpperCase()
//                    }
//                }
            }
        }

        //modbus站号 : modbus id.
        Text {
            x:152
            y:204
            font.pixelSize: 21
            text: resourceManager.label(42,73,resourceManager.language)
            color: resourceManager.rgb(42,73)
        }
        Rectangle {
            width: 94
            height: 48
            x:334
            y:195
            color: "#999999"
            border.color: "#999999"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 22
                color: local.edited1_modbusId ? penSet.editTextColorRGB : "black"
                text: local.networkSetting1.modbusId
            }
        }
//        LimitNumericEditBox {
//            width: 94
//            height: 48
//            x:334
//            y:195
//            color: "#ffffff"
//            borderColor: "#999999"
//            borderWidth: 2
//            rectRadius: 6
//            fontPixelSize: 22
//            min: "0"
//            max: "247"
//            keyLayoutType: 0
//            textColor: local.edited1_modbusId ? penSet.editTextColorRGB : "black"
//            value: local.networkSetting1.modbusId
//            onEnter: {
//                if (value === newValue) { return }
//                local.networkSetting1.editModbusId(Number(newValue))
//                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
//                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
//                local.edited1_modbusId = true;
//            }
//        }
    }

    //通道2
    Rectangle{
        width: 960
        height: 250
        x:4
        y:264
        color: "#eaeaea"
        border.color: "#999999"
        border.width: 1
        Rectangle{
            width: 150
            height: 250
            color: "#999999"
            border.color: "#999999"
            border.width: 1
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                wrapMode: Text.Wrap
                color: resourceManager.rgb(42,69)
                text: resourceManager.label(42,69,resourceManager.language) + "\n" + "(LAN1)"
            }
        }
        //行分隔线
        Rectangle{
            width: 808
            height: 2
            x:151
            y:62
            color: "white"
        }
        Rectangle{
            width: 808
            height: 2
            x:151
            y:125
            color: "white"
        }
        Rectangle{
            width: 808
            height: 2
            x:151
            y:188
            color: "white"
        }
        //列分隔线
        Rectangle{
            width: 2
            height: 50
            x:326
            y:6
            color: "white"
        }
        Rectangle{
            width: 2
            height: 50
            x:326
            y:70
            color: "white"
        }
        Rectangle{
            width: 2
            height: 50
            x:326
            y:132
            color: "white"
        }
        Rectangle{
            width: 2
            height: 50
            x:326
            y:195
            color: "white"
        }
        //IP地址 : ip address.
        Text {
            x:152
            y:16
            font.pixelSize: 21
            text: resourceManager.label(42,70,resourceManager.language)
            color: resourceManager.rgb(42,70)
        }
        Row {
            x:334
            y:7
            spacing: 4
            Repeater {
                model: local.networkSetting2.ipAddress.length
                LimitNumericEditBox {
                    width: 94
                    height: 48
                    color: "#ffffff"
                    borderColor: "#999999"
                    fontPixelSize: 22
                    min: "0"
                    max: "255"
                    keyLayoutType: 0
                    textColor: local.edited2_ip[index] ? penSet.editTextColorRGB : "black"
                    value: local.networkSetting2.ipAddress[index]
                    onEnter: {
                        if (value === newValue) { return }
                        local.networkSetting2.editIpAddress(index, Number(newValue))
                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        local.edited2_ip[index] = true;
                        local.edited2_ipChanged();
                    }
                }
            }
        }

        //子网掩码 : subnet mask.
        Text {
            x:152
            y:78
            font.pixelSize: 21
            text: resourceManager.label(42,71,resourceManager.language)
            color: resourceManager.rgb(42,71)
        }
        Row {
            x:334
            y:71
            spacing: 4
            Repeater {
                model: local.networkSetting2.subnetMask.length
                LimitNumericEditBox {
                    width: 94
                    height: 48
                    color: "#ffffff"
                    borderColor: "#999999"
                    fontPixelSize: 22
                    min: "0"
                    max: "255"
                    keyLayoutType: 0
                    textColor: local.edited2_subnet[index] ? penSet.editTextColorRGB : "black"
                    value: local.networkSetting2.subnetMask[index]
                    onEnter: {
                        if (value === newValue) { return }
                        local.networkSetting2.editSubnetMask(index, Number(newValue))
                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        local.edited2_subnet[index] = true;
                        local.edited2_subnetChanged();
                    }
                }
            }
        }

        //MAC地址
        Text {
            x:152
            y:140
            font.pixelSize: 21
            text: resourceManager.label(42,72,resourceManager.language)
            color:resourceManager.rgb(42,72)
        }
        Row {
            x:334
            y:133
            spacing: 4
            Repeater {
                model: local.networkSetting2.macAddress.length
                Rectangle {
                    width: 94
                    height: 48
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        text: local.networkSetting2.macAddress[index].toString(16).toUpperCase()
                    }
                }
            }
        }

        //modbus站号 : modbus id.
        Text {
            x:152
            y:204
            font.pixelSize: 21
            text: resourceManager.label(42,73,resourceManager.language)
            color:resourceManager.rgb(42,73)
        }
        LimitNumericEditBox {
            width: 94
            height: 48
            x:334
            y:195
            color: "#ffffff"
            borderColor: "#999999"
            fontPixelSize: 22
            min: "0"
            max: "247"
            keyLayoutType: 0
            textColor: local.edited2_modbusId ? penSet.editTextColorRGB : "black"
            value: local.networkSetting2.modbusId
            onEnter: {
                if (value === newValue) { return }
                local.networkSetting2.editModbusId(Number(newValue))
                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                local.edited2_modbusId = true;
            }
        }
    }

}
