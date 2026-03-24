#include "viewmodelbase.h"

ViewModelBase::ViewModelBase(ResourceManager *resource, CommunicationThread *communication)
{
    m_resource = resource;
    m_communication = communication;
}

void ViewModelBase::activate()
{
    bool prev = m_viewActive;
    setViewActive(true);
    if (prev != m_viewActive)
        onActivate();
}

void ViewModelBase::deactivate()
{
    bool prev = m_viewActive;
    setViewActive(false);
    if (prev != m_viewActive)
        onDeactivate();
}

void ViewModelBase::onActivate()
{

}

void ViewModelBase::onDeactivate()
{

}

void ViewModelBase::setViewActive(bool viewActive)
{
    if (m_viewActive == viewActive) { return; }
    m_viewActive = viewActive;
    emit isViewActiveChanged(m_viewActive);
}
