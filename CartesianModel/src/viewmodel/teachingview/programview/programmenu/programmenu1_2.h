#ifndef PROGRAMMENU1_2_H
#define PROGRAMMENU1_2_H

#include "programmenu.h"
#include "programmenu1_2item.h"

class ProgramMenu1_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(int numItems MEMBER m_numItems NOTIFY numItemsChanged)

public:
    explicit ProgramMenu1_2(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu1_2() override;
    Q_INVOKABLE int setTime(int itemIndex, int timeIndex, double time);
    Q_INVOKABLE int setCounter(int itemIndex, int counterIndex, int counter);
    Q_INVOKABLE int getKeyNo(int itemIndex, int keyIndex);
    Q_INVOKABLE void modbusGetSignal();
    Q_INVOKABLE void resetUse();
    Q_INVOKABLE bool isCounterHrAddress(int itemIndex, int timeIndex);
    NCPCommand *createNcpCommand() override;
    void show(NCPCommand *command) override;
private:
    void init();
    void readDataFormatCsv();
    void readPenSetData();
    void readProgramNo();
    void readKey();
    void readDiAddress();
    void readHrAddress();
    void onActivate() override;
    void onDeactivate() override;
    void modbusSaveData(int menuIndex);
    void modbusSaveTime(quint16 hrAddress, int time);
    void modbusGetTime(quint16 hrAddress, quint16 size);
    void modbusSaveCounter(quint16 hrAddress, int counter);
    void modbusGetCounter(quint16 hrAddress, quint16 size);
    void updateTime(quint16 hrAddress, quint16 size);
    void updateCounter();
    void updateSignal();
    void resetData();
    ProgramMenu1_2Item *item(int index) { return static_cast<ProgramMenu1_2Item *>(m_itemList.at(index)); }
    void setNumItems(int num);
    int getItemIndex(int programNo);
    bool isCounterHrAddress(int address);
signals:
    void itemListChanged();
    void numItemsChanged();
public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    QList<QObject *>        m_itemList;
    int                     m_numItems = 0;
};


#endif // PROGRAMMENU1_2_H
