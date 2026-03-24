#include <QtTest>

#include "mock/mock_modbusmanager.h"
#include "mock/mock_communicationthread.h"

#include "tst_ioforceviewmodel.h"

#define private public
#include "ioforceviewmodel.h"
#undef public

#include "appenvironment.h"

IOForceViewModelTest::IOForceViewModelTest(QObject *parent)
    : QObject(parent)
    , m_resource(nullptr)
    , m_communication(nullptr)
    , m_modbus(nullptr)
    , m_model(nullptr)
{
}
void IOForceViewModelTest::createModel() {
    if (m_model == nullptr) {
        m_model = new IOForceViewModel(m_resource, m_communication);
    }
}
void IOForceViewModelTest::deleteModel() {
    if (m_model != nullptr) {
        delete m_model;
        m_model = nullptr;
    }
}
void IOForceViewModelTest::initTestCase()
{
    AppEnvironment::initialize("../CartesianModel/bin");
    m_resource = new ResourceManager(".");
}
void IOForceViewModelTest::cleanupTestCase()
{
    delete m_resource;
    m_resource = nullptr;
}
void IOForceViewModelTest::init()
{
    m_communication = CommunicationThread::create_instance(nullptr);
    ModbusManager::instanceRenew();
    m_modbus = ModbusManager::get_instance();
    createModel();
}
void IOForceViewModelTest::cleanup()
{
    deleteModel();
    CommunicationThread::destroy_instance();
    m_communication = nullptr;
}
//*----------------------------------------------------
//* test case.
//*----------------------------------------------------
// case:
//  - caller == CallerDefault.
void IOForceViewModelTest::onOneShotReadFinished001()
{
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_ON_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 0x0001);
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 1);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerDefault;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 1);
    // teardown.
}
//*----------------------------------------------------
// case:
//  - command.comSize == 0.
void IOForceViewModelTest::onOneShotReadFinished002()
{
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 0;
    command.readAddress = FORCED_ON_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 0x0001);
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 1);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerDefault;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 1);
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced on.
//    HR0
//    bit offset : 0
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOn001()
{
    // prepare.
    quint16 bitOffset = 0;
    quint16 targetDi = 8;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_ON_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOn(), true);
        } else {
            QCOMPARE(forceIo->forcedOn(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOff(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced on.
//    HR0
//    bit offset : 1
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOn002()
{
    // prepare.
    quint16 bitOffset = 1;
    quint16 targetDi = 9;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_ON_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOn(), true);
        } else {
            QCOMPARE(forceIo->forcedOn(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOff(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced on.
//    HR0
//    bit offset : 7
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOn003()
{
    // prepare.
    quint16 bitOffset = 7;
    quint16 targetDi = 15;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_ON_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOn(), true);
        } else {
            QCOMPARE(forceIo->forcedOn(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOff(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced on.
//    HR0
//    bit offset : 8
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOn004()
{
    // prepare.
    quint16 bitOffset = 8;
    quint16 targetDi = 0;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_ON_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOn(), true);
        } else {
            QCOMPARE(forceIo->forcedOn(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOff(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced on.
//    HR0, bit offset : 15
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOn005()
{
    // prepare.
    quint16 bitOffset = 15;
    quint16 targetDi = 7;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_ON_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOn(), true);
        } else {
            QCOMPARE(forceIo->forcedOn(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOff(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced on.
//    HR1
//    bit offset : 0
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOn006()
{
    // prepare.
    quint16 bitOffset = 8;
    quint16 targetDi = 16;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_ON_HR + 1;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOn(), true);
        } else {
            QCOMPARE(forceIo->forcedOn(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOff(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced on.
//    HR0 - HR16
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOn007()
{
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 16;
    command.readAddress = FORCED_ON_HR;
    for (int i = 0; i < 16; i++) {
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i, 0xFFFF);
    }
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOn(), true);
        QCOMPARE(forceIo->forcedOff(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced off.
//    HR496, bit offset : 0
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOff001()
{
    // prepare.
    quint16 bitOffset = 0;
    quint16 targetDi = 8;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_OFF_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOff(), true);
        } else {
            QCOMPARE(forceIo->forcedOff(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOn(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced off.
//    HR496, bit offset : 15
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOff002()
{
    // prepare.
    quint16 bitOffset = 15;
    quint16 targetDi = 7;
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 1;
    command.readAddress = FORCED_OFF_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress, 1 << bitOffset);
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->forcedOff(), true);
        } else {
            QCOMPARE(forceIo->forcedOff(), false);
        }
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOn(), false);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update forced off.
//    HR496 - HR512
void IOForceViewModelTest::onOneShotReadFinishe_udupdateForcedOff003()
{
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = 16;
    command.readAddress = FORCED_OFF_HR;
    for (int i = 0; i < 16; i++) {
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i, 0xFFFF);
    }
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        auto* forceIo = dynamic_cast<ForceIOData*>(data);
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->forcedOn(), false);
        QCOMPARE(forceIo->forcedOff(), true);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update swap di.
//    HR1008 - HR1009.
void IOForceViewModelTest::onOneShotReadFinishe_updateSwapData001()
{
    // preprare.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* fromForceIo = dynamic_cast<ForceIOData*>(forceIoDataTbl[0]);
    auto* toForceIo   = dynamic_cast<ForceIOData*>(forceIoDataTbl[1]);
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.readAddress = SWAP_INSTRUCTION_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress    , fromForceIo->bankNo());
    m_modbus->m_receiveDataMapHR.insert(command.readAddress + 1, toForceIo->bankNo());
    command.comSize = m_modbus->m_receiveDataMapHR.size();
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    QCOMPARE(fromForceIo->swapDi(), toForceIo->forcedDio());
    QCOMPARE(m_model->canSwapIO(), true);
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update swap di.
//    in use -> unused.
void IOForceViewModelTest::onOneShotReadFinishe_updateSwapData002()
{
    // preprare.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* fromForceIo = dynamic_cast<ForceIOData*>(forceIoDataTbl[0]);
    auto* toForceIo   = dynamic_cast<ForceIOData*>(forceIoDataTbl[1]);
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.readAddress = SWAP_INSTRUCTION_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress    , fromForceIo->bankNo());
    m_modbus->m_receiveDataMapHR.insert(command.readAddress + 1, toForceIo->bankNo());
    command.comSize = m_modbus->m_receiveDataMapHR.size();
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    m_modbus->m_receiveDataMapHR.insert(command.readAddress    , 0);
    m_modbus->m_receiveDataMapHR.insert(command.readAddress + 1, 0);
    // test.
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    QCOMPARE(fromForceIo->swapDi(), static_cast<quint16>(-1));
    QCOMPARE(m_model->canSwapIO(), true);
    // teardown.
}
//*----------------------------------------------------
// case:
//  - update swap di.
//    toBankNo has changed.
void IOForceViewModelTest::onOneShotReadFinishe_updateSwapData003()
{
    // preprare.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* fromForceIo = dynamic_cast<ForceIOData*>(forceIoDataTbl[0]);
    auto* toForceIo   = dynamic_cast<ForceIOData*>(forceIoDataTbl[1]);
    auto* toForceIo2  = dynamic_cast<ForceIOData*>(forceIoDataTbl[2]);
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.readAddress = SWAP_INSTRUCTION_HR;
    m_modbus->m_receiveDataMapHR.insert(command.readAddress    , fromForceIo->bankNo());
    m_modbus->m_receiveDataMapHR.insert(command.readAddress + 1, toForceIo->bankNo());
    command.comSize = m_modbus->m_receiveDataMapHR.size();
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    m_modbus->m_receiveDataMapHR.insert(command.readAddress    , fromForceIo->bankNo());
    m_modbus->m_receiveDataMapHR.insert(command.readAddress + 1, toForceIo2->bankNo());
    // test.
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    QCOMPARE(fromForceIo->swapDi(), toForceIo2->forcedDio());
    QCOMPARE(m_model->canSwapIO(), true);
    // teardown.
}
// case:
//  - update swap di.
//    HR1008 - HR1023.
void IOForceViewModelTest::onOneShotReadFinishe_updateSwapData004()
{
    // preprare.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.readAddress = SWAP_INSTRUCTION_HR;
    for (int i = 0; i < 16; i+=2) {
        auto* fromForceIo = dynamic_cast<ForceIOData*>(forceIoDataTbl[i]);
        auto* toForceIo   = dynamic_cast<ForceIOData*>(forceIoDataTbl[i+1]);
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i    , fromForceIo->bankNo());
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i + 1, toForceIo->bankNo());
    }
    command.comSize = m_modbus->m_receiveDataMapHR.size();
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    for (int i = 0; i < 8; i+=2) {
        auto* fromForceIo = dynamic_cast<ForceIOData*>(forceIoDataTbl[i]);
        auto* toForceIo   = dynamic_cast<ForceIOData*>(forceIoDataTbl[i+1]);
        QCOMPARE(fromForceIo->swapDi(), toForceIo->forcedDio());
    }
    QCOMPARE(m_model->canSwapIO(), false);
    // teardown.
}
// case:
//  - update swap di.
//    HR1022 - HR1023.
void IOForceViewModelTest::onOneShotReadFinishe_updateSwapData005()
{
    // preprare.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* fromForceIo = dynamic_cast<ForceIOData*>(forceIoDataTbl[0]);
    auto* toForceIo   = dynamic_cast<ForceIOData*>(forceIoDataTbl[1]);
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.readAddress = SWAP_INSTRUCTION_HR + ((NUM_OF_SWAP_DATA - 1) * 2);
    m_modbus->m_receiveDataMapHR.insert(command.readAddress    , fromForceIo->bankNo());
    m_modbus->m_receiveDataMapHR.insert(command.readAddress + 1, toForceIo->bankNo());
    command.comSize = m_modbus->m_receiveDataMapHR.size();
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerMonitorIOForcePageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_receiveDataMapHR.size(), 0);
    QCOMPARE(fromForceIo->swapDi(), toForceIo->forcedDio());
    QCOMPARE(m_model->canSwapIO(), true);
    // teardown.
}
// case:
//  - update periodic data (value).
void IOForceViewModelTest::onFinished_updatePeriodicData001()
{
    // prepare.
    quint16 targetDi = 1;
    // setup.
    m_modbus->m_receiveDataMapDI[targetDi] = true;
    // test.
    m_model->onFinished();
    // verify.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        const auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->forcedDio() == targetDi) {
            QCOMPARE(forceIo->value(), true);
        } else {
            QCOMPARE(forceIo->value(), false);
        }
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
// case:
//  - update periodic data (value).
void IOForceViewModelTest::onFinished_updatePeriodicData002()
{
    // setup.
    int numOfPage = m_resource->getPenSetIntValue(PENSET_GROUP, 5, 0);
    for (int i = 0; i < numOfPage * NUM_OF_IO_DATA_PAR_PAGE; i++) {
        auto penSetRow = PENSET_OFFSET_IO_ADDRESS + i;
        auto diStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 0);
        bool isOk;
        auto di = static_cast<quint16>(diStr.toInt(&isOk));
        if (!isOk) continue;
        m_modbus->m_receiveDataMapDI[di] = true;
    }
    // test.
    m_model->onFinished();
    // verify.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        const auto* forceIo = dynamic_cast<ForceIOData*>(data);
        QCOMPARE(forceIo->value(), true);
        QCOMPARE(forceIo->invers(), false);
    }
    // teardown.
}
// case:
//  - update periodic data (invers value).
void IOForceViewModelTest::onFinished_updatePeriodicData003()
{
    // prepare.
    quint16 targetDo = 5124;
    // setup.
    m_modbus->m_receiveDataMapDO[targetDo] = true;
    // test.
    m_model->onFinished();
    // verify.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        const auto* forceIo = dynamic_cast<ForceIOData*>(data);
        if (forceIo->inversDo() == targetDo) {
            QCOMPARE(forceIo->invers(), true);
        } else {
            QCOMPARE(forceIo->invers(), false);
        }
        QCOMPARE(forceIo->value(), false);
    }
    // teardown.
}
// case:
//  - update periodic data (invers value).
void IOForceViewModelTest::onFinished_updatePeriodicData004()
{
    // setup.
    int numOfPage = m_resource->getPenSetIntValue(PENSET_GROUP, 5, 0);
    for (int i = 0; i < numOfPage * NUM_OF_IO_DATA_PAR_PAGE; i++) {
        auto penSetRow = PENSET_OFFSET_IO_ADDRESS + i;
        auto doStr = m_resource->getPenSetStringValue(PENSET_GROUP, penSetRow, 2);
        bool isOk;
        qDebug("doStr:%s", qPrintable(doStr));
        auto doAddr = static_cast<quint16>(doStr.toInt(&isOk));
        if (!isOk) continue;
        m_modbus->m_receiveDataMapDO[doAddr] = true;
    }
    // test.
    m_model->onFinished();
    // verify.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (auto& data : forceIoDataTbl) {
        const auto* forceIo = dynamic_cast<ForceIOData*>(data);
        qDebug("%d", forceIo->inversDo());
        QCOMPARE(forceIo->value(), false);
        QCOMPARE(forceIo->invers(), true);
    }
    // teardown.
}
// case:
// index out of bounds.
void IOForceViewModelTest::forciblyTurnOn001()
{
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    // test.
    m_model->forciblyTurnOn(forceIoDataTbl.size());
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 0);
    // teardown.
}
// case:
// index 0.
void IOForceViewModelTest::forciblyTurnOn002()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    // test.
    m_model->forciblyTurnOn(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->forcedDio()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->forcedDio()), 1);

    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 2);
    stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_ON_HR);
    QCOMPARE(data.comSize, 1);
    data = m_communication->m_oneShotModbusCommandList.at(1);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_OFF_HR);
    QCOMPARE(data.comSize, 1);
    // teardown.
}
// case:
// index last - 1.
void IOForceViewModelTest::forciblyTurnOn003()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    targetIndex = forceIoDataTbl.size() - 1;
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    // test.
    m_model->forciblyTurnOn(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->forcedDio()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->forcedDio()), 1);

    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 2);
    stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_ON_HR + (forceIo->forcedDio() / 16));
    QCOMPARE(data.comSize, 1);
    data = m_communication->m_oneShotModbusCommandList.at(1);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_OFF_HR + (forceIo->forcedDio() / 16));
    QCOMPARE(data.comSize, 1);
    // teardown.
}
void IOForceViewModelTest::forciblyTurnOn004()
{
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (int i = 0; i < forceIoDataTbl.size(); i++) {
        // setup.
        m_communication->m_writeModbusDOucharMap.clear();
        m_communication->m_oneShotModbusCommandList.clear();
        auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[i]);
        // test.
        m_model->forciblyTurnOn(i);
        // verify.
        // write DO.
        QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->forcedDio()), true);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->forcedDio()), 1);

        // read HR.
        QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 2);
        stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
        QCOMPARE(data.readAddress, FORCED_ON_HR + (forceIo->forcedDio() / 16));
        QCOMPARE(data.comSize, 1);
        data = m_communication->m_oneShotModbusCommandList.at(1);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
        QCOMPARE(data.readAddress, FORCED_OFF_HR + (forceIo->forcedDio() / 16));
        QCOMPARE(data.comSize, 1);
        // teardown.
    }
}
// case:
// index out of bounds.
void IOForceViewModelTest::forciblyTurnOff001()
{
    // setup.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    // test.
    m_model->forciblyTurnOff(forceIoDataTbl.size());
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 0);
    // teardown.
}
// case:
// index 0.
void IOForceViewModelTest::forciblyTurnOff002()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    // test.
    m_model->forciblyTurnOff(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->forcedDio()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->forcedDio()), 0);

    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 2);
    stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_ON_HR);
    QCOMPARE(data.comSize, 1);
    data = m_communication->m_oneShotModbusCommandList.at(1);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_OFF_HR);
    QCOMPARE(data.comSize, 1);
    // teardown.
}
// case:
// index last - 1.
void IOForceViewModelTest::forciblyTurnOff003()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    targetIndex = forceIoDataTbl.size() - 1;
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    // test.
    m_model->forciblyTurnOff(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->forcedDio()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->forcedDio()), 0);

    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 2);
    stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_ON_HR + (forceIo->forcedDio() / 16));
    QCOMPARE(data.comSize, 1);
    data = m_communication->m_oneShotModbusCommandList.at(1);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, FORCED_OFF_HR + (forceIo->forcedDio() / 16));
    QCOMPARE(data.comSize, 1);
    // teardown.
}
void IOForceViewModelTest::forciblyTurnOff004()
{
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (int i = 0; i < forceIoDataTbl.size(); i++) {
        // setup.
        m_communication->m_writeModbusDOucharMap.clear();
        m_communication->m_oneShotModbusCommandList.clear();
        auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[i]);
        // test.
        m_model->forciblyTurnOff(i);
        // verify.
        // write DO.
        QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->forcedDio()), true);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->forcedDio()), 0);

        // read HR.
        QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 2);
        stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
        QCOMPARE(data.readAddress, FORCED_ON_HR + (forceIo->forcedDio() / 16));
        QCOMPARE(data.comSize, 1);
        data = m_communication->m_oneShotModbusCommandList.at(1);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
        QCOMPARE(data.readAddress, FORCED_OFF_HR + (forceIo->forcedDio() / 16));
        QCOMPARE(data.comSize, 1);
        // teardown.
    }
}
// case:
// index out of bounds.
void IOForceViewModelTest::inversTurnOn001()
{
    // setup.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    // test.
    m_model->inversTurnOn(forceIoDataTbl.size());
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 0);
    // teardown.
}
// case:
// index 0.
void IOForceViewModelTest::inversTurnOn002()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    // test.
    m_model->inversTurnOn(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->inversDo()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->inversDo()), 1);
    // teardown.
}
// case:
// index last - 1.
void IOForceViewModelTest::inversTurnOn003()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    targetIndex = forceIoDataTbl.size() - 1;
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    // test.
    m_model->inversTurnOn(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->inversDo()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->inversDo()), 1);
    // teardown.
}
void IOForceViewModelTest::inversTurnOn004()
{
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (int i = 0; i < forceIoDataTbl.size(); i++) {
        // setup.
        m_communication->m_writeModbusDOucharMap.clear();
        m_communication->m_oneShotModbusCommandList.clear();
        auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[i]);
        // test.
        m_model->inversTurnOn(i);
        // verify.
        // write DO.
        QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->inversDo()), true);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->inversDo()), 1);
        // teardown.
    }
}
// case:
// index out of bounds.
void IOForceViewModelTest::inversTurnOff001()
{
    // setup.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    // test.
    m_model->inversTurnOff(forceIoDataTbl.size());
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 0);
    // teardown.
}
// case:
// index 0.
void IOForceViewModelTest::inversTurnOff002()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    forceIo->setInvers(true);
    // test.
    m_model->inversTurnOff(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->inversDo()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->inversDo()), 0);
    // teardown.
}
// case:
// index last - 1.
void IOForceViewModelTest::inversTurnOff003()
{
    // prepare.
    int targetIndex = 0;
    // setup.
    m_communication->m_writeModbusDOucharMap.clear();
    m_communication->m_oneShotModbusCommandList.clear();
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    targetIndex = forceIoDataTbl.size() - 1;
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[targetIndex]);
    forceIo->setInvers(true);
    // test.
    m_model->inversTurnOff(targetIndex);
    // verify.
    QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->inversDo()), true);
    QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->inversDo()), 0);
    // teardown.
}
void IOForceViewModelTest::inversTurnOff004()
{
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    for (int i = 0; i < forceIoDataTbl.size(); i++) {
        // setup.
        m_communication->m_writeModbusDOucharMap.clear();
        m_communication->m_oneShotModbusCommandList.clear();
        auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[i]);
        forceIo->setInvers(true);
        // test.
        m_model->inversTurnOff(i);
        // verify.
        // write DO.
        QCOMPARE(m_communication->m_writeModbusDOucharMap.size(), 1);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.contains(forceIo->inversDo()), true);
        QCOMPARE(m_communication->m_writeModbusDOucharMap.value(forceIo->inversDo()), 0);
        // teardown.
    }
}
// case:
// forceIoPtr is nullPtr.
void IOForceViewModelTest::swapIO001()
{
    // setup.
    auto ioSwapConfigTbl = m_model->ioSwapConfigTbl();
    auto* ioSwapConfig = static_cast<IOSwapConfiguration*>(ioSwapConfigTbl[0]);
    // test.
    m_model->swapIO(nullptr, ioSwapConfig);
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 0);
    // teardown.
}
// case:
// ioSwapConfigPtr is nullPtr.
void IOForceViewModelTest::swapIO002()
{
    // setup.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[0]);
    // test.
    m_model->swapIO(forceIo, nullptr);
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 0);
    // teardown.
}
// case:
// find Io Swap Data.
void IOForceViewModelTest::swapIO003()
{
    // setup.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[0]);
    auto ioSwapConfigTbl = m_model->ioSwapConfigTbl();
    auto* ioSwapConfig = static_cast<IOSwapConfiguration*>(ioSwapConfigTbl[0]);
    auto* ioSwapData = static_cast<IOSwapData*>(m_model->m_ioSwapDataTbl[0]);
    quint16 prevBankNo;
    ioSwapData->setFromBankNo(static_cast<quint16>(forceIo->bankNo()), prevBankNo);
    ioSwapData->setToBankNo(static_cast<quint16>(ioSwapConfig->bankNo()), prevBankNo);
    // test.
    m_model->swapIO(forceIo, ioSwapConfig);
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 1);
    stModBusCommunicationData data;
    data = m_communication->m_modbusWriteCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::WriteMultipleRegisters);
    QCOMPARE(data.writeAddress, SWAP_INSTRUCTION_HR);
    QCOMPARE(data.comSize, 2);
    QCOMPARE(data.writeBuf[0], forceIo->bankNo());
    QCOMPARE(data.writeBuf[1], ioSwapConfig->bankNo());

    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 1);
    data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, SWAP_INSTRUCTION_HR);
    QCOMPARE(data.comSize, 2);
    // teardown.
}
// case:
// get free Io Swap Data.
void IOForceViewModelTest::swapIO004()
{
    // setup.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[0]);
    auto ioSwapConfigTbl = m_model->ioSwapConfigTbl();
    auto* ioSwapConfig = static_cast<IOSwapConfiguration*>(ioSwapConfigTbl[0]);
    // test.
    m_model->swapIO(forceIo, ioSwapConfig);
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 1);
    stModBusCommunicationData data;
    data = m_communication->m_modbusWriteCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::WriteMultipleRegisters);
    QCOMPARE(data.writeAddress, SWAP_INSTRUCTION_HR);
    QCOMPARE(data.comSize, 2);
    QCOMPARE(data.writeBuf[0], forceIo->bankNo());
    QCOMPARE(data.writeBuf[1], ioSwapConfig->bankNo());

    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 1);
    data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, SWAP_INSTRUCTION_HR);
    QCOMPARE(data.comSize, 2);
    // teardown.
}
// case:
// not found free Io Swap Data.
void IOForceViewModelTest::swapIO005()
{
    // setup.
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[0]);
    auto ioSwapConfigTbl = m_model->ioSwapConfigTbl();
    auto* ioSwapConfig = static_cast<IOSwapConfiguration*>(ioSwapConfigTbl[0]);
    for (auto& data: m_model->m_ioSwapDataTbl) {
        quint16 prevBankNo;
        auto* ioSwapData = static_cast<IOSwapData*>(data);
        ioSwapData->setFromBankNo(static_cast<quint16>(65535), prevBankNo);
        ioSwapData->setToBankNo(static_cast<quint16>(65535), prevBankNo);
    }
    // test.
    m_model->swapIO(forceIo, ioSwapConfig);
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 0);
    // teardown.
}
// case:
void IOForceViewModelTest::swapIO006()
{
    auto forceIoDataTbl = m_model->forceIoDataTbl();
    auto ioSwapConfigTbl = m_model->ioSwapConfigTbl();
    auto m_ioSwapDataTbl = m_model->m_ioSwapDataTbl;
    for (int i = 0; i < 8; i++) {
        // setup.
        m_communication->m_modbusWriteCommandList.clear();
        m_communication->m_oneShotModbusCommandList.clear();
        auto* forceIo = static_cast<ForceIOData*>(forceIoDataTbl[i]);
        auto* ioSwapConfig = static_cast<IOSwapConfiguration*>(ioSwapConfigTbl[i]);
        auto* ioSwapData = static_cast<IOSwapData*>(m_ioSwapDataTbl[i]);
        // test.
        m_model->swapIO(forceIo, ioSwapConfig);
        // verify.
        QCOMPARE(m_communication->m_modbusWriteCommandList.size(), 1);
        stModBusCommunicationData data;
        data = m_communication->m_modbusWriteCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::WriteMultipleRegisters);
        QCOMPARE(data.writeAddress, SWAP_INSTRUCTION_HR + (i * 2));
        QCOMPARE(data.comSize, 2);
        QCOMPARE(data.writeBuf[0], forceIo->bankNo());
        QCOMPARE(data.writeBuf[1], ioSwapConfig->bankNo());

        QCOMPARE(m_communication->m_oneShotModbusCommandList.size(), 1);
        data = m_communication->m_oneShotModbusCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
        QCOMPARE(data.readAddress, SWAP_INSTRUCTION_HR + (i * 2));
        QCOMPARE(data.comSize, 2);
        // teardown.
        quint16 prevBankNo;
        ioSwapData->setFromBankNo(forceIo->bankNo(),prevBankNo);
        ioSwapData->setToBankNo(ioSwapConfig->bankNo(), prevBankNo);
    }
}
