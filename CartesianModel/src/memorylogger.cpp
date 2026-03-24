#include "memorylogger.h"
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>


MemoryLogger MemoryLogger::m_instance;

MemoryLogger::MemoryLogger()
{
}
void MemoryLogger::addLog(const QString& log)
{    
#ifdef LOG_MEMORY_ENABLE
    m_queue.append(log);
    if(m_queue.count() == 500){
        m_queue.dequeue();
    }
#else
    Q_UNUSED(log)
#endif //#ifdef LOG_MEMORY_ENABLE
}

MemoryLogger* MemoryLogger::instance() {
    return &m_instance;
};

void MemoryLogger::save()
{
#ifdef LOG_MEMORY_ENABLE
    //logを格納するフォルダーの存在確認
    QString applicationPath = QCoreApplication::applicationDirPath();
    QString logPath = applicationPath + "/logs";
    QDir logDir(logPath);
    if(!logDir.exists()){
        QDir applicationDir(applicationPath);
        applicationDir.mkdir(logPath);
    }

    QFile file(logPath + "/memorylog.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream stream(&file);
    while (!m_queue.isEmpty()) {
        auto log = m_queue.dequeue();
        stream << log << endl;
    }
    file.close();
#endif  //LOG_MEMORY_ENABLE
}

