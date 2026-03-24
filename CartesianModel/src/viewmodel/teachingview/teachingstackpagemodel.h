#ifndef TEACHINGSTACKPAGEMODEL_H
#define TEACHINGSTACKPAGEMODEL_H

#include <QObject>
#include <math.h>
#include "pagemodelbase.h"
#include "axisdata.h"
#include "axisinfo.h"
#include "pointinfo.h"
#include "commonviewmodel.h"

#define PALLETIZE_ORDER_NUM_MAX                             8
#define STANDARD_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM    6
#define FREE_PALLETIZE_NUM_MAX                              8
#define FREE_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM        6
#define FREE_PALLETIZE_TOP_POINT_CMD_LIST_INDEX             4
#define FREE_PALLETIZE_LOW_POINT_CMD_LIST_INDEX             5
#define PALLETIZE_STORAGE_METHOD_DI                         2689
#define PALLETIZE_DOWN_DECELERATE_POS_HR_AXIS_M             7609
#define PALLETIZE_DOWN_DECELERATE_SPEED_HR_AXIS_M           7611
#define PALLETIZE_UP_DECELERATE_POS_HR_AXIS_M               7633
#define PALLETIZE_UP_DECELERATE_SPEED_HR_AXIS_M             7635
#define PALLETIZE_DOWN_DECELERATE_POS_HR_AXIS_S             7612
#define PALLETIZE_DOWN_DECELERATE_SPEED_HR_AXIS_S           7614
#define PALLETIZE_UP_DECELERATE_POS_HR_AXIS_S               7636
#define PALLETIZE_UP_DECELERATE_SPEED_HR_AXIS_S             7638
#define PALLETIZE_PITCH_PEN_MSG_INDEX                       43
#define PALLETIZE_SPEED_PEN_MSG_INDEX                       44
#define PALLETIZE_POINT_NUMBER_HR                           2527
#define FREE_PALLETIZE_PAGE1_READ_SIZE_MAX                  142
#define FREE_PALLETIZE_ONE_PAGE_READ_SIZE_MAX               94
#define STANDARD_PALLETIZE_MONITOR_STORAGE_METHOD_INDEX     1
#define PALLETIZE_AXIS_ORDER_VALUE_TOP_HR                   2485
#define PALLETIZE_AXIS_ORDER_VALUE_BOTTOM_HR                2492
#define PALLETIZE_TENKEY_DEFAULT_VALUE                      0x81000000

typedef struct {
    quint16 hrNumber[PALLETIZE_ORDER_NUM_MAX];
    int     orderValue[PALLETIZE_ORDER_NUM_MAX];
}stPalletizeTable;

const QList<QList<quint16>> AXIS_CURPOS_ADDR_LIST = {
    // pos-upper(SR) pos-lower(SR)
    {          2761,         2762   },      // Y-Axis
    {          2763,         2764   },      // MX-Axis
    {          2765,         2766   },      // MZ-Axis
    {          2767,         2768   },      // SX-Axis
    {          2769,         2770   },      // SZ-Axis
    {          2771,         2772   },      // 6-Axis
    {          2773,         2774   },      // 7-Axis
    {          2775,         2776   },      // 8-Axis
};

const QList<QList<quint16>> PALLETIZE_AXIS_STARTPOS_SPEED_HR_ADDR_LIST = {
    // Y-sPos(HR)   Y-speed(HR)  MX-sPos(HR) MX-speed(HR) MZ-sPos(HR) MZ-speed(HR) SX-sPos(HR) SX-speed(HR) SZ-sPos(HR) SZ-speed(HR)
    {       7408,         7410,        7411,        7413,       7414,        7416,       7417,        7419,       7420,        7422 },     // 標準パレタイズ1
    {       7432,         7434,        7435,        7437,       7438,        7440,       7441,        7443,       7444,        7446 },     // 標準パレタイズ2
    {       7456,         7458,        7459,        7461,       7462,        7464,       7465,        7467,       7468,        7470 },     // 標準パレタイズ3
    {       7480,         7482,        7483,        7485,       7486,        7488,       7489,        7491,       7492,        7494 },     // 標準パレタイズ4
    {       7504,         7506,        7507,        7509,       7510,        7512,       7513,        7515,       7516,        7518 },     // 標準パレタイズ5
    {       7528,         7530,        7531,        7533,       7534,        7536,       7537,        7539,       7540,        7542 },     // 標準パレタイズ6
    {       7552,         7554,        7555,        7557,       7558,        7560,       7561,        7563,       7564,        7566 },     // 標準パレタイズ7
    {       7576,         7578,        7579,        7581,       7582,        7584,       7585,        7587,       7588,        7590 },     // 標準パレタイズ8
};

const QList<QList<quint16>> PALLETIZE_AXIS_STARTPOS_SPEED_SR_ADDR_LIST = {
    // Y-sPos(SR)   Y-speed(SR)  MX-sPos(SR) MX-speed(SR) MZ-sPos(SR) MZ-speed(SR) SX-sPos(SR) SX-speed(SR) SZ-sPos(SR) SZ-speed(SR)
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ1
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ2
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ3
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ4
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ5
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ6
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ7
    {       2777,         2779,        2780,        2782,       2783,        2785,       2786,        2788,       2789,        2791 },     // 標準パレタイズ8
};

const QList<QList<quint16>> PALLETIZE_COUNT_POINT_ADDR_LIST = {
    // Y-count(SR)   Y-point(HR)  MX-count(SR) MX-point(HR) MZ-count(SR) MZ-point(HR)
    {        2801,         2493,         2802,        2494,        2803,        2495 },     // 標準パレタイズ1
    {        2804,         2496,         2805,        2497,        2806,        2498 },     // 標準パレタイズ2
    {        2807,         2499,         2808,        2500,        2809,        2501 },     // 標準パレタイズ3
    {        2810,         2502,         2811,        2503,        2812,        2504 },     // 標準パレタイズ4
    {        2813,         2505,         2814,        2506,        2815,        2507 },     // 標準パレタイズ5
    {        2816,         2508,         2817,        2509,        2818,        2510 },     // 標準パレタイズ6
    {        2819,         2511,         2820,        2512,        2821,        2513 },     // 標準パレタイズ7
    {        2822,         2514,         2823,        2515,        2824,        2516 },     // 標準パレタイズ8
};

const QList<QList<quint16>> PALLETIZE_COUNT_HR_ADDR_LIST = {
    // Y-count(HR)   MX-count(HR) MZ-count(SR)
    {        1855,          1856,        1857, },     // 標準パレタイズ1
    {        1858,          1859,        1860, },     // 標準パレタイズ2
    {        1861,          1862,        1863, },     // 標準パレタイズ3
    {        1864,          1865,        1866, },     // 標準パレタイズ4
    {        1867,          1868,        1869, },     // 標準パレタイズ5
    {        1870,          1871,        1872, },     // 標準パレタイズ6
    {        1873,          1874,        1875, },     // 標準パレタイズ7
    {        1876,          1877,        1878, },     // 標準パレタイズ8
};

const QList<QList<quint16>> PALLETIZE_PITCH_ADDR_LIST = {
    //  Y(HR)   MX(HR)  MZ(HR)  SX(HR)  SZ(HR)   6(HR)   7(HR)   8(HR)
    {  26944,   26947,  26950,  26953,  26956,  26959,  26962,   26965 },     // 标准堆叠1
    {  26968,   26971,  26974,  26977,  26980,  26983,  26986,   26989 },     // 标准堆叠2
    {  26992,   26995,  26998,  27001,  27004,  27007,  27010,   27013 },     // 标准堆叠3
    {  27016,   27019,  27022,  27025,  27028,  27031,  27034,   27037 },     // 标准堆叠4
    {  27040,   27043,  27046,  27049,  27052,  27055,  27058,   27061 },     // 标准堆叠5
    {  27064,   27067,  27070,  27073,  27076,  27079,  27082,   27085 },     // 标准堆叠6
    {  27088,   27091,  27094,  27097,  27100,  27103,  27106,   27109 },     // 标准堆叠7
    {  27112,   27115,  27118,  27121,  27124,  27127,  27130,   27133 },     // 标准堆叠8
};

const QList<QList<int>> PALLETIZE_POINT_NUMBER = {
    { 201,        202,        203,        204,        205,        206,        207,        208},  // 標準パレタイズ
    { 211,        212,        213,        214,        215,        216,        217,        218},  // 自由パレタイズ
};

const QList<QList<quint16>> PALLETIZE_POINT_NUM_ADDR_LIST = {
    // Pos_M(HR)   Speed_M(HR)    Pos_S(HR)   Speed_S(HR)
    {    7609,        7611,        7612,         7614},     // 下降減速(P209)
    {    7633,        7635,        7636,         7638},     // 上昇減速(P210)
};

const QList<QList<quint16>> FREE_PALLETIZE_POINT_COUNT_VALUE_ADDR_LIST = {
    // Point
    // Count(SR)  Value(HR)
    {      2801,       1879     },     // 自由パレタイズ1
    {      2802,       1880     },     // 自由パレタイズ2
    {      2803,       1881     },     // 自由パレタイズ3
    {      2804,       1882     },     // 自由パレタイズ4
    {      2805,       1883     },     // 自由パレタイズ5
    {      2806,       1884     },     // 自由パレタイズ6
    {      2807,       1885     },     // 自由パレタイズ7
    {      2808,       1886     },     // 自由パレタイズ8
};

const QList<QList<quint16>> FREE_PALLETIZE_POINT_VALUE_ADDR_LIST = {
    // Point-Value(HR)
    {      2517     },     // 自由パレタイズ1
    {      2518     },     // 自由パレタイズ2
    {      2519     },     // 自由パレタイズ3
    {      2520     },     // 自由パレタイズ4
    {      2521     },     // 自由パレタイズ5
    {      2522     },     // 自由パレタイズ6
    {      2523     },     // 自由パレタイズ7
    {      2524     },     // 自由パレタイズ8
};

const QList<QList<quint16>> FREE_PALLETIZE_AXIS_CORRECT_VALUE_HR_ADDR_LIST = {
    // 軸1(HR)   軸2(HR)  軸3(HR)  軸4(HR)  軸5(HR)  軸6(HR)  軸7(HR)  軸8(HR)
    {    7648,     7651,    7654,    7657,    7660,   7663,    7666,    7669    },     // 自由パレタイズ1
    {    7672,     7675,    7678,    7681,    7684,   7687,    7690,    7693    },     // 自由パレタイズ2
    {    7696,     7699,    7702,    7705,    7708,   7711,    7714,    7717    },     // 自由パレタイズ3
    {    7720,     7723,    7726,    7729,    7732,   7735,    7738,    7741    },     // 自由パレタイズ4
    {    7744,     7747,    7750,    7753,    7756,   7759,    7762,    7765    },     // 自由パレタイズ5
    {    7768,     7771,    7774,    7777,    7780,   7783,    7786,    7789    },     // 自由パレタイズ6
    {    7792,     7795,    7798,    7801,    7804,   7807,    7810,    7813    },     // 自由パレタイズ7
    {    7816,     7819,    7822,    7825,    7828,   7831,    7834,    7837    },     // 自由パレタイズ8
};

const QList<QList<quint16>> FREE_PALLETIZE_AXIS_CORRECT_VALUE_SR_ADDR_LIST = {
    // 軸1(SR)   軸2(SR)  軸3(SR)  軸4(SR)  軸5(SR)  軸6(SR)  軸7(SR)  軸8(SR)
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ1
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ2
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ3
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ4
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ5
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ6
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ7
    {    2777,     2780,    2783,    2786,    2789,   2792,    2795,    2798    },     // 自由パレタイズ8
};

const QList<QList<quint16>> FREE_PALLETIZE_AXIS_SPEED_HR_ADDR_LIST = {
    // 軸1(HR)   軸2(HR)  軸3(HR)  軸4(HR)  軸5(HR)  軸6(HR)  軸7(HR)  軸8(HR)
    {    7650,     7653,    7656,    7659,    7662,   7665,    7668,    7671    },     // 自由パレタイズ1
    {    7674,     7677,    7680,    7683,    7686,   7689,    7692,    7695    },     // 自由パレタイズ2
    {    7698,     7701,    7704,    7707,    7710,   7713,    7716,    7719    },     // 自由パレタイズ3
    {    7722,     7725,    7728,    7731,    7734,   7737,    7740,    7743    },     // 自由パレタイズ4
    {    7746,     7749,    7752,    7755,    7758,   7761,    7764,    7767    },     // 自由パレタイズ5
    {    7770,     7773,    7776,    7779,    7782,   7785,    7788,    7791    },     // 自由パレタイズ6
    {    7794,     7797,    7800,    7803,    7806,   7809,    7812,    7815    },     // 自由パレタイズ7
    {    7818,     7821,    7824,    7827,    7830,   7833,    7836,    7839    },     // 自由パレタイズ8
};

const QList<QList<quint16>> FREE_PALLETIZE_AXIS_SPEED_SR_ADDR_LIST = {
    // 軸1(SR)   軸2(SR)  軸3(SR)  軸4(SR)  軸5(SR)  軸6(SR)  軸7(SR)  軸8(SR)
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ1
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ2
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ3
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ4
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ5
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ6
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ7
    {    2779,     2782,    2785,    2788,    2791,   2794,    2797,    2800    },     // 自由パレタイズ8
};

const QList<QList<quint16>> FREE_PALLETIZE_AXIS_POS_ADDR_TOP_LIST = {                   // Point No.1+x～No.4+x
    // 軸1(HR)   軸2(HR)  軸3(HR)  軸4(HR)  軸5(HR)  軸6(HR)  軸7(HR)  軸8(HR)
    {   27328,    27331,   27334,   27337,   27340,  27343,   27346,   27349    },     // Point No.1+x
    {   27352,    27355,   27358,   27361,   27364,  27367,   27370,   27373    },     // Point No.2+x
    {   27376,    27379,   27382,   27385,   27388,  27391,   27394,   27397    },     // Point No.3+x
    {   27400,    27403,   27406,   27409,   27412,  27415,   27418,   27421    },     // Point No.4+x
};

const QList<QList<quint16>> FREE_PALLETIZE_AXIS_POS_ADDR_LOW_LIST = {                   // Point No.5+x～No.8+x
    // 軸1(HR)   軸2(HR)  軸3(HR)  軸4(HR)  軸5(HR)  軸6(HR)  軸7(HR)  軸8(HR)
    {   27424,    27427,   27430,   27433,   27436,  27439,   27442,   27445    },     // Point No.5+x
    {   27448,    27451,   27454,   27457,   27460,  27463,   27466,   27469    },     // Point No.6+x
    {   27472,    27475,   27478,   27481,   27484,  27487,   27490,   27493    },     // Point No.7+x
    {   27496,    27499,   27502,   27505,   27508,  27511,   27514,   27517    },     // Point No.8+x
};

const QList<QList<int>> FREE_PALLETIZE_POINT_INFO_LIST = {
    // 開始ポイント番号  ポイント数
    {     1,            200       },     // 自由パレタイズ1
    {     1,            200       },     // 自由パレタイズ2
    {     1,            200       },     // 自由パレタイズ3
    {     1,            200       },     // 自由パレタイズ4
    {     1,             20       },     // 自由パレタイズ5
    {     1,             20       },     // 自由パレタイズ6
    {     1,             20       },     // 自由パレタイズ7
    {     1,            164       },     // 自由パレタイズ8
};

class TeachingStackPageModel : public PageModelBase
{
    Q_OBJECT
    Q_PROPERTY(QObject *axisInfo READ axisInfo CONSTANT)
    Q_PROPERTY(QList<QObject *> pointList READ pointList CONSTANT)
    Q_PROPERTY(bool storageMethod READ storageMethod WRITE setStorageMethod NOTIFY storageMethodChanged)

    Q_PROPERTY(QString axisMdownDeceleratePos READ axisMdownDeceleratePos WRITE setAxisMdownDeceleratePos NOTIFY axisMdownDeceleratePosChanged)
    Q_PROPERTY(QString axisMdownDecelerateSpeed READ axisMdownDecelerateSpeed WRITE setAxisMdownDecelerateSpeed NOTIFY axisMdownDecelerateSpeedChanged)
    Q_PROPERTY(QString axisMupDeceleratePos READ axisMupDeceleratePos WRITE setAxisMupDeceleratePos NOTIFY axisMupDeceleratePosChanged)
    Q_PROPERTY(QString axisMupDecelerateSpeed READ axisMupDecelerateSpeed WRITE setAxisMupDecelerateSpeed NOTIFY axisMupDecelerateSpeedChanged)
    Q_PROPERTY(QString axisSdownDeceleratePos READ axisSdownDeceleratePos WRITE setAxisSdownDeceleratePos NOTIFY axisSdownDeceleratePosChanged)
    Q_PROPERTY(QString axisSdownDecelerateSpeed READ axisSdownDecelerateSpeed WRITE setAxisSdownDecelerateSpeed NOTIFY axisSdownDecelerateSpeedChanged)
    Q_PROPERTY(QString axisSupDeceleratePos READ axisSupDeceleratePos WRITE setAxisSupDeceleratePos NOTIFY axisSupDeceleratePosChanged)
    Q_PROPERTY(QString axisSupDecelerateSpeed READ axisSupDecelerateSpeed WRITE setAxisSupDecelerateSpeed NOTIFY axisSupDecelerateSpeedChanged)

    Q_PROPERTY(int curStandardPalletizeNum READ curStandardPalletizeNum WRITE setCurStandardPalletizeNum NOTIFY curStandardPalletizeNumChanged)
    Q_PROPERTY(QList<int> axisOrder READ axisOrder WRITE setAxisOrder NOTIFY axisOrderChanged)
    Q_PROPERTY(int curTabIndex READ curTabIndex WRITE setCurTabIndex NOTIFY curTabIndexChanged)
    Q_PROPERTY(int curFreePalletizeNum READ curFreePalletizeNum WRITE setCurFreePalletizeNum NOTIFY curFreePalletizeNumChanged)
    Q_PROPERTY(QList<int> axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)
    Q_PROPERTY(QList<QString> freePalletizeColorList READ freePalletizeColorList WRITE setFreePalletizeColorList NOTIFY freePalletizeColorListChanged)
    Q_PROPERTY(QList<QString> pointNameList READ pointNameList WRITE setPointNameList NOTIFY pointNameListChanged)
    Q_PROPERTY(QString freePointCount READ freePointCount WRITE setFreePointCount NOTIFY freePointCountChanged)
    Q_PROPERTY(QString freePointValue READ freePointValue WRITE setFreePointValue NOTIFY freePointValueChanged)
    Q_PROPERTY(QList<QString> pointNameColorList READ pointNameColorList WRITE setPointNameColorList NOTIFY pointNameColorListChanged)
    Q_PROPERTY(int curAxisOrder READ curAxisOrder WRITE setCurAxisOrder NOTIFY curAxisOrderChanged)
    Q_PROPERTY(bool dataLoaded READ dataLoaded NOTIFY dataLoadedChanged)
    Q_PROPERTY(bool editFromProgEditScreen MEMBER m_editFromProgEditScreen NOTIFY editFromProgEditScreenChanged)
    Q_PROPERTY(QList<int> stackType READ stackType NOTIFY stackTypeChanged)
    Q_PROPERTY(bool editFromProgEditScreenIng MEMBER m_editFromProgEditScreenIng NOTIFY editFromProgEditScreenIngChanged)

public:
    explicit TeachingStackPageModel(ResourceManager *resource, CommunicationThread * comm, CommonViewModel *commonView);
    explicit TeachingStackPageModel(QObject *object = nullptr);
    ~TeachingStackPageModel() override;

    enum PalletizeType {
        PalletizeTypeSandard = 0,
        PalletizeTypeFree,
        PalletizeTypeMax,
    };
    enum PalletizeOrder {
        PalletizeOrderYMxMz = 0,
        PalletizeOrderYMzMx,
        PalletizeOrderMzYMx,
        PalletizeOrderMxYMz,
        PalletizeOrderMxMzY,
        PalletizeOrderMzMxY,
        PalletizeOrderMax,
    };
    enum PalletizePointNum {
        OptionalPointNum = -1,
        DownDecelerationPointNum = 209,
        UpDecelerationPointNum,
    };

    stPalletizeTable orderNumTable[PalletizeOrder::PalletizeOrderMax] = {
        {
            { 2485, 2486, 2487, 2488, 2489, 2490, 2491, 2492},
            {   16,   40,   64,   88,  112,  136,  160,  184},
        },
        {
            { 2485, 2486, 2487, 2488, 2489, 2490, 2491, 2492},
            {    9,   33,   57,   81,  105,  129,  153,  177},
        },
        {
            { 2485, 2486, 2487, 2488, 2489, 2490, 2491, 2492},
            {   11,   35,   59,   83,  107,  131,  155,  179},
        },
        {
            { 2485, 2486, 2487, 2488, 2489, 2490, 2491, 2492},
            {   18,   42,   66,   90,  114,  138,  162,  186},
        },
        {
            { 2485, 2486, 2487, 2488, 2489, 2490, 2491, 2492},
            {    4,   28,   52,   76,  100,  124,  148,  172},
        },
        {
            { 2485, 2486, 2487, 2488, 2489, 2490, 2491, 2492},
            {    5,   29,   53,   77,  101,  125,  149,  173},
        },
    };
    stModbusPeridiocMonitor initStandardModbusPageCmd[STANDARD_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM] = {
#if 1
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1CF0, 0x0057 },      // 開始位置・速度(標準パレタイズ1～4) FuncCode:0x03 addr:HR7408 size:87
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D50, 0x0057 },      // 開始位置・速度(標準パレタイズ5～8) FuncCode:0x03 addr:HR7504 size:87
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x073F, 0x0018 },      // ポイントカウント                  FuncCode:0x03 addr:HR1855 size:24
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x09BD, 0x0018 },      // ポイント設定値                    FuncCode:0x03 addr:HR2493 size:24
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x6940, 0x005F },      // ピッチ(標準パレタイズ1～4)         FuncCode:0x03 addr:HR26944 size:95
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x69A0, 0x005F },      // ピッチ(標準パレタイズ5～8)         FuncCode:0x03 addr:HR27040 size:95
#else
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1CF0, 0x000F },      // 開始位置・速度(標準パレタイズ1)    FuncCode:0x03 addr:HR7408 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D08, 0x000F },      // 開始位置・速度(標準パレタイズ2)    FuncCode:0x03 addr:HR7432 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D20, 0x000F },      // 開始位置・速度(標準パレタイズ3)    FuncCode:0x10 addr:HR7456 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D38, 0x000F },      // 開始位置・速度(標準パレタイズ4)    FuncCode:0x03 addr:HR7480 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D50, 0x000F },      // 開始位置・速度(標準パレタイズ5)    FuncCode:0x03 addr:HR7504 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D68, 0x000F },      // 開始位置・速度(標準パレタイズ6)    FuncCode:0x03 addr:HR7528 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D80, 0x000F },      // 開始位置・速度(標準パレタイズ7)    FuncCode:0x03 addr:HR7552 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1D98, 0x000F },      // 開始位置・速度(標準パレタイズ8)    FuncCode:0x03 addr:HR7576 size:15
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x073F, 0x0018 },      // ポイントカウント                  FuncCode:0x03 addr:HR1855 size:24
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x09BD, 0x0018 },      // ポイント設定値                    FuncCode:0x03 addr:HR2493 size:24
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x6940, 0x0019 },      // ピッチ(標準パレタイズ1)           FuncCode:0x03 addr:HR26944 size:25
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x6958, 0x0019 },      // ピッチ(標準パレタイズ2)           FuncCode:0x03 addr:HR26968 size:25
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x6970, 0x0019 },      // ピッチ(標準パレタイズ3)           FuncCode:0x03 addr:HR26992 size:25
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x6988, 0x0019 },      // ピッチ(標準パレタイズ4)           FuncCode:0x03 addr:HR27016 size:25
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x69A0, 0x0019 },      // ピッチ(標準パレタイズ5)           FuncCode:0x03 addr:HR27040 size:25
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x69B8, 0x0019 },      // ピッチ(標準パレタイズ6)           FuncCode:0x03 addr:HR27064 size:25
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x69D0, 0x0019 },      // ピッチ(標準パレタイズ7)           FuncCode:0x03 addr:HR27088 size:25
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x69E8, 0x0019 },      // ピッチ(標準パレタイズ8)           FuncCode:0x03 addr:HR27112 size:25
#endif
    };
    stModbusPeridiocMonitor initFreeModbusPageCmd[FREE_PALLETIZE_PERIODIC_MONITOR_CMD_LIST_NUM] = {
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x0757, 0x0008 },      // 現在ポイントカウント値(自由パレタイズ1～8)   FuncCode:0x03 addr:HR1879 size:8
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x09D5, 0x0008 },      // 現在ポイント設定値(自由パレタイズ1～8)       FuncCode:0x03 addr:HR2517 size:8
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1DE0, 0x0060 },      // 補正値・速度(自由パレタイズ1～4)            FuncCode:0x03 addr:HR7648 size:96
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x1E40, 0x0060 },      // 補正値・速度(自由パレタイズ5～8)            FuncCode:0x03 addr:HR7744 size:96
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x6AC0, 0x0060 },      // POINT位置No.1+x～No.4+x                   FuncCode:0x03 addr:HR27328+x size:96
        { QModbusPdu::FunctionCode::ReadHoldingRegisters, 0x6B20, 0x0060 },      // POINT位置No.5+x～No.8+x                   FuncCode:0x03 addr:HR27424+x size:96
    };

    Q_INVOKABLE void initPointList();
    Q_INVOKABLE void initPointNameList();
    Q_INVOKABLE void modbusSetPalletizeNumber(int order, int palletizeNum);
    Q_INVOKABLE void modbusSetPointData(int pointNum, int index, int writeValue);
    Q_INVOKABLE void modbusSetStartPos(int palletizeNum, int index, int axisNum, quint32 writeValue, float decimalWriteValue);
    Q_INVOKABLE void modbusSetStartSpeed(int palletizeNum, int axisNum, int writeValue);
    Q_INVOKABLE void modbusSetPointCount(int palletizeNum, int pointAxisNum, int writeValue);
    Q_INVOKABLE void modbusSetPointValue(int palletizeNum, int pointAxisNum, int writeValue);
    Q_INVOKABLE void modbusSetPitch(int palletizeNum, int index, int axisNum, float writeValue);
    Q_INVOKABLE void modbusSetDirection(int palletizeNum, int index, int axisNum, bool direction);
    Q_INVOKABLE int getAxisCount();
    Q_INVOKABLE void startModbusMonitorStorageMethod();
    Q_INVOKABLE void stopModbusMonitorStorageMethod();
    Q_INVOKABLE void modbusGetPointData(int pointNum, int index);
    Q_INVOKABLE void setFreePalletizePageInfo(int curPage, int totalPage);
    Q_INVOKABLE void modbusSetFreePalletizeCorrectValue(int axisIndex, int axisNum, float writeValue);
    Q_INVOKABLE void modbusSetFreePalletizeSpeed(int axisIndex, int axisNum, int writeValue);
    Q_INVOKABLE void modbusSetFreePalletizePointPos(int index, int axisNum, quint32 writeValue);
    Q_INVOKABLE void modbusSetPalletizePointNumber(int palletizeNum);
    Q_INVOKABLE void modbusSetFreePalletizePointNumber(int palletizeNum);
    Q_INVOKABLE int getDecelerateInfoDispFormat(int index);
    Q_INVOKABLE int getAxisInfoDispFormat(int palletizeNum, int axisIndex, int itemIndex);
    Q_INVOKABLE void modbusGetStartPos(int palletizeNum, int index, int axisNum);
    Q_INVOKABLE void modbusGetStartSpeed(int palletizeNum, int index, int axisNum);
    Q_INVOKABLE void modbusGetPointValue(int palletizeNum, int pointAxisNum);
    Q_INVOKABLE void modbusGetPitch(int palletizeNum, int index, int axisNum);
    Q_INVOKABLE int getFreePalletizeFPNumMax(int palletizeNum);
    Q_INVOKABLE void modbusGetCorrectValue(int palletizeNum, int axisNum);
    Q_INVOKABLE void modbusGetFreePalletizePointPos(int index, int axisNum);
    Q_INVOKABLE void modbusGetFreePalletizeUpperPagePointPos(int curPage);
    Q_INVOKABLE void modbusGetFreePalletizeLowerPagePointPos(int curPage);
    Q_INVOKABLE void modbusGetAxisOrderValue(int palletizeNum);
    Q_INVOKABLE int getPalletizePointNumber(int tabIndex, int palletizeNum);
    Q_INVOKABLE void modbusGetDirection(int palletizeNum, int index, int axisNum);
    Q_INVOKABLE void startPageChanging();
    Q_INVOKABLE void recordSetPalletizeNumber(int prevOrder, int currentOrder, int palletizeNum);
    Q_INVOKABLE void modbusSetStackType(int stackno, int type);
    Q_INVOKABLE void modbusGetStackType(int palletizeNum);

    QObject *axisInfo() const { return m_axisInfo; }
    QList<QObject *> pointList() const { return m_pointList; }
    bool storageMethod() const { return m_storageMethod; }
    void setStorageMethod(bool storageMethod);

    void setAxisMdownDeceleratePos(QString downDeceleratePos);
    QString axisMdownDeceleratePos() const { return m_axisMdownDeceleratePos; }
    void setAxisMdownDecelerateSpeed(QString downDecelerateSpeed);
    QString axisMdownDecelerateSpeed() const { return m_axisMdownDecelerateSpeed; }
    void setAxisMupDeceleratePos(QString upDeceleratePos);
    QString axisMupDeceleratePos() const { return m_axisMupDeceleratePos; }
    void setAxisMupDecelerateSpeed(QString upDecelerateSpeed);
    QString axisMupDecelerateSpeed() const { return m_axisMupDecelerateSpeed; }
    void setAxisSdownDeceleratePos(QString downDeceleratePos);
    QString axisSdownDeceleratePos() const { return m_axisSdownDeceleratePos; }
    void setAxisSdownDecelerateSpeed(QString downDecelerateSpeed);
    QString axisSdownDecelerateSpeed() const { return m_axisSdownDecelerateSpeed; }
    void setAxisSupDeceleratePos(QString upDeceleratePos);
    QString axisSupDeceleratePos() const { return m_axisSupDeceleratePos; }
    void setAxisSupDecelerateSpeed(QString upDecelerateSpeed);
    QString axisSupDecelerateSpeed() const { return m_axisSupDecelerateSpeed; }

    void setCurStandardPalletizeNum(int curStandardPalletizeNum);
    int curStandardPalletizeNum() const { return m_curStandardPalletizeNum; }
    void setAxisOrder(QList<int> axisOrder);
    QList<int> axisOrder() const { return m_axisOrder; }
    void setCurTabIndex(int curTabIndex);
    int curTabIndex() const { return m_curTabIndex; }
    void setCurFreePalletizeNum(int curFreePalletizeNum);
    int curFreePalletizeNum() const { return m_curFreePalletizeNum; }
    void setAxisName(QList<int> axisName);
    QList<int> axisName() const { return m_axisName; }
    void setFreePalletizeColorList(QList<QString> freePalletizeColorList);
    QList<QString> freePalletizeColorList() const { return m_freePalletizeColorList; }
    void setPointNameList(QList<QString> pointNameList);
    QList<QString> pointNameList() const { return m_pointNameList; }
    void setFreePointCount(QString freePointCount);
    QString freePointCount() const { return m_freePointCount; }
    void setFreePointValue(QString freePointValue);
    QString freePointValue() const { return m_freePointValue; }
    void setPointNameColorList(QList<QString> pointNameColorList);
    QList<QString> pointNameColorList() const { return m_pointNameColorList; }
    void setCurAxisOrder(int curAxisOrder);
    int curAxisOrder() const { return m_curAxisOrder; }
    void setDataLoaded(bool dataLoaded);
    bool dataLoaded() { return m_dataLoaded; }
    QList<int> stackType(){return m_stackType;}
    bool editFromProgEditScreenIng(){return m_editFromProgEditScreenIng;}
    void updatePointData();//set startpos update posset page data

private:
    CsvReader               *mCsvReader;
    AxisInfo                *m_axisInfo = AxisInfo::get_instance();
    CommonViewModel         *m_commonView;

    int                     mAxisCount = 3;
    QList<QObject *>        m_pointList;
    QList<QList<quint16>>   m_axisInfoDataFormat;
    QList<QList<quint16>>   m_decelerateInfoDataFormat;
    bool                    m_storageMethod = false;
    bool                    mIsStorageMethodMonitor = false;

    QString                 m_axisMdownDeceleratePos;
    QString                 m_axisMdownDecelerateSpeed;
    QString                 m_axisMupDeceleratePos;
    QString                 m_axisMupDecelerateSpeed;
    QString                 m_axisSdownDeceleratePos;
    QString                 m_axisSdownDecelerateSpeed;
    QString                 m_axisSupDeceleratePos;
    QString                 m_axisSupDecelerateSpeed;

    int                     m_curStandardPalletizeNum = 0;
    QList<int>              m_axisOrder;
    int                     m_curTabIndex = 0;
    int                     m_curFreePalletizeNum = 0;
    QList<int>              m_axisName;
    QList<QString>          m_freePalletizeColorList;
    QList<QList<int>>       m_settableFreeFPNumMax;
    QList<QList<quint16>>   m_axisInfoFreeDataFormat;
    int                     mCurPage = 1;
    int                     mTotalPage = 0;
    QList<QString>          m_pointNameList;
    QString                 m_freePointCount;
    QString                 m_freePointValue;
    QList<QString>          m_pointNameColorList;
    int                     mStorageMethodMonitorCnt = 0;
    QList<int>              mCmnAxisOrder;
    int                     m_curAxisOrder = 0;
    bool                    m_pageChanging = false;
    bool                    m_dataLoaded = false;
    QList<QList<int>>       m_standardAxisOrder;
    bool                    m_editFromProgEditScreen = false;
    QList<int>              m_stackType = {0,0,0,0,0,0,0,0};//0：M axis stack; 1: S axis stack; 2:M S axis stack;
    bool                    m_editFromProgEditScreenIng = false;
    const QList<int>        AXIS_3_POS_ADDR_LIST = {2771, 2773, 2775};

    void init();
    void initSettingPenSetData();
    void initPageCommand();
    void deinitPageCommand();
    void reinitPageCommand();
    void startModbusMonitoring();
    void stopModbusMonitoring();
    void updateStorageMethod(stModBusCommunicationData command);
    void releaseModbusOneShotRead();
    void modBusReadStorageMethod();
    void modBusClearStorageMethod();
    void setOneShotModbusCommand(stModBusCommunicationData &data);
    void updateDecelerateInfo(stModBusCommunicationData command);
    void updateStandardPalletizeAxisInfo();
    void updateFreePalletizeAxisInfo();
    void updateFreePalletizePointNameAndColorList();
    void updateFreePalletizePointPos(int axis, int axisIndex);
    void updateStandardPalletizePointData(int pointNum, int index, int writeValue);
    void updateFreePalletizePointData(int index, int writeValue);
    void recieveStandardPalletizePointData(int pointNum, int index);
    void recieveFreePalletizePointData(int index);
    void updatePointDataInfo(stModBusCommunicationData command);
    void recieveStandardPalletizeStartPos(int palletizeNum, int index, int axisNum);
    void recieveStandardPalletizeStartSpeed(int palletizeNum, int index, int axisNum);
    void recieveStandardPalletizePointValue(int palletizeNum, int pointAxisNum);
    void recieveFreePalletizePointValue(int palletizeNum, int pointAxisNum);
    void updateStandardPalletizePointValue(int palletizeNum, int pointAxisNum, int writeValue);
    void updateFreePalletizePointValue(int palletizeNum, int pointAxisNum, int writeValue);
    void recieveFreePalletizeCorrectValue(int palletizeNum, int axisNum);
    void recieveFreePalletizeStartSpeed(int palletizeNum, int index, int axisNum);
    ushort getFreePalletizeFPNumAccumulationValue(int palletizeNum);
    void updateStandardPalletizeAxisOrderInfo(stModBusCommunicationData command);
    void updateStandardPalletizeAxisTypeInfo(stModBusCommunicationData command);
    void updateAxis6_8Position();

protected:
    void onActivate() override;
    void onDeactivate() override;

signals:
    void storageMethodChanged(bool storageMethod);
    void axisMdownDeceleratePosChanged(QString downDeceleratePos);
    void axisMdownDecelerateSpeedChanged(QString downDecelerateSpeed);
    void axisMupDeceleratePosChanged(QString upDeceleratePos);
    void axisMupDecelerateSpeedChanged(QString upDecelerateSpeed);
    void axisSdownDeceleratePosChanged(QString downDeceleratePos);
    void axisSdownDecelerateSpeedChanged(QString downDecelerateSpeed);
    void axisSupDeceleratePosChanged(QString upDeceleratePos);
    void axisSupDecelerateSpeedChanged(QString upDecelerateSpeed);

    void curStandardPalletizeNumChanged(int curStandardPalletizeNum);
    void axisOrderChanged(QList<int> axisOrder);
    void curTabIndexChanged(int curTabIndex);
    void curFreePalletizeNumChanged(int curFreePalletizeNum);
    void axisNameChanged(QList<int> axisName);
    void freePalletizeColorListChanged(QList<QString> freePalletizeColorList);
    void pointNameListChanged(QList<QString> pointNameList);
    void freePointCountChanged(QString freePointCount);
    void freePointValueChanged(QString freePointValue);
    void pointNameColorListChanged(QList<QString> pointNameColorList);
    void curAxisOrderChanged(int curAxisOrder);
    void dataLoadedChanged(bool dataLoaded);
    void editFromProgEditScreenChanged();
    void stackTypeChanged();
    void editFromProgEditScreenIngChanged(bool editFromProgEditScreen);

private slots:
    void update(stModBusCommunicationData command);
    void endModbusPageChange(int mainScreenId, int subScreenId);

public slots:
    void onLanguageChanged(int language);
    void onFinished();
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);

};

#endif // TEACHINGSTACKPAGEMODEL_H
