#ifndef PROGRAMMENU2_4_1_H
#define PROGRAMMENU2_4_1_H

#include "programmenu.h"

class AxisPointInfo;

class ProgramMenu2_4_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> pointDataList MEMBER m_pointDataList NOTIFY pointDataListChanged)
    Q_PROPERTY(int currentPointNo MEMBER m_currentPointNo NOTIFY currentPointNoChanged)
    Q_PROPERTY(bool consecutive MEMBER m_consecutive NOTIFY consecutiveChanged)
    Q_PROPERTY(int speed MEMBER m_speed NOTIFY speedChanged)
public:
    explicit ProgramMenu2_4_1(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile);
    ~ProgramMenu2_4_1() override;
    Q_INVOKABLE int setPosition(int axis, int position);
    Q_INVOKABLE void setSpeed(int speed);
    Q_INVOKABLE int setCurrentPosition();
    Q_INVOKABLE QString formatPosition(int position);
    Q_INVOKABLE QString formatSpeed(int speed);
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;
    void setNcpFile(NCPFile *ncpFile);

private:
    void init();
    void onActivate() override;
    void onDeactivate() override;
    void setConsecutive(bool consecutive);
    void modbusSetPointData();
    void modbusGetPointData(int pointNo);
    void updatePointData(const stModBusCommunicationData &command);
    void setCurrentPointNo(int no);
    void clearSelect();
    void resetPointData();

signals:
    void pointDataListChanged();
    void currentPointNoChanged();
    void consecutiveChanged();
    void speedChanged();

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    QList<QObject *>    m_pointDataList;
    AxisPointInfo       *m_axisPointInfo = nullptr;
    NCPFile             *m_ncpFile;
    int                 m_currentPointNo = 0;
    bool                m_consecutive = false;
    int                 m_speed = 0;
    bool                m_dataChanged = false;
    bool                m_show = false;
};

#endif // PROGRAMMENU2_4_1_H
