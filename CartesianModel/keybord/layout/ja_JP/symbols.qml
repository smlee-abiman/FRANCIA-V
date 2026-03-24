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

    sharedLayouts: ['main']

    property int page
    readonly property int numPages: 2
    property var keysPage1: ["1234567890"]

    sourceComponent: {
        switch (page) {
        case 1: return page2
        default: return page1
        }
    }

    // symbol 1/2
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
//                FillerKey {
//                    weight: 56
//                }
                Key {
                    key: Qt.Key_Plus
                    text: "\u002B"
                }
                Key {
                    key: Qt.Key_Minus
                    text: "\u002D"
                }
                Key {
                    key: Qt.Key_Asterisk
                    text: "\u002A"
                }
                Key {
                    key: Qt.Key_Slash
                    text: "\u002F"
                }
                Key {
                    key: Qt.Key_AsciiCircum
                    text: "\u005E"
                }
                Key {
                    key: Qt.Key_Equal
                    text: "\u003D"
                }
                Key {
                    key: Qt.Key_Period
                    text: "\u002E"
                }
                Key {
                    key: Qt.Key_Ampersand
                    text: "\u0026"
                }
                Key {
                    key: Qt.Key_Comma
                    text: "\u002C"
                }
                Key {
                    enabled: false
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
//                EnterKey {
//                    weight: 283
//                    visible: ItzKeybordUtil.enterKeyVisible(InputContext.inputMethodHints)
//                }
            }

            KeyboardRow {
//                Key {
//                    weight: 204
//                    displayText: (page + 1) + "/" + numPages
//                    functionKey: true
//                    onClicked: page = (page + 1) % numPages
//                }

                FillerKey {
                    weight: 40
                }
                Key {
                    key: Qt.Key_ParenLeft
                    text: "\u0028"
                }
                Key {
                    key: Qt.Key_ParenRight
                    text: "\u0029"
                }
                Key {
                    key: Qt.Key_Less
                    text: "\u003C"
                }
                Key {
                    key: Qt.Key_Greater
                    text: "\u003E"
                }
                Key {
                    key: Qt.Key_Dollar
                    text: "\u0024"
                }
                Key {
                    key: Qt.Key_Percent
                    text: "\u0025"
                }
                Key {
                    key: Qt.Key_Exclam
                    text: "\u0021"
                }
                Key {
                    key: Qt.Key_At
                    text: "\u0040"
                }
                Key {
                    key: Qt.Key_Question
                    text: "\u003F"
                }
                EnterKey {
                    weight: 120
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
//                Key {
//                    weight: 204
//                    displayText: (page + 1) + "/" + numPages
//                    functionKey: true
//                    onClicked: page = (page + 1) % numPages
//                }
            }

            KeyboardRow {
                Key {
                    key: Qt.Key_NumberSign
                    text: "\u0023"
                }
                Key {
                    key: Qt.Key_QuoteDbl
                    text: "\u0022"
                }
                Key {
                    enabled: false
                }
                Key {
                    key: Qt.Key_Colon
                    text: "\u003A"
                }
                Key {
                    key: Qt.Key_Semicolon
                    text: "\u003B"
                }
                Key {
                    key: Qt.Key_AsciiTilde
                    text: "\u007E"
                }
                Key {
                    key: Qt.Key_BraceLeft
                    text: "\u007B"
                }
                Key {
                    key: Qt.Key_BraceRight
                    text: "\u007D"
                }
                Key {
                    key: Qt.Key_Backslash
                    text: "\u005C"
                }
                Key {
                    key: Qt.Key_BracketLeft
                    text: "\u005B"
                }
                Key {
                    key: Qt.Key_BracketRight
                    text: "\u005D"
                }
            }

            KeyboardRow {
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
//                ChangeLanguageKey {
//                }
                SpaceKey {
                    weight: 240
                }
                Key {
                    enabled: false
                }
                SymbolModeKey {
                    weight: 80
                    displayText: "ABC"
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
//                Key {
//                    key: Qt.Key_Left
//                    displayText: "\u2190"
//                    repeat: true
//                    noModifier: true
//                    functionKey: true
//                }
//                Key {
//                    key: Qt.Key_Right
//                    displayText: "\u2192"
//                    repeat: true
//                    noModifier: true
//                    functionKey: true
//                }
//                HideKeyboardKey {
//                    weight: 200
//                }
            }

        }
    }

    // symbol 2/2
    Component {
        id: page2

        KeyboardLayout {
            keyWeight: 160

            KeyboardRow {
                Key {
                    key: Qt.Key_Plus
                    text: "\u0040"
                }
                Key {
                    key: Qt.Key_Minus
                    text: "\u003F"
                }
                Key {
                    key: Qt.Key_Asterisk
                    text: "\u0023"
                }
                Key {
                    key: Qt.Key_Slash
                    text: "\u0022"
                }
                Key {
                    key: Qt.Key_AsciiCircum
                    text: "\u005F"
                }
                Key {
                    key: Qt.Key_Equal
                    text: "\u003A"
                }
                Key {
                    key: Qt.Key_Period
                    text: "\u003B"
                }
                Key {
                    key: Qt.Key_Ampersand
                    text: "\u007E"
                }
                Key {
                    key: Qt.Key_Comma
                    text: "\u007B"
                }
                Key {
                    key: Qt.Key_Comma
                    text: "\u007D"
                }
                BackspaceKey {
                }
            }

            KeyboardRow {
                FillerKey {
                    weight: 56
                }
                Key {
                    key: Qt.Key_yen
                    text: "\u005C"
                }
                Key {
                    key: 0x0ffffff3
                    displayText: "IF\nTHEN"
                    text: "\u0049\u0046\u0020\u0020\u0054\u0048\u0045\u004E\u000D\u000A\u000D\u000A\u0045\u004E\u0044\u0049\u0046\u000D\u000A"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffff4
                    displayText: "ELSE"
                    text: "\u0045\u004C\u0053\u0045\u000D\u000A"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffff5
                    displayText: "FOR\nTO"
                    text: "\u0046\u004F\u0052\u0020\u0020\u003D\u0020\u0020\u0054\u004F\u0020\u0020\u000D\u000A\u000D\u000A\u004E\u0045\u0058\u0054\u000D\u000A"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffff6
                    displayText: "WHILE\nWEND"
                    text: "\u0057\u0048\u0049\u004C\u0045\u0020\u000D\u000A\u000D\u000A\u0057\u0045\u004E\u0044\u000D\u000A"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffff7
                    displayText: "GOTO"
                    text: "\u0047\u004F\u0054\u004F\u0020"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffff8
                    displayText: "ALARM"
                    text: "\u0041\u004C\u0041\u0052\u004D\u0020"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: Qt.Key_BracketLeft
                    text: "\u005B"
                }
                Key {
                    key: Qt.Key_BracketRight
                    text: "\u005D"
                }
                EnterKey {
                    weight: 283
					visible: ItzKeybordUtil.enterKeyVisible(InputContext.inputMethodHints)
                }
            }

            KeyboardRow {
                Key {
                    weight: 204
                    displayText: (page + 1) + "/" + numPages
                    functionKey: true
                    onClicked: page = (page + 1) % numPages
                }
                Key {
                    key: 0x0ffffff9
                    displayText: "WAIT"
                    text: "\u0057\u0041\u0049\u0054\u0020"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffffa
                    displayText: "DELAY"
                    text: "\u0044\u0045\u004C\u0041\u0059\u0020"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffffb
                    displayText: "RIGHT\nY"
                    text: "\u0052\u0049\u0047\u0048\u0054\u0059\u000D\u000A"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffffc
                    displayText: "LEFT\nY"
                    text: "\u004C\u0045\u0046\u0054\u000D\u000A"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffffd
                    displayText: "MOVE\nP"
                    text: "\u004D\u004F\u0056\u0045\u0020\u0050\u002C"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0ffffffe
                    displayText: "MOVE\nL"
                    text: "\u004D\u004F\u0056\u0045\u0020\u004C\u002C"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: 0x0fffffff
                    displayText: "MOVE\nC"
                    text: "\u004D\u004F\u0056\u0045\u0020\u0043\u002C"
                    visible: ItzKeybordUtil.multiCharKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: Qt.Key_Up
                    displayText: "\u2191"
                    repeat: true
                    noModifier: true
                    functionKey: true
                    visible: ItzKeybordUtil.upDownKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    key: Qt.Key_Down
                    displayText: "\u2193"
                    repeat: true
                    noModifier: true
                    functionKey: true
                    visible: ItzKeybordUtil.upDownKeyVisible(InputContext.inputMethodHints)
                }
                Key {
                    weight: 204
                    displayText: (page + 1) + "/" + numPages
                    functionKey: true
                    onClicked: page = (page + 1) % numPages
                }
            }

            KeyboardRow {
                SymbolModeKey {
                    weight: 200
                    displayText: "ABC"
                }
                ChangeLanguageKey {
                }
                SpaceKey {
                    weight: 1300
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
                HideKeyboardKey {
                    weight: 200
                }
            }

        }
    }
}
