#ifndef MODBUSMANAGER_H
#define MODBUSMANAGER_H

#include <QObject>
#include "communicationthread.h"
#include "modbuscommunication.h"

#define COM_THREAD_CYCLE_MONITOR_TIME   25
#define SUMMARY_BULK_MAX_SIZE           125
#define SUMMARY_BULK_IO_ADDR_TOP_SR     2729
#define SUMMARY_BULK_IO_SIZE            32
#define SUMMARY_BULK_READY_ADDR_TOP_HR  2525
#define SUMMARY_BULK_READY_ADDR_SIZE    3
#define MAIN_SCREEN_ID_TOP_DO       1168
#define MAIN_SCREEN_ID_BIT_NUM      8
#define SUB_SCREEN_ID_TOP_DO        1176
#define SUB_SCREEN_ID_BIT_NUM       8
#define AXIS_NUM_READY_ADDR_HR          2534
#define COMMON_MODBUS_ADDR_TOP_HR       2700
#define SUMMARY_BULK_DI_PATTERN_TO_WORD_SIZE(ptn)   \
    (ptn == 0 ? 22 : \
     ptn == 1 ? 16 : \
     ptn == 2 ? 16 : 16)

#define SUMMARY_BULK_DI_BIT_UNIT(ptn)   \
    (ptn == 0 ? 8 : \
     ptn == 1 ? 16 : \
     ptn == 2 ? 16 : 16)

#define SUMMARY_BULK_DO_PATTERN_TO_WORD_SIZE(ptn)   \
    (ptn == 0 ? 16 : \
     ptn == 1 ? 16 : 16)

#define SUMMARY_BULK_DO_BIT_UNIT(ptn)   \
    (ptn == 0 ? 16 : \
     ptn == 1 ? 16 : 16)

//Bulk address translation map
const QList<QList<quint16>> BULK_ADDR_COVERSION_MAP_DI = {
  //DI ADDR,REAL-ADDRESS
    { 2560, 1672 },{ 2561, 1673 },{ 2562, 1674 },{ 2563, 1675 },{ 2564, 1676 },{ 2565, 1677 },{ 2566, 1678 },{ 2567, 1679 },
    { 2568,   24 },{ 2569,   25 },{ 2570,   26 },{ 2571,   27 },{ 2572,   28 },{ 2573,   29 },{ 2574,   30 },{ 2575,   31 },
    { 2576,  552 },{ 2577,  553 },{ 2578,  554 },{ 2579,  555 },{ 2580,  556 },{ 2581,  557 },{ 2582,  558 },{ 2583,  559 },
    { 2584,  560 },{ 2585,  561 },{ 2586,  562 },{ 2587,  563 },{ 2588,  564 },{ 2589,  565 },{ 2590,  566 },{ 2591,  567 },
    { 2592,  576 },{ 2593,  577 },{ 2594,  578 },{ 2595,  579 },{ 2596,  580 },{ 2597,  581 },{ 2598,  582 },{ 2599,  583 },
    { 2600,  664 },{ 2601,  665 },{ 2602,  666 },{ 2603,  667 },{ 2604,  668 },{ 2605,  669 },{ 2606,  670 },{ 2607,  671 },
    { 2608,  672 },{ 2609,  673 },{ 2610,  674 },{ 2611,  675 },{ 2612,  676 },{ 2613,  677 },{ 2614,  678 },{ 2615,  679 },
    { 2616,  736 },{ 2617,  737 },{ 2618,  738 },{ 2619,  739 },{ 2620,  740 },{ 2621,  741 },{ 2622,  742 },{ 2623,  743 },
    { 2624, 1216 },{ 2625, 1217 },{ 2626, 1218 },{ 2627, 1219 },{ 2628, 1220 },{ 2629, 1221 },{ 2630, 1222 },{ 2631, 1223 },
    { 2632, 1224 },{ 2633, 1225 },{ 2634, 1226 },{ 2635, 1227 },{ 2636, 1228 },{ 2637, 1229 },{ 2638, 1230 },{ 2639, 1231 },
    { 2640, 1232 },{ 2641, 1233 },{ 2642, 1234 },{ 2643, 1235 },{ 2644, 1236 },{ 2645, 1237 },{ 2646, 1238 },{ 2647, 1239 },
    { 2648, 1648 },{ 2649, 1649 },{ 2650, 1650 },{ 2651, 1651 },{ 2652, 1652 },{ 2653, 1653 },{ 2654, 1654 },{ 2655, 1655 },
    { 2656, 2296 },{ 2657, 2297 },{ 2658, 2298 },{ 2659, 2299 },{ 2660, 2300 },{ 2661, 2301 },{ 2662, 2302 },{ 2663, 2303 },
    { 2664, 2360 },{ 2665, 2361 },{ 2666, 2362 },{ 2667, 2363 },{ 2668, 2364 },{ 2669, 2365 },{ 2670, 2366 },{ 2671, 2367 },
    { 2672, 1208 },{ 2673, 1209 },{ 2674, 1210 },{ 2675, 1211 },{ 2676, 1212 },{ 2677, 1213 },{ 2678, 1214 },{ 2679, 1215 },
    { 2680,  584 },{ 2681,  585 },{ 2682,  586 },{ 2683,  587 },{ 2684,  588 },{ 2685,  589 },{ 2686,  590 },{ 2687,  591 },
    { 2688,  730 },{ 2689, 1676 },{ 2690, 1784 },{ 2691, 1785 },{ 2692, 1786 },{ 2693, 1787 },{ 2694, 1788 },{ 2695, 1789 },
    { 2696, 1790 },{ 2697, 1791 }
};

const QList<QList<quint16>> BULK_ADDR_COVERSION_MAP_SR = {
  //SR ADDR,REAL-ADDRESS
    {  927, 1717 },{  928, 1937 },{  929, 2075 },{  930, 2076 },{  931, 2077 },{  932, 2078 },{  933, 2079 },{  934, 2080 },
    {  935, 2081 },{  936, 2082 },{  937, 2083 },{  938, 2084 },{  939, 2085 },{  940,  496 },{  941,  497 },{  942,  498 },
    {  943,  499 },{  944,  500 },{  945,  501 },{  946,  502 },{  947,  503 },{  948,  504 },{  949,  505 },{  950,  506 },
    {  951,  507 },{  952,  508 },{  953,  509 },{  954,  510 },{  955,  511 },{  956,  528 },{  957,  529 },{  958,  530 },
    {  959,  531 },{  960,  532 },{  961,  533 },{  962,  534 },{  963,  535 },{  964,  704 },{  965, 1695 },{  966, 1696 },
    {  967, 1697 },{  968, 1698 },{  969, 1699 },{  970, 1700 },{  971, 1701 },{  972, 1702 },{  973, 1703 },{  974, 1704 },
    {  975, 1705 },{  976, 1706 },{  977, 1707 },{  978, 1708 },{  979, 1709 },{  980,  707 },{  981,  708 },{  982,  709 },
    {  983,  710 },{  984,  711 },{  985,  712 },{  986,  713 },{  987,  714 },{  988,  715 },{  989,  725 },{  990,  726 },
    {  991,  727 },{  992,  728 },{  993,  729 },{  994,  730 },{  995,  731 },{  996,  732 },{  997,  733 },{  998,  734 },
    {  999,  735 },{ 1000,  520 },{ 1001,  521 },{ 1002,  522 },{ 1003,  523 },{ 1004,  524 },{ 1005,  525 },{ 1006,  526 },
    { 1007,  527 },{ 1008,  536 },{ 1009,  537 },{ 1010,  538 },{ 1011,  539 },{ 1012,  540 },{ 1013,  541 },{ 1014,  542 },
    { 1015,  543 },{ 1016,  576 },{ 1017,  577 },{ 1018,  578 },{ 1019,  579 },{ 1020,  580 },{ 1021,  581 },{ 1022,  582 },
    { 1023,  583 },{ 1024,  160 },{ 1025,  161 },{ 1026,  162 },{ 1027,  163 },{ 1028,  164 },{ 1029,  165 },{ 1030,  166 },
    { 1031,  167 },{ 1032, 1791 },{ 1033, 1792 },{ 1034, 1951 },{ 1035, 1953 },{ 1036, 1954 },{ 1037, 1955 },{ 1038, 1956 },
};

const QList<QList<short>> SUMMARY_BULK_ADDR_DI_COVERSION_SR_LIST = {
    // DI先頭アドレス(-1:終端の意味)
    { 2729,     -1          },                                              // Common
    { 2729,   2809,     -1  },                                              // Origin
    { 2729,   2809,     -1  },                                              // ManualIO
    { 2729,   2809,     -1  },                                              // ManualAxisOpe
    { 2729,   2809,     -1  },                                              // ManualAxisExpress
    { 2729,   2809,     -1  },                                              // AutoAxisTimer
    { 2729,   2809,     -1  },                                              // AutoAxisCounter
    { 2729,   2797,     -1  },                                              // AutoProductMngTimer
    { 2729,   2797,     -1  },                                              // AutoProductMngCounter
    { 2729,   2761,   2793,     -1  },                                      // AutoIOTimer
    { 2729,   2761,   2793,     -1  },                                      // AutoIOCounter
    { 2729,     -1          },                                              // Monitor
    { 2729,   2761,     -1  },                                              // MonitorIOExpress
    { 2729,   2761,     -1  },                                              // MonitorIOForced
    { 2729,     -1          },                                              // MonitorDriver
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    { 2729,     -1          },                                              // MonitorDriverWaveform
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    { 2729,   2761,     -1  },                                              // MonitorInternal
    { 2729,   2761,     -1  },                                              // TeachingMode
    { 2729,     -1          },                                              // TeachingPosSet
    { 2729,     -1          },                                              // TeachingPosFix
    { 2729,     -1          },                                              // TeachingStandardPalletize
    { 2729,     -1          },                                              // TeachingFreePalletize
    { 2729,   2761,   2793,     -1  },                                      // TeachingTimer
    { 2729,   2761,   2793,     -1  },                                      // TeachingCounter
    { 2729,     -1          },                                              // TeachingPass
    { 2729,     -1          },                                              // TeachingProgram
    { 2729,     -1          },                                              // Function
    { 2729,     -1          },                                              // FunctionControllerSetSystem
    { 2729,     -1          },                                              // FunctionControllerSetUser
    { 2729,     -1          },                                              // FunctionControllerSetNetwork
    { 2729,     -1          },                                              // FunctionMachineSelect
    { 2729,   2761,   2793,     -1  },                                      // FunctionMechanicalParam
    { 2729,   2761,   2793,     -1  },                                      // FunctionMechanicalParamDrvParam
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 2729,     -1          },                                              // FunctionMPDrvParamSimpleSet
#else // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 2729,   2761,   2793,     -1  },                                      // FunctionMPDrvParamSimpleSet
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 2729,     -1          },                                              // FunctionDriverMonitor
    { 2729,   2809,     -1  },                                              // FunctionAreaSet
    { 2729,   2809,     -1  },                                              // FunctionAreaFix
    { 2729,   2809,     -1  },                                              // FunctionMaintainCheck
    { 2729,   2809,     -1  },                                              // FunctionMaintain
    { 2729,   2797,     -1  },                                              // FunctionProductionMng
    { 2729,   2809,     -1  },                                              // FunctionAbsSet
    { 2729,   2809,     -1  },                                              // FunctionDeceleration
    { 2729,   2809,     -1  },                                              // FunctionVersionMng
    { 2729,     -1          },                                              // History
    { 2729,   2809,     -1  },                                              // HistoryAlarm
    { 2729,   2809,     -1  },                                              // HistoryOpe
    { 2729,   2809,     -1  },                                              // HistoryIO
    { 2729,   2809,     -1  },                                              // SetupChgFileMng
    { 2729,   2809,     -1  },                                              // SetupChgExportUsb
    { 2729,   2809,     -1  },                                              // SetupChgImportUsb
    { 2729,   2809,     -1  },                                              // PopupAlarm
    { 2729,   2809,     -1  },                                              // PopupError
    { 2729,   2809,     -1  },                                              // PopupUserLevel
    { 2729,   2809,     -1  },                                              // PopupTimerSet
    { 2729,   2809,     -1  },                                              // PopupSpeedSet
    { 2729,   2809,     -1  },                                              // PopupAxisSelect
    { 2729,   2809,     -1  },                                              // PopupLanguageSelect
    { 2729,   2809,     -1  },                                              // PopupOpeCommand
};

const QList<QList<short>> SUMMARY_BULK_ADDR_DO_COVERSION_SR_LIST = {
    // DI先頭アドレス(-1:終端の意味)
    { -1                    },                                              // Common
    { -1                    },                                              // Origin
    { -1                    },                                              // ManualIO
    { -1                    },                                              // ManualAxisOpe
    { -1                    },                                              // ManualAxisExpress
    { -1                    },                                              // AutoAxisTimer
    { -1                    },                                              // AutoAxisCounter
    { -1                    },                                              // AutoProductMngTimer
    { -1                    },                                              // AutoProductMngCounter
    { -1                    },                                              // AutoIOTimer
    { -1                    },                                              // AutoIOCounter
    { 2777,             -1  },                                              // Monitor
    { 2777,             -1  },                                              // MonitorIOExpress
    { 2777,             -1  },                                              // MonitorIOForced
    { -1                    },                                              // MonitorDriver
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    { -1                    },                                              // MonitorDriverWaveform
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    { 2777,             -1  },                                              // MonitorInternal
    { -1                    },                                              // TeachingMode
    { -1                    },                                              // TeachingPosSet
    { -1                    },                                              // TeachingPosFix
    { -1                    },                                              // TeachingStandardPalletize
    { -1                    },                                              // TeachingFreePalletize
    { -1                    },                                              // TeachingTimer
    { -1                    },                                              // TeachingCounter
    { -1                    },                                              // TeachingPass
    { -1                    },                                              // TeachingProgram
    { -1                    },                                              // Function
    { -1                    },                                              // FunctionControllerSetSystem
    { -1                    },                                              // FunctionControllerSetUser
    { -1                    },                                              // FunctionControllerSetNetwork
    { -1                    },                                              // FunctionMachineSelect
    { -1                    },                                              // FunctionMechanicalParam
    { -1                    },                                              // FunctionMechanicalParamDrvParam
    { -1                    },                                              // FunctionMPDrvParamSimpleSet
    { -1                    },                                              // FunctionDriverMonitor
    { -1                    },                                              // FunctionAreaSet
    { -1                    },                                              // FunctionAreaFix
    { -1                    },                                              // FunctionMaintainCheck
    { -1                    },                                              // FunctionMaintain
    { -1                    },                                              // FunctionProductionMng
    { -1                    },                                              // FunctionAbsSet
    { -1                    },                                              // FunctionDeceleration
    { -1                    },                                              // FunctionVersionMng
    { -1                    },                                              // History
    { -1                    },                                              // HistoryAlarm
    { -1                    },                                              // HistoryOpe
    { -1                    },                                              // HistoryIO
    { -1                    },                                              // SetupChgFileMng
    { -1                    },                                              // SetupChgExportUsb
    { -1                    },                                              // SetupChgImportUsb
    { -1                    },                                              // PopupAlarm
    { -1                    },                                              // PopupError
    { -1                    },                                              // PopupUserLevel
    { -1                    },                                              // PopupTimerSet
    { -1                    },                                              // PopupSpeedSet
    { -1                    },                                              // PopupAxisSelect
    { -1                    },                                              // PopupLanguageSelect
    { -1                    },                                              // PopupOpeCommand
};

const QList<QList<short>> SUMMARY_BULK_ADDR_COMMAND_INFO = {
    // HR2525,HR2526,HR2527,SRxxx,Size
    { 0, 0, 0, 0x0A8C, 0x003D},                                             // Common
    { 1, 1, 0, 0x0A8C, 0x007D},                                             // Origin
    { 1, 0, 0, 0x0A8C, 0x007D},                                             // ManualIO
    { 1, 0, 0, 0x0A8C, 0x007D},                                             // ManualAxisOpe
    { 1, 0, 0, 0x0A8C, 0x007D},                                             // ManualAxisExpress
    { 2, 0, 0, 0x0A8C, 0x007D},                                             // AutoAxisTimer
    { 2, 0, 0, 0x0A8C, 0x007D},                                             // AutoAxisCounter
    { 3, 0, 0, 0x0A8C, 0x0071},                                             // AutoProductMngTimer
    { 3, 0, 0, 0x0A8C, 0x0071},                                             // AutoProductMngCounter
    { 4, 0, 0, 0x0A8C, 0x006D},                                             // AutoIOTimer
    { 4, 0, 0, 0x0A8C, 0x006D},                                             // AutoIOCounter
    { 0, 0, 0, 0x0A8C, 0x0071},                                             // Monitor
    { 5, 0, 0, 0x0A8C, 0x0071},                                             // MonitorIOExpress
    { 5, 0, 0, 0x0A8C, 0x0071},                                             // MonitorIOForced
    { 6, 0, 0, 0x0A8C, 0x006D},                                             // MonitorDriver
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    {12, 0, 0, 0x0A8C, 0x0077},                                             // MonitorDriverWaveform
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    { 5, 0, 0, 0x0A8C, 0x0071},                                             // MonitorInternal
    { 5, 0, 0, 0x0A8C, 0x0071},                                             // TeachingMode
    { 8, 0, 0, 0x0A8C, 0x006D},                                             // TeachingPosSet
    { 8, 0, 0, 0x0A8C, 0x006D},                                             // TeachingPosFix
    { 9, 0, 200, 0x0A8C, 0x007D},                                           // TeachingStandardPalletize
    {10, 0, 210, 0x0A8C, 0x006D},                                           // TeachingFreePalletize
    { 4, 0, 0, 0x0A8C, 0x006D},                                             // TeachingTimer
    { 4, 0, 0, 0x0A8C, 0x006D},                                             // TeachingCounter
    { 7, 0, 0, 0x0A8C, 0x004D},                                             // TeachingPass
    { 8, 0, 0, 0x0A8C, 0x0075},                                             // TeachingProgram
    { 0, 0, 0, 0x0A8C, 0x003D},                                             // Function
    { 0, 0, 0, 0x0A8C, 0x003D},                                             // FunctionControllerSetSystem
    { 0, 0, 0, 0x0A8C, 0x003D},                                             // FunctionControllerSetUser
    { 0, 0, 0, 0x0A8C, 0x003D},                                             // FunctionControllerSetNetwork
    {11, 0, 0, 0x0A8C, 0x0065},                                             // FunctionMachineSelect
    {11, 0, 0, 0x0A8C, 0x0065},                                             // FunctionMechanicalParam
    {11, 0, 0, 0x0A8C, 0x0065},                                             // FunctionMechanicalParamDrvParam
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    {13, 0, 0, 0x0A8C, 0x005F},                                             // FunctionMPDrvParamSimpleSet
#else // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 4, 0, 0, 0x0A8C, 0x003D},                                             // FunctionMPDrvParamSimpleSet
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 6, 0, 0, 0x0A8C, 0x006D},                                             // FunctionDriverMonitor
    { 1, 0, 0, 0x0A8C, 0x007D},                                             // FunctionAreaSet
    { 1, 0, 0, 0x0A8C, 0x007D},                                             // FunctionAreaFix
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // FunctionMaintainCheck
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // FunctionMaintain
    { 3, 0, 0, 0x0A8C, 0x0071},                                             // FunctionProductionMng
    { 1, 0, 0, 0x0A8C, 0x007D},                                             // FunctionAbsSet
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // FunctionDeceleration
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // FunctionVersionMng
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // History
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // HistoryAlarm
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // HistoryOpe
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // HistoryIO
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // SetupChgFileMng
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // SetupChgExportUsb
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // SetupChgImportUsb
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupAlarm
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupError
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupUserLevel
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupTimerSet
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupSpeedSet
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupAxisSelect
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupLanguageSelect
    { 0, 0, 0, 0x0A8C, 0x007D},                                             // PopupOpeCommand
};

const QList<QList<int>> SUMMARY_BULK_DI_PATTERN_INFO = {
    // DI模式(0:一括IO,1:入出力IO 2:计时状态 -1:終端)
    { 0,    -1          },                                                  // Common
    { 0,     1,     -1  },                                                  // Origin
    { 0,     1,     -1  },                                                  // ManualIO
    { 0,     1,     -1  },                                                  // ManualAxisOpe
    { 0,     1,     -1  },                                                  // ManualAxisExpress
    { 0,     2,     -1  },                                                  // AutoAxisTimer
    { 0,     2,     -1  },                                                  // AutoAxisCounter
    { 0,     2,     -1  },                                                  // AutoProductMngTimer
    { 0,     2,     -1  },                                                  // AutoProductMngCounter
    { 0,     1,     2,     -1  },                                           // AutoIOTimer
    { 0,     1,     2,     -1  },                                           // AutoIOCounter
    { 0,    -1          },                                                  // Monitor
    { 0,     1,     -1  },                                                  // MonitorIOExpress
    { 0,     1,     -1  },                                                  // MonitorIOForced
    { 0,    -1          },                                                  // MonitorDriver
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    { 0,    -1          },                                                  // MonitorDriverWaveform
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    { 0,     1,     -1  },                                                  // MonitorInternal
    { 0,     1,     -1  },                                                  // TeachingMode
    { 0,    -1          },                                                  // TeachingPosSet
    { 0,    -1          },                                                  // TeachingPosFix
    { 0,    -1          },                                                  // TeachingStandardPalletize
    { 0,    -1          },                                                  // TeachingFreePalletize
    { 0,     1,      2,     -1  },                                          // TeachingTimer
    { 0,     1,      2,     -1  },                                          // TeachingCounter
    { 0,    -1          },                                                  // TeachingPass
    { 0,    -1          },                                                  // TeachingProgram
    { 0,    -1          },                                                  // Function
    { 0,    -1          },                                                  // FunctionControllerSetSystem
    { 0,    -1          },                                                  // FunctionControllerSetUser
    { 0,    -1          },                                                  // FunctionControllerSetNetwork
    { 0,    -1          },                                                  // FunctionMachineSelect
    { 0,     1,      2,     -1  },                                          // FunctionMechanicalParam
    { 0,     1,      2,     -1  },                                          // FunctionMechanicalParamDrvParam
#ifdef ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 0,    -1          },                                                  // FunctionMPDrvParamSimpleSet
#else // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 0,     1,      2,     -1  },                                          // FunctionMPDrvParamSimpleSet
#endif // ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC
    { 0,    -1          },                                                  // FunctionDriverMonitor
    { 0,     1,     -1  },                                                  // FunctionAreaSet
    { 0,     1,     -1  },                                                  // FunctionAreaFix
    { 0,     1,     -1  },                                                  // FunctionMaintainCheck
    { 0,     1,     -1  },                                                  // FunctionMaintain
    { 0,     2,     -1  },                                                  // FunctionProductionMng
    { 0,     1,     -1  },                                                  // FunctionAbsSet
    { 0,     1,     -1  },                                                  // FunctionDeceleration
    { 0,     1,     -1  },                                                  // FunctionVersionMng
    { 0,    -1          },                                                  // History
    { 0,     1,     -1  },                                                  // HistoryAlarm
    { 0,     1,     -1  },                                                  // HistoryOpe
    { 0,     1,     -1  },                                                  // HistoryIO
    { 0,     1,     -1  },                                                  // SetupChgFileMng
    { 0,     1,     -1  },                                                  // SetupChgExportUsb
    { 0,     1,     -1  },                                                  // SetupChgImportUsb
    { 0,     1,     -1  },                                                  // PopupAlarm
    { 0,     1,     -1  },                                                  // PopupError
    { 0,     1,     -1  },                                                  // PopupUserLevel
    { 0,     1,     -1  },                                                  // PopupTimerSet
    { 0,     1,     -1  },                                                  // PopupSpeedSet
    { 0,     1,     -1  },                                                  // PopupAxisSelect
    { 0,     1,     -1  },                                                  // PopupLanguageSelect
    { 0,     1,     -1  },                                                  // PopupOpeCommand
};

const QList<QList<int>> SUMMARY_BULK_DO_PATTERN_INFO = {
    // DOパターン(0:実入出力反転指令,1:実入出力入替指令 -1:終端)
    { -1                },                                                  // Common
    { -1                },                                                  // Origin
    { -1                },                                                  // ManualIO
    { -1                },                                                  // ManualAxisOpe
    { -1                },                                                  // ManualAxisExpress
    { -1                },                                                  // AutoAxisTimer
    { -1                },                                                  // AutoAxisCounter
    { -1                },                                                  // AutoProductMngTimer
    { -1                },                                                  // AutoProductMngCounter
    { -1                },                                                  // AutoIOTimer
    { -1                },                                                  // AutoIOCounter
    {  0,           -1  },                                                  // Monitor
    {  0,           -1  },                                                  // MonitorIOExpress
    {  0,           -1  },                                                  // MonitorIOForced
    { -1                },                                                  // MonitorDriver
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
    { -1                },                                                  // MonitorDriverWaveform
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
    {  0,           -1  },                                                  // MonitorInternal
    {  0,           -1  },                                                  // TeachingMode
    { -1                },                                                  // TeachingPosSet
    { -1                },                                                  // TeachingPosFix
    { -1                },                                                  // TeachingStandardPalletize
    { -1                },                                                  // TeachingFreePalletize
    { -1                },                                                  // TeachingTimer
    { -1                },                                                  // TeachingCounter
    { -1                },                                                  // TeachingPass
    { -1                },                                                  // TeachingProgram
    { -1                },                                                  // Function
    { -1                },                                                  // FunctionControllerSetSystem
    { -1                },                                                  // FunctionControllerSetUser
    { -1                },                                                  // FunctionControllerSetNetwork
    { -1                },                                                  // FunctionMachineSelect
    { -1                },                                                  // FunctionMechanicalParam
    { -1                },                                                  // FunctionMechanicalParamDrvParam
    { -1                },                                                  // FunctionMPDrvParamSimpleSet
    { -1                },                                                  // FunctionDriverMonitor
    { -1                },                                                  // FunctionAreaSet
    { -1                },                                                  // FunctionAreaFix
    { -1                },                                                  // FunctionMaintainCheck
    { -1                },                                                  // FunctionMaintain
    { -1                },                                                  // FunctionProductionMng
    { -1                },                                                  // FunctionAbsSet
    { -1                },                                                  // FunctionDeceleration
    { -1                },                                                  // FunctionVersionMng
    { -1                },                                                  // History
    { -1                },                                                  // HistoryAlarm
    { -1                },                                                  // HistoryOpe
    { -1                },                                                  // HistoryIO
    { -1                },                                                  // SetupChgFileMng
    { -1                },                                                  // SetupChgExportUsb
    { -1                },                                                  // SetupChgImportUsb
    { -1                },                                                  // PopupAlarm
    { -1                },                                                  // PopupError
    { -1                },                                                  // PopupUserLevel
    { -1                },                                                  // PopupTimerSet
    { -1                },                                                  // PopupSpeedSet
    { -1                },                                                  // PopupAxisSelect
    { -1                },                                                  // PopupLanguageSelect
    { -1                },                                                  // PopupOpeCommand
};

const QList<QList<QList<short>>> SUMMARY_BULK_ADDR_DI_LIST = {
    //DI2560-DI2816
    {
        { 2560, 2561, 2562, 2563, 2564, 2565, 2566, 2567 },
        { 2568, 2569, 2570, 2571, 2572, 2573, 2574, 2575 },
        { 2576, 2577, 2578, 2579, 2580, 2581, 2582, 2583 },
        { 2584, 2585, 2586, 2587, 2588, 2589, 2590, 2591 },
        { 2592, 2593, 2594, 2595, 2596, 2597, 2598, 2599 },
        { 2600, 2601, 2602, 2603, 2604, 2605, 2606, 2607 },
        { 2608, 2609, 2610, 2611, 2612, 2613, 2614, 2615 },
        { 2616, 2617, 2618, 2619, 2620, 2621, 2622, 2623 },
        { 2624, 2625, 2626, 2627, 2628, 2629, 2630, 2631 },
        { 2632, 2633, 2634, 2635, 2636, 2637, 2638, 2639 },
        { 2640, 2641, 2642, 2643, 2644, 2645, 2646, 2647 },
        { 2648, 2649, 2650, 2651, 2652, 2653, 2654, 2655 },
        { 2656, 2657, 2658, 2659, 2660, 2661, 2662, 2663 },
        { 2664, 2665, 2666, 2667, 2668, 2669, 2670, 2671 },
        { 2672, 2673, 2674, 2675, 2676, 2677, 2678, 2679 },
        { 2680, 2681, 2682, 2683, 2684, 2685, 2686, 2687 },
        { 2688, 2689, 2690, 2691, 2692, 2693, 2694, 2695 },
        { 2696, 2697, 2698, 2699, 2700, 2701, 2702, 2703 },
        { 2704, 2705, 2706, 2707, 2708, 2709, 2710, 2711 },
        { 2712, 2713, 2714, 2715, 2716, 2717, 2718, 2719 },
        { 2720, 2721, 2722, 2723, 2724, 2725, 2726, 2727 },
        { 2728, 2729, 2730, 2731, 2732, 2733, 2734, 2735 },
        { 2736, 2737, 2738, 2739, 2740, 2741, 2742, 2743 },
        { 2744, 2745, 2746, 2747, 2748, 2749, 2750, 2751 },
        { 2752, 2753, 2754, 2755, 2756, 2757, 2758, 2759 },
        { 2760, 2761, 2762, 2763, 2764, 2765, 2766, 2767 },
        { 2768, 2769, 2770, 2771, 2772, 2773, 2774, 2775 },
        { 2776, 2777, 2778, 2779, 2780, 2781, 2782, 2783 },
        { 2784, 2785, 2786, 2787, 2788, 2789, 2790, 2791 },
        { 2792, 2793, 2794, 2795, 2796, 2797, 2798, 2799 },
        { 2800, 2801, 2802, 2803, 2804, 2805, 2806, 2807 },
        { 2808, 2809, 2810, 2811, 2812, 2813, 2814, 2815 },
    },
    //DI0-DI255
    {
        {   8,    9,   10,   11,   12,   13,   14,   15,   0,    1,    2,    3,    4,    5,    6,    7     },
        {  24,   25,   26,   27,   28,   29,   30,   31,  16,   17,   18,   19,   20,   21,   22,   23     },
        {  40,   41,   42,   43,   44,   45,   46,   47,  32,   33,   34,   35,   36,   37,   38,   39     },
        {  56,   57,   58,   59,   60,   61,   62,   63,  48,   49,   50,   51,   52,   53,   54,   55     },
        {  72,   73,   74,   75,   76,   77,   78,   79,  64,   65,   66,   67,   68,   69,   70,   71     },
        {  88,   89,   90,   91,   92,   93,   94,   95,  80,   81,   82,   83,   84,   85,   86,   87     },
        { 104,  105,  106,  107,  108,  109,  110,  111,  96,   97,   98,   99,  100,  101,  102,  103     },
        { 120,  121,  122,  123,  124,  125,  126,  127, 112,  113,  114,  115,  116,  117,  118,  119     },
        { 136,  137,  138,  139,  140,  141,  142,  143, 128,  129,  130,  131,  132,  133,  134,  135     },
        { 152,  153,  154,  155,  156,  157,  158,  159, 144,  145,  146,  147,  148,  149,  150,  151     },
        { 168,  169,  170,  171,  172,  173,  174,  175, 160,  161,  162,  163,  164,  165,  166,  167     },
        { 184,  185,  186,  187,  188,  189,  190,  191, 176,  177,  178,  179,  180,  181,  182,  183     },
        { 200,  201,  202,  203,  204,  205,  206,  207, 192,  193,  194,  195,  196,  197,  198,  199     },
        { 216,  217,  218,  219,  220,  221,  222,  223, 208,  209,  210,  211,  212,  213,  214,  215     },
        { 232,  233,  234,  235,  236,  237,  238,  239, 224,  225,  226,  227,  228,  229,  230,  231     },
        { 248,  249,  250,  251,  252,  253,  254,  255, 240,  241,  242,  243,  244,  245,  246,  247     },
    },
    //DI768-DI1016
    {
        { 776,  777,  778,  779,  780,  781,  782,  783, 768,  769,  770,  771,  772,  773,  774,  775     },
        { 792,  793,  794,  795,  796,  797,  798,  799, 784,  785,  786,  787,  788,  789,  790,  791     },
        { 808,  809,  810,  811,  812,  813,  814,  815, 800,  801,  802,  803,  804,  805,  806,  807     },
        { 824,  825,  826,  827,  828,  829,  830,  831, 816,  817,  818,  819,  820,  821,  822,  823     },
        { 840,  841,  842,  843,  844,  845,  846,  847, 832,  833,  834,  835,  836,  837,  838,  839     },
        { 856,  857,  858,  859,  860,  861,  862,  863, 848,  849,  850,  851,  852,  853,  854,  855     },
        { 872,  873,  874,  875,  876,  877,  878,  879, 864,  865,  866,  867,  868,  869,  870,  871     },
        { 888,  889,  890,  891,  892,  893,  894,  895, 880,  881,  882,  883,  884,  885,  886,  887     },
        { 904,  905,  906,  907,  908,  909,  910,  911, 896,  897,  898,  899,  900,  901,  902,  903     },
        { 920,  921,  922,  923,  924,  925,  926,  927, 912,  913,  914,  915,  916,  917,  918,  919     },
        { 936,  937,  938,  939,  940,  941,  942,  943, 928,  929,  930,  931,  932,  933,  934,  935     },
        { 952,  953,  954,  955,  956,  957,  958,  959, 944,  945,  946,  947,  948,  949,  950,  951     },
        { 968,  969,  970,  971,  972,  973,  974,  975, 960,  961,  962,  963,  964,  965,  966,  967     },
        { 984,  985,  986,  987,  988,  989,  990,  991, 976,  977,  978,  979,  980,  981,  982,  983     },
        {1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 992,  993,  994,  995,  996,  997,  998,  999     },
        {1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015     },
    },
};

const QList<QList<QList<short>>> SUMMARY_BULK_ADDR_DO_LIST = {
#if 0   // comment
    //DO2560-DO2815
    {
        {2567,2566,2565,2564,2563,2562,2561,2560,2575,2574,2573,2572,2571,2570,2569,2568},
        {2583,2582,2581,2580,2579,2578,2577,2576,2591,2590,2589,2588,2587,2586,2585,2584},
        {2599,2598,2597,2596,2595,2594,2593,2592,2607,2606,2605,2604,2603,2602,2601,2600},
        {2615,2614,2613,2612,2611,2610,2609,2608,2623,2622,2621,2620,2619,2618,2617,2616},
        {2631,2630,2629,2628,2627,2626,2625,2624,2639,2638,2637,2636,2635,2634,2633,2632},
        {2647,2646,2645,2644,2643,2642,2641,2640,2655,2654,2653,2652,2651,2650,2649,2648},
        {2663,2662,2661,2660,2659,2658,2657,2656,2671,2670,2669,2668,2667,2666,2665,2664},
        {2679,2678,2677,2676,2675,2674,2673,2672,2687,2686,2685,2684,2683,2682,2681,2680},
        {2695,2694,2693,2692,2691,2690,2689,2688,2703,2702,2701,2700,2699,2698,2697,2696},
        {2711,2710,2709,2708,2707,2706,2705,2704,2719,2718,2717,2716,2715,2714,2713,2712},
        {2727,2726,2725,2724,2723,2722,2721,2720,2735,2734,2733,2732,2731,2730,2729,2728},
        {2743,2742,2741,2740,2739,2738,2737,2736,2751,2750,2749,2748,2747,2746,2745,2744},
        {2759,2758,2757,2756,2755,2754,2753,2752,2767,2766,2765,2764,2763,2762,2761,2760},
        {2775,2774,2773,2772,2771,2770,2769,2768,2783,2782,2781,2780,2779,2778,2777,2776},
        {2791,2790,2789,2788,2787,2786,2785,2784,2799,2798,2797,2796,2795,2794,2793,2792},
        {2807,2806,2805,2804,2803,2802,2801,2800,2815,2814,2813,2812,2811,2810,2809,2808},
    },
#endif
    //DO5120-DO5247,DO5248-DO5354
    {
        {5128,5129,5130,5131,5132,5133,5134,5135,5120,5121,5122,5123,5124,5125,5126,5127},
        {5144,5145,5146,5147,5148,5149,5150,5151,5136,5137,5138,5139,5140,5141,5142,5143},
        {5160,5161,5162,5163,5164,5165,5166,5167,5152,5153,5154,5155,5156,5157,5158,5159},
        {5176,5177,5178,5179,5180,5181,5182,5183,5168,5169,5170,5171,5172,5173,5174,5175},
        {5192,5193,5194,5195,5196,5197,5198,5199,5184,5185,5186,5187,5188,5189,5190,5191},
        {5208,5209,5210,5211,5212,5213,5214,5215,5200,5201,5202,5203,5204,5205,5206,5207},
        {5224,5225,5226,5227,5228,5229,5230,5231,5216,5217,5218,5219,5220,5221,5222,5223},
        {5240,5241,5242,5243,5244,5245,5246,5247,5232,5233,5234,5235,5236,5237,5238,5239},
        {5256,5257,5258,5259,5260,5261,5262,5263,5248,5249,5250,5251,5252,5253,5254,5255},
        {5272,5273,5274,5275,5276,5277,5278,5279,5264,5265,5266,5267,5268,5269,5270,5271},
        {5288,5289,5290,5291,5292,5293,5294,5295,5280,5281,5282,5283,5284,5285,5286,5287},
        {5304,5305,5306,5307,5308,5309,5310,5311,5296,5297,5298,5299,5300,5301,5302,5303},
        {5320,5321,5322,5323,5324,5325,5326,5327,5312,5313,5314,5315,5316,5317,5318,5319},
        {5336,5337,5338,5339,5340,5341,5342,5343,5328,5329,5330,5331,5332,5333,5334,5335},
        {5352,5353,5354,5355,5356,5357,5358,5359,5344,5345,5346,5347,5348,5349,5350,5351},
        {5368,5369,5370,5371,5372,5373,5374,5375,5360,5361,5362,5363,5364,5365,5366,5367},
    },
};

class ModbusManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int communicationAbnormalStatus MEMBER m_communicationAbnormalStatus NOTIFY communicationAbnormalStatusChanged)
    Q_PROPERTY(bool communicationTimeoutError MEMBER m_communicationTimeoutError NOTIFY communicationTimeoutErrorChanged)
    Q_PROPERTY(bool isFirstReadCommonModbus MEMBER m_isFirstReadCommonModbus NOTIFY isFirstReadCommonModbusChanged)
    Q_PROPERTY(bool communicationReplyAbortedError READ communicationReplyAbortedError WRITE setCommunicationReplyAbortedError NOTIFY communicationReplyAbortedErrorChanged)
private:
    explicit ModbusManager(QObject *parent = 0);
    ~ModbusManager() = default;

    static ModbusManager* instance;
public:
    static ModbusManager* create_instance(QObject *parent = 0);
    static ModbusManager* get_instance();
    static void destroy_instance();
    void startCommunication(QString applicationName);
    void stopCommunication();
    void startModbusCommunication(QString applicationName);
    void stopModbusCommunication();
    void setTimeoutAndRetry(ResourceManager* resourceManager);
    CommunicationThread* getAsyncModbusCommunicationInstance() { return m_communication; }
    ModbusCommunication* getSyncModbusCommunicationInstance() { return m_modbusCommunication; }

    Q_INVOKABLE bool getReceiveDataDI(quint16 key);
    Q_INVOKABLE bool getReceiveDataDO(quint16 key);
    Q_INVOKABLE int getReceiveDataHR(quint16 key);
    Q_INVOKABLE int getLongReceiveDataHR(quint16 key);
    Q_INVOKABLE int getReceiveDataRG(quint16 key);
    Q_INVOKABLE short getWordReceiveDataRG(quint16 key);
    Q_INVOKABLE int getLongReceiveDataRG(quint16 key);
    int getUserLevel();
    int getLanguage();
    Q_INVOKABLE int getModbusConnectStatus();
    Q_INVOKABLE int getModbusComConnectStatus();
    Q_INVOKABLE void reConnect();
    void setRequestPage(Enums::Pages page);
    void updatePageCommand(Enums::Pages page, int subInfo = 0);
    void startIsPageChanging();
    void startIsAxisChanging();
    bool getIsPageChanging() { return m_isPageChanging; }
    bool getIsPageWriteReady() { return m_isPageWriteReady; }
    bool getIsAxisWriteReady() { return m_isAxisWriteReady; }
    void setCommunicationTimeoutError(bool communicationTimeoutError);
    bool getcommunicationTimeoutError(){return m_communicationTimeoutError; }
    void setCommunicationReplyAbortedError(bool communicationReplyAbortedError);
    bool communicationReplyAbortedError() const { return m_communicationReplyAbortedError; }

private:
    // Instance for data handling worker running on the other thread.
    CommunicationThread *m_communication;
    // Instance for sync modbus communication
    ModbusCommunication *m_modbusCommunication;
    bool m_isInitial = true;
    QMap<quint16, QVariant> m_receiveDataMapDI;
    QMap<quint16, QVariant> m_receiveDataMapDO;
    QMap<quint16, QVariant> m_receiveDataMapRG;
    QMap<quint16, QVariant> m_receiveDataMapHR;
    int m_communicationAbnormalStatus = 0;
    bool m_communicationTimeoutError = false;
    Enums::Pages m_requestPage = Enums::Pages::InvalidPage;

    int m_mainScreenId = 0;
    int m_subScreenId = 0;
    bool m_isMainScreenWrote = false;
    bool m_isSubScreenWrote = false;
    bool m_isPageChanging = false;
    bool m_isPageWriteReady = false;
    int m_timeout = 450;
    int m_retry = 1;
    bool m_isAxisWriteReady = false;
    bool m_isFirstReadCommonModbus = false;
    bool m_communicationReplyAbortedError = false;
    
    void parseModbusCommandHex01(stModBusCommunicationData &command);
    void parseModbusCommandHex02(stModBusCommunicationData &command);
    void parseModbusCommandHex03(stModBusCommunicationData &command);
    void parseModbusCommandHex04(stModBusCommunicationData &command);
    int getBulkModbusMapIndex(stModBusCommunicationData &command);
    void parseSummaryModbusCommandHex04(stModBusCommunicationData &command);
    void parseSummaryModbusCommandHex02(stModBusCommunicationData &command, int DIPattern, int DIListIndex);
    void parseSummaryModbusCommandHex01(stModBusCommunicationData &command, int DOPattern, int DOListIndex);

private slots:
    // Handle trigger that was issued at communication thread when the read dat is available
    void update(stModBusCommunicationData command);
    void onTimeout(int communicationAbnormalStatus);
    void onTimeoutError();
    void onReplyAbortedError();
    void onlinkStateChange(bool isonoff);

signals:
    void updateModbusManualPressed();
    void communicationAbnormalStatusChanged(int communicationAbnormalStatus);
    void communicationTimeoutErrorChanged(bool communicationTimeoutError);

    void endModbusPageChange(int mainScreenId, int subScreenId);
    void endModbusPageWriteReady(QList<int> writeData);
    void endModbusAxisWriteReady(QList<int> writeData);
    void isFirstReadCommonModbusChanged(bool isFirstReadCommonModbus);
    void communicationReplyAbortedErrorChanged(bool communicationReplyAbortedError);
    void linkchange(bool isonoff);
};

#endif // MODBUSMANAGER_H
