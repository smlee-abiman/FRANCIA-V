#ifndef AXISINFO_H
#define AXISINFO_H

#include <QObject>
#include "resourcemanager.h"

class AxisInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> axisDataList READ axisDataList WRITE setAxisDataList NOTIFY axisDataListChanged)

private:
    explicit AxisInfo(QObject *parent = nullptr);

    static AxisInfo* instance;

public:
    enum Axis {
       AxisY = 0,
       AxisMX,
       AxisSX,
       AxisMZ,
       AxisSZ,
       Axis6,
       Axis7,
       Axis8,
       AxisNum
   };

    static AxisInfo* create_instance(QObject *parent = 0){
        if(!instance){
            instance = new AxisInfo(parent);
        }
        return instance;
    }
    static AxisInfo* get_instance(){
        return instance;
    }
    static void destroy_instance(){
        delete instance;
        instance = nullptr;
    }

    int setDatas(Axis axis, bool home, bool over, bool brake, int origin, qint32 position, qint16 encoder, qint16 torque);
    int setDatas(Axis axis, bool home, bool over, bool brake, int origin, QString position, QString encoder, QString torque);
    int setDatas(Axis axis, bool home, bool over, bool brake, bool arrival, QString position, QString encoder, QString torque, QString speed);
    int setDatas(Axis axis, bool home, bool over, bool brake, int origin, QString originPercent, QString position, QString encoder, QString torque);
    int setDatas(Axis axis, QString position, QString startPos, QString startSpeed,QString pointCount, QString pointValue, QString pitch, bool direction);
    int setDatas(Axis axis, QString position, QString correctValue, QString speed, QString pointCount, QString pointValue);

    int setOriginDatas(Axis axis, bool home, bool over, bool brake, int origin, QString originPercent, QString encoder, QString torque);
    int setStandarStackDatas(Axis axis, QString startPos, QString startSpeed,QString pointCount, QString pointValue, QString pitch, bool direction);
    int setFreeStackDatas(Axis axis, QString correctValue, QString speed, QString pointCount, QString pointValue);

    int setDatasRT(Axis axis, QString speed, QString deviation, QString torque, QString loadRate, QString resurrectionResistorRate);
    int setDatasPeak(Axis axis, QString speedPeak, QString deviationPeak, QString torquePeak);
    int setPosition(Axis axis, const QString &position);
    int setSpeed(Axis axis, const QString &speed);
    int setEncoder(Axis axis, const QString &encoder);
    int setTorque(Axis axis, const QString &torque);
    int setOriginPercent(Axis axis, const QString &originPercent);
    int setServo(Axis axis, bool servo);
    int setMaxPosition(Axis axis, const QString &max);
    int setDatasWaveform(Axis axis, int speed, int torque);
    int setHome(Axis axis, int home);
    int setOver(Axis axis, int over);
    int setBrake(Axis axis, int brake);
    int setDatasMin(Axis axis, QString speedMin, QString deviationMin, QString torqueMin);
    int numAxis() const { return m_axisDataList.length(); }

    void setAxisDataList(QList<QObject *> list);
    QList<QObject *> axisDataList() const { return m_axisDataList; }

signals:
    void axisDataListChanged(QList<QObject *> list);

public slots:
private:
    QList<QObject *>   m_axisDataList;
};

#endif // AXISINFO_H
