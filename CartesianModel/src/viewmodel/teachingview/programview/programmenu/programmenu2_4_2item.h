#ifndef PROGRAMMENU2_4_2ITEM_H
#define PROGRAMMENU2_4_2ITEM_H

#include <QObject>

class ProgramMenu2_4_2Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool select MEMBER m_select NOTIFY selectChanged)
    Q_PROPERTY(int halfwayPosition MEMBER m_halfwayPosition NOTIFY halfwayPositionChanged)
    Q_PROPERTY(int endPosition MEMBER m_endPosition NOTIFY endPositionChanged)
    Q_PROPERTY(int speed MEMBER m_speed NOTIFY speedChanged)
public:
    explicit ProgramMenu2_4_2Item(QObject *parent = nullptr);
    void init();
    void setSelect(bool select);
    void setHalfwayPosition(int position);
    void setEndPosition(int position);
    void setSpeed(int speed);
    bool select() const { return m_select; }
    int halfwayPosition() const { return m_halfwayPosition; }
    int endPosition() const { return m_endPosition; }
    int speed() const { return m_speed; }
signals:
    void selectChanged();
    void halfwayPositionChanged();
    void endPositionChanged();
    void speedChanged();

private:
    bool    m_select = false;
    int     m_halfwayPosition = 0;
    int     m_endPosition = 0;
    int     m_speed = 0;
};

#endif // PROGRAMMENU2_4_2ITEM_H
