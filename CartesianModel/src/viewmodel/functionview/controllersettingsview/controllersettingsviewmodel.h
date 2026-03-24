#ifndef CONTROLLERSETTINGSVIEWMODEL_H
#define CONTROLLERSETTINGSVIEWMODEL_H

#include <QObject>
#include "viewmodelbase.h"
#include "systemsettingviewmodel.h"
#include "usersettingviewmodel.h"
#include "networksettingviewmodel.h"

class ControllerSettingsViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(SystemSettingViewModel* systemSettingView READ systemSettingView CONSTANT)
    Q_PROPERTY(UserSettingViewModel* userSettingView READ userSettingView CONSTANT)
    Q_PROPERTY(NetworkSettingViewModel* networkSettingView READ networkSettingView CONSTANT)

public:
    explicit ControllerSettingsViewModel(ResourceManager *resource, CommunicationThread * comm);
    virtual ~ControllerSettingsViewModel() override;

    SystemSettingViewModel* systemSettingView() const { return dynamic_cast<SystemSettingViewModel*>(m_panelModel.at(0)); }
    UserSettingViewModel* userSettingView() const { return dynamic_cast<UserSettingViewModel*>(m_panelModel.at(1)); }
    NetworkSettingViewModel* networkSettingView() const { return dynamic_cast<NetworkSettingViewModel*>(m_panelModel.at(2)); }

private:
    QList<QObject *>    m_panelModel;

    void initPanel();

signals:

};

#endif // CONTROLLERSETTINGSVIEWMODEL_H
