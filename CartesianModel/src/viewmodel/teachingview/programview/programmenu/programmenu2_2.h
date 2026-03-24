#ifndef PROGRAMMENU2_2_H
#define PROGRAMMENU2_2_H

#include "programmenu.h"

class AxisPointInfo;

class PointLabelInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int labelIndex MEMBER m_labelIndex NOTIFY labelIndexChanged)
    Q_PROPERTY(int pointNo MEMBER m_pointNo NOTIFY pointNoChanged)
public:
    explicit PointLabelInfo(QObject *parent = nullptr);
    void setLabelIndex(int index);
    void setPointNo(int no);
    int pointNo() const { return m_pointNo; }
signals:
    void labelIndexChanged();
    void pointNoChanged();
private:
    int     m_labelIndex = 0;
    int     m_pointNo = 0;
};

class ProgramMenu2_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> pointList MEMBER m_pointList NOTIFY pointListChanged)
    Q_PROPERTY(QList<QObject *> pointDataList MEMBER m_pointDataList NOTIFY pointDataListChanged)
    Q_PROPERTY(int currentPointNo MEMBER m_currentPointNo NOTIFY currentPointNoChanged)
    Q_PROPERTY(int currentPointLabelIndex MEMBER m_currentPointLabelIndex NOTIFY currentPointLabelIndexChanged)
    Q_PROPERTY(bool relative MEMBER m_relative NOTIFY relativeChanged)
    Q_PROPERTY(int noWaiting MEMBER m_noWaiting NOTIFY noWaitingChanged)
public:
    explicit ProgramMenu2_2(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu2_2() override;
    Q_INVOKABLE void setCurrentPointLabelIndex(int index);
    Q_INVOKABLE int setPosition(int axis, int position);
    Q_INVOKABLE int setSpeed(int axis, int speed);
    Q_INVOKABLE int setCurrentPosition();
    Q_INVOKABLE QString formatPosition(int axis, int position);
    Q_INVOKABLE QString formatSpeed(int axis, int speed);
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;

private:
    void init();
    void readPenSetData();
    void readPenSetPointListData();
    void onActivate() override;
    void onDeactivate() override;
    void modbusSetPointData(int pointNo);
    void modbusGetPointData(int pointNo);
    void updatePointData(const stModBusCommunicationData &command);
    void setCurrentPointNo(int no);
    void setRelative(bool relative);
    void setNoWaiting(bool noWaiting);
    void resetPointData();
    int setCurrentPointLabelIndexFromPointNo(int pointNo);

signals:
    void pointListChanged();
    void pointDataListChanged();
    void currentPointNoChanged();
    void currentPointLabelIndexChanged();
    void relativeChanged();
    void noWaitingChanged();

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    QList<QObject *>    m_pointList;    //固定点点位
    QList<QObject *>    m_pointDataList;
    QHash<int, int>     m_pointAddressIndexPointNoConversionMap;
    QHash<int, int>     m_formatIndexPointNoConversionMap;
    AxisPointInfo       *m_axisPointInfo = nullptr;
    int                 m_currentPointNo = 1;
    int                 m_currentPointLabelIndex = 0;
    bool                m_relative = false;
    bool                m_noWaiting = false;
    bool                m_dataChanged = false;
    QList<QList<int>>   m_pointDispOrderSetting;
    int                 m_numPoint = 0;
};

#endif // PROGRAMMENU2_2_H
