import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent

    Button {
        id: testButton
        text: "changeApplication"

        Connections {
            target: testButton
            onClicked: {
                // controller settings
                if(appMgr.isLinux()){
                    appMgr.changeApplication("/opt/SystemSettingApp/bin/SystemSettingApp", "--fromAppName ")
                }
                else{
                    appMgr.changeApplication("./SystemSettingApp", "--fromAppName ")
                }
            }
        }
    }
}
