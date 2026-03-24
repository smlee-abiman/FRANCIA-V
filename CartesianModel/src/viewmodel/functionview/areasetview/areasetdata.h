#ifndef AREASETDATA_H
#define AREASETDATA_H

#include <QObject>

class AreaSetData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
public:
    explicit AreaSetData(QObject *parent = nullptr);
    void setValue(int value);
    void setEnable(int enable);
    int value() const { return m_value; }
    bool enable() const { return m_enable; }

signals:
    void valueChanged(int value);
    void enableChanged(bool enable);

private:
    int     m_value = 0;
    bool    m_enable = true;
};

#endif // AREASETDATA_H
