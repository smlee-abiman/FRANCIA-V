#ifndef IOFORCEVIEWMODEL_H
#define IOFORCEVIEWMODEL_H

#include <bitset>

#include <QObject>
#include <QList>

#include "viewmodelbase.h"

#define PENSET_GROUP                ("8")
#define PENSET_GROUP_NUM            (8)
#define PENSET_OFFSET_IO_ADDRESS    (11)
#define PENSET_OFFSET_BANK          (11)
#define NUM_OF_IO_DATA_PAR_PAGE     (6)
#define NUM_OF_SWAP_CONFIG_PAR_PAGE (8)
#define NUM_OF_SWAP_DATA            (8)
#define FORCED_ON_HR                (0)
#define FORCED_ON_HR_END            (FORCED_ON_HR + 16) - 1
#define FORCED_OFF_HR               (496)
#define FORCED_OFF_HR_END           (FORCED_OFF_HR + 16) - 1
#define INVERSION_INSTRUCTION_HR    (992)
#define SWAP_INSTRUCTION_HR         (1008)
#define SWAP_INSTRUCTION_HR_END     (SWAP_INSTRUCTION_HR + 16) - 1

class IOSwapConfiguration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int diAddress READ diAddress CONSTANT)
    Q_PROPERTY(int bankNo READ bankNo CONSTANT)
    Q_PROPERTY(bool isValid READ isValid CONSTANT)
public:
    explicit IOSwapConfiguration(QObject *parent = nullptr);
    explicit IOSwapConfiguration(quint16 diAddress, quint16 bankNo, QObject *parent = nullptr);

    int diAddress() const { return m_diAddress; }
    int bankNo() const { return m_bankNo; }
    bool isValid() const { return (m_diAddress != 0xFFFF); }

    // for debug.
    void display();

signals:
    void ioSwapDataChanged(QObject* ioSwapData);

private:
    quint16 m_diAddress;
    quint16 m_bankNo;
};

class IOSwapData : public QObject
{
    Q_OBJECT
public:
    explicit IOSwapData(QObject *parent = nullptr);
    explicit IOSwapData(quint16 addressOffset, QObject *parent = nullptr);

    int addressOffset() const { return m_addressOffset; }
    int hrAddress() const { return SWAP_INSTRUCTION_HR + m_addressOffset; }
    bool inUsed() const { return (m_fromBankNo && m_toBankNo); }
    bool unUsed() const { return !inUsed(); }
    bool isSameFromBankNo(quint16 fromBankNo) { return (m_fromBankNo == fromBankNo); }
    bool isSameToBankNo(quint16 toBankNo) { return (m_toBankNo == toBankNo); }
    bool setFromBankNo(const quint16& fromBankNo, quint16& prevBankNo);
    bool setToBankNo(const quint16& toBankNo, quint16& prevBankNo);

    // for debug.
    void display();

private:
    quint16 m_addressOffset;
    quint16 m_fromBankNo;
    quint16 m_toBankNo;
};

class ForceIOData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int forcedDio READ forcedDio CONSTANT)
    Q_PROPERTY(bool value READ value NOTIFY valueChanged)
    Q_PROPERTY(bool forcedOn READ forcedOn NOTIFY forcedOnChanged)
    Q_PROPERTY(bool forcedOff READ forcedOff NOTIFY forcedOffChanged)
    Q_PROPERTY(bool invers READ invers NOTIFY inversChanged)
    Q_PROPERTY(int swapDi READ swapDi NOTIFY swapDiChanged)
public:
    explicit ForceIOData(QObject *parent = nullptr);
    explicit ForceIOData(quint16 forcedDio, quint16 releasedDo, quint16 inversDo, quint16 bankNo, QObject *parent = nullptr);

    bool value() const { return m_bits[0]; }
    bool forcedOn() const { return m_bits[1]; }
    bool forcedOff() const { return m_bits[2]; }
    bool invers() const { return m_bits[3]; }
    void setValue(bool value) { if (m_bits[0] == value) { return; } m_bits[0] = value; emit valueChanged(value); }
    void setForcedOn(bool forcedOn) { if (m_bits[1] == forcedOn) { return; } m_bits[1] = forcedOn; emit forcedOnChanged(forcedOn); }
    void setForcedOff(bool forcedOff) { if (m_bits[2] == forcedOff) { return; } m_bits[2] = forcedOff; emit forcedOffChanged(forcedOff); }
    void setInvers(bool invers) { if (m_bits[3] == invers) { return; } m_bits[3] = invers; emit inversChanged(invers); }

    quint16 forcedDio() const { return m_forcedDio; }
    quint16 releasedDo() const { return m_releasedDo; }
    quint16 inversDo() const { return m_inversDo; }
    quint16 bankNo() const { return m_bankNo; }

    int swapDi() const { return m_swapDi; }
    void setSwapDi(const quint16 swapDi) { if (m_swapDi == swapDi) { return; } m_swapDi = swapDi; emit swapDiChanged(swapDi); }

    // for debug.
    void display();

signals:
    void valueChanged(bool value);
    void forcedStateChanged(int forced);
    void releasedChanged(int released);
    void forcedOnChanged(bool forcedOn);
    void forcedOffChanged(bool forcedOff);
    void inversChanged(bool forcedOff);
    void swapDiChanged(int swapDi);

private:
    std::bitset<4> m_bits;    // 0:value, 1:forcedON, 2:forcedOFF, 3:invers.
    quint16 m_forcedDio;
    quint16 m_releasedDo;
    quint16 m_inversDo;
    quint16 m_bankNo;
    quint16 m_swapDi;
};

class IOForceViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> forceIoDataTbl READ forceIoDataTbl NOTIFY forceIoDataTblChanged)
    Q_PROPERTY(QList<QObject*> ioSwapConfigTbl READ ioSwapConfigTbl NOTIFY ioSwapConfigTblChanged)
    Q_PROPERTY(int ioSwapConfigTblsIndex MEMBER m_ioSwapConfigTblsIndex WRITE setIoSwapConfigTblsIndex NOTIFY ioSwapConfigTblsIndexChanged)
    Q_PROPERTY(bool canSwapIO READ canSwapIO WRITE setCanSwapIO NOTIFY canSwapIOChanged)
    // penset.
    Q_PROPERTY(int category1StartPage READ category1StartPage CONSTANT)
    Q_PROPERTY(int category2StartPage READ category2StartPage CONSTANT)
    Q_PROPERTY(int category3StartPage READ category3StartPage CONSTANT)
    Q_PROPERTY(QString ioNameTextColorRBG READ ioNameTextColorRBG CONSTANT)
    Q_PROPERTY(QString pageTextColorRGB   READ pageTextColorRGB   CONSTANT)

public:
    explicit IOForceViewModel(QObject *object = nullptr);
    explicit IOForceViewModel(ResourceManager *resource, CommunicationThread * comm);
    virtual ~IOForceViewModel() override;
    QList<QObject*> forceIoDataTbl() { return m_forceIoDataTbl; }
    QList<QObject*> ioSwapConfigTbl() { return m_ioSwapConfigTbls[m_ioSwapConfigTblsIndex]; }
    void setIoSwapConfigTblsIndex(int ioSwapConfigTblsIndex) {
        // qDebug("call ioSwapConfigTblsIndex:%d, m_ioSwapConfigTbls.size():%d", ioSwapConfigTblsIndex, m_ioSwapConfigTbls.size());
        if ((ioSwapConfigTblsIndex < 0) ||
            (m_ioSwapConfigTbls.size() <= ioSwapConfigTblsIndex)) {
            return;
        }
        if (m_ioSwapConfigTblsIndex == ioSwapConfigTblsIndex) {
            return;
        }
        m_ioSwapConfigTblsIndex = ioSwapConfigTblsIndex;
        emit ioSwapConfigTblChanged(ioSwapConfigTbl());
        emit ioSwapConfigTblsIndexChanged(ioSwapConfigTblsIndex);
    }
    bool canSwapIO() const { return m_canSwapIO; }
    void setCanSwapIO(bool canSwapIO) { if (m_canSwapIO == canSwapIO) { return; } m_canSwapIO = canSwapIO; emit canSwapIOChanged(canSwapIO); }
    int category1StartPage() const { return m_category1StartPage; }
    int category2StartPage() const { return m_category2StartPage; }
    int category3StartPage() const { return m_category3StartPage; }
    QString ioNameTextColorRBG() const { return m_ioNameTextColorRBG; }
    QString pageTextColorRGB() const { return m_pageTextColorRGB; }

    Q_INVOKABLE void forciblyTurnOn(quint16 index);
    Q_INVOKABLE void forciblyTurnOff(quint16 index);
    Q_INVOKABLE void releaseForcedIO(quint16 index);
    Q_INVOKABLE void inversTurnOn(quint16 index);
    Q_INVOKABLE void inversTurnOff(quint16 index);
    Q_INVOKABLE void swapIO(QObject *forceIoPtr, QObject *ioSwapConfigPtr);
    Q_INVOKABLE void releaseSwappedIO(QObject *forceIoPtr);
    Q_INVOKABLE QList<bool> getIoSwapConfigTblsStatus(QObject *forceIoPtr);

signals:
    void forceIoDataTblChanged(QList<QObject*>);
    void ioSwapConfigTblChanged(QList<QObject*>);
    void ioSwapConfigTblsIndexChanged(int);
    void canSwapIOChanged(bool);

public slots:
    void onLanguageChanged(int language);
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command,
        CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

protected:
    void onActivate() override;
    void onDeactivate() override;

private:
    void init();
    void readPenSetData();
    void readPenSetForceIoDataTbl();
    void readPenSetIoSwapConfigurationTbls();
    void readPenSetIoSwapConfigurationTbl(QList<QObject *> &dataTbl, const int numOfPageRow, const int startRow);
    void startModbusPeriodic();
    void stopModbusPeriodic();
    void startModbusOneShot();
    void stopModbusOneShot();
    void writeModbusDo(quint16 dio, uchar writeData);
    void writeModbusHR(quint16 hrAddress, QList<int> &writeData);
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void readModbusHR(quint16 hrAddress, quint16 range);
    void modbusGetForcedOn(quint16 index = 0, quint16 range = 16);
    void modbusGetForcedOff(quint16 index = 0, quint16 range = 16);
    void modbusGetIOSwap(quint16 hrAddress = SWAP_INSTRUCTION_HR, quint16 range = NUM_OF_SWAP_DATA);
    void modbusWriteIOSwap(quint16 hrAddress, quint16 fromBankNo, quint16 toBankNo);
    void updatePeriodicData();
    void updateForcedOn(stModBusCommunicationData &command);
    void updateForcedOff(stModBusCommunicationData &command);
    void updateStatus(stModBusCommunicationData &command, quint16 baseHrAddress, void (ForceIOData::*setMethod)(bool));
    void updateSwapData(stModBusCommunicationData &command);
    IOSwapData *findIoSwapData(ForceIOData* forceIo);
    IOSwapData *freeIoSwapData();
    ForceIOData *findForceIoData(quint16 bankNo);
    IOSwapConfiguration *findIoSwapConfiguration(quint16 bankNo);

private:
    bool mIsModbusConnectOneShot;
    int m_numOfPage;

    QList<QObject*> m_forceIoDataTbl;           // ForceIOData.
    QList<QList<QObject*>> m_ioSwapConfigTbls;  // IOSwapConfiguration:[0]input, [1]output, [2].....
    int m_ioSwapConfigTblsIndex;
    QList<QObject*> m_ioSwapDataTbl;            // IOSwapData
    bool m_canSwapIO;
    int m_category1StartPage;
    int m_category2StartPage;
    int m_category3StartPage;
    QString m_ioNameTextColorRBG;
    QString m_pageTextColorRGB;
};

#endif // IOFORCEVIEWMODEL_H
