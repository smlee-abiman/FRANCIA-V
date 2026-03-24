#ifndef PROGRAMMENU2_4_6ITEM_H
#define PROGRAMMENU2_4_6ITEM_H

#include <QObject>

class ProgramMenu2_4_6Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool select MEMBER m_select NOTIFY selectChanged)
    Q_PROPERTY(int acceleration MEMBER m_acceleration NOTIFY accelerationChanged)
public:
    explicit ProgramMenu2_4_6Item(QObject *parent = nullptr);
    void init();
    void setSelect(bool select);
    bool select() const { return m_select; }
    void setAcceleration(int value);
    int acceleration() const { return m_acceleration; }

signals:
    void selectChanged();
    void accelerationChanged();

private:
    bool    m_select = false;
    int     m_acceleration = 0;
};

#endif // PROGRAMMENU2_4_6ITEM_H
