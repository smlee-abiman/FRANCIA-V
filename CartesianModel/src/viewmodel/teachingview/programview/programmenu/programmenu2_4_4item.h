#ifndef PROGRAMMENU2_4_4ITEM_H
#define PROGRAMMENU2_4_4ITEM_H

#include <QObject>

class ProgramMenu2_4_4Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool select MEMBER m_select NOTIFY selectChanged)
    Q_PROPERTY(int speed MEMBER m_speed NOTIFY speedChanged)
public:
    explicit ProgramMenu2_4_4Item(QObject *parent = nullptr);
    void init();
    void setSelect(bool select);
    bool select() const { return m_select; }
    void setSpeed(int speed);
    int speed() const { return m_speed; }

signals:
    void selectChanged();
    void speedChanged();

private:
    bool    m_select = false;
    int     m_speed = 0;
};

#endif // PROGRAMMENU2_4_4ITEM_H
