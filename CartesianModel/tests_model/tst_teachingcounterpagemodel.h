#ifndef TEACHINGCOUNTERPAGEMODELTEST_H
#define TEACHINGCOUNTERPAGEMODELTEST_H

#include <QObject>
#include <QtTest>

#include "mock/mock_modbusmanager.h"
#include "mock/mock_communicationthread.h"
#include "resourcemanager.h"
#include "teachingcounterpagemodel.h"

class TeachingCounterPageModelTest : public QObject
{
    Q_OBJECT

public:
    explicit TeachingCounterPageModelTest(QObject* parent = nullptr);

private:
    void createModel() {
        if (m_model == nullptr) {
            m_model = new TeachingCounterPageModel(m_resource, m_communication);
        }
    };
    void deleteModel() {
        if (m_model != nullptr) {
            delete m_model;
            m_model = nullptr;
        }
    };

private:
    ResourceManager *m_resource;
    CommunicationThread *m_communication;
    ModbusManager* m_modbus;
    TeachingCounterPageModel* m_model;

private slots:
    void initTestCase();            // will be called before the first test function is executed.
    void initTestCase_data() { };   // will be called to create a global test data table.
    void cleanupTestCase();         // will be called after the last test function was executed.
    void init();                    // will be called before each test function is executed.
    void cleanup();                 // will be called after every test function.
    void define001();
    void activate001();
    void onOneShotReadFinished001();
    void onOneShotReadFinished002();
    void onOneShotReadFinished003();
    void onOneShotReadFinished004();
    void onFinished001();
    void editSettingValue001();
    void editSettingValue002();
    void editSettingValue003();
};

#endif // TEACHINGCOUNTERPAGEMODELTEST_H
