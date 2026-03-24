#ifndef PROGRAMMENU2_4_6_H
#define PROGRAMMENU2_4_6_H

#include "programmenu.h"

class ProgramMenu2_4_6 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(bool accOn MEMBER m_accOn NOTIFY accOnChanged)
public:
    explicit ProgramMenu2_4_6(ResourceManager *resource,CommunicationThread *comm);
    Q_INVOKABLE void clearSelect();
    void setAccOn(bool on);
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;

private:
    void init();
    void onDeactivate() override;
    void initItemList();

signals:
    void itemListChanged();
    void accOnChanged();

public slots:
private:
    QList<QObject *>        m_itemList;
    bool                    m_accOn = true;
};
#endif // PROGRAMMENU2_4_6_H
