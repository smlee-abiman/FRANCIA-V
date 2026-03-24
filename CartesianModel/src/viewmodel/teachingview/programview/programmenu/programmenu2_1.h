#ifndef PROGRAMMENU2_1_H
#define PROGRAMMENU2_1_H

#include "programmenu.h"

class AxisPointInfo;

class ProgramMenu2_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> pointDataList MEMBER m_pointDataList NOTIFY pointDataListChanged)
    Q_PROPERTY(int currentPointNo MEMBER m_currentPointNo NOTIFY currentPointNoChanged)
    Q_PROPERTY(bool relative MEMBER m_relative NOTIFY relativeChanged)
    Q_PROPERTY(bool noWaiting MEMBER m_noWaiting NOTIFY noWaitingChanged)
public:
    explicit ProgramMenu2_1(ResourceManager *resource,CommunicationThread *comm, NCPFile *ncpFile);
    ~ProgramMenu2_1() override;
    Q_INVOKABLE int setPosition(int axis, int position);
    Q_INVOKABLE int setSpeed(int axis, int speed);
    Q_INVOKABLE int setCurrentPosition();
    Q_INVOKABLE QString formatPosition(int position);
    Q_INVOKABLE QString formatSpeed(int speed);
    void show(NCPCommand *command) override;
    CreateMethod commandCreateMethod() override { return CreateMethod::ComplexNcpCommand; }
    int insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    int replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    void setNcpFile(NCPFile *ncpFile);

private:
    void init();
    void onActivate() override;
    void onDeactivate() override;
    void modbusSetPointData(int pointNo);
    void modbusGetPointData(int pointNo);
    void updatePointData(const stModBusCommunicationData &command);
    void resetPointData();
    void setCurrentPointNo(int no);
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void setRelative(bool relative);
    void setNoWaiting(bool noWaiting);
    int insertCommand(NCPProgram *program, int lineIndex, NCPCommand *command);
    int getNcpCommandPointNo(NCPProgram *program, int lineIndex);
    int findFreeArbitraryPoint();
    NCPCommand *createMoveCommand(int pointNo);
signals:
    void pointDataListChanged();
    void currentPointNoChanged();
    void relativeChanged();
    void noWaitingChanged();

public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

private:
    QList<QObject *>    m_pointDataList;
    AxisPointInfo       *m_axisPointInfo = nullptr;
    NCPFile             *m_ncpFile;
    int                 m_currentPointNo = 0;
    bool                m_relative = false;
    bool                m_noWaiting = false;
    bool                m_dataChanged = false;
    bool                m_show = false;
};

#endif // PROGRAMMENU2_1_H
