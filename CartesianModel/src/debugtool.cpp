#include <QFile>
#include "debugtool.h"
#include "resourcemanager/memorymodel.h"

DebugTool::DebugTool(QObject *parent) : QObject(parent) {}

bool DebugTool::feedTestData(QString url)
{
    url.replace("file://", "");

    QFile file(url);
    file.open(QIODevice::ReadOnly);

    QByteArray arr = file.readAll();
    size_t sz = static_cast<size_t>(arr.length());
    if (arr.length() > 256) {
        return false;
    }
    const char *data = arr.data();
    char *ptr = reinterpret_cast<char *>(MemoryModel::modBus()->acquireBuffer());
    std::memcpy(ptr, data, sz);
    MemoryModel::modBus()->releaseBuffer();

    return true;
}
