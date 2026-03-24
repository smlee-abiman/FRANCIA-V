import QtQuick 2.0

Item {
    objectName: "MasterView"
    signal loaderItemChanged(string source)
    signal componentDestructed(string source)
    signal folderSelected(int index)
    signal folderOpen(string folderPath, int depth)
    signal fileSelected(int index)
    signal childFolderOpened(int index)
    signal inputTestExecute()
    signal outputTestExecute()
    signal retClicked(string ipAddr, string subnetMask)
    signal calibrationClicked()
    signal executeClicked(string currentPath)
    signal directionClicked()
    signal createFolderClicked()
    signal eraceFolderClicked()
    signal displaySelectClicked()
    signal touchClicked()
    signal exitTouch()
    signal backLightCountDown()
    signal backLightCountUp()

    Loader {
        id: pageLoader
        anchors.fill: parent
        source: mainWindowManager.loaderSource
        onItemChanged: { loaderItemChanged(pageLoader.source) }
    }

    Connections {
        target: pageLoader.item
        onComponentDestructed: { componentDestructed(source) }
        onFolderSelected: { folderSelected(index) }
        onFolderOpen: { folderOpen(folderPath, depth) }
        onFileSelected: { fileSelected(index) }
        onChildFolderOpened: { childFolderOpened(index) }
        onShowColorPattern: { pageLoader.source = "ColorPattern.qml" }
        onCloseColorPattern: { pageLoader.source = "TestAndSetPage.qml" }
        onInputTestExecute: { inputTestExecute() }
        onOutputTestExecute: { outputTestExecute() }
        onRetClicked: { retClicked(ipAddr, subnetMask); pageLoader.source = "SystemSettingPage.qml" }
        onCalibrationClicked: { calibrationClicked() }
        onTestAndSetClicked: { pageLoader.source = "TestAndSetPage.qml" }
        onExecuteClicked: { executeClicked(currentPath) }
        onDirectionClicked: { directionClicked() }
        onCreateFolderClicked: { createFolderClicked() }
        onEraceFolderClicked: { eraceFolderClicked() }
        onDisplaySelectClicked: { displaySelectClicked() }
        onTouchClicked: { pageLoader.source = "Touch.qml" }
        onExitTouch: { pageLoader.source = "SystemSettingPage.qml" }
        onBackLightCountDown: { backLightCountDown() }
        onBackLightCountUp: { backLightCountUp() }
    }
}
