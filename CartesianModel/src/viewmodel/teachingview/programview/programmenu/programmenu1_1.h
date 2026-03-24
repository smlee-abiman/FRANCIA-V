#ifndef PROGRAMMENU1_1_H
#define PROGRAMMENU1_1_H

#include "programmenu.h"

class ProgramMenu1_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(QList<int> programNoList MEMBER m_programNoList NOTIFY programNoListChanged)
    Q_PROPERTY(bool detect MEMBER m_detect NOTIFY detectChanged)

public:
    explicit ProgramMenu1_1(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu1_1() override;
    Q_INVOKABLE int setTime(int programNo, int timerIndex, double time);
    Q_INVOKABLE void setDetect(bool detect);
    Q_INVOKABLE int getItemIndex(int programNo);
    NCPCommand *createNcpCommand() override;
    void show(NCPCommand *command) override;
    QList<int> programNoList() const { return m_programNoList; }
private:
    void init();
    void readDataFormatCsv();
    void readPenSetData();
    void readPenSetProgramNo();
    void readPenSetTimerAddress();
    void onActivate() override;
    void onDeactivate() override;
    void modbusSaveData(int menuIndex);
    int modbusSaveTime(int programNo);
    void modbusSaveTime(quint16 hrAddress, int time);
    void modbusGetTime(quint16 hrAddress, quint16 size);
    void modbusSaveDetect(bool detect);
    void modbusGetDetect();
    void updateTime();
    void updateDetect();
    void writeHR(quint16 addr, int value);
    void readHR(quint16 addr, quint16 size);
    void setOneShotModbusCommand(CommunicationThread::OneShotReadModBusCaller caller, stModBusCommunicationData &data);

signals:
    void itemListChanged();
    void programNoListChanged();
    void detectChanged();
public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    QList<QObject *>    m_itemList;
    QList<int>          m_programNoList;
    bool                m_detect = false;
};

#endif // PROGRAMMENU1_1_H
