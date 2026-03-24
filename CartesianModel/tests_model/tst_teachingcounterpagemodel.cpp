#include <QtTest>

#include "mock/mock_modbusmanager.h"
#include "mock/mock_communicationthread.h"

#include "tst_teachingcounterpagemodel.h"

#include "appenvironment.h"
#include "teachingviewdef.h"
#include "teachingcounterpagemodel.h"

TeachingCounterPageModelTest::TeachingCounterPageModelTest(QObject* parent)
    : QObject(parent)
    , m_resource(nullptr)
    , m_communication(nullptr)
    , m_modbus(nullptr)
    , m_model(nullptr)
{
}
void TeachingCounterPageModelTest::initTestCase()
{
    AppEnvironment::initialize("../CartesianModel/bin");
    m_resource = new ResourceManager(".");
}
void TeachingCounterPageModelTest::cleanupTestCase()
{
    delete m_resource;
    m_resource = nullptr;
}
void TeachingCounterPageModelTest::init()
{
    m_communication = CommunicationThread::create_instance(nullptr);
    ModbusManager::instanceRenew();
    m_modbus = ModbusManager::get_instance();
    createModel();
}
void TeachingCounterPageModelTest::cleanup()
{
    deleteModel();
    CommunicationThread::destroy_instance();
    m_communication = nullptr;
}
void TeachingCounterPageModelTest::define001()
{
    // verify.
    QCOMPARE(CURRENTVALUE_SR, 2777);
    QCOMPARE(SETTINGVALUE_HR, 2293);
}
void TeachingCounterPageModelTest::activate001()
{
    // setup.
    QCOMPARE(m_communication->m_oneShotModbusCommandList.length(), 0);
    // test.
    m_model->activate();
    // verify.
    QCOMPARE(m_communication->m_oneShotModbusCommandList.length(), 1);
    stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, SETTINGVALUE_HR);
    QCOMPARE(data.comSize, m_model->numOfCounter() * 2);
    // teardown.
}
void TeachingCounterPageModelTest::onOneShotReadFinished001()
{
    // setup.
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], 0);
    }
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = m_model->numOfCounter() * 2;
    command.readAddress = SETTINGVALUE_HR;
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        m_modbus->m_longReceiveDataHR.insert(command.readAddress + (i * 2), i);
    }
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), m_model->numOfCounter());
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerTeachingCounterPageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), 0);
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], i);
    }
    // teardown.
}
void TeachingCounterPageModelTest::onOneShotReadFinished002()
{
    // setup.
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], 0);
    }
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = m_model->numOfCounter() * 2;
    command.readAddress = SETTINGVALUE_HR;
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        m_modbus->m_longReceiveDataHR.insert(command.readAddress + (i * 2), i);
    }
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), m_model->numOfCounter());
    // test.
    // caller is not CallerTeachingTimerPageModel.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerDefault;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), m_model->numOfCounter());
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], 0);
    }
    // teardown.
}
void TeachingCounterPageModelTest::onOneShotReadFinished003()
{
    // setup.
    // The read address is before SETTINGVALUE_HR.
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], 0);
    }
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = m_model->numOfCounter() * 2;
    command.readAddress = SETTINGVALUE_HR - 1;
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        m_modbus->m_longReceiveDataHR.insert(command.readAddress + (i * 2), i);
    }
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), m_model->numOfCounter());
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerTeachingCounterPageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), m_model->numOfCounter());
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], 0);
    }
    // teardown.
}
void TeachingCounterPageModelTest::onOneShotReadFinished004()
{
    // setup.
    // The read address is after SETTINGVALUE_HR_END.
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], 0);
    }
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = m_model->numOfCounter() * 2;
    command.readAddress = SETTINGVALUE_HR_END + 1;
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        m_modbus->m_longReceiveDataHR.insert(command.readAddress + (i * 2), i);
    }
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), m_model->numOfCounter());
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerTeachingCounterPageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(m_modbus->m_longReceiveDataHR.size(), m_model->numOfCounter());
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> settingValueTbl = m_model->settingValueTbl();
        QCOMPARE(settingValueTbl[i], 0);
    }
    // teardown.
}
void TeachingCounterPageModelTest::onFinished001()
{
    // setup.
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> currentValueTbl = m_model->currentValueTbl();
        QCOMPARE(currentValueTbl[i], 0);
    }
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        m_modbus->m_longReceiveDataRG.insert(CURRENTVALUE_SR + (i * 2), i);
    }
    QCOMPARE(m_modbus->m_longReceiveDataRG.size(), m_model->numOfCounter());
    // test.
    m_model->onFinished();
    // verify.
    QCOMPARE(m_modbus->m_longReceiveDataRG.size(), 0);
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        QList<int> currentValueTbl = m_model->currentValueTbl();
        QCOMPARE(currentValueTbl[i], i);
    }
    // teardown.
}
void TeachingCounterPageModelTest::editSettingValue001()
{
    for (int i = 0; i < m_model->numOfCounter(); i++) {
        // setup.
        m_communication->m_modbusWriteCommandList.clear();
        m_communication->m_oneShotModbusCommandList.clear();
        // test.
        int editValue = i + (i << 16);
        m_model->editSettingValue(i, editValue);
        // verify.
        stModBusCommunicationData data;
        QCOMPARE(m_communication->m_modbusWriteCommandList.length(), 1);
        data = m_communication->m_modbusWriteCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::WriteMultipleRegisters);
        QCOMPARE(data.writeAddress, SETTINGVALUE_HR + (i * 2));
        QCOMPARE(data.comSize, 2);
        QCOMPARE(data.writeBuf[0], (editValue & 0x0000FFFF));
        QCOMPARE(data.writeBuf[1], (editValue & 0xFFFF0000) >> 16);

        QCOMPARE(m_communication->m_oneShotModbusCommandList.length(), 1);
        data = m_communication->m_oneShotModbusCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
        QCOMPARE(data.readAddress, SETTINGVALUE_HR + (i * 2));
        QCOMPARE(data.comSize, 2);
        // teardown.
    }
}
void TeachingCounterPageModelTest::editSettingValue002()
{
    // setup.
    // test.
    m_model->editSettingValue(-1, 0);
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.length(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.length(), 0);
    // teardown.
}
void TeachingCounterPageModelTest::editSettingValue003()
{
    // setup.
    QList<int> settingValueTbl = m_model->settingValueTbl();
    // test.
    m_model->editSettingValue(settingValueTbl.size(), 0);
    // verify.
    QCOMPARE(m_communication->m_modbusWriteCommandList.length(), 0);
    QCOMPARE(m_communication->m_oneShotModbusCommandList.length(), 0);
    // teardown.
}
