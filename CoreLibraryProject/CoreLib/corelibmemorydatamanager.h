#ifndef CORELIBMEMORYDATAMANAGER_H
#define CORELIBMEMORYDATAMANAGER_H

#include <QObject>
#include <QMap>
#include "corelibmodbusdatamanager.h"

#define GET_POINT_MEMORY_INFO(index, data) \
    ((index == 0) ? ((data & 0x000F) >> (index * 4)) : \
     (index == 1) ? ((data & 0x00F0) >> (index * 4)) : \
     (index == 2) ? ((data & 0x0F00) >> (index * 4)) : \
     (index == 3) ? ((data & 0xF000) >> (index * 4)) : -1)

class CoreLibMemoryDataManager : public QObject
{
    Q_OBJECT
public:
    static CoreLibMemoryDataManager* get_instance() {
        static CoreLibMemoryDataManager instance;
        return &instance;
    }

private:
    explicit CoreLibMemoryDataManager(QObject *parent = nullptr);
    ~CoreLibMemoryDataManager();

    CoreLibModBusDataManager* mbDataMgr = CoreLibModBusDataManager::get_instance();

public:
    typedef struct
    {
        ushort address;                             // address
        ushort memoryNum;                           // メモリ数
        ushort byteNum;                             // byte数

    } stModBusMemoryTable;

    // ModBusメモリの一覧
    enum FixedModBusMemory {
        BitFirstMemory = 0,
        BitActInputInversion = BitFirstMemory,          // 実入力反転指定
        BitActOutputInversion,                          // 実出力反転指定
        BitForcesOnOff,                                 // 強制ON/OFF
        BitForcesOnOffClear,                            // 強制ON/OFF CLEAR
        BitSytemInput,                                  // システム入力
        BitActInput,                                    // 実入力
        BitACtOutput,                                   // 実出力
        BitSytemOutput,                                 // システム出力
        BitTimerStart,                                  // タイマー起動
        BitUserMemory1,                                 // ユーザメモリー1
        BitUserMemory2,                                 // ユーザメモリー2
        BitUserMemory3,                                 // ユーザメモリー3
        BitStepRAM,                                     // ステップRAM
        BitOrRAM,                                       // OR RAM
        BitNorRAM,                                      // NOR RAM
        BitLastRAM,                                     // LAST RAM
        BitLastMemory = BitLastRAM,
        ByteFirstMemory,
        BytePendant1InputOutput = ByteFirstMemory,      // pendant1入出力
        BytePendant2InputOutput,                        // pendant2入出力
        BytePendant3InputOutput,                        // pendant3入出力
        BytePendant4InputOutput,                        // pendant4入出力
        ByteAbnormalMotion,                             // モーション異常
        BytePeripheralAbnormality,                      // 周辺異常
        ByteReserverd1,                                 // 空き
        ByteReserverd2,                                 // 空き
        ByteTeachingModeSpeedCommand1,                  // 教示モード/速度指令1
        ByteTeachingModeSpeedCommand2,                  // 教示モード/速度指令2
        ByteTeachingModeSpeedCommand3,                  // 教示モード/速度指令3
        ByteTeachingModeSpeedCommand4,                  // 教示モード/速度指令4
        ByteAxisAlarm,                                  // 軸アラーム
        ByteCodeMode,                                   // code mode
        ByteStandardPalletizingSettingValue,            // 標準パレタイズ設定値
        ByteOverallSpeed,                               // 全体速度
        ByteOverallAcceleration,                        // 全体加速度
        ByteMeomory,                                    // 記憶メモリー
        ByteStandardPalletizingCountValue,              // 標準パレタイズカウント値
        ByteLanguageSelect,                             // 言語選択
        ByteUserLevel,                                  // ユーザーレベル
        ByteScreenOffTime,                              // 画面OFF時間
        ByteReserverd3,                                 // 空き
        ByteNumOfOnceTakenOut,                          // 1回取出数
        ByteReserverd4,                                 // 空き
        ByteUserMemory,                                 // ユーザーメモリー
        ByteAxisZPhaseDetectionPosition,                // 軸Z相検出位置
        ByteReserverd5,                                 // 空き
        ByteMenuIconArray,                              // メニュアイコン配列
        ByteReserverd6,                                 // 空き
        ByteLastMemory = ByteReserverd6,
        WordFirstMemory,
        WordAnalogCounterInput = WordFirstMemory,       // アナログ/カウンタ－入力
        WordReserverd1,                                 // 空き
        WordRobot1IndirectValuePatternNo,               // robot1 間接変数(patternNo.)
        WordRobot1IndirectValuePointNo,                 // robot1 間接変数(pointNo.)
        WordRobot1SetupChangeRequestNo,                 // robot1 段取換要求No.
        WordRobot1CurrentSetupChangeNo,                 // robot1 段取換要求No.
        WordRobot2CurrentSetupChangeNo,                 // robot2 段取換要求No.
        WordRobot3CurrentSetupChangeNo,                 // robot3 段取換要求No.
        WordRobot4CurrentSetupChangeNo,                 // robot4 段取換要求No.
        WordUserMemory,                                 // ユーザーメモリー
        WordCurrentCommandTorqueValue,                  // 現在指令トルク値
        WordAxisCommandTorqueValue,                     // 軸現在トルク値
        WordCurrentCommandSppedValue,                   // 現在指令速度値
        WordCurrentAxisSpped,                           // 現在軸速度
        WordActInputReplaceDesignaiton,                 // 実入力入替指定
        WordActOutputReplaceDesignaiton,                // 実出力入替指定
        WordRobot1ProductHistoryRelated,                // robot1 生産履歴関連
        WordRobot2ProductHistoryRelated,                // robot2 生産履歴関連
        WordRobot3ProductHistoryRelated,                // robot3 生産履歴関連
        WordRobot4ProductHistoryRelated,                // robot4 生産履歴関連
        WordReserverd2,                                 // 空き
        WordExecutionProgramStepNumber1,                // 実行プログラム/ステップ番号1
        WordExecutionProgramStepNumber2,                // 実行プログラム/ステップ番号2
        WordExecutionProgramStepNumber3,                // 実行プログラム/ステップ番号3
        WordExecutionProgramStepNumber4,                // 実行プログラム/ステップ番号4
        WordMotionCpuInfo,                              // MotionCPU情報
        WordReserverd3,                                 // 空き
        WordVariousMonitorInMotionCpu,                  // MotionCPU内各種モニター
        WordMonitorCommandAnswer,                       // Motion指令アンサー
        WordReserverd4,                                 // 空き
        WordLastMemory = WordReserverd4,
        LongFirstMemory,
        LongTimerSetValue = LongFirstMemory,            // Timer設定値
        LongCounterSetValue,                            // counter設定値
        LongAxis1ParamaterSetValue,                     // 軸1パラメータ設定値
        LongAxisLubricationWarnSetDistance,             // 軸給油警告設定距離
        LongUserMemory,                                 // User Memory
        LongTimerCounter,                               // TimerCounter
        LongPresetCounter,                              // preset counter
        LongProductionNumberSetting,                    // 生産数設定（各robot毎)
        LongReserverd1,                                 // 空き
        LongExternalSyncCommandValue,                   // 外部同期指令値
        LongReserverd2,                                 // 空き
        LongExternalCounterInputValue,                  // 外部カウンター入力値
        LongAxisTargetPulsePosition,                    // 軸目標パルス位置
        LongAxisEncoderPulsePosition,                   // 軸エンコーダーパルス位置
        LongCurrentAxisPosition,                        // 軸現在位置(mm、deg）
        LongArmTipPosition,                             // アーム先端位置(mm)
        LongCylindricalCoordinates,                     // 円筒座標
        LongToolCoordinates,                            // ツール座標
        LongAxisStopPosition,                           // 軸停止可能位置
        LongAxialAccumulatedDistance,                   // 軸移動積算距離(給油)
        LongLastMemory = LongAxialAccumulatedDistance,
        StringFirstMemory,
        StringCurrentTime = StringFirstMemory,          // 現在時刻
        StringMachineInstallationDate,                  // 機械設置日時
        StringReserverd1,                               // 空き
        StringReserverd2,                               // 空き
        StringHistory1TimeStamp,                        // 履歴1タイムスタンプ
        StringHistory2TimeStamp,                        // 履歴2タイムスタンプ
        StringHistory3TimeStamp,                        // 履歴3タイムスタンプ
        StringHistory4TimeStamp,                        // 履歴4タイムスタンプ
        StringProductStartTime1,                        // 生産開始時刻1
        StringProductStartTime2,                        // 生産開始時刻2
        StringProductStartTime3,                        // 生産開始時刻3
        StringProductStartTime4,                        // 生産開始時刻4
        StringScheduledProductEntTime1,                 // 生産終了予定時刻1
        StringScheduledProductEntTime2,                 // 生産終了予定時刻2
        StringScheduledProductEntTime3,                 // 生産終了予定時刻3
        StringScheduledProductEntTime4,                 // 生産終了予定時刻4
        StringIPAddress1,                               // IP ADDRESS 1
        StringSubNetMask1,                              // SUBNET MASK 1
        StringMACAddress1,                              // MAC ADDRESS 1
        Stringeserverd3,                                // 空き
        StringIPAddress2,                               // IP ADDRESS 2
        StringSubNetMask2,                              // SUBNET MASK 2
        StringMACAddress2,                              // MAC ADDRESS 2
        StringEeserverd4,                               // 空き
        StringMachineName,                              // 機械名称
        StringCurrentFileComment1,                      // 現在ファイルコメント１
        StringCurrentFileComment2,                      // 現在ファイルコメント2
        StringCurrentFileComment3,                      // 現在ファイルコメント3
        StringCurrentFileComment4,                      // 現在ファイルコメント4
        StringTextWork,                                 // テキストwork
        StringDriveControl,                             // Drive control
        StringReserverd5,                               // 空き
        StringLastMemory = StringReserverd5,
        PointFirstMemory,
        PointDataHeader = PointFirstMemory,             // POINT DATA Header
        PointFilesAdd,                                  // files added
        FixedModBusMemory,                              // 固定MODBUSMEMORY MAX
    };

    stModBusMemoryTable fixedMemoryInfo[FixedModBusMemory::FixedModBusMemory] =
    {
        // bit type memory
        {0x0000,    256,    32},       // 実入力反転指定
        {0x0020,    256,    32},       // 実出力反転指定
        {0x0040,   1024,   128},       // 実出力反転指定
        {0x00C0,   1024,   128},       // 出力反転指定
        {0x0140,    256,    32},       // システム入力
        {0x0160,    256,    32},       // 実入力
        {0x0180,    256,    32},       // 実出力
        {0x01A0,    256,    32},       // システム出力
        {0x01C0,    256,    32},       // タイマー起動
        {0x01E0,    256,    32},       // ユーザーメモリー1
        {0x0200,    256,    32},       // ユーザーメモリー2
        {0x0220,    256,    32},       // ユーザーメモリー3
        {0x0240,   1536,   192},       // ステップRAM
        {0x0300,   1536,   192},       // OR RAM
        {0x03C0,   1536,   192},       // NOR RAM
        {0x0480,   1536,   192},       // LAST RAM
        // byte type memory
        {0x0540,      8,     8},       // pendant1入出力
        {0x0548,      8,     8},       // pendant2入出力
        {0x0550,      8,     8},       // pendant3入出力
        {0x0558,      8,     8},       // pendant4入出力
        {0x0560,      1,     1},       // モーション異常
        {0x0561,      1,     1},       // 周辺異常
        {0x0562,      1,     1},       // 空き
        {0x0563,      1,     1},       // 空き
        {0x0564,      1,     1},       // 教示モード/速度指令1
        {0x0565,      1,     1},       // 教示モード/速度指令2
        {0x0566,      1,     1},       // 教示モード/速度指令3
        {0x0567,      1,     1},       // 教示モード/速度指令4
        {0x0568,     16,    16},       // 軸アラーム
        {0x0578,      8,     8},       // code mode
        {0x0580,     64,    64},       // 標準パレタイズ設定値
        {0x05C0,      4,     4},       // 全体速度
        {0x05C4,      4,     4},       // 全体加速度
        {0x05C8,      8,     8},       // 記憶メモリー
        {0x05D0,     48,    48},       // 標準パレタイズカウント値
        {0x0600,      1,     1},       // 言語選択
        {0x0601,      1,     1},       // ユーザーレベル
        {0x0602,      1,     1},       // 画面OFF時間
        {0x0603,      1,     1},       // 空き
        {0x0604,      4,     4},       // 1回取出数
        {0x0608,      8,     8},       // 空き
        {0x0610,     48,    48},       // ユーザーメモリー
        {0x0640,     16,    16},       // 軸Z相検出位置
        {0x0650,     24,    24},       // 空き
        {0x0668,    140,   140},       // メニュアイコン配列
        {0x06F4,     76,    76},       // 空き
        // word type memory
        {0x0740,     16,    32},       // アナログ/カウンタ－入力
        {0x0760,     16,    32},       // 空き
        {0x0780,      1,     2},       // robot1 間接変数(patternNo.)
        {0x0782,      1,     2},       // robot1 間接変数(pointNo.)
        {0x0784,      1,     2},       // robot1 段取換要求No.
        {0x0786,      1,     2},       // robot1 段取換要求No.
        {0x0788,      4,     8},       // robot2 段取換要求No.
        {0x0790,      4,     8},       // robot3 段取換要求No.
        {0x0798,      4,     8},       // robot4 段取換要求No.
        {0x07A0,     80,   160},       // ユーザーメモリー
        {0x0840,     16,    32},       // 現在指令トルク値
        {0x0860,     16,    32},       // 軸現在トルク値
        {0x0880,     16,    32},       // 現在指令速度値
        {0x08A0,     16,    32},       // 現在軸速度
        {0x08C0,     16,    32},       // 実入力入替指定
        {0x08E0,     16,    32},       // 実出力入替指定
        {0x0900,      5,    10},       // robot1 生産履歴関連
        {0x090A,      5,    10},       // robot2 生産履歴関連
        {0x0914,      5,    10},       // robot3 生産履歴関連
        {0x091E,      5,    10},       // robot4 生産履歴関連
        {0x0928,     12,    24},       // 空き
        {0x0940,    128,   256},       // 実行プログラム/ステップ番号1
        {0x0A40,    128,   256},       // 実行プログラム/ステップ番号2
        {0x0B40,    128,   256},       // 実行プログラム/ステップ番号3
        {0x0C40,    128,   256},       // 実行プログラム/ステップ番号4
        {0x0D40,      1,     2},       // MotionCPU情報
        {0x0D42,      1,     2},       // 空き
        {0x0D44,     28,    58},       // MotionCPU内各種モニター
        {0x0D7C,     64,   128},       // Motion指令アンサー
        {0x0DFC,    162,   324},       // 空き
        // long type memory
        {0x0F40,    256,  1024},       // Timer設定値
        {0x1340,    256,  1024},       // counter設定値
        {0x1740,   4080,  4080},       // 軸1パラメータ設定値
        {0x5700,     16,    64},       // 軸給油警告設定距離
        {0x5740,     80,   320},       // User Memory
        {0x5880,    256,  1024},       // TimerCounter
        {0x5C80,    256,  1024},       // preset counter
        {0x6080,      4,    16},       // 生産数設定（各robot毎)
        {0x6090,     12,    48},       // 空き
        {0x60C0,      1,     4},       // 外部同期指令値
        {0x60C4,     11,    44},       // 空き
        {0x60F0,      4,    16},       // 外部カウンター入力値
        {0x6100,     16,    64},       // 軸目標パルス位置
        {0x6140,     16,    64},       // 軸エンコーダーパルス位置
        {0x6180,     16,    64},       // 軸現在位置(mm、deg）
        {0x61C0,     16,    64},       // アーム先端位置(mm)
        {0x6200,     16,    64},       // 円筒座標
        {0x6240,     16,    64},       // ツール座標
        {0x6280,     16,    64},       // 軸停止可能位置
        {0x62C0,     16,    64},       // 軸移動積算距離(給油)
        // string type memory
        {0x6300,      1,     6},       // 現在時刻
        {0x6306,      1,     6},       // 機械設置日時
        {0x630C,      1,     6},       // 空き
        {0x6312,      1,     6},       // 空き
        {0x6318,      1,     6},       // 履歴1タイムスタンプ
        {0x631E,      1,     6},       // 履歴2タイムスタンプ
        {0x6324,      1,     6},       // 履歴3タイムスタンプ
        {0x632A,      1,     6},       // 履歴4タイムスタンプ
        {0x6330,      1,     6},       // 生産開始時刻1
        {0x6336,      1,     6},       // 生産開始時刻2
        {0x633C,      1,     6},       // 生産開始時刻3
        {0x6342,      1,     6},       // 生産開始時刻4
        {0x6348,      1,     6},       // 生産終了予定時刻1
        {0x634E,      1,     6},       // 生産終了予定時刻2
        {0x6354,      1,     6},       // 生産終了予定時刻3
        {0x635A,      1,     6},       // 生産終了予定時刻4
        {0x6360,      1,     6},       // IP ADDRESS 1
        {0x6366,      1,     6},       // SUBNET MASK 1
        {0x636C,      1,     6},       // MAC ADDRESS 1
        {0x6372,      1,     6},       // 空き
        {0x6378,      1,     6},       // IP ADDRESS 2
        {0x637E,      1,     6},       // SUBNET MASK 2
        {0x6384,      1,     6},       // MAC ADDRESS 2
        {0x638A,      1,     6},       // 空き
        {0x6390,      1,    20},       // 機械名称
        {0x63A4,      1,    20},       // 現在ファイルコメント１
        {0x63B8,      1,    20},       // 現在ファイルコメント2
        {0x63CC,      1,    20},       // 現在ファイルコメント3
        {0x63E0,      1,    20},       // 現在ファイルコメント4
        {0x63F4,      1,    32},       // テキストwork
        {0x6414,      1,    32},       // Drive control
        {0x6434,      1,    32},       // 空き
        // point type memory
        {0x6480,      1,     2},       //POINT DATA Header
        {0x6482,      1,     2},       // files added
    };

    enum ModBusMemoryType {
        bit_type,
        byte_type,
        word_type,
        long_type,
        string_type,
        point_type
    };

    typedef struct
    {
        long realAddr;
        ModBusMemoryType type;
        ushort size;
        long func01;
        long func02;
        long func03;
        long func04;
        long func05;
        long func06;
        long func0F;
        long func10;
        long func03ModBusSummary;
        long func10ModBusSummary;
    } stModBusAddressTable;

    QVector<stModBusAddressTable> addrTableList = {
        // bit_type
        {0x0000, bit_type, 32, 0x0000, -1, -1, 0x0000, 0x0000, -1, 0x0000, -1, 0x0100, 0x0100},
        {0x0020, bit_type, 32, 0x0100, -1, -1, 0x0010, 0x0100, -1, 0x0100, -1, 0x0200, 0x0300},
        {0x0040, bit_type, 128, 0x0200, -1, -1, 0x0020, 0x0200, -1, 0x0200, -1, 0x0300, 0x0500},
        {0x00C0, bit_type, 128, 0x0600, -1, -1, 0x0060, 0x0600, -1, 0x0600, -1, 0x0700, 0x0D00},
        {0x0140, bit_type, 32, -1, 0x0000, -1, 0x00A0, -1, -1, -1, -1, 0x0B00, -1},
        {0x0160, bit_type, 32, -1, 0x0100, -1, 0x00B0, -1, -1, -1, -1, 0x0C00, -1},
        {0x0180, bit_type, 32, -1, 0x0200, -1, 0x00C0, -1, -1, -1, -1, 0x0D00, -1},
        {0x01A0, bit_type, 32, -1, 0x0300, -1, 0x00D0, -1, -1, -1, -1, 0x0E00, -1},
        {0x01C0, bit_type, 32, -1, 0x0400, -1, 0x00E0, -1, -1, -1, -1, 0x0F00, -1},
        {0x01E0, bit_type, 32, -1, 0x0500, -1, 0x00F0, -1, -1, -1, -1, 0x1000, -1},
        {0x0200, bit_type, 32, -1, 0x0600, -1, 0x0100, -1, -1, -1, -1, 0x1100, -1},
        {0x0220, bit_type, 32, -1, 0x0700, -1, 0x0110, -1, -1, -1, -1, 0x1200, -1},
        {0x0240, bit_type, 192, 0x0A00, -1, -1, 0x0120, 0x0A00, -1, 0x0A00, -1, 0x1300, 0x1500},
        {0x0300, bit_type, 192, 0x1000, -1, -1, 0x0180, 0x1000, -1, 0x1000, -1, 0x1900, 0x2100},
        {0x03C0, bit_type, 192, 0x1600, -1, -1, 0x01E0, 0x1600, -1, 0x1600, -1, 0x1F00, 0x2D00},
        {0x0480, bit_type, 192, -1, 0x0800, -1, 0x0240, -1, -1, -1, -1, 0x2500, -1},
        //byte_type
        {0x0540, byte_type, 8, -1, -1, 0x02A0, 0x02A0, -1, 0x02A0, -1, 0x02A0, 0x2B00, 0x3900},
        {0x0548, byte_type, 8, -1, -1, 0x02A8, 0x02A8, -1, 0x02A8, -1, 0x02A8, 0x2B02, 0x3902},
        {0x0550, byte_type, 8, -1, -1, 0x02B0, 0x02B0, -1, 0x02B0, -1, 0x02B0, 0x2B04, 0x3904},
        {0x0558, byte_type, 8, -1, -1, 0x02B8, 0x02B8, -1, 0x02B8, -1, 0x02B8, 0x2B06, 0x3906},
        {0x0560, byte_type, 1, -1, -1, -1, 0x02C0, -1, 0x02C0, -1, 0x02C0, 0x2B08, 0x3908},
        {0x0561, byte_type, 1, -1, -1, 0x02C1, 0x02C1, -1, 0x02C1, -1, 0x02C1, -1, -1},
        {0x0562, byte_type, 1, -1, -1, 0x02C2, 0x02C2, -1, 0x02C2, -1, 0x02C2, 0x2B09, 0x3909},
        {0x0563, byte_type, 1, -1, -1, 0x02C3, 0x02C3, -1, 0x02C3, -1, 0x02C3, 0x2B0A, 0x390A},
        {0x0564, byte_type, 1, -1, -1, 0x02C4, 0x02C4, -1, 0x02C4, -1, 0x02C4, 0x2B0B, 0x390B},
        {0x0565, byte_type, 1, -1, -1, 0x02C5, 0x02C5, -1, 0x02C5, -1, 0x02C5, 0x2B0C, 0x390C},
        {0x0566, byte_type, 1, -1, -1, 0x02C6, 0x02C6, -1, 0x02C6, -1, 0x02C6, 0x2B0D, 0x390D},
        {0x0567, byte_type, 1, -1, -1, 0x02C7, 0x02C7, -1, 0x02C7, -1, 0x02C7, 0x2B0E, 0x390E},
        {0x0568, byte_type, 16, -1, -1, -1, 0x02C8, -1, 0x02C8, -1, 0x02C8, 0x2B0F, 0x390F},
        {0x0578, byte_type, 8, -1, -1, 0x02D8, 0x02D8, -1, 0x02D8, -1, 0x02D8, 0x2B1F, 0x391F},
        {0x0580, byte_type, 64, -1, -1, 0x02E0, 0x02E0, -1, 0x02E0, -1, 0x02E0, 0x2B27, 0x3927},
        {0x05C0, byte_type, 4, -1, -1, 0x0320, 0x0320, -1, 0x0320, -1, 0x0320, 0x2B37, 0x3937},
        {0x05C4, byte_type, 4, -1, -1, 0x0324, 0x0324, -1, 0x0324, -1, 0x0324, 0x2B3B, 0x393B},
        {0x05C8, byte_type, 8, -1, -1, 0x0328, 0x0328, -1, 0x0328, -1, 0x0328, 0x2B3F, 0x393F},
        {0x05D0, byte_type, 48, -1, -1, 0x0330, 0x0330, -1, 0x0330, -1, 0x0330, 0x2B47, 0x3947},
        {0x0600, byte_type, 1, -1, -1, 0x0360, 0x0360, -1, 0x0360, -1, 0x0360, 0x2B57, 0x3957},
        {0x0601, byte_type, 1, -1, -1, 0x0361, 0x0361, -1, 0x0361, -1, 0x0361, 0x2B58, 0x3958},
        {0x0602, byte_type, 1, -1, -1, 0x0362, 0x0362, -1, 0x0362, -1, 0x0362, 0x2B59, 0x3959},
        {0x0603, byte_type, 1, -1, -1, 0x0363, 0x0363, -1, 0x0363, -1, 0x0363, 0x2B5A, 0x395A},
        {0x0604, byte_type, 4, -1, -1, 0x0364, 0x0364, -1, 0x0364, -1, 0x0364, 0x2B5B, 0x395B},
        {0x0608, byte_type, 8, -1, -1, 0x0368, 0x0368, -1, 0x0368, -1, 0x0368, 0x2B5F, 0x395F},
        {0x0610, byte_type, 48, -1, -1, 0x0370, 0x0370, -1, 0x0370, -1, 0x0370, 0x2B67, 0x3967},
        {0x0640, byte_type, 16, -1, -1, -1, 0x03A0, -1, 0x03A0, -1, 0x03A0, 0x2B97, -1},
        {0x0650, byte_type, 24, -1, -1, 0x03B0, 0x03B0, -1, 0x03B0, -1, 0x03B0, 0x2BA7, 0x3997},
        {0x0668, byte_type, 140, -1, -1, 0x03C8, 0x03C8, -1, 0x03C8, -1, 0x03C8, 0x2BBF, 0x39AF},
        {0x06F4, byte_type, 76, -1, -1, 0x0454, 0x0454, -1, 0x0454, -1, 0x0454, 0x2C4B, 0x3A3B},
        //word_type
        {0x0740, word_type, 32, -1, -1, 0x04A0, 0x04A0, -1, 0x04A0, -1, 0x04A0, 0x2C97, -1},
        {0x0760, word_type, 32, -1, -1, 0x04B0, 0x04B0, -1, 0x04B0, -1, 0x04B0, 0x2CA7, 0x3A87},
        {0x0780, word_type, 2, -1, -1, 0x04C0, 0x04C0, -1, 0x04C0, -1, 0x04C0, 0x2CB7, 0x3A97},
        {0x0782, word_type, 2, -1, -1, 0x04C1, 0x04C1, -1, 0x04C1, -1, 0x04C1, 0x2CB8, 0x3A98},
        {0x0784, word_type, 2, -1, -1, 0x04C2, 0x04C2, -1, 0x04C2, -1, 0x04C2, 0x2CB9, 0x3A99},
        {0x0786, word_type, 2, -1, -1, 0x04C3, 0x04C3, -1, 0x04C3, -1, 0x04C3, 0x2CBA, 0x3A9A},
        {0x0788, word_type, 8, -1, -1, 0x04C4, 0x04C4, -1, 0x04C4, -1, 0x04C4, 0x2CBB, 0x3A9B},
        {0x0790, word_type, 8, -1, -1, 0x04C8, 0x04C8, -1, 0x04C8, -1, 0x04C8, 0x2CBF, 0x3A9F},
        {0x0798, word_type, 8, -1, -1, 0x04CC, 0x04CC, -1, 0x04CC, -1, 0x04CC, 0x2CC3, 0x3AA3},
        {0x07A0, word_type, 160, -1, -1, 0x04D0, 0x04D0, -1, 0x04D0, -1, 0x04D0, 0x2CC7, 0x3AA7},
        {0x0840, word_type, 32, -1, -1, -1, 0x0520, -1, 0x0520, -1, 0x0520, 0x2D17, -1},
        {0x0860, word_type, 32, -1, -1, -1, 0x0530, -1, 0x0530, -1, 0x0530, 0x2D27, -1},
        {0x0880, word_type, 32, -1, -1, -1, 0x0540, -1, 0x0540, -1, 0x0540, 0x2D37, -1},
        {0x08A0, word_type, 32, -1, -1, -1, 0x0550, -1, 0x0550, -1, 0x0550, 0x2D47, -1},
        {0x08C0, word_type, 32, -1, -1, 0x0560, 0x0560, -1, 0x0560, -1, 0x0560, 0x2D57, 0x3AF7},
        {0x08E0, word_type, 32, -1, -1, 0x0570, 0x0570, -1, 0x0570, -1, 0x0570, 0x2D67, 0x3B07},
        {0x0900, word_type, 10, -1, -1, 0x0580, 0x0580, -1, 0x0580, -1, 0x0580, 0x2D77, 0x3B17},
        {0x090A, word_type, 10, -1, -1, 0x0585, 0x0585, -1, 0x0585, -1, 0x0585, 0x2D7C, 0x3B1C},
        {0x0914, word_type, 10, -1, -1, 0x058A, 0x058A, -1, 0x058A, -1, 0x058A, 0x2D81, 0x3B21},
        {0x091E, word_type, 10, -1, -1, 0x058F, 0x058F, -1, 0x058F, -1, 0x058F, 0x2D86, 0x3B26},
        {0x0928, word_type, 24, -1, -1, 0x0594, 0x0594, -1, 0x0594, -1, 0x0594, 0x2D8B, 0x3B2B},
        {0x0940, word_type, 256, -1, -1, -1, 0x05A0, -1, 0x05A0, -1, 0x05A0, 0x2D97, -1},
        {0x0A40, word_type, 256, -1, -1, -1, 0x0620, -1, 0x0620, -1, 0x0620, 0x2E17, -1},
        {0x0B40, word_type, 256, -1, -1, -1, 0x06A0, -1, 0x06A0, -1, 0x06A0, 0x2E97, -1},
        {0x0C40, word_type, 256, -1, -1, -1, 0x0720, -1, 0x0720, -1, 0x0720, 0x2F17, -1},
        {0x0D40, word_type, 2, -1, -1, -1, 0x07A0, -1, 0x07A0, -1, 0x07A0, 0x2F97, -1},
        {0x0D42, word_type, 2, -1, -1, -1, 0x07A1, -1, 0x07A1, -1, 0x07A1, 0x2F98, -1},
        {0x0D44, word_type, 56, -1, -1, -1, 0x07A2, -1, 0x07A2, -1, 0x07A2, 0x2F99, -1},
        {0x0D7C, word_type, 128, -1, -1, -1, 0x07BE, -1, 0x07BE, -1, 0x07BE, 0x2FB5, -1},
        {0x0DFC, word_type, 324, -1, -1, 0x07FE, 0x07FE, -1, 0x07FE, -1, 0x07FE, 0x2FF5, 0x3B37},
        //long_type
        {0x0F40, long_type, 1024, -1, -1, 0x08A0, 0x08A0, -1, 0x08A0, -1, 0x08A0, 0x3097, 0x3BD9},
        {0x1340, long_type, 1024, -1, -1, 0x0AA0, 0x0AA0, -1, 0x0AA0, -1, 0x0AA0, 0x3197, 0x3CD9},
        {0x1740, long_type, 16320, -1, -1, 0x0CA0, 0x0CA0, -1, 0x0CA0, -1, 0x0CA0, 0x3297, 0x3DD9},
        {0x5700, long_type, 64, -1, -1, 0x2C80, 0x2C80, -1, 0x2C80, -1, 0x2C80, 0x4287, 0x4DC9},
        {0x5740, long_type, 320, -1, -1, 0x2CA0, 0x2CA0, -1, 0x2CA0, -1, 0x2CA0, 0x4297, 0x4DD9},
        {0x5880, long_type, 1024, -1, -1, 0x2D40, 0x2D40, -1, 0x2D40, -1, 0x2D40, 0x42E7, 0x4E29},
        {0x5C80, long_type, 1024, -1, -1, 0x2F40, 0x2F40, -1, 0x2F40, -1, 0x2F40, 0x43E7, 0x4F29},
        {0x6080, long_type, 16, -1, -1, 0x3140, 0x3140, -1, 0x3140, -1, 0x3140, 0x44E7, 0x5029},
        {0x6090, long_type, 48, -1, -1, 0x3148, 0x3148, -1, 0x3148, -1, 0x3148, 0x44EB, 0x502D},
        {0x60C0, long_type, 4, -1, -1, 0x3160, 0x3160, -1, 0x3160, -1, 0x3160, 0x44F7, -1},
        {0x60C4, long_type, 44, -1, -1, 0x3162, 0x3162, -1, 0x3162, -1, 0x3162, 0x44F8, -1},
        {0x60F0, long_type, 16, -1, -1, 0x3178, 0x3178, -1, 0x3178, -1, 0x3178, 0x4503, -1},
        {0x6100, long_type, 64, -1, -1, 0x3180, 0x3180, -1, 0x3180, -1, 0x3180, 0x4507, -1},
        {0x6140, long_type, 64, -1, -1, 0x31A0, 0x31A0, -1, 0x31A0, -1, 0x31A0, 0x4517, -1},
        {0x6180, long_type, 64, -1, -1, 0x31C0, 0x31C0, -1, 0x31C0, -1, 0x31C0, 0x4527, -1},
        {0x61C0, long_type, 64, -1, -1, 0x31E0, 0x31E0, -1, 0x31E0, -1, 0x31E0, 0x4537, -1},
        {0x6200, long_type, 64, -1, -1, 0x3200, 0x3200, -1, 0x3200, -1, 0x3200, 0x4547, -1},
        {0x6240, long_type, 64, -1, -1, 0x3220, 0x3220, -1, 0x3220, -1, 0x3220, 0x4557, -1},
        {0x6280, long_type, 64, -1, -1, 0x3240, 0x3240, -1, 0x3240, -1, 0x3240, 0x4567, -1},
        {0x62C0, long_type, 64, -1, -1, 0x3260, 0x3260, -1, 0x3260, -1, 0x3260, 0x4577, -1},
        //string_type
/*
        {0x6300, string_type, 6, -1, -1, 0x3280, 0x3280, -1, 0x3280, -1, 0x3280, 0x4587, 0x5039},
        {0x6306, string_type, 6, -1, -1, 0x3283, 0x3283, -1, 0x3283, -1, 0x3283, 0x4588, 0x503A},
        {0x630C, string_type, 6, -1, -1, 0x3286, 0x3286, -1, 0x3286, -1, 0x3286, 0x4589, 0x503B},
        {0x6312, string_type, 6, -1, -1, 0x3289, 0x3289, -1, 0x3289, -1, 0x3289, 0x458A, 0x503C},
        {0x6318, string_type, 6, -1, -1, 0x328C, 0x328C, -1, 0x328C, -1, 0x328C, 0x458B, 0x503D},
        {0x631E, string_type, 6, -1, -1, 0x328F, 0x328F, -1, 0x328F, -1, 0x328F, 0x458C, 0x503E},
        {0x6324, string_type, 6, -1, -1, 0x3292, 0x3292, -1, 0x3292, -1, 0x3292, 0x458D, 0x503F},
        {0x632A, string_type, 6, -1, -1, 0x3295, 0x3295, -1, 0x3295, -1, 0x3295, 0x458E, 0x5040},
        {0x6330, string_type, 6, -1, -1, 0x3298, 0x3298, -1, 0x3298, -1, 0x3298, 0x458F, 0x5041},
        {0x6336, string_type, 6, -1, -1, 0x329B, 0x329B, -1, 0x329B, -1, 0x329B, 0x4590, 0x5042},
        {0x633C, string_type, 6, -1, -1, 0x329E, 0x329E, -1, 0x329E, -1, 0x329E, 0x4591, 0x5043},
        {0x6342, string_type, 6, -1, -1, 0x32A1, 0x32A1, -1, 0x32A1, -1, 0x32A1, 0x4592, 0x5044},
        {0x6348, string_type, 6, -1, -1, 0x32A4, 0x32A4, -1, 0x32A4, -1, 0x32A4, 0x4593, -1},
        {0x634E, string_type, 6, -1, -1, 0x32A7, 0x32A7, -1, 0x32A7, -1, 0x32A7, 0x4594, -1},
        {0x6354, string_type, 6, -1, -1, 0x32AA, 0x32AA, -1, 0x32AA, -1, 0x32AA, 0x4595, -1},
        {0x635A, string_type, 6, -1, -1, 0x32AD, 0x32AD, -1, 0x32AD, -1, 0x32AD, 0x4596, -1},
        {0x6360, string_type, 6, -1, -1, 0x32B0, 0x32B0, -1, 0x32B0, -1, 0x32B0, 0x4597, 0x5045},
        {0x6366, string_type, 6, -1, -1, 0x32B3, 0x32B3, -1, 0x32B3, -1, 0x32B3, 0x4598, 0x5046},
        {0x636C, string_type, 6, -1, -1, 0x32B6, 0x32B6, -1, 0x32B6, -1, 0x32B6, 0x4599, 0x5047},
        {0x6372, string_type, 6, -1, -1, 0x32B9, 0x32B9, -1, 0x32B9, -1, 0x32B9, 0x459A, 0x5048},
        {0x6378, string_type, 6, -1, -1, 0x32BC, 0x32BC, -1, 0x32BC, -1, 0x32BC, 0x459B, 0x5049},
        {0x637E, string_type, 6, -1, -1, 0x32BF, 0x32BF, -1, 0x32BF, -1, 0x32BF, 0x459C, 0x504A},
        {0x6384, string_type, 6, -1, -1, 0x32C2, 0x32C2, -1, 0x32C2, -1, 0x32C2, 0x459D, 0x504B},
        {0x638A, string_type, 6, -1, -1, 0x32C5, 0x32C5, -1, 0x32C5, -1, 0x32C5, 0x459E, 0x504C},
        {0x6390, string_type, 6, -1, -1, 0x32C8, 0x32C8, -1, 0x32C8, -1, 0x32C8, 0x459F, 0x504D},
        {0x6396, string_type, 6, -1, -1, 0x32CB, 0x32CB, -1, 0x32CB, -1, 0x32CB, 0x45A0, 0x504E},
        {0x639C, string_type, 20, -1, -1, 0x32CE, 0x32CE, -1, 0x32CE, -1, 0x32CE, 0x45A1, 0x504F},
        {0x63B0, string_type, 20, -1, -1, 0x32D8, 0x32D8, -1, 0x32D8, -1, 0x32D8, 0x45A2, -1},
        {0x63C4, string_type, 20, -1, -1, 0x32E2, 0x32E2, -1, 0x32E2, -1, 0x32E2, 0x45A3, -1},
        {0x63D8, string_type, 20, -1, -1, 0x32EC, 0x32EC, -1, 0x32EC, -1, 0x32EC, 0x45A4, -1},
        {0x63EC, string_type, 20, -1, -1, 0x32F6, 0x32F6, -1, 0x32F6, -1, 0x32F6, 0x45A5, -1},
        {0x6400, string_type, 20, -1, -1, 0x3300, 0x3300, -1, 0x3300, -1, 0x3300, 0x45A6, 0x5050},
        {0x6420, string_type, 20, -1, -1, 0x3310, 0x3310, -1, 0x3310, -1, 0x3310, 0x45A7, 0x5051},
*/
        {0x6300, string_type, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x639C, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63A4, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63AC, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63B4, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63BC, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63C4, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63CC, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63D4, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63DC, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63E4, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63EC, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63F4, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x63FC, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x6404, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x640C, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x6414, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x641C, string_type, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {0x6424, string_type, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        //point_type
        {0x6440, point_type, 2, -1, -1, 0x3320, 0x3320, -1, 0x3320, -1, 0x3320, -1, -1},
    };

    // ModBusメモリ実体
    uchar mModBusData[704*1024];
    // 実アドレスからテーブルデータへのリンク
    QMap<long, stModBusAddressTable*> mMapRealAddrToTable;


    typedef struct
    {
        int startIndex;
        int endIndex;
        int startAddress;
        int endAddress;
        bool isLong;
    } stModBusIndexToAddressMap;

    // read only 0x02
    QVector<stModBusIndexToAddressMap> DIList = {
        {   0,  255, 0x000000, 0x00001F, false},
        { 256,  511, 0x000020, 0x00003F, false},
        { 512,  767, 0x000040, 0x00005F, false},
        { 768, 1023, 0x000060, 0x00007F, false},
        {1024, 1279, 0x000080, 0x00009F, false},
        {1280, 1535, 0x0000A0, 0x0000BF, false},
        {1536, 1791, 0x0000C0, 0x0000DF, false},
        {1792, 2047, 0x0000E0, 0x0000FF, false},
        {2048, 2303, 0x000100, 0x00011F, false},
        {2304, 2559, 0x000120, 0x00013F, false},
        {2560, 2815, 0x000140, 0x00015F, false},
    };

    // read/write 0x01
    QVector<stModBusIndexToAddressMap> DOList = {
        {   0,  255, 0x000300, 0x00001F, false},
        { 256,  511, 0x000320, 0x00003F, false},
        { 512,  767, 0x000340, 0x00005F, false},
        { 768, 1023, 0x000360, 0x00007F, false},
        {1024, 1279, 0x000380, 0x00009F, false},
        {1280, 1535, 0x0003A0, 0x0000BF, false},
        {1536, 1791, 0x0003C0, 0x0000DF, false},
        {1792, 2047, 0x0003E0, 0x0000FF, false},
        {2048, 2303, 0x000400, 0x00011F, false},
        {2304, 2559, 0x000420, 0x00013F, false},
        {2560, 2815, 0x000440, 0x00015F, false},
        {2816, 3071, 0x000460, 0x00015F, false},
        {3072, 3327, 0x000480, 0x00015F, false},
        {3328, 3583, 0x0004A0, 0x00015F, false},
        {3584, 3839, 0x0004C0, 0x00015F, false},
        {3840, 4095, 0x0004E0, 0x00015F, false},
        {4096, 4351, 0x000500, 0x00015F, false},
        {4352, 4607, 0x000520, 0x00015F, false},
        {4608, 4863, 0x000540, 0x00015F, false},
        {4864, 5119, 0x000560, 0x00015F, false},
        {5120, 5375, 0x000580, 0x00015F, false},
        {5376, 5439, 0x0005A0, 0x00015F, false},
        {5440, 5455, 0x0005C0, 0x00015F, false},
        {5456, 8015, 0x0005E0, 0x00015F, false},
    };

    // read only 0x04
    QVector<stModBusIndexToAddressMap> RGList = {
        {  1,  64, 0x0F40, 0x0FBE, true},
        { 65,  80, 0x0740, 0x075E, false},
        { 81, 144, 0x0FC0, 0x103E, true},
        {145, 152, 0x0760, 0x076E, false},
        {153, 160, 0x0770, 0x077E, false},
        {161, 196, 0x639C, 0x63E2, false},
        {197, 200, 0x0780, 0x0786, false},
        {201, 260, 0x0788, 0x07FE, false},
    };

    // read/write 0x03/0x06/0x10
    QVector<stModBusIndexToAddressMap> HRList = {
        {  1,  59, 0x0800, 0x0874, false},
        { 60,  67, 0x0876, 0x0884, false},
        { 68,  72, 0x0886, 0x088E, false},
        { 73, 104, 0x63E4, 0x6422, false},
        {105, 127, 0x0890, 0x08BC, false},
        {128, 152, 0x08BE, 0x08EE, false},
        {153, 159, 0x08F0, 0x08FC, false},
        {160, 168, 0x08FE, 0x090E, false},
        {169, 175, 0x0910, 0x091C, false},
        {176, 356, 0x091E, 0x0A86, false},
        {357, 359, 0x0A88, 0x0A8C, false},
        {360, 520, 0x0A8E, 0x0BCE, false},
        {521, 527, 0x0BD0, 0x0BDC, false},
        {528, 568, 0x0BDE, 0x0C2E, false},
        {596, 575, 0x0C30, 0x0C3C, false},
        {576, 616, 0x0C3E, 0x0C8E, false},
        {617, 623, 0x0C90, 0x0C9C, false},
        {624, 640, 0x0C9E, 0x0CBE, false},
        {641, 647, 0x0CC0, 0x0CCC, false},
        {648, 680, 0x0CCE, 0x0D0E, false},
        {681, 687, 0x0D10, 0x0D1C, false},
        {688, 704, 0x0D1E, 0x0D3E, false},
        {705, 711, 0x0D40, 0x0D4C, false},
        {712, 744, 0x0D4E, 0x0D8E, false},
        {745, 760, 0x1040, 0x105E, true},
        {761, 763, 0x0D90, 0x0D94, false},
        {764, 792, 0x0D96, 0x0DCE, false},
    };

public:
    uchar getBitData(int index, int bit_idx);
    bool setBitData(int index, int bit_idx, uchar data);
    uchar getByteData(int index);
    bool setByteData(int index, uchar data);
    ushort getWordData(int index);
    ulong getLongData(int index);

    QByteArray getModBusSummaryData(int group);
    QByteArray getModBusBitData(QModbusPdu::FunctionCode funcCode, int startIndex, int size);
    bool setModBusBitData(int startIndex, int size, QByteArray byteAry);
    QByteArray getModBusData(QModbusPdu::FunctionCode funcCode, int startIndex, int size);
    bool setModBusData(int startIndex, int size, QByteArray byteAry);

signals:
    void updateAllValue();

};

#endif // CORELIBMEMORYDATAMANAGER_H
