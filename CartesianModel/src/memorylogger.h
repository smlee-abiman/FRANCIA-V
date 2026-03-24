#ifndef MEMORYLOGGER_H
#define MEMORYLOGGER_H

#include <QObject>
#include <QString>
#include <QQueue>

class MemoryLogger : public QObject
{
    Q_OBJECT

public:
    static MemoryLogger* instance();

    Q_INVOKABLE void addLog(const QString& log);
    Q_INVOKABLE void save();

signals:

private:
    MemoryLogger();
    QQueue<QString> m_queue;
    static MemoryLogger m_instance;
};

#endif // MEMORYLOGGER_H
