#ifndef VIEWMODELBASE_H
#define VIEWMODELBASE_H

#include <QObject>
#include "resourcemanager.h"
#include "modbusmanager.h"
#include "communicationthread.h"
#include "operationrecord.h"
#include "axisinfo.h"

class ViewModelBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isViewActive READ isViewActive NOTIFY isViewActiveChanged)
public:
    explicit ViewModelBase(ResourceManager *resource, CommunicationThread *communication);
    Q_INVOKABLE void activate();
    Q_INVOKABLE void deactivate();

    bool isViewActive() { return m_viewActive; }

protected:
    virtual void onActivate();
    virtual void onDeactivate();

private:
    void setViewActive(bool viewActive);

signals:
    bool isViewActiveChanged(bool isViewActive);

public slots:
    virtual void update() {}

protected:
    ResourceManager     *m_resource;
    ModbusManager       *m_modbus = ModbusManager::get_instance();
    CommunicationThread *m_communication;
    OperationRecord     *m_operationRecord = OperationRecord::get_instance();

private:
    bool    m_viewActive = false;

};

#endif // VIEWMODELBASE_H
