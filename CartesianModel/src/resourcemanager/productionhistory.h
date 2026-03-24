#ifndef PRODUCTIONHISTORY_H
#define PRODUCTIONHISTORY_H

#include <QObject>

class ProductionHistory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int prodution READ prodution WRITE setProdution NOTIFY produtionChanged)
    Q_PROPERTY(int open READ open WRITE setOpen NOTIFY openChanged)
    Q_PROPERTY(int eject READ eject WRITE setEject NOTIFY ejectChanged)
public:
    explicit ProductionHistory(QObject *parent = nullptr);
    explicit ProductionHistory(int production, int open, int eject, QObject *parent = nullptr);

    int prodution() const { return m_prodution; }

    int open() const { return m_open; }

    int eject() const { return m_eject; }

signals:

    void produtionChanged(int prodution);

    void openChanged(int open);

    void ejectChanged(int eject);

public slots:

    void setProdution(int prodution)
    {
        if (m_prodution == prodution)
            return;

        m_prodution = prodution;
        emit produtionChanged(m_prodution);
    }

    void setOpen(int open)
    {
        if (m_open == open)
            return;

        m_open = open;
        emit openChanged(m_open);
    }

    void setEject(int eject)
    {
        if (m_eject == eject)
            return;

        m_eject = eject;
        emit ejectChanged(m_eject);
    }

private:
    int m_prodution;
    int m_open;
    int m_eject;
};

#endif // PRODUCTIONHISTORY_H
