#ifndef TEACHINGVIEWMODEL_H
#define TEACHINGVIEWMODEL_H

#include "viewmodelbase.h"
#include "teachingview/pagemodelbase.h"
#include "axisinfo.h"
#include "commonviewmodel.h"
#include "externalmemoryviewmodel.h"

class TeachingViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> pageModel READ pageModel CONSTANT)
    Q_PROPERTY(int currentPageTab READ currentPageTab WRITE setCurrentPageTab NOTIFY currentPageTabChanged)
public:
    explicit TeachingViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView, ExternalMemoryViewModel *extMemView);
    explicit TeachingViewModel(QObject *object = nullptr);
    ~TeachingViewModel();
    void setCurrentPageTab(int index);
    int currentPageTab() const { return m_currentPageTab; }
    QList<QObject *> pageModel() const { return m_pageModel; }

signals:
    void currentPageTabChanged(int index);

private:
    void initPage(CommonViewModel *commonView, ExternalMemoryViewModel *extMemView);

private:
    QList<QObject *>            m_pageModel;
    int                         m_currentPageTab = 0;
};

#endif // TEACHINGVIEWMODEL_H
