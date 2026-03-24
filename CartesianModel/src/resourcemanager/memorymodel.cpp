#include <algorithm>
#include "memorymodel.h"

MemoryModel::MemoryModel(QObject *parent) : QObject(parent) {}

MemoryModel::~MemoryModel() {}

bool MemoryModel::origins(uchar *dest, size_t size) const
{
    s_read_address_0000 *adr = modBus()->lockAddress0000();
    if (adr == nullptr) {
        return false;
    }
    s_read_address_base b = adr->base;
    std::memcpy(dest, b.origin, std::min(size, 7 * sizeof(uchar)));
    modBus()->unlockAddress();
    return true;
}

uchar *MemoryModel::plannedTime()
{
    s_read_address_common *adr = modBus()->lockCommonAddress();
    if (adr == nullptr) {
        return nullptr;
    }

    uchar *ret = reinterpret_cast<uchar *>(std::malloc(6));
    std::memcpy(ret, adr->expectedTime, 6);
    modBus()->unlockAddress();
    return ret;
}

QList<int> MemoryModel::ioList()
{
    QList<int> ret;
    s_read_address_0000 *adr = modBus()->lockAddress0000();
    if (adr == nullptr) {
        return ret;
    }

    uchar *io = adr->extend.io;
    for (int i = 0; i < 32; ++i) {
        ret.append(*(io + i));
    }

    modBus()->unlockAddress();
    return ret;
}

QList<int> MemoryModel::menuList()
{
    QList<int> ret;
    s_read_address_0005 *adr = modBus()->lockAddress0005();
    if (adr == nullptr) {
        return ret;
    }

    uchar *icons = reinterpret_cast<uchar *>(adr->icons);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 24; ++j) {
            ret.append(*(icons + i * 28 + 4 + j));
        }
    modBus()->unlockAddress();
    return ret;
}

QList<int> MemoryModel::menuShortcut()
{
    QList<int> ret;

    s_read_address_0005 *adr = modBus()->lockAddress0005();
    if (adr == nullptr) {
        return ret;
    }

    for (int i = 0; i < 5; ++i) {
        uchar *shortcuts = reinterpret_cast<uchar *>(adr->icons[i].shortcut);
        for (int j = 0; j < 4; ++j)
            ret.append(shortcuts[j]);
    }
    modBus()->unlockAddress();
    return ret;
}

QList<int> MemoryModel::summary()
{
    QList<int> ret;
    s_read_address_0000 *adr = modBus()->lockAddress0000();
    if (adr == nullptr) {
        return ret;
    }
    s_read_address_common base = adr->common;

    ret.append(static_cast<int>(base.numOfGood));
    ret.append(static_cast<int>(base.numOfBad));
    ret.append(static_cast<int>(base.ejectTime));
    ret.append(static_cast<int>(base.creationTime));
    ret.append(static_cast<int>(base.ejectTime));
    ret.append(static_cast<int>(base.openTime));
    ret.append(static_cast<int>(base.creationTime));
    ret.append(0);
    //    ret.append(base->expectedTime);
    modBus()->unlockAddress();
    return ret;
}

QList<s_production_history *> MemoryModel::productionHistoryList()
{
    QList<s_production_history *> ret;
    s_read_address_0004 *s = modBus()->lockAddress0004();
    if (s == nullptr) {
        return ret;
    }
    for (int i = 0; i < 12; ++i) {
        s_production_history *h = new s_production_history();
        *h = s->hist.hist[i];
        ret.append(h);
    }
    modBus()->unlockAddress();
    return ret;
}

int MemoryModel::wholeSpeed()
{
    int ws;
    s_read_address_header *s = modBus()->lockHeaderAddress();
    ws = s->wholeSpeed;
    modBus()->unlockAddress();

    return (ws % 10);
}

int MemoryModel::teachingSpeed()
{
    int ts;
    s_read_address_header *s = modBus()->lockHeaderAddress();
    ts = s->teachingSpeed;
    modBus()->unlockAddress();

    return (ts % 7);
}

int MemoryModel::language()
{
    int l;
    s_read_address_header *s = modBus()->lockHeaderAddress();
    l = s->language;
    modBus()->unlockAddress();

    return (l % 12);
}

QString MemoryModel::comment()
{
    s_read_address_header *h = modBus()->lockHeaderAddress();
    int no = modBus()->toUShort(h->fileNumber);
    QString cmt(reinterpret_cast<char *>(h->fileComment));
    modBus()->unlockAddress();
    QString fileNumber;
    if(no <= 0) {
        fileNumber = "NEW";
        cmt = "";
    } else {
        fileNumber = QString::number(no).rightJustified(3, '0');
    }
    return fileNumber + ":" + cmt;
}

void MemoryModel::writeDebugDataIntoReadAddress(const uchar *data, int size)
{
    int len = size > 256 ? 256 : size;
    uchar *src = reinterpret_cast<uchar *>(modBus()->acquireBuffer());
    memcpy(src, data, static_cast<size_t>(len));
    modBus()->releaseBuffer();
}

ModBus *MemoryModel::m_modBus = new ModBus();
