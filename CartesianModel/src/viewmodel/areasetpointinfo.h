#ifndef AREASETPOINTINFO_H
#define AREASETPOINTINFO_H

#include <QObject>

class AreaSetPointInfo
{
public:
    explicit AreaSetPointInfo();
    void setPointSetting(quint8 value);
    quint8 pointSetting() const { return m_pointSetting; }
    void setAxis(quint8 axis);
    quint8 axis() const { return m_axis; }
    void setPointNoMax(quint8 no);
    quint8 pointNoMax() { return m_pointNoMax; }
    void setPointNoMin(quint8 no);
    quint8 pointNoMin() { return m_pointNoMin; }
    void setMaxHrAddr(quint16 addr);
    quint16 maxHrAddr() { return m_maxHrAddr; }
    void setMinHrAddr(quint16 addr);
    quint16 minHrAddr() { return m_minHrAddr; }

private:
    quint16     m_maxHrAddr = 0;
    quint16     m_minHrAddr = 0;
    quint8      m_pointSetting = 0;     // Point設定.
    quint8      m_axis = 0;             // 軸
    quint8      m_pointNoMax = 0;       // POINT MAX設定.
    quint8      m_pointNoMin = 0;       // POINT MIN設定.
};

#endif // AREASETPOINTINFO_H
