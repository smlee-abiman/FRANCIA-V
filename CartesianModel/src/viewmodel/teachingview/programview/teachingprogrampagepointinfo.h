#ifndef TEACHINGPROGRAMPAGEPOINTINFO_H
#define TEACHINGPROGRAMPAGEPOINTINFO_H

#include <QObject>
#include "communicationthread.h"

class TeachingProgramPageModel;
class NCPCommand;
class AxisPointInfo;

class TeachingProgramPagePointInfo : public QObject
{
    Q_OBJECT
public:
    explicit TeachingProgramPagePointInfo(TeachingProgramPageModel *model, CommunicationThread *comm, QObject *parent = nullptr);
    ~TeachingProgramPagePointInfo() override;
private:
    int getPointNoListFromProgram(int programNo, QList<int> &pointNoList);
    void getNcpCommandPointNo(NCPCommand *command, QList<int> &pointNoList);
    void modbusGetPointData(QList<int> &pointNoList);
    void modbusGetPointData(int pointNo, int numPoints);
signals:
private slots:
    void onSelectedProgramNoChanged(int no);
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
private:
    TeachingProgramPageModel    *m_programPageModel = nullptr;
    CommunicationThread         *m_communication = nullptr;
    AxisPointInfo               *m_axisPointInfo = nullptr;
    QHash<int, bool>            m_accuiredPoints;
    QHash<int, int>             m_accuiringPoints;
};

#endif // TEACHINGPROGRAMPAGEPOINTINFO_H
