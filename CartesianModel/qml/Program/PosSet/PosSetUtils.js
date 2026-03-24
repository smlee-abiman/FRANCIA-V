.pragma library

const INVALID_POSITION_VALUE = 0x81000000
const AXIS_LABEL_OFFSET = [0, // Y軸
                1,  // MX軸
                2,  // SX軸
                3,  // MZ軸
                4,  // SZ軸
                5,  // 6軸
                6,  // 7軸
                7]  // 8軸

function getManualSpeedValue(speed, limitSetting)
{
    let ret = speed
    if (ret >= 0 && ret <= 6) {
        if ((ret === 0) && (limitSetting & 0x01) === 0) {
            ret = 1
        }
        if ((ret === 1) && (limitSetting & 0x02) === 0) {
            ret = 2
        }
    } else {
        ret = 2
    }
    return ret
}

function getDecrementedManualSpeedValue(speed, limitSetting, maxValue)
{
    let ret = speed
    if ((ret === 1) && (limitSetting & 0x02) === 0) {
        ret = 0
    }
    return ret
}

function axisLabelIndex(axisIndex)
{
    if (axisIndex < 0 || axisIndex >= AXIS_LABEL_OFFSET.length)
        return 0
    return AXIS_LABEL_OFFSET[axisIndex]
}
