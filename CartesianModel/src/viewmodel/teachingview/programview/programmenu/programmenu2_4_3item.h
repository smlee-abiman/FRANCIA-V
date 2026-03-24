#ifndef PROGRAMMENU2_4_3ITEM_H
#define PROGRAMMENU2_4_3ITEM_H

#include <QObject>

class ProgramMenu2_4_3Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool select MEMBER m_select NOTIFY selectChanged)
    Q_PROPERTY(int position MEMBER m_position NOTIFY positionChanged)
public:
    explicit ProgramMenu2_4_3Item(QObject *parent = nullptr);
    void init();
    void setSelect(bool select);
    bool select() const { return m_select; }
    void setPosition(int position);
    int position() const { return m_position; }

signals:
    void selectChanged();
    void positionChanged();

public slots:
private:
    bool    m_select = false;
    int     m_position = 0;
};

#endif // PROGRAMMENU2_4_3ITEM_H
