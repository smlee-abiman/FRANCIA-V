import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("ModBus test")

    signal connectSignal()
    signal readBitSignal()
    signal readByteSignal()
    signal readWordSignal()
    signal readLongSignal()
    signal readStringSignal()
    signal writeBitSignal()
    signal writeByteSignal()
    signal writeWordSignal()
    signal writeLongSignal()
    signal writeStringSignal()
    signal disconnectSignal()
    signal initializeSignal()
    signal syncConnectSignal()
    signal syncDisconnectSignal()

    Button {
        id: connectButton
        x: 20
        y: 20
        width: 100
        height: 50
        text: "Connect"
        onClicked: {
            connectSignal()
        }
    }
    Button {
        id: syncConnectButton
        anchors.top: connectButton.top
        anchors.left: connectButton.right
        anchors.leftMargin: 5
        width: connectButton.width
        height: connectButton.height
        text: "SyncConnect"
        onClicked: {
            syncConnectSignal()
        }
    }
    Button {
        id: readBitButton
        anchors.top: connectButton.bottom
        anchors.topMargin: 5
        anchors.left: connectButton.left
        width: connectButton.width
        height: connectButton.height
        text: "ReadBit"
        onClicked: {
            readBitSignal()
        }
    }
    Button {
        id: readByteButton
        anchors.top: connectButton.bottom
        anchors.topMargin: 5
        anchors.left: readBitButton.right
        anchors.leftMargin: 5
        width: connectButton.width
        height: connectButton.height
        text: "ReadByte"
        onClicked: {
            readByteSignal()
        }
    }
    Button {
        id: readWordButton
        anchors.top: connectButton.bottom
        anchors.topMargin: 5
        anchors.left: readByteButton.right
        anchors.leftMargin: 5
        width: connectButton.width
        height: connectButton.height
        text: "ReadWord"
        onClicked: {
            readWordSignal()
        }
    }
    Button {
        id: readLongButton
        anchors.top: connectButton.bottom
        anchors.topMargin: 5
        anchors.left: readWordButton.right
        anchors.leftMargin: 5
        width: connectButton.width
        height: connectButton.height
        text: "ReadLong"
        onClicked: {
            readLongSignal()
        }
    }
    Button {
        id: readStringButton
        anchors.top: connectButton.bottom
        anchors.topMargin: 5
        anchors.left: readLongButton.right
        anchors.leftMargin: 5
        width: connectButton.width
        height: connectButton.height
        text: "ReadString"
        onClicked: {
            readStringSignal()
        }
    }

    Button {
        id: writeBitButton
        anchors.top: readBitButton.bottom
        anchors.topMargin: 5
        anchors.left: readBitButton.left
        width: readBitButton.width
        height: readBitButton.height
        text: "WriteBit"
        onClicked: {
            writeBitSignal()
        }
    }
    Button {
        id: writeByteButton
        anchors.top: readBitButton.bottom
        anchors.topMargin: 5
        anchors.left: writeBitButton.right
        anchors.leftMargin: 5
        width: readBitButton.width
        height: readBitButton.height
        text: "WriteByte"
        onClicked: {
            writeByteSignal()
        }
    }
    Button {
        id: writeWordButton
        anchors.top: readBitButton.bottom
        anchors.topMargin: 5
        anchors.left: writeByteButton.right
        anchors.leftMargin: 5
        width: readBitButton.width
        height: readBitButton.height
        text: "WriteWord"
        onClicked: {
            writeWordSignal()
        }
    }
    Button {
        id: writeLongButton
        anchors.top: readBitButton.bottom
        anchors.topMargin: 5
        anchors.left: writeWordButton.right
        anchors.leftMargin: 5
        width: readBitButton.width
        height: readBitButton.height
        text: "WriteLong"
        onClicked: {
            writeLongSignal()
        }
    }
    Button {
        id: writeStringButton
        anchors.top: readBitButton.bottom
        anchors.topMargin: 5
        anchors.left: writeLongButton.right
        anchors.leftMargin: 5
        width: readBitButton.width
        height: readBitButton.height
        text: "WriteString"
        onClicked: {
            writeStringSignal()
        }
    }

    Button {
        id: disconnectButton
        anchors.top: writeBitButton.bottom
        anchors.topMargin: 5
        anchors.left: writeBitButton.left
        width: writeBitButton.width
        height: writeBitButton.height
        text: "Disconnect"
        onClicked: {
            disconnectSignal()
        }
    }
    Button {
        id: syncDisconnectButton
        anchors.top: writeBitButton.bottom
        anchors.topMargin: 5
        anchors.left: disconnectButton.right
        anchors.leftMargin: 5
        width: writeBitButton.width
        height: writeBitButton.height
        text: "SyncDisconnect"
        onClicked: {
            syncDisconnectSignal()
        }
    }

    Button {
        id: initializeButton
        anchors.top: disconnectButton.bottom
        anchors.topMargin: 5
        anchors.left: disconnectButton.left
        width: disconnectButton.width
        height: disconnectButton.height
        text: "initialize"
        onClicked: {
            initializeSignal()
        }
    }
}
