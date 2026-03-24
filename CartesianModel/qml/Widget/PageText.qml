import QtQuick 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: PageText.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.19
* Worker: SH.HONG
**************************************************************************************/
Text {
    property int page: 1
    property int totalPage: 1

    function toStringPage(currentPage, totalPage)
    {
        let ret = currentPage
        ret += "\n/\n"
        ret += totalPage
        return ret
    }

    width: 76
    height: 76
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.pixelSize: 22
    text: toStringPage(page, totalPage)
}
