#ifndef ALARMHISTORYDATAPARSER_H
#define ALARMHISTORYDATAPARSER_H

#include <QObject>
#include "commonviewmodel.h"

class AlarmHistoryData;

class AlarmHistoryDataParser
{
public:
    AlarmHistoryDataParser();
    int parseAlarmHistory(const QString &filePath, QList<AlarmHistoryData *> &alarmDataList);
    AlarmHistoryData *parseAlarmInfo(const AlarmInfo &alarmInfo, const QDateTime &dateTime);
};

#endif // ALARMHISTORYDATAPARSER_H
