#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include "ncpcommandout.h"

static const QHash<QString, NCPCommandOut::IoType> JIG_IO_HASH_TABLE = {
    {"SCK1", NCPCommandOut::IoType::Open },
    {"SCK2", NCPCommandOut::IoType::Open },
    {"SCK3", NCPCommandOut::IoType::Open },
    {"SCK4", NCPCommandOut::IoType::Open },
    {"SCK5", NCPCommandOut::IoType::Open },
    {"SCK6", NCPCommandOut::IoType::Open },
    {"SCK7", NCPCommandOut::IoType::Open },
    {"SCK8", NCPCommandOut::IoType::Open },
    {"CCK1", NCPCommandOut::IoType::Open },
    {"CCK2", NCPCommandOut::IoType::Open },
    {"CCK3", NCPCommandOut::IoType::Open },
    {"CCK4", NCPCommandOut::IoType::Open },
    {"CCK5", NCPCommandOut::IoType::Open },
    {"CCK6", NCPCommandOut::IoType::Open },
    {"CCK7", NCPCommandOut::IoType::Open },
    {"CCK8", NCPCommandOut::IoType::Open },
    {"SOK1", NCPCommandOut::IoType::Close },
    {"SOK2", NCPCommandOut::IoType::Close },
    {"SOK3", NCPCommandOut::IoType::Close },
    {"SOK4", NCPCommandOut::IoType::Close },
    {"SOK5", NCPCommandOut::IoType::Close },
    {"SOK6", NCPCommandOut::IoType::Close },
    {"SOK7", NCPCommandOut::IoType::Close },
    {"SOK8", NCPCommandOut::IoType::Close },
    {"COK1", NCPCommandOut::IoType::Close },
    {"COK2", NCPCommandOut::IoType::Close },
    {"COK3", NCPCommandOut::IoType::Close },
    {"COK4", NCPCommandOut::IoType::Close },
    {"COK5", NCPCommandOut::IoType::Close },
    {"COK6", NCPCommandOut::IoType::Close },
    {"COK7", NCPCommandOut::IoType::Close },
    {"COK8", NCPCommandOut::IoType::Close },
    {"CSC1", NCPCommandOut::IoType::Detect },
    {"CSC2", NCPCommandOut::IoType::Detect  },
    {"CSC3", NCPCommandOut::IoType::Detect  },
    {"CSC4", NCPCommandOut::IoType::Detect  },
    {"CSC5", NCPCommandOut::IoType::Detect  },
    {"CSC6", NCPCommandOut::IoType::Detect  },
    {"CSC7", NCPCommandOut::IoType::Detect  },
    {"CSC8", NCPCommandOut::IoType::Detect  },
    {"CSC9", NCPCommandOut::IoType::Detect  },
    {"CSC10", NCPCommandOut::IoType::Detect  },
    {"CSC11", NCPCommandOut::IoType::Detect  },
    {"CSC12", NCPCommandOut::IoType::Detect  },
    {"CSC13", NCPCommandOut::IoType::Detect  },
    {"CSC14", NCPCommandOut::IoType::Detect  },
    {"CSC15", NCPCommandOut::IoType::Detect  },
    {"CSC16", NCPCommandOut::IoType::Detect  },
    {"CSO1", NCPCommandOut::IoType::Detect  },
    {"CSO2", NCPCommandOut::IoType::Detect  },
    {"CSO3", NCPCommandOut::IoType::Detect  },
    {"CSO4", NCPCommandOut::IoType::Detect  },
    {"CSO5", NCPCommandOut::IoType::Detect  },
    {"CSO6", NCPCommandOut::IoType::Detect  },
    {"CSO7", NCPCommandOut::IoType::Detect  },
    {"CSO8", NCPCommandOut::IoType::Detect  },
    {"CSO9", NCPCommandOut::IoType::Detect  },
    {"CSO10", NCPCommandOut::IoType::Detect  },
    {"CSO11", NCPCommandOut::IoType::Detect  },
    {"CSO12", NCPCommandOut::IoType::Detect  },
    {"CSO13", NCPCommandOut::IoType::Detect  },
    {"CSO14", NCPCommandOut::IoType::Detect  },
    {"CSO15", NCPCommandOut::IoType::Detect  },
    {"CSO16", NCPCommandOut::IoType::Detect  },
    {"CCM1", NCPCommandOut::IoType::Wait  },
    {"CCM2", NCPCommandOut::IoType::Wait },
    {"CCM3", NCPCommandOut::IoType::Wait },
    {"CCM4", NCPCommandOut::IoType::Wait },
    {"CCM5", NCPCommandOut::IoType::Wait },
    {"CCM6", NCPCommandOut::IoType::Wait },
    {"CCM7", NCPCommandOut::IoType::Wait },
    {"CCM8", NCPCommandOut::IoType::Wait },
    {"CCM9", NCPCommandOut::IoType::Wait },
    {"CCM10", NCPCommandOut::IoType::Wait },
    {"CCM11", NCPCommandOut::IoType::Wait },
    {"CCM12", NCPCommandOut::IoType::Wait },
    {"CCM13", NCPCommandOut::IoType::Wait },
    {"CCM14", NCPCommandOut::IoType::Wait },
    {"CCM15", NCPCommandOut::IoType::Wait },
    {"CCM16", NCPCommandOut::IoType::Wait },
    {"COM1", NCPCommandOut::IoType::Wait },
    {"COM2", NCPCommandOut::IoType::Wait },
    {"COM3", NCPCommandOut::IoType::Wait },
    {"COM4", NCPCommandOut::IoType::Wait },
    {"COM5", NCPCommandOut::IoType::Wait },
    {"COM6", NCPCommandOut::IoType::Wait },
    {"COM7", NCPCommandOut::IoType::Wait },
    {"COM8", NCPCommandOut::IoType::Wait },
    {"COM9", NCPCommandOut::IoType::Wait },
    {"COM10", NCPCommandOut::IoType::Wait },
    {"COM11", NCPCommandOut::IoType::Wait },
    {"COM12", NCPCommandOut::IoType::Wait },
    {"COM13", NCPCommandOut::IoType::Wait },
    {"COM14", NCPCommandOut::IoType::Wait },
    {"COM15", NCPCommandOut::IoType::Wait },
    {"COM16", NCPCommandOut::IoType::Wait },
};


const QHash<QString, int> JIG_IO_STRING_HASH_TABLE = {
    {"SCK1", 335 },
    {"SCK2", 336 },
    {"SCK3", 337 },
    {"SCK4", 338 },
    {"SCK5", 339 },
    {"SCK6", 340 },
    {"SCK7", 341 },
    {"SCK8", 342 },
    {"CCK1", 343 },
    {"CCK2", 344 },
    {"CCK3", 345 },
    {"CCK4", 346 },
    {"CCK5", 347 },
    {"CCK6", 348 },
    {"CCK7", 349 },
    {"CCK8", 350 },
    {"SOK1", 335 },
    {"SOK2", 336 },
    {"SOK3", 337 },
    {"SOK4", 338 },
    {"SOK5", 339 },
    {"SOK6", 340 },
    {"SOK7", 341 },
    {"SOK8", 342 },
    {"COK1", 343 },
    {"COK2", 344 },
    {"COK3", 345 },
    {"COK4", 346 },
    {"COK5", 347 },
    {"COK6", 348 },
    {"COK7", 349 },
    {"COK8", 350 },
};

NCPCommandOut::NCPCommandOut(QObject *parent)
    : NCPCommand(parent)
{

}

NCPCommandOut::NCPCommandOut(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{

}

//------------------------------------------------------
bool NCPCommandOut::parse(const QString& commandLine)
{
//    qDebug() << "parse " << commandLine;
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }
    parseInternalOutputCmd(splitList);
    return true;
}

QString NCPCommandOut::nameString() const
{
    return m_resource->label(41, 27, m_resource->language());
}

QString NCPCommandOut::toString() const
{
    return toInternalOutputString();
}

QString NCPCommandOut::toInternalOutputString() const
{
    auto string = QString("");
    int curSeparateCommaNum = mSeparatedCommaNum;
    for (auto data : m_outDataList) {
        int index = m_resource->indexAt(data.symbolStr());
        string += QString("%1(%2)").arg(data.symbolStr()).arg(m_resource->getMessageStringValue("1", index ,m_resource->language()));

        if (data.type() == Data::DataType::Bool) {
            string.append(data.value() == 0 ? "OFF" : "ON");
        } else {
            string.append("=");
            string.append(QString::number(data.value()));
        }
        if (curSeparateCommaNum >= 1) {
            string.append(",");
        }
        curSeparateCommaNum--;
    }
    return string;
}

QString NCPCommandOut::toFixtureString() const
{
    auto string = QString("");
    if (m_outDataList.count() <= 0) {
        return string;
    }

    if (isJigType()) {
        string.append(toJigOpenCloseString());
        string.append(toJigString());
    }
    return string;
}

QString NCPCommandOut::toJigOpenCloseString() const
{
    auto string = QString("");
    QString symbolStrKey;
    auto topData = m_outDataList.at(0);
    symbolStrKey = topData.symbolStr();
    IoType ioType = JIG_IO_HASH_TABLE.value(symbolStrKey, IoType::Other);
    if (ioType == IoType::Open) {
        string.append("ON");
    } else if (ioType == IoType::Close) {
        string.append("OFF");
    }
    return string;
}

QString NCPCommandOut::toJigString() const
{
    auto string = QString("");
    QString symbolStrKey;
    for (auto data : m_outDataList) {
        symbolStrKey = data.symbolStr();
        IoType ioType = JIG_IO_HASH_TABLE.value(symbolStrKey, IoType::Other);
        switch (ioType) {
        case IoType::Open:
        case IoType::Close:
        {
            int labelIndex = JIG_IO_STRING_HASH_TABLE.value(symbolStrKey, -1);
            if (labelIndex != -1)
                string.append(m_resource->label(41, labelIndex));
            string.append(",");
        }
            break;
        case IoType::Detect:
        {
            string.chop(1);
            string.append(QString("_%1%2,").arg(m_resource->label(41, 301)).arg(data.value() == 0 ? "OFF" : "ON"));
        }
            break;
        case IoType::Wait:
            string.append(QString("%1 %2,").arg(symbolStrKey).arg(data.value() == 0 ? "OFF" : "ON"));
            break;
        default:
            string.append(QString("%1=%2,").arg(symbolStrKey).arg(data.value()));
            break;
        }
    }
    string.chop(1);
    return string;
}

bool NCPCommandOut::isJigType() const
{
    bool ret = true;
    QString symbolStrKey;
    if ((m_outDataList.length() % 2) == 1) {
        return false;
    }

    for (int i = 0; i < m_outDataList.length(); i++) {
        symbolStrKey = m_outDataList.at(i).symbolStr();
        IoType ioType = JIG_IO_HASH_TABLE.value(symbolStrKey, IoType::Other);
        if (i % 2 == 0) {
            if ((ioType != NCPCommandOut::IoType::Open) && (ioType != NCPCommandOut::IoType::Close)) {
                ret = false;
                break;
            }
        } else {
            if (ioType != NCPCommandOut::IoType::Detect) {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

QString NCPCommandOut::toCode() const
{
    auto code = command() + "\t";
    Data::DataType datatype;
    for (auto data : m_outDataList) {
        code.append(data.symbolStr());
        datatype = data.type();
        if (datatype == Data::Bool) {
            code.append(" ");
            code.append(data.value() == 0 ? "OFF" : "ON");
        } else if (datatype == Data::Int) {
            code.append("=");
            code.append(QString("%1").arg(data.value(), 3, 10, QChar('0')));
        }
        code.append(",");
    }
    code.chop(1);
    return code;
}

NCPCommand* NCPCommandOut::clone() const
{
    auto selfClone = new NCPCommandOut(m_resource);
    if (selfClone != nullptr) {
        for (auto data : m_outDataList)
            selfClone->append(data);
    }
    return selfClone;
}

void NCPCommandOut::append(Data& data)
{
    m_outDataList.append(data);
    mSeparatedCommaNum = m_outDataList.length() - 1;
}

void NCPCommandOut::parseInternalOutputCmd(QStringList splitList)
{
    if (!m_resource){ return; }
    int length = splitList.length();
    if (length < 2){
        qDebug() << "parseInternalOutputCmd splitList length:" << splitList.length();
        return;
    }
    int splitStrCommaNum = (length / 2) - 1;
    mSeparatedCommaNum = splitStrCommaNum;
    QString word;
    for (int i = 0; i < length; i++) {
        word = splitList.at(i);
        if (word.contains("=")) {
            auto splitDataList = word.split("=");
            bool ok;
            int value = splitDataList.at(1).toInt(&ok);
            if (ok) {
                Data data(Data::Int, splitDataList.at(0), value);
                m_outDataList.append(data);
            }
        } else if (word == "ON") {
            Data data(Data::Bool, splitList.at(i - 1), 1);
            m_outDataList.append(data);
        } else if (word == "OFF") {
            Data data(Data::Bool, splitList.at(i - 1), 0);
            m_outDataList.append(data);
        }
    }
}
