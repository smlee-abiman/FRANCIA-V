#ifndef OPERATIONHISTORYDATAPARSER_H
#define OPERATIONHISTORYDATAPARSER_H

#include <QObject>

class OperationHistoryDataParser
{
public:
    explicit OperationHistoryDataParser();
    int parseOperationHistory(const QString &filePath, QList<QObject *> &operationDataList);
};

#endif // OPERATIONHISTORYDATAPARSER_H
