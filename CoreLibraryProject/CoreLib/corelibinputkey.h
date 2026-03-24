#ifndef CORELIBINPUTKEY_H
#define CORELIBINPUTKEY_H

#include <QList>
#include <QBitArray>

#define INPUT_KEY_01 100
#define INPUT_KEY_02 101
#define INPUT_KEY_03 102
#define INPUT_KEY_04 103
#define INPUT_KEY_05 104
#define INPUT_KEY_06 105
#define INPUT_KEY_07 106
#define INPUT_KEY_08 107
#define INPUT_KEY_09 108
#define INPUT_KEY_10 109
#define INPUT_KEY_11 110
#define INPUT_KEY_12 111
#define INPUT_KEY_13 112
#define INPUT_KEY_14 113
#define INPUT_KEY_15 114
#define INPUT_KEY_16 115
#define INPUT_KEY_17 116
#define INPUT_KEY_18 117

const int mKeyNumberConvertTable[2][9] = {
    // bit number
    //      0             1             2             3             4             5             6             7             8
    { INPUT_KEY_01, INPUT_KEY_03, INPUT_KEY_05, INPUT_KEY_07, INPUT_KEY_09, INPUT_KEY_11, INPUT_KEY_13, INPUT_KEY_15, INPUT_KEY_17 },  // SEL0
    { INPUT_KEY_02, INPUT_KEY_04, INPUT_KEY_06, INPUT_KEY_08, INPUT_KEY_10, INPUT_KEY_12, INPUT_KEY_14, INPUT_KEY_16, INPUT_KEY_18 },  // SEL1
};

class CoreLibInputKey
{
public:
    enum KeySelect {
        InvalidSelect = 0,
        Select0,
        Select1
    };
    CoreLibInputKey();
    static CoreLibInputKey fromInputDataBuffer(QBitArray inputDataBuffer);
    void setKIN0(bool kin0);
    void setKIN1(bool kin1);
    void setKIN2(bool kin2);
    void setKIN3(bool kin3);
    void setKIN4(bool kin4);
    void setKIN5(bool kin5);
    void setKIN6(bool kin6);
    void setKIN7(bool kin7);
    void setKIN8(bool kin8);
    void setSEL0(bool sel0);
    void setSEL1(bool sel1);
    CoreLibInputKey::KeySelect getKeySelect();
    QList<int> getSwitchOnKeyList();

private:
    bool mKIN0 = true;  // 0: ON , 1: OFF
    bool mKIN1 = true;  // 0: ON , 1: OFF
    bool mKIN2 = true;  // 0: ON , 1: OFF
    bool mKIN3 = true;  // 0: ON , 1: OFF
    bool mKIN4 = true;  // 0: ON , 1: OFF
    bool mKIN5 = true;  // 0: ON , 1: OFF
    bool mKIN6 = true;  // 0: ON , 1: OFF
    bool mKIN7 = true;  // 0: ON , 1: OFF
    bool mKIN8 = true;  // 0: ON , 1: OFF
    bool mSEL0 = true;  // 0: Select , 1: Not Select
    bool mSEL1 = true;  // 0: Select , 1: Not Select
};

#endif // CORELIBINPUTKEY_H
