#include <QtTest>
// #include <QDebug>

#include "mock/mock_modbusmanager.h"
#include "mock/mock_communicationthread.h"

#include "tst_teachingtimerpagemodel.h"

#include "appenvironment.h"
#include "teachingviewdef.h"
#include "teachingtimerpagemodel.h"

TeachingTimerPageModelTest::TeachingTimerPageModelTest(QObject* parent)
    : QObject(parent)
    , m_resource(nullptr)
    , m_communication(nullptr)
    , m_modbus(nullptr)
    , m_model(nullptr)
{
}
void TeachingTimerPageModelTest::initTestCase()
{
    AppEnvironment::initialize("../CartesianModel/bin");
    m_resource = new ResourceManager(".");
}
void TeachingTimerPageModelTest::cleanupTestCase()
{
    delete m_resource;
    m_resource = nullptr;
}
void TeachingTimerPageModelTest::init()
{
    m_communication = CommunicationThread::create_instance(nullptr);
    ModbusManager::instanceRenew();
    m_modbus = ModbusManager::get_instance();
    createModel();
}
void TeachingTimerPageModelTest::cleanup()
{
    deleteModel();
    CommunicationThread::destroy_instance();
    m_communication = nullptr;
}
void TeachingTimerPageModelTest::testCase1()
{
    // setup.
    // test.
    m_model->activate();
    // verify.
    QCOMPARE(1, m_communication->m_oneShotModbusCommandList.length());
    stModBusCommunicationData data = m_communication->m_oneShotModbusCommandList.at(0);
    QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
    QCOMPARE(data.readAddress, TIMER_TIME_TOP_HR);
    QCOMPARE(data.comSize, TIMER_TIME_HR_NUM);
    // teardown.
}
void TeachingTimerPageModelTest::testCase2()
{
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = TIMER_TIME_HR_NUM;
    command.readAddress = TIMER_TIME_TOP_HR;
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i, i);
    }
    QCOMPARE(TIMER_LIST_ITEM_NUM, m_modbus->m_receiveDataMapHR.size());   // all readouts done.
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerTeachingTimerPageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(0, m_modbus->m_receiveDataMapHR.size());   // all readouts done.
    QList<QObject *> dataList = m_model->dataList();
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->time(), i);
    }
    // teardown.
}
void TeachingTimerPageModelTest::testCase3()
{
    // setup.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = TIMER_TIME_HR_NUM;
    command.readAddress = TIMER_TIME_TOP_HR;
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i, i);
    }
    QCOMPARE(TIMER_LIST_ITEM_NUM, m_modbus->m_receiveDataMapHR.size());
    QList<QObject *> dataList = m_model->dataList();
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->time(), 0);
    }
    // test.
    // caller is not CallerTeachingTimerPageModel.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerDefault;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(TIMER_LIST_ITEM_NUM, m_modbus->m_receiveDataMapHR.size());
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->time(), 0);  // not update.
    }
    // teardown.
}
void TeachingTimerPageModelTest::testCase4()
{
    // setup.
    // The read address is before TIMER_TIME_TOP_HR.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = TIMER_TIME_HR_NUM;
    command.readAddress = TIMER_TIME_TOP_HR - 1;
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i, i);
    }
    QCOMPARE(TIMER_LIST_ITEM_NUM, m_modbus->m_receiveDataMapHR.size());
    QList<QObject *> dataList = m_model->dataList();
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->time(), 0);
    }
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerTeachingTimerPageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(TIMER_LIST_ITEM_NUM, m_modbus->m_receiveDataMapHR.size());
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->time(), 0);  // not update.
    }
    // teardown.
}
void TeachingTimerPageModelTest::testCase5()
{
    // setup.
    // The read address is after TIMER_TIME_BOTTOM_HR.
    stModBusCommunicationData command;
    command.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    command.comSize = TIMER_TIME_HR_NUM;
    command.readAddress = TIMER_TIME_BOTTOM_HR + 1;
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        m_modbus->m_receiveDataMapHR.insert(command.readAddress + i, i);
    }
    QCOMPARE(TIMER_LIST_ITEM_NUM, m_modbus->m_receiveDataMapHR.size());
    QList<QObject *> dataList = m_model->dataList();
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->time(), 0);
    }
    // test.
    CommunicationThread::OneShotReadModBusCaller oneShotReadCaller =
        CommunicationThread::CallerTeachingTimerPageModel;
    m_model->onOneShotReadFinished(command, oneShotReadCaller);
    // verify.
    QCOMPARE(TIMER_LIST_ITEM_NUM, m_modbus->m_receiveDataMapHR.size());
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->time(), 0);  // not update.
    }
    // teardown.
}
void TeachingTimerPageModelTest::testCase6()
{
    // setup.
    QList<QObject *> dataList = m_model->dataList();
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->startState(), false);
        QCOMPARE(data->upState(), false);
    }
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_TIMER_PAGE, PEN_SET_TIMER_ADDRESS_START_ROW + i);
        m_modbus->m_receiveDataMapDI.insert(rowData[1], true);
        m_modbus->m_receiveDataMapDI.insert(rowData[2], true);
    }
    // test.
    m_model->onFinished();
    // verify.
    QCOMPARE(0, m_modbus->m_receiveDataMapDI.size());
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->startState(), true);
        QCOMPARE(data->upState(), true);
    }
    // teardown.
    // setup.
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_TIMER_PAGE, PEN_SET_TIMER_ADDRESS_START_ROW + i);
        m_modbus->m_receiveDataMapDI.insert(rowData[1], false);
        m_modbus->m_receiveDataMapDI.insert(rowData[2], false);
    }
    // test.
    m_model->onFinished();
    // verify.
    QCOMPARE(0, m_modbus->m_receiveDataMapDI.size());
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        TimerData *data = static_cast<TimerData *>(dataList[i]);
        QCOMPARE(data->startState(), false);
        QCOMPARE(data->upState(), false);
    }
}
void TeachingTimerPageModelTest::testCase7()
{
    for (int i = 0; i < TIMER_LIST_ITEM_NUM; i++) {
        // setup.
        m_communication->m_modbusWriteCommandList.clear();
        m_communication->m_oneShotModbusCommandList.clear();
        QVector<int> rowData = m_resource->getIntRowArray(PEN_SET_GROUP_TIMER_PAGE, PEN_SET_TIMER_ADDRESS_START_ROW + i);
        // test.
        QCOMPARE(0, m_model->setTime(i, i));
        // verify.
        QCOMPARE(1, m_communication->m_modbusWriteCommandList.length());
        stModBusCommunicationData data = m_communication->m_modbusWriteCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::WriteMultipleRegisters);
        QCOMPARE(data.writeAddress, rowData[0]);
        QCOMPARE(data.comSize, 1);
        QCOMPARE(i * TIMER_TIME_MAGNIFICATION, data.writeBuf[0]);

        QCOMPARE(1, m_communication->m_oneShotModbusCommandList.length());
        data = m_communication->m_oneShotModbusCommandList.at(0);
        QCOMPARE(data.funcCode, QModbusPdu::FunctionCode::ReadHoldingRegisters);
        QCOMPARE(data.readAddress, TIMER_TIME_TOP_HR + i);
        QCOMPARE(data.comSize, 1);
        // teardown.
    }
}
