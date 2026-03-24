#ifndef PROGRAMMENU2_4_4_H
#define PROGRAMMENU2_4_4_H

#include "programmenu.h"

class AxisPointInfo;

class ProgramMenu2_4_4 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(bool noWaiting MEMBER m_noWaiting NOTIFY noWaitingChanged)
    Q_PROPERTY(bool plusDirection MEMBER m_plusDirection NOTIFY plusDirectionChanged)
public:
    explicit ProgramMenu2_4_4(ResourceManager *resource,CommunicationThread *comm);
    Q_INVOKABLE int setSpeed(int axis, int speed);
    Q_INVOKABLE QString formatSpeed(int speed);
    void setNoWaiting(bool noWaiting);
    void setPlusDirection(bool plusDirection);
    void clearSelect();
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;

private:
    void init();
    void onActivate() override;
    void onDeactivate() override;
    void modbusSetAxisOriginSpeedData();
    void modbusGetAxisOriginSpeedData();
    void updateAxisOriginSpeedData();
    void resetPointData();

signals:
    void itemListChanged();
    void noWaitingChanged();
    void plusDirectionChanged();

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    QList<QObject *>    m_itemList;
    bool                m_noWaiting = false;
    bool                m_plusDirection = false;
    AxisPointInfo       *m_axisPointInfo = nullptr;
    quint16             m_axisOriginTopHrAddress = 0;
};

#endif // PROGRAMMENU2_4_4_H
