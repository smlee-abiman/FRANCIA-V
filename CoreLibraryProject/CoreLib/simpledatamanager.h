#ifndef SIMPLEDATAMANAGER_H
#define SIMPLEDATAMANAGER_H

#include <QObject>
#include "import_src/packetdata.h"

class SimpleDataManager : public QObject
{
    Q_OBJECT

    //--- BIT Type Memory ------------------------------------------
    Q_PROPERTY(QStringList ioCategoryCapt READ ioCategoryCapt NOTIFY ioCategoryCaptChanged)
    Q_PROPERTY(int ioCategoryId READ ioCategoryId WRITE setIoCategoryId NOTIFY ioCategoryIdChanged)
    Q_PROPERTY(QList<QObject *> ioListDisp READ ioListDisp NOTIFY ioListDispChanged)
    Q_PROPERTY(QList<bool> ioCategoryReadOnly READ ioCategoryReadOnly)
    Q_PROPERTY(QList<uint> ioCategoryOffset READ ioCategoryOffset)

    //--- BYTE Type Memory ------------------------------------------
    Q_PROPERTY(QStringList byteTypeCategoryGroupCapt READ byteTypeCategoryGroupCapt NOTIFY byteTypeCategoryGroupCaptChanged)
    Q_PROPERTY(int byteTypeCategoryGroupId READ byteTypeCategoryGroupId WRITE setByteTypeCategoryGroupId NOTIFY byteTypeCategoryGroupIdChanged)
    Q_PROPERTY(QList<QObject *> byteTypeListDisp READ byteTypeListDisp NOTIFY byteTypeListDispChanged)
    //Q_PROPERTY(QList<bool> byteTypeCategoryReadOnly READ byteTypeCategoryReadOnly)          // remove
    //Q_PROPERTY(QList<uint> byteTypeCategoryOffset READ byteTypeCategoryOffset)              // remove


    //--- WORD Type Memory ------------------------------------------
    Q_PROPERTY(QStringList wordTypeCategoryGroupCapt READ wordTypeCategoryGroupCapt NOTIFY wordTypeCategoryGroupCaptChanged)
    Q_PROPERTY(int wordTypeCategoryGroupId READ wordTypeCategoryGroupId WRITE setWordTypeCategoryGroupId NOTIFY wordTypeCategoryGroupIdChanged)
    Q_PROPERTY(QList<QObject *> wordTypeListDisp READ wordTypeListDisp NOTIFY wordTypeListDispChanged)


    //--- LONG Type Memory ------------------------------------------
    Q_PROPERTY(QStringList longTypeCategoryGroupCapt READ longTypeCategoryGroupCapt NOTIFY longTypeCategoryGroupCaptChanged)
    Q_PROPERTY(int longTypeCategoryGroupId READ longTypeCategoryGroupId WRITE setLongTypeCategoryGroupId NOTIFY longTypeCategoryGroupIdChanged)
    Q_PROPERTY(QList<QObject *> longTypeListDisp READ longTypeListDisp NOTIFY longTypeListDispChanged)

public:
    explicit SimpleDataManager(QObject *parent = nullptr);

    //--- BIT Type Memory ------------------------------------------
    QStringList ioCategoryCapt() const
    {
        return m_ioCategoryCapt;
    }

    int ioCategoryId() const
    {
        return m_ioCategoryId;
    }

    void setIoCategoryId(int ioCategoryId)
    {
        if (m_ioCategoryId == ioCategoryId) {
            return;
        }
        if (false == m_ioCategoryReadOnly[ioCategoryId]) {
            saveIoList();
        }
        m_ioCategoryId = ioCategoryId;
        setIoList();
    }

    QList<QObject *> ioListDisp() {
        return m_ioListDisp;
    }

    QList<bool> ioCategoryReadOnly() {
        return m_ioCategoryReadOnly;
    }

    QList<uint> ioCategoryOffset() {
        return m_ioCategoryOffset;
    }

    //--- BYTE Type Memory ------------------------------------------
/*
    QStringList byteTypeCategoryCapt() const
    {
        return m_byteTypeCategoryCapt;
    }
*/
    QStringList byteTypeCategoryGroupCapt() const
    {
        return m_byteTypeCategoryGroupCapt;
    }

    int byteTypeCategoryGroupId() const
    {
        return m_byteTypeCategoryGroupId;
    }

    void setByteTypeCategoryGroupId(int byteTypeCategoryGroupId)
    {
        if (m_byteTypeCategoryGroupId == byteTypeCategoryGroupId) {
            return;
        }
        if (false == m_byteTypeCategoryReadOnly[byteTypeCategoryGroupId]) {
            saveByteTypeList();
        }
        m_byteTypeCategoryGroupId = byteTypeCategoryGroupId;
        setByteTypeList();
    }

    QList<QObject *> byteTypeListDisp() {
        return m_byteTypeListDisp;
    }

    QList<bool> byteTypeCategoryReadOnly() {
        return m_byteTypeCategoryReadOnly;
    }

    QList<uint> byteTypeCategoryOffset() {
        return m_byteTypeCategoryOffset;
    }


    //--- WORD Type Memory ------------------------------------------

    QStringList wordTypeCategoryGroupCapt() const
    {
        return m_wordTypeCategoryGroupCapt;
    }

    int wordTypeCategoryGroupId() const
    {
        return m_wordTypeCategoryGroupId;
    }

    void setWordTypeCategoryGroupId(int wordTypeCategoryGroupId)
    {
        if (m_wordTypeCategoryGroupId == wordTypeCategoryGroupId) {
            return;
        }
        if (false == m_wordTypeCategoryReadOnly[wordTypeCategoryGroupId]) {
            saveWordTypeList();
        }
        m_wordTypeCategoryGroupId = wordTypeCategoryGroupId;
        setWordTypeList();
    }

    QList<QObject *> wordTypeListDisp() {
        return m_wordTypeListDisp;
    }

    QList<bool> wordTypeCategoryReadOnly() {
        return m_wordTypeCategoryReadOnly;
    }

    QList<uint> wordTypeCategoryOffset() {
        return m_wordTypeCategoryOffset;
    }


    //--- LONG Type Memory ------------------------------------------

    QStringList longTypeCategoryGroupCapt() const
    {
        return m_longTypeCategoryGroupCapt;
    }

    int longTypeCategoryGroupId() const
    {
        return m_longTypeCategoryGroupId;
    }

    void setLongTypeCategoryGroupId(int longTypeCategoryGroupId)
    {
        if (m_longTypeCategoryGroupId == longTypeCategoryGroupId) {
            return;
        }
        if (false == m_longTypeCategoryReadOnly[longTypeCategoryGroupId]) {
            saveLongTypeList();
        }
        m_longTypeCategoryGroupId = longTypeCategoryGroupId;
        setLongTypeList();
    }

    QList<QObject *> longTypeListDisp() {
        return m_longTypeListDisp;
    }

    QList<bool> longTypeCategoryReadOnly() {
        return m_longTypeCategoryReadOnly;
    }

    QList<uint> longTypeCategoryOffset() {
        return m_longTypeCategoryOffset;
    }
signals:
    //--- BIT Type Memory ------------------------------------------
    void ioListDispChanged(const QList<QObject *> &m_ioListDisp);
    void ioCategoryCaptChanged(const QList<QObject *> &ioCategoryCapt);
    void ioCategoryIdChanged(const QList<bool> &ioCategoryId);

    //--- BYTE Type Memory ------------------------------------------
    void byteTypeListDispChanged(const QList<QObject *> &m_byteTypeListDisp);
    void byteTypeCategoryGroupCaptChanged(const QList<QObject *> &byteTypeCategoryGroupCapt);
    void byteTypeCategoryGroupIdChanged(const QList<bool> &byteTypeCategoryGroupId);

    //--- WORD Type Memory ------------------------------------------
    void wordTypeListDispChanged(const QList<QObject *> &m_wordTypeListDisp);
    void wordTypeCategoryGroupCaptChanged(const QList<QObject *> &wordTypeCategoryGroupCapt);
    void wordTypeCategoryGroupIdChanged(const QList<bool> &wordTypeCategoryGroupId);

    //--- LONG Type Memory ------------------------------------------
    void longTypeListDispChanged(const QList<QObject *> &m_longTypeListDisp);
    void longTypeCategoryGroupCaptChanged(const QList<QObject *> &longTypeCategoryGroupCapt);
    void longTypeCategoryGroupIdChanged(const QList<bool> &longTypeCategoryGroupId);

private:
    //--- BIT Type Memory ------------------------------------------
    QStringList m_ioCategoryCapt = {            // <ToDo> Read from CSV file
        tr("実入力反転指定"),
        tr("実出力反転指定"),
        tr("強制ON/OFF"),
        tr("強制ON/OFF　CLEAR"),
        tr("システム入力"),
        tr("実入力"),
        tr("実出力"),
        tr("システム出力"),
        tr("タイマー起動"),
        tr("ユーザーメモリー1"),
        tr("ユーザーメモリー2"),
        tr("ユーザーメモリー3"),
        tr("ステップRAM"),
        tr("OR RAM"),
        tr("NOR RAM"),
        tr("LAST RAM")
    };
    QList<uint> m_ioCategoryOffset;
    QList<uint> m_ioCategoryLength = {          // <ToDo> Read from CSV file
        256*1, 256*1, 256*4, 256*4,
        256*1, 256*1, 256*1, 256*1,
        256*1, 256*1, 256*1, 256*1,
        256*6, 256*6, 256*6, 256*6
    };
    uint m_ioCategoryLengthAmount;
    //uint m_ioCategoryMaxLength;
    QList<bool> m_ioCategoryReadOnly = {        // <ToDo> Read from CSV file
        false, false, false, false,
        true , true , true,  true ,
        true , true , true,  true ,
        false, false, false, true
    };

    int m_ioCategoryId = -1;
    QList<QObject *> m_ioListDisp;
    QList<QObject *> m_ioListAll;
    void setIoList();
    void saveIoList();

    //--- BYTE Type Memory ------------------------------------------
    QStringList m_byteTypeCategoryCapt = {      // <ToDo> Read from CSV file
        tr("システム入力"),
        tr("システム出力"),
        tr("軸アラーム"),
        tr("code mode"),
        tr("標準パレタイズ設定値"),
        tr("全体速度"),
        tr("全体加速度"),
        tr("バックアップメモリー"),
        tr("標準パレタイズカウント値"),
        tr("軸加速度/減速度(基本設定)"),
        tr("拡張外部入力"),
        tr("ユーザーメモリー"),
        tr("軸加速度/減速度(個別指定)"),
        tr("メニューアイコン配列"),
        tr("空き")
    };
    QList<int> m_byteTypeCategoryGroup = {      // <ToDo> Read from CSV file
        0, 0, 0, 0, 0,
        1, 1, 1, 1,
        2,
        3, 3,
        4,
        5,
        6,
        -1
    };
    QStringList m_byteTypeCategoryGroupCapt;

    QList<uint> m_byteTypeCategoryOffset;
    uint m_byteTypeCategoryOffsetTop = 0x2A00;      // <ToDo> Read from CSV file
    QList<uint> m_byteTypeCategoryLength = {        // <ToDo> Read from CSV file
          8,   8,  16,   8,  48,
          4,   4,   8,  48,
         40,
         16,  48,
         40,
        140,
         76
    };
    uint m_byteTypeAmountItems;
    QList<uint> m_byteTypeCategoryGroupLength;
    QList<uint> m_byteTypeCategoryHeadIdxOfGroup;
    //uint m_byteTypeCategoryGroupMaxLength;
    QList<bool> m_byteTypeCategoryReadOnly = {      // <ToDo> Read from CSV file
        false, false, false, false, false,
        false, false, false, false,
        false,
        false, false,
        false,
        false,
        false
    };

    int m_byteTypeCategoryGroupId = -1;
    QList<QObject *> m_byteTypeListDisp;
    QList<QObject *> m_byteTypeListAll;
    void setByteTypeList();
    void saveByteTypeList();
    void makeByteTypeCategoryGroup();



    //--- WORD Type Memory ------------------------------------------
    QStringList m_wordTypeCategoryCapt = {      // <ToDo> Read from CSV file
        tr("アナログ入力"),
        tr("軸指令速度"),
        tr("補間指令速度"),
        tr("軸指令間接変数"),
        tr("空き"),
        tr("空き"),
        tr("段取換要求No."),
        tr("現在段取換No."),
        tr("空き"),
        tr("ユーザーメモリー"),
        tr("実入力入替指定"),
        tr("実出力入替指定"),
        tr("軸Z相検出位置"),
        tr("軸現在トルク値"),
        tr("現在軸速度"),
        tr("空き"),
        tr("空き"),
        tr("生産履歴データ要求No."),
        tr("空き"),
        tr("実行プログラム・ステップ番号")
    };
    QList<int> m_wordTypeCategoryGroup = {      // <ToDo> Read from CSV file
        0, 0, 0, 0, 0,
        1, 1, 1, 1,
        2,
        3, 3, 3, 3,
        4, 4, 4, 4, 4,
        5,
        -1
    };
    QStringList m_wordTypeCategoryGroupCapt;

    QList<uint> m_wordTypeCategoryOffset;
    uint m_wordTypeCategoryOffsetTop = 0x2C00;      // <ToDo> Read from CSV file
    QList<uint> m_wordTypeCategoryLength = {        // <ToDo> Read from CSV file
          16,   16,    4,   4,  8,
          12,    1,    1,   2,
          80,
          16,   16,   16,  16,
          16,   16,   16,   1,   15,

         516
    };
    uint m_wordTypeAmountItems;
    QList<uint> m_wordTypeCategoryGroupLength;
    QList<uint> m_wordTypeCategoryHeadIdxOfGroup;
    //uint m_byteTypeCategoryGroupMaxLength;
    QList<bool> m_wordTypeCategoryReadOnly = {      // <ToDo> Read from CSV file
        false, false, false, false, false,
        false, false, false, false,
        false,
        false, false, false, false,
        false, false, false,
        false, false,
        false
    };

    int m_wordTypeCategoryGroupId = -1;
    QList<QObject *> m_wordTypeListDisp;
    QList<QObject *> m_wordTypeListAll;
    void setWordTypeList();
    void saveWordTypeList();
    void makeWordTypeCategoryGroup();


    //--- LONG Type Memory ------------------------------------------
    QStringList m_longTypeCategoryCapt = {      // <ToDo> Read from CSV file
        tr("Timer設定値"),
        tr("counter設定値"),
        tr("軸パラメータ設定値"),
        tr("軸給油警告設定距離"),
        tr("User memory"),
        tr("Timer counter"),
        tr("preset counter."),
        tr("軸指令値"),
        tr("外部同期指令値"),
        tr("空き"),
        tr("外部カウンター入力値"),
        tr("軸現在位置"),
        tr("軸停止可能位置"),
        tr("軸アンサー"),
        tr("軸移動積算時間"),
        tr("軸移動残り時間"),
        tr("軸移動残り距離"),
        tr("軸移動積算距離(給油)"),
        tr("XYZ座標系　現在位置"),
        tr("ツール座標　現在位置"),
        tr("軸エンコーダパルス値"),
        tr("空き")
    };
    QList<int> m_longTypeCategoryGroup = {      // <ToDo> Read from CSV file
        0,
        1,
        2,
        3, 3,
        4,
        5,
        6,  6,  6,  6,  6,  6,  6,
        7,  7,  7,  7,  7,
        8,  8,  8,
        -1
    };
    QStringList m_longTypeCategoryGroupCapt;

    QList<uint> m_longTypeCategoryOffset;
    uint m_longTypeCategoryOffsetTop = 0x2C00;      // <ToDo> Read from CSV file
    QList<uint> m_longTypeCategoryLength = {        // <ToDo> Read from CSV file
        256,
        256,
        6400,
        16,     80,
        256,
        256,
        16,     1,      11,      4,      16,     16,     16,
        16,     16,     16,     16,     16,
        16,     16,     16
    };
    uint m_longTypeAmountItems;
    QList<uint> m_longTypeCategoryGroupLength;
    QList<uint> m_longTypeCategoryHeadIdxOfGroup;
    //uint m_byteTypeCategoryGroupMaxLength;
    QList<bool> m_longTypeCategoryReadOnly = {      // <ToDo> Read from CSV file
        false, false, false, false, false,
        false, false, false, false,
        false,
        false, false, false, false,
        false, false, false,
        false, false,
        false
    };

    int m_longTypeCategoryGroupId = -1;
    QList<QObject *> m_longTypeListDisp;
    QList<QObject *> m_longTypeListAll;
    void setLongTypeList();
    void saveLongTypeList();
    void makeLongTypeCategoryGroup();

    //初期化
    void InitDummyParam();
};

#endif // SIMPLEDATAMANAGER_H
