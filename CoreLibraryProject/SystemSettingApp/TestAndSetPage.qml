import QtQuick 2.0
import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    id: root
    anchors.fill: parent

    signal componentDestructed(string source)
    signal folderSelected(int index)                                         // dummy. not use
    signal folderOpen(string folderPath, int depth)                          // dummy. not use
    signal fileSelected(int index)                                           // dummy. not use
    signal childFolderOpened(int index)                                      // dummy. not use
    signal showColorPattern()
    signal closeColorPattern()                                               // dummy. not use
    signal inputTestExecute()
    signal outputTestExecute()
    signal retClicked(string ipAddr, string subnetMask)
    signal calibrationClicked()                                              // dummy. not use
    signal testAndSetClicked()                                               // dummy. not use
    signal executeClicked(string currentPath)                                // dummy. not use
    signal directionClicked()                                                // dummy. not use
    signal createFolderClicked()                                             // dummy. not use
    signal eraceFolderClicked()                                              // dummy. not use
    signal displaySelectClicked()                                            // dummy. not use
    signal touchClicked()                                                    // dummy. not use
    signal exitTouch()                                                       // dummy. not use
    signal backLightCountDown()
    signal backLightCountUp()

    enum TenKeyTarget {
        None, IPAddress, SubnetMask
    }
    property int tenKeyTarget: TestAndSetPage.TenKeyTarget.None

    Component.onDestruction: {
        componentDestructed("TestAndSetPage.qml")
    }

    QtObject {
        id: local
        function isNetworkAddressFormat(address) {
            if (!address) {
                console.log(`address is empty or null. [${address}]`)
                return false
            }
            let octetStrList = address.split(".")
            if (octetStrList.length !== 4) {
                console.log(`The number of octets is not 4. [${octetStrList.length}], [${address}]`)
                return false
            }
            for (const octetStr of octetStrList) {
                if (!octetStr) {
                    console.log(`Octet is empty or null.[${octetStr}], [${octetStrList}]`)
                    return false
                }
                if (isNaN(octetStr)) {
                    console.log(`Octet is not a number.[${octetStr}], [${octetStrList}]`)
                    return false
                }
                let octetNum = parseInt(octetStr)
                if ((octetNum < 0) || (255 < octetNum)) {
                    console.log(`Octet is out of range.[${octetStr}], [${octetStrList}]`)
                    return false
                }
            }
            return true
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        onClicked: mouse.accepted = false
        onWheel: {
            if(!REAL_TARGET) {
                if(wheel.angleDelta.y > 0) {
                    // Wheeled up
                    testAndSetManager.incrementEmulatedJogCount()
                } else if(wheel.angleDelta.y < 0) {
                    // Wheeled down
                    testAndSetManager.decrementEmulatedJogCount()
                }
            }
        }
    }

    Rectangle {
        id: selectSW
        width: 261
        height: 64
        x: 19
        y: 19
        border.color: "black"
        Text {
            id: selectSWText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("SELECT SW")
        }
    }

    Rectangle {
        id: sel1
        width: 135
        height: 64
        anchors.top: selectSW.top
        anchors.left: selectSW.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isSelect01 === false ? "gray" : "white"
        Text {
            id: sel1Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("SEL1")
        }
    }

    Rectangle {
        id: sel2
        width: 135
        height: 64
        anchors.top: sel1.top
        anchors.left: sel1.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isSelect02 === false ? "gray" : "white"
        Text {
            id: sel2Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("SEL2")
        }
    }

    Rectangle {
        id: sel3
        width: 135
        height: 64
        anchors.top: sel2.top
        anchors.left: sel2.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isSelect03 === false ? "gray" : "white"
        Text {
            id: sel3Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("SEL3")
        }
    }

    Rectangle {
        id: key01
        width: 135
        height: 64
        anchors.top: sel3.top
        anchors.left: sel3.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey01 === false ? "gray" : "white"
        Text {
            id: key01Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY01")
        }
    }

    Rectangle {
        id: key02
        width: 135
        height: 64
        anchors.top: key01.top
        anchors.left: key01.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey02 === false ? "gray" : "white"
        Text {
            id: key02Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY02")
        }
    }

    Rectangle {
        id: jogDial
        width: 261
        height: 64
        anchors.top: selectSW.bottom
        anchors.topMargin: 10
        anchors.left: selectSW.left
        border.color: "black"
        Text {
            id: jogDialText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("JOG DIAL")
        }
    }

    Rectangle {
        id: jogPulse
        width: 135
        height: 64
        anchors.top: jogDial.top
        anchors.left: jogDial.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: jogPulseText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.jogCounter
        }
    }

    Rectangle {
        id: cw
        width: 135
        height: 64
        anchors.top: jogPulse.top
        anchors.left: jogPulse.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: cwText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("CW")
        }
    }

    Rectangle {
        id: ccw
        width: 135
        height: 64
        anchors.top: cw.top
        anchors.left: cw.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: ccwText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("CCW")
        }
    }

    Rectangle {
        id: key03
        width: 135
        height: 64
        anchors.top: ccw.top
        anchors.left: ccw.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey03 === false ? "gray" : "white"
        Text {
            id: key03Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY03")
        }
    }

    Rectangle {
        id: key04
        width: 135
        height: 64
        anchors.top: key03.top
        anchors.left: key03.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey04 === false ? "gray" : "white"
        Text {
            id: key04Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY04")
        }
    }

    Rectangle {
        id: jogEnable
        width: 261
        height: 64
        anchors.top: jogDial.bottom
        anchors.topMargin: 10
        anchors.left: jogDial.left
        border.color: "black"
        Text {
            id: jogEnableText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("JOG/ENABLE")
        }
    }

    Rectangle {
        id: cwEnable
        width: 135
        height: 64
        anchors.top: jogEnable.top
        anchors.left: jogEnable.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: cwEnableText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.jogDialSwStatus ? "ON" : "OFF"
        }
    }

    Rectangle {
        id: ccwEnable
        width: 135
        height: 64
        anchors.top: cwEnable.top
        anchors.left: cwEnable.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: ccwEnableText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.deadmanSwStatus ? "ON" : "OFF"
        }
    }

    Rectangle {
        id: modbusErrorCount
        width: 135
        height: 64
        anchors.top: ccwEnable.top
        anchors.left: ccwEnable.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: modbusErrorCountText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.modbusErrorCount
        }
    }

    Rectangle {
        id: key05
        width: 135
        height: 64
        anchors.top: modbusErrorCount.top
        anchors.left: modbusErrorCount.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey05 === false ? "gray" : "white"
        Text {
            id: key05Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY05")
        }
    }

    Rectangle {
        id: key06
        width: 135
        height: 64
        anchors.top: key05.top
        anchors.left: key05.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey06 === false ? "gray" : "white"
        Text {
            id: key06Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY06")
        }
    }

    Rectangle {
        id: macAddress
        width: 261
        height: 64
        anchors.top: jogEnable.bottom
        anchors.topMargin: 10
        anchors.left: jogEnable.left
        border.color: "black"
        Text {
            id: macAddressText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("MAC ADDRESS")
        }
        TenKey {
            id: tenkey
            x: parent.width - width
            y: 0
            width: 250
            height: 360
            modal: true
            closePolicy: "NoAutoClose"
            onClosed: {
                if (!local.isNetworkAddressFormat(tenkey.valueStr)) {
                    return
                }
                if(root.tenKeyTarget === TestAndSetPage.TenKeyTarget.IPAddress) {
                    testAndSetManager.ipAddress = tenkey.valueStr
                } else if(root.tenKeyTarget === TestAndSetPage.TenKeyTarget.SubnetMask) {
                    testAndSetManager.subnetMask = tenkey.valueStr
                }
            }
        }
    }

    Rectangle {
        id: macAddressInput
        width: 425
        height: 64
        anchors.top: macAddress.top
        anchors.left: macAddress.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: macAddressInputText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.macAddress.replace(/:/g, '.')
        }
    }

    Rectangle {
        id: key07
        width: 135
        height: 64
        anchors.top: macAddressInput.top
        anchors.left: macAddressInput.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey07 === false ? "gray" : "white"
        Text {
            id: key07Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY07")
        }
    }

    Rectangle {
        id: key08
        width: 135
        height: 64
        anchors.top: key07.top
        anchors.left: key07.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey08 === false ? "gray" : "white"
        Text {
            id: key08Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY08")
        }
    }

    Rectangle {
        id: ipAddress
        width: 261
        height: 64
        anchors.top: macAddress.bottom
        anchors.topMargin: 10
        anchors.left: macAddress.left
        border.color: "black"
        Text {
            id: ipAddressText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("IP ADDRESS")
        }
        TapHandler {
            onTapped: {
                root.tenKeyTarget = TestAndSetPage.TenKeyTarget.IPAddress
                tenkey.open()
            }
        }
    }

    Rectangle {
        id: ipAddressInput
        width: 425
        height: 64
        anchors.top: ipAddress.top
        anchors.left: ipAddress.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: ipAddressInputText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.ipAddress
        }
    }

    Rectangle {
        id: key09
        width: 135
        height: 64
        anchors.top: ipAddressInput.top
        anchors.left: ipAddressInput.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey09 === false ? "gray" : "white"
        Text {
            id: key09Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY09")
        }
    }

    Rectangle {
        id: key10
        width: 135
        height: 64
        anchors.top: key09.top
        anchors.left: key09.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey10 === false ? "gray" : "white"
        Text {
            id: key10Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY10")
        }
    }

    Rectangle {
        id: subnetMask
        width: 261
        height: 64
        anchors.top: ipAddress.bottom
        anchors.topMargin: 10
        anchors.left: ipAddress.left
        border.color: "black"
        Text {
            id: subnetMaskText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("SUBNET MASK")
        }
        TapHandler {
            onTapped: {
                root.tenKeyTarget = TestAndSetPage.TenKeyTarget.SubnetMask
                tenkey.open()
            }
        }
    }

    Rectangle {
        id: subnetMaskInput
        width: 425
        height: 64
        anchors.top: subnetMask.top
        anchors.left: subnetMask.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: subnetMaskInputText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.subnetMask
        }
    }

    Rectangle {
        id: key11
        width: 135
        height: 64
        anchors.top: subnetMaskInput.top
        anchors.left: subnetMaskInput.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey11 === false ? "gray" : "white"
        Text {
            id: key11Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY11")
        }
    }

    Rectangle {
        id: key12
        width: 135
        height: 64
        anchors.top: key11.top
        anchors.left: key11.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey12 === false ? "gray" : "white"
        Text {
            id: key12Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY12")
        }
    }

    Rectangle {
        id: space01
        width: 261
        height: 64
        anchors.top: subnetMask.bottom
        anchors.topMargin: 10
        anchors.left: subnetMask.left
        border.color: "black"
        Text {
            id: space01Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("")
        }
    }

    Rectangle {
        id: space02
        width: 425
        height: 64
        anchors.top: space01.top
        anchors.left: space01.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: space02Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("")
        }
    }

    Rectangle {
        id: key13
        width: 135
        height: 64
        anchors.top: space02.top
        anchors.left: space02.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey13 === false ? "gray" : "white"
        Text {
            id: key13Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY13")
        }
    }

    Rectangle {
        id: key14
        width: 135
        height: 64
        anchors.top: key13.top
        anchors.left: key13.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey14 === false ? "gray" : "white"
        Text {
            id: key14Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY14")
        }
    }

    Rectangle {
        id: serialNo
        width: 261
        height: 64
        anchors.top: space01.bottom
        anchors.topMargin: 10
        anchors.left: space01.left
        border.color: "black"
        Text {
            id: serialNoText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("SERIAL NO.")
        }
    }

    Rectangle {
        id: serialNoInput
        width: 425
        height: 64
        anchors.top: serialNo.top
        anchors.left: serialNo.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: serialNoInputText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.serialNo
        }
    }

    Rectangle {
        id: key15
        width: 135
        height: 64
        anchors.top: serialNoInput.top
        anchors.left: serialNoInput.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey15 === false ? "gray" : "white"
        Text {
            id: key15Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY15")
        }
    }

    Rectangle {
        id: key16
        width: 135
        height: 64
        anchors.top: key15.top
        anchors.left: key15.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey16 === false ? "gray" : "white"
        Text {
            id: key16Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY16")
        }
    }

    Rectangle {
        id: backLight
        width: 261
        height: 64
        anchors.top: serialNo.bottom
        anchors.topMargin: 10
        anchors.left: serialNo.left
        border.color: "black"
        Text {
            id: backLightText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("BACK LIGHT")
        }
    }

    Button {
        id: minusButton
        width: 64
        height: 64
        anchors.top: backLight.top
        anchors.left: backLight.right
        anchors.leftMargin: 10
        text: qsTr("-")
        onClicked: { backLightCountDown() }
    }

    Rectangle {
        id: backLightValue
        width: 277
        height: 64
        anchors.top: minusButton.top
        anchors.left: minusButton.right
        anchors.leftMargin: 10
        border.color: "black"
        Text {
            id: backLightValueText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: testAndSetManager.backLightValue
        }
    }

    Button {
        id: plusButton
        width: 64
        height: 64
        anchors.top: backLightValue.top
        anchors.left: backLightValue.right
        anchors.leftMargin: 10
        text: qsTr("+")
        onClicked: { backLightCountUp() }
    }

    Rectangle {
        id: key17
        width: 135
        height: 64
        anchors.top: plusButton.top
        anchors.left: plusButton.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey17 === false ? "gray" : "white"
        Text {
            id: key17Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY17")
        }
    }

    Rectangle {
        id: key18
        width: 135
        height: 64
        anchors.top: key17.top
        anchors.left: key17.right
        anchors.leftMargin: 10
        border.color: "black"
        color: testAndSetManager.isPressedKey18 === false ? "gray" : "white"
        Text {
            id: key18Text
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("KEY18")
        }
    }

    Rectangle {
        id: displayButton
        width: 273
        height: 64
        anchors.top: backLight.bottom
        anchors.topMargin: 10
        anchors.left: backLight.left
        color: "#e0e0e0"
        border.color: "black"

        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("DISPLAY")
        }

        TapHandler {
            onTapped: { showColorPattern() }
        }
    }

    Rectangle {
        id: inputTestButton
        width: 273
        height: 64
        anchors.top: displayButton.top
        anchors.left: displayButton.right
        anchors.leftMargin: 11
        color: testAndSetManager.isExecuteInputTest === true ? "gray" : "#e0e0e0"
        border.color: "black"

        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("INPUT TEST")
        }

        TapHandler {
            onTapped: { inputTestExecute() }
        }
    }

    Rectangle {
        id: outputTestButton
        width: 273
        height: 64
        anchors.top: inputTestButton.top
        anchors.left: inputTestButton.right
        anchors.leftMargin: 11
        color: testAndSetManager.isExecuteOutputTest === true ? "gray" : "#e0e0e0"
        border.color: "black"

        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("OUTPUT")
        }

        TapHandler {
            onTapped: { outputTestExecute() }
        }
    }

    Rectangle {
        id: retButton
        width: 135
        height: 64
        anchors.top: outputTestButton.top
        anchors.left: outputTestButton.right
        anchors.leftMargin: 10
        color: "#e0e0e0"
        border.color: "black"

        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("RET")
        }

        TapHandler {
            onTapped: {
                if (local.isNetworkAddressFormat(ipAddressInputText.text) &&
                    local.isNetworkAddressFormat(subnetMaskInputText.text)) {
                    retClicked(ipAddressInputText.text, subnetMaskInputText.text)
                } else {
                    retClicked("", "")
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:768;width:1024}
}
##^##*/
