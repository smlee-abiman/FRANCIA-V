/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Virtual Keyboard module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.VirtualKeyboard 2.3
import "KeybordType.js" as ItzKeybordUtil

KeyboardLayoutLoader {
//    function createInputMethod() {
//        return Qt.createQmlObject('import QtQuick 2.0; import QtQuick.VirtualKeyboard.Plugins 2.3; JapaneseInputMethod {}', parent, "japaneseInputMethod")
//    }

    sharedLayouts: ['symbol']
    sourceComponent: page1
	property var keysPage1: ["1234567890"]


    // English
    Component {
        id: page1

        KeyboardLayout {
            keyWeight: 80
           
			KeyboardRow {
                Repeater {
                    model: keysPage1[0].length
                    Key {
                        key: keysPage1[0][index].charCodeAt(0)
                        text: keysPage1[0][index]
                    }
                }
                BackspaceKey {}
            }

            KeyboardRow {
                Key {
                    key: Qt.Key_Q
                    text: "q"
                }
                Key {
                    key: Qt.Key_W
                    text: "w"
                }
                Key {
                    key: Qt.Key_E
                    text: "e"
                }
                Key {
                    key: Qt.Key_R
                    text: "r"
                }
                Key {
                    key: Qt.Key_T
                    text: "t"
                }
                Key {
                    key: Qt.Key_Y
                    text: "y"
                }
                Key {
                    key: Qt.Key_U
                    text: "u"
                }
                Key {
                    key: Qt.Key_I
                    text: "i"
                }
                Key {
                    key: Qt.Key_O
                    text: "o"
                }
                Key {
                    key: Qt.Key_P
                    text: "p"
                }
                Key{
                    displayText: "AC"
                    noModifier: true
                    onClicked: {
                        var str = InputContext.surroundingText;
                        InputContext.sendKeyClick(Qt.Key_Home, "", 0);
                        for(var i = 0; i < str.length; i++){
                            InputContext.sendKeyClick(Qt.Key_Delete, "", 0);
                        }
                    }
                }
            }

            KeyboardRow {
                FillerKey {
                    weight: 40
                }
                Key {
                    key: Qt.Key_A
                    text: "a"
                }
                Key {
                    key: Qt.Key_S
                    text: "s"
                }
                Key {
                    key: Qt.Key_D
                    text: "d"
                }
                Key {
                    key: Qt.Key_F
                    text: "f"
                }
                Key {
                    key: Qt.Key_G
                    text: "g"
                }
                Key {
                    key: Qt.Key_H
                    text: "h"
                }
                Key {
                    key: Qt.Key_J
                    text: "j"
                }
                Key {
                    key: Qt.Key_K
                    text: "k"
                }
                Key {
                    key: Qt.Key_L
                    text: "l"
                }
                EnterKey {
                    weight: 120
                }
            }

            KeyboardRow {
                keyWeight: 80

                ShiftKey {
                    weight: 80
                }
                Key {
                    key: Qt.Key_Z
                    text: "z"
                }
                Key {
                    key: Qt.Key_X
                    text: "x"
                }
                Key {
                    key: Qt.Key_C
                    text: "c"
                }
                Key {
                    key: Qt.Key_V
                    text: "v"
                }
                Key {
                    key: Qt.Key_B
                    text: "b"
                }
                Key {
                    key: Qt.Key_N
                    text: "n"
                }
                Key {
                    key: Qt.Key_M
                    text: "m"
                }
                Key {
                    key: Qt.Key_Comma
                    text: ","
                }
                Key {
                    key: Qt.Key_Period
                    text: "."
                }
                Key {
                    key: Qt.Key_Slash
                    text: "/"
                }
//                Key {
//                    key: Qt.Key_Up
//                    displayText: "\u2191"
//                    repeat: true
//                    noModifier: true
//                    functionKey: true
//                    visible: ItzKeybordUtil.upDownKeyVisible(InputContext.inputMethodHints)
//                }
//                Key {
//                    key: Qt.Key_Down
//                    displayText: "\u2193"
//                    repeat: true
//                    noModifier: true
//                    functionKey: true
//                    visible: ItzKeybordUtil.upDownKeyVisible(InputContext.inputMethodHints)
//                }
//                ShiftKey {
//                    weight: 180
//                }
            }

            KeyboardRow {
                keyWeight: 80

//                ChangeLanguageKey {
//                }
                Key {
                    enabled: false
                }
                Key {
                    enabled: false
                }
                Key {
                    enabled: false
                }
                Key {
                    enabled: false
                }
                SpaceKey {
                    weight: 240
                }
                Key {
                    enabled: false
                }
                SymbolModeKey {
                    displayText: "?123"
                    weight: 80
                }
                Key {
                    key: Qt.Key_Left
                    displayText: "\u2190"
                    repeat: true
                    noModifier: true
                    functionKey: true
                }
                Key {
                    key: Qt.Key_Right
                    displayText: "\u2192"
                    repeat: true
                    noModifier: true
                    functionKey: true
                }
//                HideKeyboardKey {
//                    weight: 200
//                }
            }

        }
    }
}
