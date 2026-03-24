/*
 * This class handles the data that will go or came from/to controller communication layer.
 */

#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <memory>
#include "modbus.h"

class MemoryModel : public QObject
{
    Q_OBJECT

public:
    explicit MemoryModel(QObject *parent = nullptr);
    ~MemoryModel();

    void sendRequest(QString f_code, wchar_t addr, wchar_t nod);

    static ModBus *modBus() { return m_modBus; }

    // Parse bits og origins ('原点/オーバーラン/メカブレーキ/予備')
    bool origins(uchar *dest, size_t size) const;

    uchar *plannedTime();
    // Parse I/O status list ('I/O表示') from read address 0000
    QList<int> ioList();
    // Parse menu layout from read address 0005
    QList<int> menuList();
    // Parse 4 short cuts in menu screen from read address 0005
    QList<int> menuShortcut();
    // Parse status summary list ('生産管理関連データ') from common area of read address
    QList<int> summary();
    // Parse production history data ('生産状況履歴データ') from read address 0004
    QList<s_production_history *> productionHistoryList();

    // Parse common read address and give whole speed ('全体速度')
    int wholeSpeed();
    // Parse common read address and give teaching speec ('教示速度')
    int teachingSpeed();
    // Parse common read address and give teaching speec ('選択言語')
    int language();
    // Parse header read address and give comment string
    // ('現在選択ファイル番号＆ファイルコメント表示')
    QString comment();

    // Debug feature, fill the data into shared memory (read address) that was retrieved from
    // outside
    void writeDebugDataIntoReadAddress(const uchar *data, int size);

signals:

public slots:

private:
    s_axis_info axis[7];
    static ModBus *m_modBus;
    s_production_history history[352];
    int m_production_history_index = 0;
};

#endif // MEMORYMODEL_H
