#include <QQmlEngine>
#include "teachingviewdef.h"
#include "teachingviewmodel.h"
#include "teachingmodepagemodel.h"
#include "teachingpossetpagemodel.h"
#include "teachingstackpagemodel.h"
#include "teachingtimerpagemodel.h"
#include "teachingcounterpagemodel.h"
#include "teachingpasspagemodel.h"
#include "teachingprogrampagemodel.h"

TeachingViewModel::TeachingViewModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView, ExternalMemoryViewModel *extMemView)
    : ViewModelBase(resource, comm)
{
    qmlRegisterType<TeachingViewModel>("TeachingViewModel", 1, 0, "TeachingViewModel");
    qmlRegisterType<TeachingModePageModel>("TeachingModePageModel", 1, 0, "TeachingModePageModel");
    qmlRegisterType<TeachingModeData>("TeachingModeData", 1, 0, "TeachingModeData");
    qmlRegisterType<Label>("Label", 1, 0, "Label");
    initPage(commonView, extMemView);
}

TeachingViewModel::TeachingViewModel(QObject *object)
    : ViewModelBase(nullptr, nullptr)
{
    Q_UNUSED(object)
}

TeachingViewModel::~TeachingViewModel()
{
    qDeleteAll(m_pageModel);
}
void TeachingViewModel::initPage(CommonViewModel *commonView, ExternalMemoryViewModel *extMemView)
{
    TeachingModePageModel *modePageModel = new TeachingModePageModel(m_resource, m_communication, commonView);
    m_pageModel.append(modePageModel);
    m_pageModel.append(new TeachingPosSetPageModel(m_resource, m_communication, modePageModel,commonView));
    m_pageModel.append(new TeachingStackPageModel(m_resource, m_communication, commonView));
    m_pageModel.append(new TeachingTimerPageModel(m_resource, m_communication));
    m_pageModel.append(new TeachingCounterPageModel(m_resource, m_communication));
    m_pageModel.append(new TeachingPassPageModel(m_resource, m_communication));
    m_pageModel.append(new TeachingProgramPageModel(m_resource, m_communication, extMemView));
}

void TeachingViewModel::setCurrentPageTab(int index)
{
    if (m_currentPageTab != index) {
        m_currentPageTab = index;
        emit currentPageTabChanged(m_currentPageTab);
    }
}
