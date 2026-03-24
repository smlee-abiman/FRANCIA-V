#ifndef IOHISTORYDATAPARSER_H
#define IOHISTORYDATAPARSER_H

#include <QObject>

class IoHistoryDataParser
{
public:
    explicit IoHistoryDataParser();
    int parseIoHistory(const QString &filePath, QList<QObject *> &ioDataList);
    short toShort(const QByteArray &array, int index);
    void getDateTime(const QByteArray &array, int index, QDateTime &dateTime);
};

#endif // IOHISTORYDATAPARSER_H
