/*
 * This class is container for QML. QML can understand this class members
 */

#ifndef PACKETDATA_H
#define PACKETDATA_H

#include <QObject>
#include <QDebug>
#include <QString>

class PacketData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit PacketData(QObject *parent = nullptr);
    explicit PacketData(QString label, QString value, QObject *parent = nullptr);
    explicit PacketData(QString label, QString value, QString unit, QObject *parent = nullptr);

    ~PacketData() {}

    QString label() const { return m_label; }

    QString value() const { return m_value; }

    QString unit() const { return m_unit; }

    QString color() const { return m_color; }

    quint16 dataFormat() const { return m_dataFormat; }
    void setDataFormat(quint16 dataFormat) { m_dataFormat = dataFormat; }

signals:

    void labelChanged(QString label);

    void valueChanged(QString value);

    void unitChanged(QString unit);

    void colorChanged(QString color);

public slots:

    void setLabel(QString label)
    {
        if (m_label == label)
            return;

        m_label = label;
        emit labelChanged(m_label);
    }

    void setValue(QString value)
    {
        if (m_value == value)
            return;

        m_value = value;
        emit valueChanged(m_value);
    }

    void setUnit(QString unit)
    {
        if (m_unit == unit)
            return;

        m_unit = unit;
        emit unitChanged(m_unit);
    }

    void setColor(QString color)
    {
        if (m_color == color)
            return;

        m_color = color;
        emit colorChanged(m_color);
    }

private:
    QString m_label;
    QString m_value;
    QString m_unit;
    QString m_color;
    quint16 m_dataFormat;
};
Q_DECLARE_METATYPE(PacketData *);

#endif // PACKETDATA_H
