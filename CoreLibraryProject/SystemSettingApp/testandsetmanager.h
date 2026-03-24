#ifndef TESTANDSETMANAGER_H
#define TESTANDSETMANAGER_H

#include <QObject>
#include "outputtestthread.h"
#include "portwatcher.h"
#include "corelibsystemcontrol.h"
#include "corelibsdcontrol.h"
#include "corelibdisplaycontrol.h"
#include "corelibinputkey.h"
#include "jogwatcher.h"
#include "corelibjogcontrol.h"

class TestAndSetManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isExecuteInputTest READ isExecuteInputTest WRITE setIsExecuteInputTest NOTIFY isExecuteInputTestChanged)
    Q_PROPERTY(bool isExecuteOutputTest READ isExecuteOutputTest WRITE setIsExecuteOutputTest NOTIFY isExecuteOutputTestChanged)
    Q_PROPERTY(int backLightValue READ backLightValue WRITE setBackLightValue NOTIFY backLightValueChanged)
    Q_PROPERTY(bool isPressedKey01 READ isPressedKey01 WRITE setIsPressedKey01 NOTIFY isPressedKey01Changed)
    Q_PROPERTY(bool isPressedKey02 READ isPressedKey02 WRITE setIsPressedKey02 NOTIFY isPressedKey02Changed)
    Q_PROPERTY(bool isPressedKey03 READ isPressedKey03 WRITE setIsPressedKey03 NOTIFY isPressedKey03Changed)
    Q_PROPERTY(bool isPressedKey04 READ isPressedKey04 WRITE setIsPressedKey04 NOTIFY isPressedKey04Changed)
    Q_PROPERTY(bool isPressedKey05 READ isPressedKey05 WRITE setIsPressedKey05 NOTIFY isPressedKey05Changed)
    Q_PROPERTY(bool isPressedKey06 READ isPressedKey06 WRITE setIsPressedKey06 NOTIFY isPressedKey06Changed)
    Q_PROPERTY(bool isPressedKey07 READ isPressedKey07 WRITE setIsPressedKey07 NOTIFY isPressedKey07Changed)
    Q_PROPERTY(bool isPressedKey08 READ isPressedKey08 WRITE setIsPressedKey08 NOTIFY isPressedKey08Changed)
    Q_PROPERTY(bool isPressedKey09 READ isPressedKey09 WRITE setIsPressedKey09 NOTIFY isPressedKey09Changed)
    Q_PROPERTY(bool isPressedKey10 READ isPressedKey10 WRITE setIsPressedKey10 NOTIFY isPressedKey10Changed)
    Q_PROPERTY(bool isPressedKey11 READ isPressedKey11 WRITE setIsPressedKey11 NOTIFY isPressedKey11Changed)
    Q_PROPERTY(bool isPressedKey12 READ isPressedKey12 WRITE setIsPressedKey12 NOTIFY isPressedKey12Changed)
    Q_PROPERTY(bool isPressedKey13 READ isPressedKey13 WRITE setIsPressedKey13 NOTIFY isPressedKey13Changed)
    Q_PROPERTY(bool isPressedKey14 READ isPressedKey14 WRITE setIsPressedKey14 NOTIFY isPressedKey14Changed)
    Q_PROPERTY(bool isPressedKey15 READ isPressedKey15 WRITE setIsPressedKey15 NOTIFY isPressedKey15Changed)
    Q_PROPERTY(bool isPressedKey16 READ isPressedKey16 WRITE setIsPressedKey16 NOTIFY isPressedKey16Changed)
    Q_PROPERTY(bool isPressedKey17 READ isPressedKey17 WRITE setIsPressedKey17 NOTIFY isPressedKey17Changed)
    Q_PROPERTY(bool isPressedKey18 READ isPressedKey18 WRITE setIsPressedKey18 NOTIFY isPressedKey18Changed)
    Q_PROPERTY(bool isSelect01 READ isSelect01 WRITE setIsSelect01 NOTIFY isSelect01Changed)
    Q_PROPERTY(bool isSelect02 READ isSelect02 WRITE setIsSelect02 NOTIFY isSelect02Changed)
    Q_PROPERTY(bool isSelect03 READ isSelect03 WRITE setIsSelect03 NOTIFY isSelect03Changed)
    Q_PROPERTY(bool jogDialSwStatus READ jogDialSwStatus NOTIFY jogDialSwStatusChanged)
    Q_PROPERTY(bool deadmanSwStatus READ deadmanSwStatus NOTIFY deadmanSwStatusChanged)
    Q_PROPERTY(QString macAddress READ macAddress WRITE setMacAddress NOTIFY macAddressChanged)
    Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(QString subnetMask READ subnetMask WRITE setSubnetMask NOTIFY subnetMaskChanged)
    Q_PROPERTY(QString serialNo READ serialNo WRITE setSerialNo NOTIFY serialNoChanged)
    Q_PROPERTY(int modbusErrorCount READ modbusErrorCount WRITE setModbusErrorCount NOTIFY modbusErrorCountChanged)
    Q_PROPERTY(QString jogCounter READ jogCounter WRITE setJogCounter NOTIFY jogCounterChanged)

public:
    explicit TestAndSetManager(QObject *parent = nullptr);
    void startTestAndSet();
    void endTestAndSet();
    QString getFromAppName();
    void setFromAppName(QString fromAppName);

    // Property
    bool isExecuteInputTest() const;
    void setIsExecuteInputTest(bool);
    bool isExecuteOutputTest() const;
    void setIsExecuteOutputTest(bool);
    int backLightValue() const;
    void setBackLightValue(int);
    bool isPressedKey01() const;
    void setIsPressedKey01(bool);
    bool isPressedKey02() const;
    void setIsPressedKey02(bool);
    bool isPressedKey03() const;
    void setIsPressedKey03(bool);
    bool isPressedKey04() const;
    void setIsPressedKey04(bool);
    bool isPressedKey05() const;
    void setIsPressedKey05(bool);
    bool isPressedKey06() const;
    void setIsPressedKey06(bool);
    bool isPressedKey07() const;
    void setIsPressedKey07(bool);
    bool isPressedKey08() const;
    void setIsPressedKey08(bool);
    bool isPressedKey09() const;
    void setIsPressedKey09(bool);
    bool isPressedKey10() const;
    void setIsPressedKey10(bool);
    bool isPressedKey11() const;
    void setIsPressedKey11(bool);
    bool isPressedKey12() const;
    void setIsPressedKey12(bool);
    bool isPressedKey13() const;
    void setIsPressedKey13(bool);
    bool isPressedKey14() const;
    void setIsPressedKey14(bool);
    bool isPressedKey15() const;
    void setIsPressedKey15(bool);
    bool isPressedKey16() const;
    void setIsPressedKey16(bool);
    bool isPressedKey17() const;
    void setIsPressedKey17(bool);
    bool isPressedKey18() const;
    void setIsPressedKey18(bool);
    bool isSelect01() const;
    void setIsSelect01(bool);
    bool isSelect02() const;
    void setIsSelect02(bool);
    bool isSelect03() const;
    void setIsSelect03(bool);
    bool jogDialSwStatus() const { return mJogDialSwStatus; }
    void setJogDialSwStatus(bool);
    bool deadmanSwStatus() const { return mDeadmanSwStatus; }
    void setDeadmanSwStatus(bool);
    QString macAddress() const;
    void setMacAddress(QString);
    QString ipAddress() const;
    void setIpAddress(QString);
    QString subnetMask() const;
    void setSubnetMask(QString);
    QString serialNo() const;
    void setSerialNo(QString);
    int modbusErrorCount() const;
    void setModbusErrorCount(int);
    QString jogCounter() const;
    void setJogCounter(QString);

#ifdef REAL_TARGET
#else
    Q_INVOKABLE void incrementEmulatedJogCount() {
        if(jogWatcher != nullptr) {
            jogWatcher->incrementEmulatedJogCount();
        }
    }

    Q_INVOKABLE void decrementEmulatedJogCount() {
        if(jogWatcher != nullptr) {
            jogWatcher->decrementEmulatedJogCount();
        }
    }

    Q_INVOKABLE void emulatedKey01() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey01();
        }
    }

    Q_INVOKABLE void emulatedKey02() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey02();
        }
    }

    Q_INVOKABLE void emulatedKey03() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey03();
        }
    }

    Q_INVOKABLE void emulatedKey04() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey04();
        }
    }

    Q_INVOKABLE void emulatedKey05() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey05();
        }
    }

    Q_INVOKABLE void emulatedKey06() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey06();
        }
    }

    Q_INVOKABLE void emulatedKey07() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey07();
        }
    }

    Q_INVOKABLE void emulatedKey08() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey08();
        }
    }

    Q_INVOKABLE void emulatedKey09() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey09();
        }
    }

    Q_INVOKABLE void emulatedKey10() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey10();
        }
    }

    Q_INVOKABLE void emulatedKey11() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey11();
        }
    }

    Q_INVOKABLE void emulatedKey12() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey12();
        }
    }

    Q_INVOKABLE void emulatedKey13() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey13();
        }
    }

    Q_INVOKABLE void emulatedKey14() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey14();
        }
    }

    Q_INVOKABLE void emulatedKey15() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey15();
        }
    }

    Q_INVOKABLE void emulatedKey16() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey16();
        }
    }

    Q_INVOKABLE void emulatedKey17() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey17();
        }
    }

    Q_INVOKABLE void emulatedKey18() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKey18();
        }
    }

    Q_INVOKABLE void emulatedKeyAllOff() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedKeyAllOff();
        }
    }

    Q_INVOKABLE void toggleJogDialSwStatus() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedJogDialSwStatus(!mJogDialSwStatus);
        }
    }

    Q_INVOKABLE void toggleDeadmanSwStatus() {
        if(portWatcher != nullptr) {
            portWatcher->emulatedDeadmanSwStatus(!mDeadmanSwStatus);
        }
    }
#endif

signals:
    void isExecuteInputTestChanged();
    void isExecuteOutputTestChanged();
    void backLightValueChanged();
    void isPressedKey01Changed();
    void isPressedKey02Changed();
    void isPressedKey03Changed();
    void isPressedKey04Changed();
    void isPressedKey05Changed();
    void isPressedKey06Changed();
    void isPressedKey07Changed();
    void isPressedKey08Changed();
    void isPressedKey09Changed();
    void isPressedKey10Changed();
    void isPressedKey11Changed();
    void isPressedKey12Changed();
    void isPressedKey13Changed();
    void isPressedKey14Changed();
    void isPressedKey15Changed();
    void isPressedKey16Changed();
    void isPressedKey17Changed();
    void isPressedKey18Changed();
    void isSelect01Changed();
    void isSelect02Changed();
    void isSelect03Changed();
    void macAddressChanged();
    void ipAddressChanged();
    void subnetMaskChanged();
    void serialNoChanged();
    void modbusErrorCountChanged();
    void jogCounterChanged();
    void jogDialSwStatusChanged(bool);
    void deadmanSwStatusChanged(bool);

public slots:
    void onInputTestExecute();
    void outputTestExecute();
    void onBackLightCountDown();
    void onBackLightCountUp();
    void onUpdatePortInfo(QBitArray inputDataBuf);
    void onRetClicked(QString ipAddr, QString subnetMask);
    void onUpdateJogInfo(QString jogCounterInfo);

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

    struct SelectSwInfo {
        bool select01;
        bool select02;
        bool select03;
    };

    QString getEthernetInterfaceName();
    void loadNetworkSetting();
    void loadSerialNo();
    void loadBackLightSetting();
    int loadModbusErrorCountFile(QString appName);
    void updateMembraneKey(QList<CoreLibInputKey> *pInputKeyList);
    QList<int> getConfirmedInputKeyNumber(QList<CoreLibInputKey> *pInputKeyList, CoreLibInputKey::KeySelect select);

    void updateJogDialSwStatus(const QBitArray &inputDataBuf);
    void updateDeadmanSwStatus(const QBitArray &inputDataBuf);

private:
    OutputTestThread outputTestThread;
    PortWatcher *portWatcher = nullptr;
    QList<QBitArray> mInputDataBuffer;
    CoreLibSystemControl mCoreLibSystemControl;
    CoreLibSdControl mSdControl;
    CoreLibDisPlayControl mDisplayControl;
    QList<CoreLibInputKey> mInputKeyList;
    JogWatcher *jogWatcher = nullptr;

    // Property
    bool mIsExecuteInputTest = false;
    bool mIsExecuteOutputTest = false;
    int mBackLightValue = 8;
    bool mIsPressedKey01 = true;
    bool mIsPressedKey02 = true;
    bool mIsPressedKey03 = true;
    bool mIsPressedKey04 = true;
    bool mIsPressedKey05 = true;
    bool mIsPressedKey06 = true;
    bool mIsPressedKey07 = true;
    bool mIsPressedKey08 = true;
    bool mIsPressedKey09 = true;
    bool mIsPressedKey10 = true;
    bool mIsPressedKey11 = true;
    bool mIsPressedKey12 = true;
    bool mIsPressedKey13 = true;
    bool mIsPressedKey14 = true;
    bool mIsPressedKey15 = true;
    bool mIsPressedKey16 = true;
    bool mIsPressedKey17 = true;
    bool mIsPressedKey18 = true;
    bool mIsSelect01 = true;
    bool mIsSelect02 = true;
    bool mIsSelect03 = true;
    bool mJogDialSwStatus = false;
    bool mDeadmanSwStatus = false;
    QString mMacAddress = "";
    QString mIpAddress = "";
    QString mSubnetMask = "";
    QString mSerialNo = "";
    int mModbusErrorCount = 0;
    QString mFromAppName = "";
    QString mJogCounter = "0";

};

#endif // TESTANDSETMANAGER_H
