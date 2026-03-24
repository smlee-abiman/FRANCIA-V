#ifndef OPERATIONPROGRAMLINEDATA_H
#define OPERATIONPROGRAMLINEDATA_H

#include <QObject>

class OperationProgramLineData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int line READ line WRITE setLine NOTIFY lineChanged)
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)

public:
    int line();
    void setLine(int line);
    QString data();
    void setData(QString data);

signals:
    void lineChanged(int line);
    void dataChanged(QString data);

private:
    int m_line = 0;
    QString m_data = "";
};

Q_DECLARE_METATYPE(OperationProgramLineData *);

#endif // OPERATIONPROGRAMLINEDATA_H
