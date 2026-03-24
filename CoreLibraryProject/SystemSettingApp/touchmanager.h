#ifndef TOUCHMANAGER_H
#define TOUCHMANAGER_H

#include <QObject>
#include "portwatcher.h"
#include "corelibinputkey.h"

class TouchManager : public QObject
{
    Q_OBJECT
    // trueが非押下状態
    Q_PROPERTY(bool isPressedKey01 READ isPressedKey01 WRITE setIsPressedKey01 NOTIFY isPressedKey01Changed)

public:
    explicit TouchManager(QObject *parent = nullptr);
    void startTouch();
    void endTouch();

    // Property
    bool isPressedKey01() const;
    void setIsPressedKey01(bool);

#ifdef REAL_TARGET
#else

    Q_INVOKABLE void emulatedKey01() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey01();
        }
    }

    Q_INVOKABLE void emulatedKeyAllOff() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKeyAllOff();
        }
    }

#endif

signals:
    void isPressedKey01Changed();

public slots:
    void onUpdatePortInfo(QBitArray inputDataBuf);

private:
    struct AllKeyInfo {
        bool key01;
        bool key02;
        bool key03;
        bool key04;
        bool key05;
        bool key06;
        bool key07;
        bool key08;
        bool key09;
        bool key10;
        bool key11;
        bool key12;
        bool key13;
        bool key14;
        bool key15;
        bool key16;
        bool key17;
        bool key18;
    };

    void updateMembraneKey(QList<CoreLibInputKey> *pInputKeyList);
    QList<int> getConfirmedInputKeyNumber(QList<CoreLibInputKey> *pInputKeyList, CoreLibInputKey::KeySelect select);

private:
    PortWatcher *portWatcher = nullptr;

    // Property
    // trueが非押下状態
    bool mIsPressedKey01 = true;

};


#endif // TOUCHMANAGER_H
