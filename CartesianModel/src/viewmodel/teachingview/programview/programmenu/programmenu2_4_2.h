#ifndef PROGRAMMENU2_4_2_H
#define PROGRAMMENU2_4_2_H

#include "programmenu.h"

class AxisPointInfo;

class ProgramMenu2_4_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> pointDataList MEMBER m_pointDataList NOTIFY pointDataListChanged)
    Q_PROPERTY(bool consecutive MEMBER m_consecutive NOTIFY consecutiveChanged)
    Q_PROPERTY(bool isEndPoint MEMBER m_isEndPoint NOTIFY isEndPointChanged)
    Q_PROPERTY(int speed MEMBER m_speed NOTIFY speedChanged)
    Q_PROPERTY(int halfwayPointNo MEMBER m_halfwayPointNo NOTIFY halfwayPointNoChanged)
    Q_PROPERTY(int endPointNo MEMBER m_endPointNo NOTIFY endPointNoChanged)
public:
    explicit ProgramMenu2_4_2(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile);
    ~ProgramMenu2_4_2() override;
    Q_INVOKABLE int setHalfwayPosition(int axis, int position);
    Q_INVOKABLE int setEndPosition(int axis, int position);
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
    void modbusSetPointData(int pointNo, bool endPoint);
    void modbusGetPointData(int pointNo);
    void updatePointData(const stModBusCommunicationData &command);
    void clearSelect();
    void setHalfwayPointNo(int no);
    void setEndPointNo(int no);
    void setIsEndPoint(bool isEndPoint);
    void resetPointData();

signals:
    void pointDataListChanged();
    void consecutiveChanged();
    void isEndPointChanged();
    void speedChanged();
    void halfwayPointNoChanged();
    void endPointNoChanged();

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    QList<QObject *>    m_pointDataList;
    AxisPointInfo       *m_axisPointInfo = nullptr;
    NCPFile             *m_ncpFile;
    int                 m_halfwayPointNo = 0;
    int                 m_endPointNo = 0;
    bool                m_consecutive = false;
    bool                m_isEndPoint = false;
    int                 m_speed = 0;
    bool                m_dataChanged = false;
    bool                m_show = false;
};

#endif // PROGRAMMENU2_4_2_H
