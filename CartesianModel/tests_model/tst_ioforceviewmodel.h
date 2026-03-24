#ifndef IOFORCEVIEWMODELTEST_H
#define IOFORCEVIEWMODELTEST_H

#include <QObject>
#include <QtTest>

#include "mock/mock_modbusmanager.h"
#include "mock/mock_communicationthread.h"
#include "resourcemanager.h"

class IOForceViewModel;

class IOForceViewModelTest : public QObject
{
    Q_OBJECT
public:
    explicit IOForceViewModelTest(QObject *parent = nullptr);

private:
    void createModel();
    void deleteModel();

private:
    ResourceManager *m_resource;
    CommunicationThread *m_communication;
    ModbusManager* m_modbus;
    IOForceViewModel* m_model;

private slots:
    void initTestCase();            // will be called before the first test function is executed.
    void initTestCase_data() { };   // will be called to create a global test data table.
    void cleanupTestCase();         // will be called after the last test function was executed.
    void init();                    // will be called before each test function is executed.
    void cleanup();                 // will be called after every test function.
    void onOneShotReadFinished001();
    void onOneShotReadFinished002();
    void onOneShotReadFinishe_udupdateForcedOn001();
    void onOneShotReadFinishe_udupdateForcedOn002();
    void onOneShotReadFinishe_udupdateForcedOn003();
    void onOneShotReadFinishe_udupdateForcedOn004();
    void onOneShotReadFinishe_udupdateForcedOn005();
    void onOneShotReadFinishe_udupdateForcedOn006();
    void onOneShotReadFinishe_udupdateForcedOn007();
    void onOneShotReadFinishe_udupdateForcedOff001();
    void onOneShotReadFinishe_udupdateForcedOff002();
    void onOneShotReadFinishe_udupdateForcedOff003();
    void onOneShotReadFinishe_updateSwapData001();
    void onOneShotReadFinishe_updateSwapData002();
    void onOneShotReadFinishe_updateSwapData003();
    void onOneShotReadFinishe_updateSwapData004();
    void onOneShotReadFinishe_updateSwapData005();
    void onFinished_updatePeriodicData001();
    void onFinished_updatePeriodicData002();
    void onFinished_updatePeriodicData003();
    void onFinished_updatePeriodicData004();
    void forciblyTurnOn001();
    void forciblyTurnOn002();
    void forciblyTurnOn003();
    void forciblyTurnOn004();
    void forciblyTurnOff001();
    void forciblyTurnOff002();
    void forciblyTurnOff003();
    void forciblyTurnOff004();
    void inversTurnOn001();
    void inversTurnOn002();
    void inversTurnOn003();
    void inversTurnOn004();
    void inversTurnOff001();
    void inversTurnOff002();
    void inversTurnOff003();
    void inversTurnOff004();
    void swapIO001();
    void swapIO002();
    void swapIO003();
    void swapIO004();
    void swapIO005();
    void swapIO006();
};

#endif // IOFORCEVIEWMODELTEST_H
