import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: FunctionMenuDialog.qml
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

Dialog{
    id: func_dialog
    width: 600
    height: 400
    modal: true
    signal menuOperate(int id)

    // ✅ 현재 유저 레벨
    property int userLevel: commonViewModel.userLevel

    // ✅ 전체 메뉴 정의 (minLevel: 이 레벨 이상이면 표시)
    property var menuListAll: [
        { label: resourceManager.label(21, 8, resourceManager.language),  menuId: 5,  minLevel: 3 },
        { label: resourceManager.label(21, 9, resourceManager.language),  menuId: 6,  minLevel: 3 },
        { label: resourceManager.label(21,10, resourceManager.language),  menuId: 7,  minLevel: 2 },
        { label: resourceManager.label(21,11, resourceManager.language),  menuId: 8,  minLevel: 4 },
        { label: resourceManager.label(21,12, resourceManager.language),  menuId: 9,  minLevel: 2 },
        { label: resourceManager.label(21,13, resourceManager.language),  menuId: 10, minLevel: 3 },
        { label: resourceManager.label(21,14, resourceManager.language),  menuId: 11, minLevel: 3 },
        { label: resourceManager.label(21,15, resourceManager.language),  menuId: 12, minLevel: 0 }
    ]

    // ✅ “보여줄 메뉴만” 필터링해서 Grid가 빈칸 없이 좌상단부터 채워지게 함
    property var menuListVisible: []

    function rebuildVisibleMenuList(){
        var out = []
        for (var i = 0; i < menuListAll.length; i++){
            var m = menuListAll[i]
            // ✅ userLevel이 minLevel보다 작으면 숨김(=리스트에서 제외)
            if (userLevel >= m.minLevel){
                out.push(m)
            }
        }
        menuListVisible = out
    }

    onOpened: rebuildVisibleMenuList()
    onUserLevelChanged: rebuildVisibleMenuList()

    background: Rectangle{
        anchors.centerIn: parent
        width: func_dialog.width
        height: func_dialog.height
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
    }

    Rectangle {
        width: parent.width + 18
        height: 50
        x: -9
        y: -9
        color: "#999999"

        Text {
            anchors.centerIn: parent
            text: "FUNCTION"
            font.pixelSize: 24
            font.bold: true
            color: "#ffffff"
        }
    }

    Grid{
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 20
        columns: 3
        spacing: 10

        Repeater{
            model: func_dialog.menuListVisible

            Button{
                width: 170
                height: 90
                text: modelData.label
                font.pixelSize: 22
                font.bold: true

                onClicked: {
                    menuOperate(modelData.menuId)
                    func_dialog.close()
                }
            }
        }
    }
}
