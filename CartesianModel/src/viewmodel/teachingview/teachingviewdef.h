#ifndef TEACHINGVIEWDEF_H
#define TEACHINGVIEWDEF_H
#include <QString>


// Icon file name
const QString TEACHING_VIEW_ICON_DIR = "/16_program/";
const QString TEACHING_MODE_PAGE_ACTIVATE_ICON_NAME = "d160_15.png";
const QString TEACHING_MODE_PAGE_DEACTIVATE_ICON_NAME = "d160_08.png";
const QString TEACHING_POSSET_PAGE_ACTIVATE_ICON_NAME = "d160_16.png";
const QString TEACHING_POSSET_PAGE_DEACTIVATE_ICON_NAME = "d160_09.png";
const QString TEACHING_STACK_PAGE_ACTIVATE_ICON_NAME = "d160_17.png";
const QString TEACHING_STACK_PAGE_DEACTIVATE_ICON_NAME = "d160_10.png";
const QString TEACHING_TIMER_PAGE_ACTIVATE_ICON_NAME = "d160_18.png";
const QString TEACHING_TIMER_PAGE_DEACTIVATE_ICON_NAME = "d160_11.png";
const QString TEACHING_COUNTER_PAGE_ACTIVATE_ICON_NAME = "d160_19.png";
const QString TEACHING_COUNTER_PAGE_DEACTIVATE_ICON_NAME = "d160_12.png";
const QString TEACHING_PASS_PAGE_ACTIVATE_ICON_NAME = "d160_20.png";
const QString TEACHING_PASS_PAGE_DEACTIVATE_ICON_NAME = "d160_13.png";
const QString TEACHING_PROGRAM_PAGE_ACTIVATE_ICON_NAME = "d160_21.png";
const QString TEACHING_PROGRAM_PAGE_DEACTIVATE_ICON_NAME = "d160_14.png";

// MODBUS communication
const ushort MODBUS_ADDRESS_MODE_STATUS    = 0x0AF9;        // SR2809
const ushort MODBUS_ADDRESS_KEY_NO         = 0x04A0;
const ushort MODBUS_NOD_MODE_STATUS        = 0x0004;
const ushort MODBUS_NOD_KEY_NO             = 0x0001;

/// pen_msg
// Group
const int PEN_MSG_GROUP_TEACHING_VIEW = 34;
const int PEN_MSG_GROUP_MODE_PAGE = 35;
const int PEN_MSG_GROUP_POS_SET_PAGE = 36;
const int PEN_MSG_GROUP_PALLETIZE_PAGE = 37;

// Teaching view group
typedef enum {
    ModePage = 1,
    PosSetPage,
    StackPage,
    TimerPage,
    CounterPage,
    PassPage,
    ProgramPage
} PenMsgTeachingGroupRow;

// Mode page group
const int PEN_MSG_MODE_START_ROW = 1;
const int PEN_MSG_DESCTIPTION_START_ROW = 3;

// Pos set page group
const int PEN_MSG_POINT_NAME_START_ROW = 30;

/// pen_set
// Group
const QString PEN_SET_GROUP_TEACHING_VIEW = "9";
const QString PEN_SET_GROUP_MODE_PAGE = "10";
const QString PEN_SET_GROUP_POS_SET_PAGE = "11";
const QString PEN_SET_GROUP_TIMER_PAGE = "13";
const QString PEN_SET_GROUP_PASS_PAGE = "15";

// Teaching view group
typedef enum {
    SelectedTabTextColor = 1,
} PenSetTeachingGroupRow;

typedef enum {
    Red = 0,
    Green,
    Blue
} SelectedTabTextColorColumn;

// Mode page group
const int PEN_SET_KEY_DI_START_ROW = 7;
const int PEN_SET_COLUMN_KEY_NO = 0;
const int PEN_SET_COLUMN_DI = 1;

// Pos set page group
const int PEN_SET_POS_SET_CURRENT_POSITION_COLOR_ROW = 1;
const int PEN_SET_POS_SET_ENCODER_COLOR_ROW = 2;
const int PEN_SET_POS_SET_POSITION_COLOR_ROW = 3;
const int PEN_SET_POS_SET_NUM_POINT_ROW = 6;
const int PEN_SET_POS_SET_AXIS_STYLE_START_ROW = 8;
const int PEN_SET_POS_ADJUST_CURRENT_POSITION_COLOR_ROW = 17;
const int PEN_SET_POS_ADJUST_DATA_COLOR_ROW = 18;
const int PEN_SET_POS_ADJUST_PAGE_COLOR_ROW = 19;
const int PEN_SET_POS_ADJUST_POINT_DISP_ORDER_ROW = 26;
const int PEN_SET_POS_ADJUST_CURRENT_POSITION_STYLE_ROW = 38;
const int PEN_SET_POS_SET_ADDRESS_START_ROW = 39;
const int PEN_SET_POS_SET_STYLE_START_ROW = 127;
const int PEN_SET_POS_ADJUST_POINT_SETTABLE_AXIS_START_ROW = 216;

// Timer page group
const int PEN_SET_TIMER_FORMAT_START_ROW = 11;
const int PEN_SET_TIMER_ADDRESS_START_ROW = 63;

// Pass page group
const int PEN_SET_PASS_FORMAT_ROW = 7;
const int PEN_SET_PASS_MAX_MIN_ROW = 16;

#endif // TEACHINGVIEWDEF_H
