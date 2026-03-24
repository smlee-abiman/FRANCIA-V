#ifndef PAGEMODELBASE_H
#define PAGEMODELBASE_H

#include <QObject>
#include "viewmodelbase.h"
#include "label.h"

class PageModelBase : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(Label *pageLabel READ pageLabel WRITE setPageLabel NOTIFY pageLabelChanged)
    Q_PROPERTY(QString activateIconPath READ activateIconPath WRITE setActivateIconPath NOTIFY activateIconPathChanged)
    Q_PROPERTY(QString deactivateIconPath READ deactivateIconPath WRITE setDeactivateIconPath NOTIFY deactivateIconPathChanged)

public:
    explicit PageModelBase(ResourceManager *resource, CommunicationThread * comm);

    void setPageLabelName(const QString &name);
    void setPageLabelColor(const QString &color);

    void setPageLabel(Label *label);
    void setActivateIconPath(const QString &path);
    void setDeactivateIconPath(const QString &path);
    Label *pageLabel() const { return m_pageLabel; }
    QString activateIconPath() const { return m_activateIconPath; }
    QString deactivateIconPath() const { return m_deactivateIconPath; }

signals:
    void pageLabelChanged(Label *label);
    void activateIconPathChanged(QString path);
    void deactivateIconPathChanged(QString path);

public slots:
protected:
    Label           *m_pageLabel = nullptr;
    QString         m_activateIconPath = "";
    QString         m_deactivateIconPath = "";
};

#endif // PAGEMODELBASE_H
