#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

class Enums : public QObject
{
    Q_OBJECT

public:
    enum Pages {
        InvalidPage = 0,
        Origin,
        ManualIO,
        ManualAxisOpe,
        ManualAxisExpress,
        AutoAxisTimer,
        AutoAxisCounter,
        AutoProductMngTimer,
        AutoProductMngCounter,
        AutoIOTimer,
        AutoIOCounter,
        Monitor,
        MonitorIOExpress,
        MonitorIOForced,
        MonitorDriver,
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
        MonitorDriverWaveform,
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
        MonitorInternal,
        TeachingMode,
        TeachingPosSet,
        TeachingPosFix,
        TeachingStandardPalletize,
        TeachingFreePalletize,
        TeachingTimer,
        TeachingCounter,
        TeachingPass,
        TeachingProgram,
        Function,
        FunctionControllerSetSystem,
        FunctionControllerSetUser,
        FunctionControllerSetNetwork,
        FunctionMechanicalParamModelSelect,
        FunctionMechanicalParam,
        FunctionMechanicalParamDrvParam,
        FunctionMPDrvParamSimpleSet,
        FunctionDriverMonitor,
        FunctionAreaSet,
        FunctionAreaFix,
        FunctionMaintainCheck,
        FunctionMaintain,
        FunctionProductionMng,
        FunctionAbsSet,
        FunctionDeceleration,
        FunctionVersionMng,
        History,
        HistoryAlarm,
        HistoryOpe,
        HistoryIO,
        SetupChgFileMng,
        SetupChgExportUsb,
        SetupChgImportUsb,
        PopupAlarm,
        PopupError,
        PopupUserLevel,
        PopupTimerSet,
        PopupSpeedSet,
        PopupAxisSelect,
        PopupLanguageSelect,
        PopupOpeCommand,
        pageMax
    };
    Q_ENUM(Pages)

    enum MainScreenId {
        MainScreenIdInvalid = 0,
        MainScreenIdStartup = 1,
        MainScreenIdOrigin = 5,
        MainScreenIdManual,
        MainScreenIdAuto,
        MainScreenIdMonitor,
        MainScreenIdTeaching,
        MainScreenIdTeachingMode = 10,
        MainScreenIdTeachingPosSet,
        MainScreenIdTeachingPosFix = 11,
        MainScreenIdTeachingPalletize,
        MainScreenIdTeachingTimer,
        MainScreenIdTeachingCounter,
        MainScreenIdTeachingPass,
        MainScreenIdTeachingProgram,
        MainScreenIdFunctionControllerSetSystem,
        MainScreenIdFunctionMachineParam,
        MainScreenIdFunctionAreaSet,
        MainScreenIdFunctionMaintain,
        MainScreenIdFunctionProductionMng,
        MainScreenIdFunctionAbsSet,
        MainScreenIdFunctionDeceleration,
        MainScreenIdFunctionVersionMng,
        MainScreenIdHistory,
        MainScreenIdSetupChgExtMem,
        MainScreenIdMax = 255,
    };
    Q_ENUM(MainScreenId)

    enum SubScreenId {
        SubScreenIdInvalid = 0,
        SubScreenIdOrigin = 0,
        SubScreenIdManualIO1 = 0,
        SubScreenIdManualIO2,
        SubScreenIdManualAxisOpe,
        SubScreenIdManualAxisExpress,
        SubScreenIdAutoAxisDefault = 0,
        SubScreenIdAutoAxisTimer = 11,
        SubScreenIdAutoAxisCounter,
        SubScreenIdAutoProductMngDefault = 0,
        SubScreenIdAutoProductMngTimer = 21,
        SubScreenIdAutoProductMngCounter,
        SubScreenIdAutoIODefault = 0,
        SubScreenIdAutoIOTimer = 31,
        SubScreenIdAutoIOCounter,
        SubScreenIdMonitorIOExpress = 0,
        SubScreenIdMonitorIOForced,
        SubScreenIdMonitorRealTime,
#ifdef DRV_MONITOR_GRAPH_10MS_CYCLE
        SubScreenIdMonitorWaveform = 2,
#endif // DRV_MONITOR_GRAPH_10MS_CYCLE
        SubScreenIdMonitorInternal,
        SubScreenIdTeachingDefault = 0,
        SubScreenIdTeachingPosSet = 0,
        SubScreenIdTeachingPosFix,
        SubScreenIdTeachingStandardPalletize = 0,
        SubScreenIdTeachingFreePalletize,
        SubScreenIdTeachingTimerDefault = 0,
        SubScreenIdTeachingCounterDefault = 0,
        SubScreenIdTeachingCounterPage1 = 0,
        SubScreenIdTeachingCounterPage2,
        SubScreenIdTeachingCounterPage3,
        SubScreenIdTeachingCounterPage4,
        SubScreenIdTeachingPassDefault = 0,
        SubScreenIdTeachingProgramDefault = 0,
        SubScreenIdFunctionControllerSetDefault = 0,
        SubScreenIdFunctionControllerSetUser,
        SubScreenIdFunctionControllerSetNetwork,
        SubScreenIdFunctionMachineParamModelSelect = 0,
        SubScreenIdFunctionMachineParam,
        SubScreenIdFunctionMachineParamDriverParam,
        SubScreenIdFunctionMPDriverParamSimpleSet = 2,
        SubScreenIdFunctionMPDriverParamDrvMonitor = 2,
        SubScreenIdFunctionAreaSetDefault = 0,
        SubScreenIdFunctionAreaFix,
        SubScreenIdFunctionMaintainDefault = 0,
        SubScreenIdFunctionMaintain,
        SubScreenIdFunctionProductionMngDefault = 0,
        SubScreenIdFunctionAbsSetDefault = 0,
        SubScreenIdFunctionDecelerationDefault = 0,
        SubScreenIdFunctionVersionMngDefault = 0,
        SubScreenIdFunctionHistoryAlarm = 0,
        SubScreenIdFunctionHistoryOperation,
        SubScreenIdFunctionHistoryIO,
        SubScreenIdSetupChgDefault = 0,
        SubScreenIdSetupChgUsbEntry,
        SubScreenIdSetupChgUsbImport,
        SubScreenIdMax = 255,
    };
    Q_ENUM(SubScreenId)

    enum FileNumCollate {
        FileNumMisMatch = -2,
        FileNumOutOfRange,
        FileNumCollateSuccess,
    };
    Q_ENUM(FileNumCollate)

    enum MemblemKeyNo {
        StorageMethodHorizon = 71,
        StorageMethodVertical = 72,
        MemblemKeyNoKey1 = 201,
        MemblemKeyNoKey2,
        MemblemKeyNoKey3,
        MemblemKeyNoKey4,
        MemblemKeyNoKey5,
        MemblemKeyNoKey6,
        MemblemKeyNoKey7,
        MemblemKeyNoKey8,
        MemblemKeyNoKey9,
        MemblemKeyNoKey10,
        MemblemKeyNoKey11,
        MemblemKeyNoKey12,
        MemblemKeyNoKey13,
        MemblemKeyNoKey14,
        MemblemKeyNoKey15,
        MemblemKeyNoKey16,
        MemblemKeyNoKey17,
        MemblemKeyNoKey18,
        CounterKeyResetBase = 65,
        PalletizeKeyReset = 73,
        PalletizeKeyAllReset,
        TenkeyInputValid = 10,
        OriginStartKeyNo = 242,
        OriginPauseKeyNo = 248,
        OriginCancelKeyNo = 249,
        ReturnCtrlkeyNo = 247,
    };
    Q_ENUM(MemblemKeyNo)

    enum LedInfo {
        LedBlue = 0,
        LedGreen,
        LedRed,
    };
    Q_ENUM(LedInfo)

    enum AuthorityInfo {
        AuthorityNotOperated = 0,
        AuthorityOperationPossible = 1,
        AuthorityNothing = 255,
    };
    Q_ENUM(AuthorityInfo)

    enum DateTimeInfo {
        DateTimeYear = 0,
        DateTimeMonth,
        DateTimeDay,
        DateTimeHour,
        DateTimeMinutes,
        DateTimeSeconds,
    };
    Q_ENUM(DateTimeInfo)

    enum ManualSpeedValue {
        ManualSpeed0001 = 0,
        ManualSpeed001,
        ManualSpeed01,
        ManualSpeed1,
        ManualSpeedSlow,
        ManualSpeedMedium,
        ManualSpeedHigh,
        ManualSpeedMax,
    };
    Q_ENUM(ManualSpeedValue)

    enum AutoSpeedValue {
        AutoSpeed10Percent = 0,
        AutoSpeed20Percent,
        AutoSpeed30Percent,
        AutoSpeed40Percent,
        AutoSpeed50Percent,
        AutoSpeed60Percent,
        AutoSpeed70Percent,
        AutoSpeed80Percent,
        AutoSpeed90Percent,
        AutoSpeed100Percent,
        AutoSpeedMax,
    };
    Q_ENUM(AutoSpeedValue)

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
    Q_ENUM(Axis)

    enum AreaType {
        AreaTypeOutputSideMax,      // 取出側区域(最大)
        AreaTypeOutputSideMin,      // 取出側区域(最小)
        AreaTypeOpenSideMax,        // 開放側区域(最大)
        AreaTypeOpenSideMin,        // 開放側区域(最小)
        AreaTypeDescentMax,         // 下降待機(最大)
        AreaTypeDescentMaxMovement, // 最大移動値
        AreaTypeNum
    };
    Q_ENUM(AreaType)

    enum FtpStatus {
        FtpStatusUnconnected = 0,
        FtpStatusHostLookup,
        FtpStatusConnecting,
        FtpStatusConnected,
        FtpStatusLoggedIn,
        FtpStatusClosing,
        FtpStatusExeListCommand,
        FtpStatusGetFinished,
        FtpStatusPutFinished,
    };
    Q_ENUM(FtpStatus)

    enum FtpCommand {
        FtpCommandUSER = 0,         // ユーザ名入力
        FtpCommandPASS,             // パスワード入力
        FtpCommandPORT,             // アクティブモード開始
        FtpCommandPASV,             // パッシブモード開始
        FtpCommandLIST,             // リスト取得
        FtpCommandRETR,             // ファイル取得（TRC→クライアント)
        FtpCommandSTOR,             // ファイル取得（クライアント→TRC)
        FtpCommandSIZE,             // ファイルサイズ取得
        FtpCommandPWD,              // ディレクトリ表示
        FtpCommandXPWD,             // ディレクトリ表示
        FtpCommandCWD,              // ディレクトリ移動
        FtpCommandNOOP,             // 通信確認用
        FtpCommandALLO,             // 送信データ指定
        FtpCommandMDTM,             // ファイル最終更新時間
    };
    Q_ENUM(FtpCommand)

    // Element Number of VersionViewModel::versionTbl().
    enum VersionNo {
        // from modbus.
        VersionNoAxis1 = 0,
        VersionNoAxis2,
        VersionNoAxis3,
        VersionNoAxis4,
        VersionNoAxis5,
        VersionNoAxis6,
        VersionNoAxis7,
        VersionNoAxis8,
        VersionNoIPL,
        VersionNoMOT,
        VersionNoSPC,
            // Reserve,
            // Reserve,
        VersionNoNCP = 13,
        VersionNoTABLE,
        VersionNoINIT,

        // individually.
        VersionNoQt,
        VersionNoApplication,
        VersionNoPenMsg,
        VersionNoPenSet,
    };
    Q_ENUM(VersionNo)

    // CommonViewModel::mPopupDialogStatus.
    enum PopupDialogStatus {
        PopupDialogStatusNone = -1,

        // TopTab.qml definition.
        PopupDialogStatusUserSwitch = 0,
        PopupDialogStatusDate,
        PopupDialogStatusSpeed,
        PopupDialogStatusLanguage,
        PopupDialogStatusAlarm,
        PopupDialogStatusError,
        PopupDialogStatusEmergencyStop,
        PopupDialogStatusOpeCmd,
        PopupDialogStatusFile,
        PopupDialogStatusConnectError,
        PopupDialogStartup,

        // BottomTab.qml definition.
        PopupDialogStatusFunction,

        // end.
        PopupDialogStatusNum,
    };
    Q_ENUM(PopupDialogStatus)

    enum JogMagnification {
        JogMagnification1,
        JogMagnification10,
        JogMagnification100,
        JogMagnification1000,
    };
    Q_ENUM(JogMagnification)

    enum JogDialSpeed {
        JogDialSpeedUnknown,
        JogDialSpeedSlow,
        JogDialSpeedMiddle,
        JogDialSpeedHigh,
    };
    Q_ENUM(JogDialSpeed)

    enum UsbDownloadType {
        PenUpdate = 0,         // ペンダントアップデートデータ
        ControllerUpdate,      // コントローラアップデートデータ
        NcpData,               // NCP
        BackupDateData,        // 日付ごとバックアップデータ
        PenMsgData,            // PenMsg
        PenSetData,            // PenSet
        LogoImgData,           // logo-image
        ImageData,             //图片文件夹
        ComSpeedSetting,       // 通信スピード設定
    };
    Q_ENUM(UsbDownloadType)
};

#endif // ENUMS_H
