import QtQuick 2.12
import Enums 1.0

QtObject {
    function getPointNo(areaType, axis)
    {
        let pointNo = 0
        switch (areaType) {
        case Enums.AreaTypeDescentMaxMovement:  // 最大移動値
            pointNo = 253
            break
        case Enums.AreaTypeOutputSideMax:       // 取出側区域(最大)
            if ((axis >= 0) && (axis <= 2)) {
                pointNo = 221
            } else if ((axis === 3) || (axis === 4)) {
                pointNo = 223
            }
            break
        case Enums.AreaTypeOutputSideMin:       // 取出側区域(最小)
            if ((axis >= 0) && (axis <= 2)) {
                pointNo = 231
            }
            break
        case Enums.AreaTypeOpenSideMax:         // 開放側区域(最大)
            if ((axis >= 0) && (axis <= 4)) {
                pointNo = 222
            }
            break
        case Enums.AreaTypeOpenSideMin:         // 開放側区域(最小)
            if ((axis >= 0) && (axis <= 2)) {
                pointNo = 232
            }
            break
        case Enums.AreaTypeDescentMax:          // 下降待機(最大)
            if ((axis === 3) || (axis === 4)) {
                pointNo = 221
            }
            break
        }
        return pointNo
    }
}
