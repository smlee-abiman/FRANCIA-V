import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id: root
    width: 1280
    height: 768
    visible: true
    title: qsTr("ITZ3000 Demo")

    Row {
        spacing: 10
        MasterView {
            width: 1024
            height: root.height
            clip: true
        }
        Frame {
            width: 246
            height: root.height
            Item {
                Button {
                    id: key01
                    width:72
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    text: "KEY01"
                    onPressed: {
                        testAndSetManager.emulatedKey01();
                        touchManager.emulatedKey01();
                    }
                    onReleased: {
                        testAndSetManager.emulatedKeyAllOff();
                        touchManager.emulatedKeyAllOff();
                    }
                }
                Button {
                    width:72
                    anchors.top: key01.top
                    anchors.left: key01.right
                    anchors.leftMargin: 42
                    text: "KEY02"
                    onPressed: testAndSetManager.emulatedKey02()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key03
                    width:72
                    anchors.top: key01.bottom
                    anchors.topMargin: 20
                    anchors.left: key01.left
                    text: "KEY03"
                    onPressed: testAndSetManager.emulatedKey03()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    width:72
                    anchors.top: key03.top
                    anchors.left: key03.right
                    anchors.leftMargin: 42
                    text: "KEY04"
                    onPressed: testAndSetManager.emulatedKey04()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key05
                    width:72
                    anchors.top: key03.bottom
                    anchors.topMargin: 20
                    anchors.left: key03.left
                    text: "KEY05"
                    onPressed: testAndSetManager.emulatedKey05()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    width:72
                    anchors.top: key05.top
                    anchors.left: key05.right
                    anchors.leftMargin: 42
                    text: "KEY06"
                    onPressed: testAndSetManager.emulatedKey06()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key07
                    width:72
                    anchors.top: key05.bottom
                    anchors.topMargin: 20
                    anchors.left: key05.left
                    text: "KEY07"
                    onPressed: testAndSetManager.emulatedKey07()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    width:72
                    anchors.top: key07.top
                    anchors.left: key07.right
                    anchors.leftMargin: 42
                    text: "KEY08"
                    onPressed: testAndSetManager.emulatedKey08()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key09
                    width:72
                    anchors.top: key07.bottom
                    anchors.topMargin: 20
                    anchors.left: key07.left
                    text: "KEY09"
                    onPressed: testAndSetManager.emulatedKey09()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    width:72
                    anchors.top: key09.top
                    anchors.left: key09.right
                    anchors.leftMargin: 42
                    text: "KEY10"
                    onPressed: testAndSetManager.emulatedKey10()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key11
                    width:72
                    anchors.top: key09.bottom
                    anchors.topMargin: 20
                    anchors.left: key09.left
                    text: "KEY11"
                    onPressed: testAndSetManager.emulatedKey11()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    width:72
                    anchors.top: key11.top
                    anchors.left: key11.right
                    anchors.leftMargin: 42
                    text: "KEY12"
                    onPressed: testAndSetManager.emulatedKey12()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key13
                    width:72
                    anchors.top: key11.bottom
                    anchors.topMargin: 20
                    anchors.left: key11.left
                    text: "KEY13"
                    onPressed: testAndSetManager.emulatedKey13()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    width:72
                    anchors.top: key13.top
                    anchors.left: key13.right
                    anchors.leftMargin: 42
                    text: "KEY14"
                    onPressed: testAndSetManager.emulatedKey14()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key15
                    width:72
                    anchors.top: key13.bottom
                    anchors.topMargin: 20
                    anchors.left: key13.left
                    text: "KEY15"
                    onPressed: testAndSetManager.emulatedKey15()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    width:72
                    anchors.top: key15.top
                    anchors.left: key15.right
                    anchors.leftMargin: 42
                    text: "KEY16"
                    onPressed: testAndSetManager.emulatedKey16()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key17
                    width:72
                    anchors.top: key15.bottom
                    anchors.topMargin: 20
                    anchors.left: key15.left
                    text: "KEY17"
                    onPressed: testAndSetManager.emulatedKey17()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: key18
                    width:72
                    anchors.top: key17.top
                    anchors.left: key17.right
                    anchors.leftMargin: 42
                    text: "KEY18"
                    onPressed: testAndSetManager.emulatedKey18()
                    onReleased: testAndSetManager.emulatedKeyAllOff()
                }
                Button {
                    id: jogDialSw
                    width:72
                    anchors.top: key17.bottom
                    anchors.topMargin: 20
                    anchors.left: key17.left
                    text: "JOG-SW"
                    onClicked: testAndSetManager.toggleJogDialSwStatus();
                }
                Button {
                    // width:72
                    anchors.top: jogDialSw.top
                    anchors.right: key18.right
                    text: "DEADMAN"
                    onClicked: testAndSetManager.toggleDeadmanSwStatus()
                }
            }
        }
    }
}
