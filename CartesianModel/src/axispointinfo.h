#ifndef AXISPOINTINFO_H
#define AXISPOINTINFO_H

#include <QObject>

class ResourceManager;

class AxisPointInfo : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        Fix,                // Point001～080
        Arbitrary,          // Point101～200, 256～999
        StandardPalletize,  // Point201～208
        FreePalletize,      // Point211～218
        UnKnown,
    } PointType;

    explicit AxisPointInfo(ResourceManager *resource, QObject *parent = nullptr);
    static AxisPointInfo* create_instance(ResourceManager *resource, QObject *parent = nullptr);
    static AxisPointInfo* get_instance();
    void init();

    static bool isValidPoint(int pointNo);
    static int getStartFixPointNo();
    static int getEndFixPointNo();
    static int getStartStandardPalletizePointNo();
    static int getEndStandardPalletizePointNo();
    static int getStartFreePalletizePointNo();
    static int getEndFreePalletizePointNo();
    static PointType getPointType(int pointNo);
    quint16 getPointPositionAddress(int pointNo, int axis);
    quint16 getPointSpeedAddress(int pointNo, int axis);
    quint16 positionTopHrAddr() const;
    quint16 pointDataSize() const;
    QString getPointName(int pointNo);

    // Point101～200, 256～999
    int formatArbitraryPointPosition();
    int formatArbitraryPointSpeed();
    // for palletize
    static int getStandardPalletizePointNo(int palletizeNo);
    static int getFreePalletizePointNo(int palletizeNo);
    static int getPalletizeNo(int pointNo);

private:
    static AxisPointInfo    *m_instance;
    ResourceManager         *m_resource = nullptr;
};

#endif // AXISPOINTINFO_H
