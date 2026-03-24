#ifndef TST_TEACHINGTIMERPAGEMODEL_H
#define TST_TEACHINGTIMERPAGEMODEL_H

#include <QObject>
#include <QtTest>

#include "mock/mock_modbusmanager.h"
#include "mock/mock_communicationthread.h"
#include "resourcemanager.h"
#include "teachingtimerpagemodel.h"

class TeachingTimerPageModelTest : public QObject
{
    Q_OBJECT

public:
    explicit TeachingTimerPageModelTest(QObject* parent = nullptr);

private:
    void createModel() {
        if (m_model == nullptr) {
            m_model = new TeachingTimerPageModel(m_resource, m_communication);
        }
    }
    void deleteModel() {
        if (m_model != nullptr) {
            delete m_model;
            m_model = nullptr;
        }
    }

private:
    ResourceManager *m_resource;
    CommunicationThread *m_communication;
    ModbusManager* m_modbus;
    TeachingTimerPageModel* m_model;

private slots:
    void initTestCase();            // will be called before the first test function is executed.
    void initTestCase_data() { };   // will be called to create a global test data table.
    void cleanupTestCase();         // will be called after the last test function was executed.
    void init();                    // will be called before each test function is executed.
    void cleanup();                 // will be called after every test function.
    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();
    void testCase5();
    void testCase6();
    void testCase7();
};

#endif // TST_TEACHINGTIMERPAGEMODEL_H
