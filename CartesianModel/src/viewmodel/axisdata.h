#ifndef AXISDATA_H
#define AXISDATA_H

#include <QObject>
#include <QDebug>

class AxisData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool home READ home WRITE setHome NOTIFY homeChanged)
    Q_PROPERTY(bool over READ over WRITE setOver NOTIFY overChanged)
    Q_PROPERTY(bool brake READ brake WRITE setBrake NOTIFY brakeChanged)
    Q_PROPERTY(int origin READ origin WRITE setOrigin NOTIFY originChanged)
    Q_PROPERTY(QString origin_s READ origin_s WRITE setOrigin_s NOTIFY origin_sChanged)
    Q_PROPERTY(QString position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QString encoder READ encoder WRITE setEncoder NOTIFY encoderChanged)
    Q_PROPERTY(QString torque READ torque WRITE setTorque NOTIFY torqueChanged)
    Q_PROPERTY(bool servo READ servo WRITE setServo NOTIFY servoChanged)
    Q_PROPERTY(bool arrival READ arrival WRITE setArrival NOTIFY arrivalChanged)
    Q_PROPERTY(QString speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(QString maxPosition READ maxPosition WRITE setMaxPosition NOTIFY maxPositionChanged)
    Q_PROPERTY(QString originPercent READ originPercent WRITE setOriginPercent NOTIFY originPercentChanged)
    Q_PROPERTY(QString startPos READ startPos WRITE setStartPos NOTIFY startPosChanged)
    Q_PROPERTY(QString startSpeed READ startSpeed WRITE setStartSpeed NOTIFY startSpeedChanged)
    Q_PROPERTY(QString pointCount READ pointCount WRITE setPointCount NOTIFY pointCountChanged)
    Q_PROPERTY(QString pointValue READ pointValue WRITE setPointValue NOTIFY pointValueChanged)
    Q_PROPERTY(QString pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(bool direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QString correctValue READ correctValue WRITE setCorrectValue NOTIFY correctValueChanged)
    Q_PROPERTY(QString deviation READ deviation WRITE setDeviation NOTIFY deviationChanged)
    Q_PROPERTY(QString loadRate READ loadRate WRITE setLoadRate NOTIFY loadRateChanged)
    Q_PROPERTY(QString resurrectionResistorRate READ resurrectionResistorRate WRITE setResurrectionResistorRate NOTIFY resurrectionResistorRateChanged)
    Q_PROPERTY(QString speedPeak READ speedPeak WRITE setSpeedPeak NOTIFY speedPeakChanged)
    Q_PROPERTY(QString torquePeak READ torquePeak WRITE setTorquePeak NOTIFY torquePeakChanged)
    Q_PROPERTY(QString deviationPeak READ deviationPeak WRITE setDeviationPeak NOTIFY deviationPeakChanged)
    Q_PROPERTY(int speedValue READ speedValue WRITE setSpeedValue NOTIFY speedValueChanged)
    Q_PROPERTY(int torqueValue READ torqueValue WRITE setTorqueValue NOTIFY torqueValueChanged)
    Q_PROPERTY(QString speedMin READ speedMin WRITE setSpeedMin NOTIFY speedMinChanged)
    Q_PROPERTY(QString deviationMin READ deviationMin WRITE setDeviationMin NOTIFY deviationMinChanged)
    Q_PROPERTY(QString torqueMin READ torqueMin WRITE setTorqueMin NOTIFY torqueMinChanged)

public:
    explicit AxisData(QObject *parent = nullptr);

    bool operator == (AxisData &dat);
    AxisData &operator =(AxisData &dat);
    AxisData &operator =(AxisData dat);

    bool home() const
    {
        //qDebug() << m_home;
        return m_home;
    }

    bool over() const
    {
        return m_over;
    }

    bool brake() const
    {
        return m_brake;
    }

    int origin() const
    {
        return m_origin;
    }

    QString origin_s() const
    {
        return m_origin_s;
    }

    QString position() const
    {
        return m_position;
    }

    QString encoder() const
    {
        return m_encoder;
    }

    QString torque() const
    {
        return m_torque;
    }

    bool servo() const
    {
        return m_servo;
    }

    bool arrival() const
    {
        return m_arrival;
    }

    QString speed() const
    {
        return m_speed;
    }

    QString maxPosition() const
    {
        return m_maxPosition;
    }

    QString originPercent() const
    {
        return m_originPercent;
    }

    QString startPos() const
    {
        return m_startPos;
    }

    QString startSpeed() const
    {
        return m_startSpeed;
    }

    QString pointCount() const
    {
        return m_pointCount;
    }

    QString pointValue() const
    {
        return m_pointValue;
    }

    QString pitch() const
    {
        return m_pitch;
    }

    bool direction() const
    {
        return m_direction;
    }
    QString correctValue() const
    {
        return m_correctValue;
    }
    QString deviation() const
    {
        return m_deviation;
    }
    QString loadRate() const
    {
        return m_loadRate;
    }
    QString resurrectionResistorRate() const
    {
        return m_resurrectionResistorRate;
    }
    QString speedPeak() const
    {
        return m_speedPeak;
    }
    QString torquePeak() const
    {
        return m_torquePeak;
    }
    QString deviationPeak() const
    {
        return m_deviationPeak;
    }
    int speedValue() const
    {
        return m_speedValue;
    }
    int torqueValue() const
    {
        return m_torqueValue;
    }
    QString speedMin() const
    {
        return m_speedMin;
    }
    QString deviationMin() const
    {
        return m_deviationMin;
    }
    QString torqueMin() const
    {
        return m_torqueMin;
    }

    void setDatas(bool home, bool over, bool brake, int origin, qint32 position, qint16 encoder, qint16 torque);
    void setDatas(bool home, bool over, bool brake, int origin, QString position, QString encoder, QString torque);
    void setDatas(bool home, bool over, bool brake, bool arrival, QString position, QString encoder, QString torque, QString speed);
    void setDatas(bool home, bool over, bool brake, int origin, QString originPercent, QString position, QString encoder, QString torque);
    void setDatas(QString position, QString startPos, QString startSpeed,QString pointCount, QString pointValue, QString pitch, bool direction);
    void setDatas(QString startPos, QString startSpeed,QString pointCount, QString pointValue, QString pitch, bool direction);
    void setDatas(QString position, QString correctValue, QString speed, QString pointCount, QString pointValue);
    void setDatas(QString correctValue, QString speed, QString pointCount, QString pointValue);
    void setDatasRT(QString speed, QString deviation, QString torque, QString loadRate, QString resurrectionResistorRate);
    void setDatasPeak(QString speedPeak, QString deviationPeak, QString torquePeak);
    void setDatasWaveform(int speed, int torque);
    void setDatasMin(QString speedMin, QString deviationMin, QString torqueMin);
    void setorgDatas(bool home, bool over, bool brake, int origin, QString originPercent, QString encoder, QString torque);

    int row() const
    {
        return m_row;
    }

public slots:
    void setHome(bool home)
    {
        if ((home == false) && (m_home == true)) {
            //qDebug() << m_home;
        }
        if (m_home == home){
            //qDebug() << m_home;
            return;
        }
        m_home = home;
        emit homeChanged(m_home);
    }
    void setOver(bool over)
    {
        if (m_over == over)
            return;

        m_over = over;
        emit overChanged(m_over);
    }

    void setBrake(bool brake)
    {
        if (m_brake == brake)
            return;

        m_brake = brake;
        emit brakeChanged(m_brake);
    }

    void setOrigin(int origin)
    {
        if (m_origin == origin)
            return;

        m_origin = origin;
        emit originChanged(m_origin);
    }

    void setOrigin_s(const QString &origin_s)
    {
        if (m_origin_s == origin_s)
            return;

        m_origin_s = origin_s;
        emit origin_sChanged(m_origin_s);
    }

    void setPosition(const QString &position)
    {
        if (m_position == position)
            return;

        m_position = position;
        emit positionChanged(m_position);
    }

    void setEncoder(const QString &encoder)
    {
        if (m_encoder == encoder)
            return;

        m_encoder = encoder;
        emit encoderChanged(m_encoder);
    }

    void setTorque(const QString &torque)
    {
        if (m_torque == torque)
            return;

        m_torque = torque;
        emit torqueChanged(m_torque);
    }

    void setServo(bool servo)
    {
        if (m_servo != servo) {
            m_servo = servo;
            emit servoChanged(m_servo);
        }
    }

    void setSpeed(const QString &speed)
    {
        if (m_speed == speed)
            return;

        m_speed = speed;
        emit speedChanged(m_speed);
    }

    void setRow(int row)
    {
        if (m_row == row)
            return;

        m_row = row;
        emit rowChanged(m_row);
    }

    void setArrival(bool arrival)
    {
        if (m_arrival == arrival)
            return;

        m_arrival = arrival;
        emit arrivalChanged(m_arrival);
    }

    void setMaxPosition(const QString &max)
    {
        if (m_maxPosition != max) {
            m_maxPosition = max;
            emit maxPositionChanged(m_maxPosition);
        }
    }

    void setOriginPercent(const QString &originPercent)
    {
        if (m_originPercent == originPercent)
            return;

        m_originPercent = originPercent;
        emit originPercentChanged(m_originPercent);
    }

    void setStartPos(QString startPos)
    {
        if (m_startPos == startPos)
            return;

        m_startPos = startPos;
        emit startPosChanged(m_startPos);
    }

    void setStartSpeed(QString startSpeed)
    {
        if (m_startSpeed == startSpeed)
            return;

        m_startSpeed = startSpeed;
        emit startSpeedChanged(m_startSpeed);
    }

    void setPointCount(QString pointCount)
    {
        if (m_pointCount == pointCount)
            return;

        m_pointCount = pointCount;
        emit pointCountChanged(m_pointCount);
    }

    void setPointValue(QString pointValue)
    {
        if (m_pointValue == pointValue)
            return;

        m_pointValue = pointValue;
        emit pointValueChanged(m_pointValue);
    }

    void setPitch(QString pitch)
    {
        if (m_pitch == pitch)
            return;

        m_pitch = pitch;
        emit pitchChanged(m_pitch);
    }

    void setDirection(bool direction)
    {
        if (m_direction == direction)
            return;

        m_direction = direction;
        emit directionChanged(m_direction);
    }
    void setCorrectValue(const QString &correctValue)
    {
        if (m_correctValue == correctValue)
            return;

        m_correctValue = correctValue;
        emit correctValueChanged(m_correctValue);
    }
    void setDeviation(const QString &deviation)
    {
        if (m_deviation == deviation)
            return;

        m_deviation = deviation;
        emit deviationChanged(m_deviation);
    }
    void setLoadRate(const QString &loadRate)
    {
        if (m_loadRate == loadRate)
            return;

        m_loadRate = loadRate;
        emit loadRateChanged(m_loadRate);
    }
    void setResurrectionResistorRate(const QString &resurrectionResistorRate)
    {
        if (m_resurrectionResistorRate == resurrectionResistorRate)
            return;

        m_resurrectionResistorRate = resurrectionResistorRate;
        emit resurrectionResistorRateChanged(m_resurrectionResistorRate);
    }
    void setSpeedPeak(const QString &speedPeak)
    {
        if (m_speedPeak == speedPeak)
            return;

        m_speedPeak = speedPeak;
        emit speedPeakChanged(m_speedPeak);
    }
    void setTorquePeak(const QString &torquePeak)
    {
        if (m_torquePeak == torquePeak)
            return;

        m_torquePeak = torquePeak;
        emit torquePeakChanged(m_torquePeak);
    }
    void setDeviationPeak(const QString &deviationPeak)
    {
        if (m_deviationPeak == deviationPeak)
            return;

        m_deviationPeak = deviationPeak;
        emit deviationPeakChanged(m_deviationPeak);
    }
    void setSpeedValue(int speedValue)
    {
        m_speedValue = speedValue;
        emit speedValueChanged(m_speedValue);
    }
    void setTorqueValue(int torqueValue)
    {
        m_torqueValue = torqueValue;
        emit torqueValueChanged(m_torqueValue);
    }
    void setSpeedMin(const QString &speedMin)
    {
        if (m_speedMin == speedMin)
            return;

        m_speedMin = speedMin;
        emit speedMinChanged(m_speedMin);
    }
    void setDeviationMin(const QString &deviationMin)
    {
        if (m_deviationMin == deviationMin)
            return;

        m_deviationMin = deviationMin;
        emit deviationMinChanged(m_deviationMin);
    }
    void setTorqueMin(const QString &torqueMin)
    {
        if (m_torqueMin == torqueMin)
            return;

        m_torqueMin = torqueMin;
        emit torqueMinChanged(m_torqueMin);
    }

signals:

    void homeChanged(bool home);

    void overChanged(bool over);

    void brakeChanged(bool brake);

    void originChanged(int origin);

    void origin_sChanged(QString origin_s);

    void positionChanged(QString position);

    void encoderChanged(QString encoder);

    void torqueChanged(QString torque);

    void servoChanged(bool servo);

    void rowChanged(int row);

    void arrivalChanged(bool arrival);

    void speedChanged(QString speed);

    void maxPositionChanged(QString max);

    void originPercentChanged(QString originPercent);

    void startPosChanged(QString startPos);

    void startSpeedChanged(QString startSpeed);

    void pointCountChanged(QString pointCount);

    void pointValueChanged(QString pointValue);

    void pitchChanged(QString pitch);

    void directionChanged(bool direction);

    void correctValueChanged(QString correctValue);

    void deviationChanged(QString deviation);

    void loadRateChanged(QString loadRate);

    void resurrectionResistorRateChanged(QString resurrectionResistorRate);

    void speedPeakChanged(QString speedPeak);

    void torquePeakChanged(QString torquePeak);

    void deviationPeakChanged(QString deviationPeak);

    void speedValueChanged(int speedValue);

    void torqueValueChanged(int torqueValue);

    void speedMinChanged(QString speedMin);

    void deviationMinChanged(QString deviationMin);

    void torqueMinChanged(QString torqueMin);

private:
    bool m_home = false;
    bool m_over = false;
    bool m_brake = false;
    int m_origin = 0;
    QString m_origin_s = "";
    QString m_position = "";
    QString m_encoder = "";
    QString m_torque = "";
    QString m_maxPosition = "";
    int m_row = 0;
    bool m_servo;
    bool m_arrival = false;
    QString m_speed = "";
    QString m_originPercent = "";
    QString m_startPos;
    QString m_startSpeed;
    QString m_pointCount;
    QString m_pointValue;
    QString m_pitch;
    bool m_direction;
    QString m_correctValue = "";
    QString m_deviation = "";
    QString m_loadRate = "";
    QString m_resurrectionResistorRate = "";
    QString m_speedPeak = "";
    QString m_torquePeak = "";
    QString m_deviationPeak = "";
    int m_speedValue = 0;
    int m_torqueValue = 0;
    QString m_speedMin = "";
    QString m_deviationMin = "";
    QString m_torqueMin = "";
};

#endif // AXISDATA_H
