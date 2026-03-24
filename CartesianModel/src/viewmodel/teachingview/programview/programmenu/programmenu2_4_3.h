#ifndef PROGRAMMENU2_4_3_H
#define PROGRAMMENU2_4_3_H

#include "programmenu.h"

class ProgramMenu2_4_3 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(bool passOn MEMBER m_passOn NOTIFY passOnChanged)
public:
    explicit ProgramMenu2_4_3(ResourceManager *resource,CommunicationThread *comm);
    Q_INVOKABLE int setPosition(int axis, int position);
    Q_INVOKABLE QString formatPosition(int position);
    Q_INVOKABLE void clearSelect();
    void setPassOn(bool on);
    void show(NCPCommand *command) override;
    NCPCommand *createNcpCommand() override;

private:
    void init();
    void onDeactivate() override;
    void initItemList();

signals:
    void itemListChanged();
    void passOnChanged();

public slots:
private:
    QList<QObject *>    m_itemList;
    bool                m_passOn = true;
};

#endif // PROGRAMMENU2_4_3_H
