/*
 * This class is container for QML. QML can understand this class members
 */

#ifndef LIBPACKETDATA_H
#define LIBPACKETDATA_H

#include <QObject>
#include <QDebug>
#include <QString>

class LibPacketData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)

public:
    explicit LibPacketData(QObject *parent = nullptr);
    explicit LibPacketData(QString label, QString value, QObject *parent = nullptr);
    explicit LibPacketData(QString label, QString value, QString unit, QObject *parent = nullptr);

    ~LibPacketData() {
    }

    QString label() const
    {
        return m_label;
    }

    QString value() const
    {
        return m_value;
    }

    QString unit() const
    {
        return m_unit;
    }

signals:

    void labelChanged(QString label);

    void valueChanged(QString value);

    void unitChanged(QString unit);

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

private:
    QString m_label;
    QString m_value;
    QString m_unit;
};
Q_DECLARE_METATYPE(LibPacketData *);

#endif // LIBPACKETDATA_H
