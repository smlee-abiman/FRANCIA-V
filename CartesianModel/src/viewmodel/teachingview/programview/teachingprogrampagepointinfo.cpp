#include "teachingprogrampagepointinfo.h"
#include "../teachingprogrampagemodel.h"
#include "../axispointinfo.h"
#include "command/ncpcommandmove.h"
#include "command/ncpcommandline.h"
#include "command/ncpcommandarc.h"
#include "command/ncpcommandgosub.h"

TeachingProgramPagePointInfo::TeachingProgramPagePointInfo(TeachingProgramPageModel *model, CommunicationThread *comm, QObject *parent)
    : QObject{parent},
      m_programPageModel(model),
      m_communication(comm)
{
    m_axisPointInfo = AxisPointInfo::get_instance();
    connect(m_programPageModel, SIGNAL(selectedProgramNoChanged(int)), this, SLOT(onSelectedProgramNoChanged(int)));
}



TeachingProgramPagePointInfo::~TeachingProgramPagePointInfo()
{
    disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingProgramPagePointInfo::onOneShotReadFinished);
    disconnect(m_programPageModel, SIGNAL(selectedProgramNoChanged(int)), this, SLOT(onSelectedProgramNoChanged(int)));
}

void TeachingProgramPagePointInfo::onSelectedProgramNoChanged(int no)
{
    QList<int> pointNoList;
    getPointNoListFromProgram(no, pointNoList);
    modbusGetPointData(pointNoList);
}

int TeachingProgramPagePointInfo::getPointNoListFromProgram(int programNo, QList<int> &pointNoList)
{
    auto ncpFile = m_programPageModel->ncpFile();
    if (ncpFile == nullptr) {
        return -1;
    }

    auto program = ncpFile->getProgram(programNo);
    if (program == nullptr) {
        return -1;
    }

    const int numSteps = program->numSteps();
    for (int i = 0; i < numSteps; i++) {
        auto step = program->step(i);
        getNcpCommandPointNo(step->command(), pointNoList);
    }
    return 0;
}

void TeachingProgramPagePointInfo::getNcpCommandPointNo(NCPCommand *command, QList<int> &pointNoList)
{
    auto commandType = command->type();
    switch (commandType) {
    case NCPCommand::Type::Move:
    case NCPCommand::Type::Movep:
    {
        auto moveCommnad = dynamic_cast<NCPCommandMove *>(command);
        pointNoList.append(moveCommnad->pointNo());
    }
        break;
    case NCPCommand::Type::Line:
    case NCPCommand::Type::Linec:
    {
        auto lineCommnad = dynamic_cast<NCPCommandLine *>(command);
        pointNoList.append(lineCommnad->pointNo());
    }
        break;
    case NCPCommand::Type::Arc:
    case NCPCommand::Type::Arcc:
    {
        auto arcCommnad = dynamic_cast<NCPCommandArc *>(command);
        pointNoList.append(arcCommnad->pointNo());
        pointNoList.append(arcCommnad->endPointNo());
    }
        break;
    case NCPCommand::Type::GoSub:
    {
        auto gosubCommnad = dynamic_cast<NCPCommandGoSub *>(command);
        int pointNo = gosubCommnad->pointNo();
        if (pointNo >= 1) {
            pointNoList.append(pointNo);
        }
    }
        break;
    default:
        break;
    }
}

void TeachingProgramPagePointInfo::modbusGetPointData(QList<int> &pointNoList)
{
    bool needConnect = false;
    const int maxNumPoints = 5;
    for (int pointNo : pointNoList) {
        if (m_axisPointInfo->isValidPoint(pointNo)) {
            if (!m_accuiredPoints.contains(pointNo)) {
                for (int j = 0; j < maxNumPoints; j++)
                    m_accuiredPoints.insert(pointNo + j, true);
                modbusGetPointData(pointNo, maxNumPoints);
                needConnect = true;
            }
        }
    }

    if (needConnect)
        connect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingProgramPagePointInfo::onOneShotReadFinished);
}

void TeachingProgramPagePointInfo::modbusGetPointData(int pointNo, int numPoints)
{
    quint16 hrAddress = m_axisPointInfo->getPointPositionAddress(pointNo, 0);
    auto size = m_axisPointInfo->pointDataSize() * numPoints;
    if (hrAddress >= m_axisPointInfo->positionTopHrAddr())
        m_communication->readModbusHR(CommunicationThread::CallerTeachingProgramPageModel, hrAddress, size);
    m_accuiringPoints.insert(hrAddress, size);
}

void TeachingProgramPagePointInfo::onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller)
{
    if (oneShotReadCaller != CommunicationThread::CallerTeachingProgramPageModel)
        return;

    if (command.funcCode == QModbusPdu::FunctionCode::ReadHoldingRegisters) {
        auto size = m_accuiringPoints.value(command.readAddress, -1);
        if (command.comSize == size) {
            m_accuiringPoints.remove(command.readAddress);
            if (m_accuiringPoints.isEmpty()) {
                m_programPageModel->updateNcpStepList();
                disconnect(m_communication, &CommunicationThread::oneShotReadFinishedIndividualSignal, this, &TeachingProgramPagePointInfo::onOneShotReadFinished);
            }
        }
    }
}
