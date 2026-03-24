#ifndef PROGRAMMENU2_1ITEM_H
#define PROGRAMMENU2_1ITEM_H

#include <QObject>

class ProgramMenu2_1Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool select MEMBER m_select NOTIFY selectChanged)
    Q_PROPERTY(int position MEMBER m_position NOTIFY positionChanged)
    Q_PROPERTY(int speed MEMBER m_speed NOTIFY speedChanged)
public:
    explicit ProgramMenu2_1Item(QObject *parent = nullptr);
    void init();
    void setSelect(bool select);
    void setPositon(int position);
    void setSpeed(int speed);
    bool select() const { return m_select; }
    int position() const { return m_position; }
    int speed() const { return m_speed; }
signals:
    void selectChanged();
    void positionChanged();
    void speedChanged();

private:
    bool    m_select = false;
    int     m_position = 0;
    int     m_speed = 0;
};

#endif // PROGRAMMENU2_1ITEM_H
