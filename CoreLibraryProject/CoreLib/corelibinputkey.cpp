#include "corelibinputkey.h"

CoreLibInputKey::CoreLibInputKey()
{

}

CoreLibInputKey CoreLibInputKey::fromInputDataBuffer(QBitArray inputDataBuffer)
{
    CoreLibInputKey inputKey;
    int inputDataBufferSize = inputDataBuffer.size();

    if(inputDataBufferSize != 32) {
        // invalid inputDataBuffer data
        return inputKey;
    }

    for(int i = 0; i < inputDataBufferSize; i++) {
        bool value = inputDataBuffer.at(i);
        switch(i) {
        case 0:
            inputKey.setKIN0(value);
            break;
        case 1:
            inputKey.setKIN1(value);
            break;
        case 2:
            inputKey.setKIN2(value);
            break;
        case 3:
            inputKey.setKIN3(value);
            break;
        case 4:
            inputKey.setKIN4(value);
            break;
        case 5:
            inputKey.setKIN5(value);
            break;
        case 6:
            inputKey.setKIN6(value);
            break;
        case 7:
            inputKey.setKIN7(value);
            break;
        case 8:
            inputKey.setKIN8(value);
            break;
        case 14:
            inputKey.setSEL1(value);
            break;
        case 15:
            inputKey.setSEL0(value);
            break;
        }
    }

    return inputKey;
}

void CoreLibInputKey::setKIN0(bool kin0)
{
    mKIN0 = kin0;
}

void CoreLibInputKey::setKIN1(bool kin1)
{
    mKIN1 = kin1;
}

void CoreLibInputKey::setKIN2(bool kin2)
{
    mKIN2 = kin2;
}

void CoreLibInputKey::setKIN3(bool kin3)
{
    mKIN3 = kin3;
}

void CoreLibInputKey::setKIN4(bool kin4)
{
    mKIN4 = kin4;
}

void CoreLibInputKey::setKIN5(bool kin5)
{
    mKIN5 = kin5;
}

void CoreLibInputKey::setKIN6(bool kin6)
{
    mKIN6 = kin6;
}

void CoreLibInputKey::setKIN7(bool kin7)
{
    mKIN7 = kin7;
}

void CoreLibInputKey::setKIN8(bool kin8)
{
    mKIN8 = kin8;
}

void CoreLibInputKey::setSEL0(bool sel0)
{
    mSEL0 = sel0;
}

void CoreLibInputKey::setSEL1(bool sel1)
{
    mSEL1 = sel1;
}

CoreLibInputKey::KeySelect CoreLibInputKey::getKeySelect()
{
    KeySelect keySelect = KeySelect::InvalidSelect;

    if(mSEL0 == mSEL1) {
        return keySelect;
    }

    if(!mSEL0) {
        keySelect = KeySelect::Select0;
    } else if(!mSEL1) {
        keySelect = KeySelect::Select1;
    }

    return keySelect;
}

QList<int> CoreLibInputKey::getSwitchOnKeyList()
{
    QList<int> switchOnKeyList;
    KeySelect keySelect = getKeySelect();

    if(keySelect == KeySelect::InvalidSelect) {
        // Invalid statement
        // return empty list.
        return switchOnKeyList;
    }

    if(keySelect == KeySelect::Select0) {
        // SEL0 selected.
        if(!mKIN0) switchOnKeyList << mKeyNumberConvertTable[0][0];
        if(!mKIN1) switchOnKeyList << mKeyNumberConvertTable[0][1];
        if(!mKIN2) switchOnKeyList << mKeyNumberConvertTable[0][2];
        if(!mKIN3) switchOnKeyList << mKeyNumberConvertTable[0][3];
        if(!mKIN4) switchOnKeyList << mKeyNumberConvertTable[0][4];
        if(!mKIN5) switchOnKeyList << mKeyNumberConvertTable[0][5];
        if(!mKIN6) switchOnKeyList << mKeyNumberConvertTable[0][6];
        if(!mKIN7) switchOnKeyList << mKeyNumberConvertTable[0][7];
        if(!mKIN8) switchOnKeyList << mKeyNumberConvertTable[0][8];
    }

    if(keySelect == KeySelect::Select1) {
        // SEL1 selected.
        if(!mKIN0) switchOnKeyList << mKeyNumberConvertTable[1][0];
        if(!mKIN1) switchOnKeyList << mKeyNumberConvertTable[1][1];
        if(!mKIN2) switchOnKeyList << mKeyNumberConvertTable[1][2];
        if(!mKIN3) switchOnKeyList << mKeyNumberConvertTable[1][3];
        if(!mKIN4) switchOnKeyList << mKeyNumberConvertTable[1][4];
        if(!mKIN5) switchOnKeyList << mKeyNumberConvertTable[1][5];
        if(!mKIN6) switchOnKeyList << mKeyNumberConvertTable[1][6];
        if(!mKIN7) switchOnKeyList << mKeyNumberConvertTable[1][7];
        if(!mKIN8) switchOnKeyList << mKeyNumberConvertTable[1][8];
    }

    return switchOnKeyList;
}
