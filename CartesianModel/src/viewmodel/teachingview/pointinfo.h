#ifndef POINTINFO_H
#define POINTINFO_H

#include <QObject>
#include "label.h"
#include "pointdata.h"

class PointInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString labelName READ labelName WRITE setLabelName NOTIFY labelNameChanged)
    Q_PROPERTY(QString labelColor READ labelColor WRITE setLabelColor NOTIFY labelColorChanged)
    Q_PROPERTY(QList<QObject *> positionList READ positionList WRITE setPositionList NOTIFY positionListChanged)
    Q_PROPERTY(QList<QObject *> sppedList READ sppedList WRITE setSppedList NOTIFY sppedListChanged)
    Q_PROPERTY(int pointNo READ pointNo CONSTANT)
public:
    explicit PointInfo(int numAxis, QObject *parent = nullptr);
    explicit PointInfo(QObject *parent = nullptr);

    void setAddress(int axis, int positionAddress, int speedAddress);
    void setDataSize(quint16 size);
    quint16 dataSize() const { return m_dataSize; }
    void setStyle(int axis, int positionStyle, int speedStyle);
    void setPosition(int axis, int position);
    void setPosition(int axis, QString position);
    int getPosition(int axis);
    void setSpeed(int axis, int speed);
    int getSpeed(int axis);
    int getPositionAddress(int axis, int &address);
    int getSpeedAddress(int axis, int &address);
    int positionAddress(int axis);
    int speedAddress(int axis);
    void setPointNo(int no) { m_pointNo = no; }
    int pointNo() { return m_pointNo; }
    void setLabelName(const QString &name);
    QString labelName() const { return m_labelName; }
    void setLabelColor(const QString &color);
    QString labelColor() const { return m_labelColor; }
    void setPositionList(QList<QObject *>& list);
    QList<QObject *> positionList() const { return m_positionList; }
    void setSppedList(QList<QObject *>& list);
    QList<QObject *> sppedList() const { return m_sppedList; }

signals:
    void labelNameChanged(const QString &color);
    void labelColorChanged(const QString &color);
    void positionListChanged(QList<QObject *> list);
    void sppedListChanged(QList<QObject *> list);

public slots:
private:
    QString             m_labelName = "";
    QString             m_labelColor = "";
    QList<QObject *>    m_positionList;
    QList<QObject *>    m_sppedList;
    quint16             m_dataSize = 0;
    int                 m_pointNo = 0;
};

#endif // POINTINFO_H
