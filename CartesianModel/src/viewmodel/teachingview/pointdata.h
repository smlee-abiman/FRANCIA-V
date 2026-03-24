#ifndef POINTDATA_H
#define POINTDATA_H

#include <QObject>

class PointData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString valueStr READ valueStr WRITE setValueStr NOTIFY valueStrChanged)
public:
    explicit PointData(QObject *parent = nullptr);

    void setValue(int value);
    int value() const { return m_value; }
    void setStyle(int style);
    int style() const { return m_style; }
    void setAddress(int address);
    int address() const { return  m_address; }
    void setValueStr(QString valueStr);
    QString valueStr() const { return m_valueStr; }

signals:
    void valueChanged(int value);
    void styleChanged(int style);
    void addressChanged(int address);
    void valueStrChanged(QString valueStr);

public slots:
private:
    int     m_value = 0;
    int     m_style = 0;
    int     m_address = 0;
    QString m_valueStr = "";
};

#endif // POINTDATA_H
