#include "touchmanager.h"

TouchManager::TouchManager(QObject *parent) : QObject(parent)
{

}

bool TouchManager::isPressedKey01() const
{
    return mIsPressedKey01;
}

void TouchManager::setIsPressedKey01(bool isPressed)
{
    if(mIsPressedKey01 != isPressed){
        mIsPressedKey01 = isPressed;
        emit isPressedKey01Changed();
    }
}

void TouchManager::startTouch()
{
    portWatcher = new PortWatcher();
    connect(portWatcher, SIGNAL(updatePortInfo(QBitArray)), this, SLOT(onUpdatePortInfo(QBitArray)));
    portWatcher->start();
}

void TouchManager::endTouch()
{
    portWatcher->quit();
    portWatcher->wait();
    disconnect(portWatcher, SIGNAL(updatePortInfo(QBitArray)), this, SLOT(onUpdatePortInfo(QBitArray)));
    delete portWatcher;
    portWatcher = nullptr;
}

void TouchManager::onUpdatePortInfo(QBitArray inputDataBuf)
{
    QList<CoreLibInputKey> inputKeyList;
    inputKeyList.push_back(CoreLibInputKey::fromInputDataBuffer(inputDataBuf));

    updateMembraneKey(&inputKeyList);
}

void TouchManager::updateMembraneKey(QList<CoreLibInputKey> *pInputKeyList)
{
    if((pInputKeyList == nullptr) || (pInputKeyList->size() == 0)) {
        return;
    }

    QList<int> sel0KeyNumberList = getConfirmedInputKeyNumber(pInputKeyList, CoreLibInputKey::KeySelect::Select0);
    QList<int> sel1KeyNumberList = getConfirmedInputKeyNumber(pInputKeyList, CoreLibInputKey::KeySelect::Select1);

    QList<int> confirmedInputKeyNumberList;
    confirmedInputKeyNumberList.append(sel0KeyNumberList);
    confirmedInputKeyNumberList.append(sel1KeyNumberList);

    AllKeyInfo allKeyInfo{ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };

    for(int i = 0; i < confirmedInputKeyNumberList.count(); i++) {
        int keyNumber = confirmedInputKeyNumberList.at(i);
        switch(keyNumber) {
        case INPUT_KEY_01:
            allKeyInfo.key01 = false;
            break;
        default:
            break;
        }
    }

    setIsPressedKey01(allKeyInfo.key01);
}

QList<int> TouchManager::getConfirmedInputKeyNumber(QList<CoreLibInputKey> *pInputKeyList, CoreLibInputKey::KeySelect select)
{
    QList<int> confirmedKeyNumberList;

    if((pInputKeyList == nullptr) || (pInputKeyList->size() == 0)) {
        return confirmedKeyNumberList;
    }

    for(int i = 0; i < pInputKeyList->size(); i++) {
        CoreLibInputKey inputKey = pInputKeyList->at(i);
        if(inputKey.getKeySelect() == select) {
            if(confirmedKeyNumberList.isEmpty()) {
                confirmedKeyNumberList = inputKey.getSwitchOnKeyList();
            } else {
                QList<int> oldSwitchOnList = inputKey.getSwitchOnKeyList();
                for(int i = 0; i < confirmedKeyNumberList.count(); i++) {
                    int keyNumber = confirmedKeyNumberList.at(i);
                    if(!oldSwitchOnList.contains(keyNumber)) {
                        confirmedKeyNumberList.removeAt(i);
                        i--;
                    }
                }
            }
        }
    }

    return confirmedKeyNumberList;
}
