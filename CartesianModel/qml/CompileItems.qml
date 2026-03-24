import QtQuick 2.0
import "Monitor"
import "Program"
import "Program/PosSet"
import "Program/Stack"
import "Program/Program"
import "Record"
import "Widget"
import "Functions/MechanicalConfig/DriveParameters"

Item {
    // 初回右下タブ画面遷移時間短縮対応.
    // 初回右下タブ画面遷移時にSet.qmlのコンパイルに時間がかかっていた.
    // ここにSet.qmlを配置してあらかじめコンパイルしておくことで初回右下タブ画面遷移時間を短縮する.
    // このSet{}を使用して表示はしない.
    Component { RecordPage{} }
    Component { Alarm_2{} }
    Component { Alarm_3{} }
    Component { Driver_2 {} }
    Component { DrvParamSimpleData {} }
}
