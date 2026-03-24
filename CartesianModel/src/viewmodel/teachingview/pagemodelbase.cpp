#include "pagemodelbase.h"

PageModelBase::PageModelBase(ResourceManager *resource, CommunicationThread * comm) :
    ViewModelBase(resource, comm)
{
    m_pageLabel = new Label(this);
}

void PageModelBase::setPageLabelName(const QString &name)
{
    m_pageLabel->setName(name);
}

void PageModelBase::setPageLabelColor(const QString &color)
{
    m_pageLabel->setColor(color);
}


void PageModelBase::setPageLabel(Label *label)
{
    if (m_pageLabel != label) {
        m_pageLabel = label;
        emit pageLabelChanged(m_pageLabel);
    }
}

void PageModelBase::setActivateIconPath(const QString &path)
{
    if (m_activateIconPath != path) {
        m_activateIconPath = path;
        emit activateIconPathChanged(m_activateIconPath);
    }
}

void PageModelBase::setDeactivateIconPath(const QString &path)
{
    if (m_deactivateIconPath != path) {
        m_deactivateIconPath = path;
        emit deactivateIconPathChanged(m_deactivateIconPath);
    }
}
