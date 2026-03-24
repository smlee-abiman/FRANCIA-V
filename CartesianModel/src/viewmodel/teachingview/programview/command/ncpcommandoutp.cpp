#include <QRegularExpression>
#include "ncpcommandoutp.h"

//------------------------------------------------------
// NCPCommandOutp.
//------------------------------------------------------
NCPCommandOutp::NCPCommandOutp(QObject *parent)
    : NCPCommandOut(parent)
{
}
//------------------------------------------------------
NCPCommandOutp::NCPCommandOutp(ResourceManager *resource, QObject *parent)
    : NCPCommandOut(resource, parent)
{
}

//------------------------------------------------------
bool NCPCommandOutp::parse(const QString &commandLine)
{
    QRegularExpression commandRe("(\\s+|\\t+|,)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 2) {
        qDebug("Syntax error commandLine:[%s]", qPrintable(commandLine));
        return false;
    }
    QString word;
    int length = splitList.length();
    for (int i = 1; i < length; i++) {
        word = splitList.at(i);
        if (word.at(word.length() - 1) == QChar('F')) {
            Data data(Data::Bool, word.left(word.length() - 1), 0);
            m_outDataList.append(data);
        } else {
            Data data(Data::Bool, word, 1);
            m_outDataList.append(data);
        }
    }
    return true;
}
//------------------------------------------------------
QString NCPCommandOutp::nameString() const
{
    return m_resource->label(41, 16, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandOutp::toString() const
{
    auto string = QString("");
    Data::DataType datatype;
    auto loop = m_outDataList.size();
    for (int i = 0; i < loop; i++) {
        auto& data = m_outDataList.at(i);
        if (i != 0) { string.append("　"); }
        string.append(data.symbolStr());
        string.append("(");
        int index = m_resource->indexAt(data.symbolStr());
        string.append(m_resource->getMessageStringValue("1", index ,m_resource->language()));
        string.append(")");
        datatype = data.type();
        if (datatype == Data::Bool) {
            if(data.value() == 0) {
                string.append("OFF");
            } else {
                string.append("ON");
            }
        }
        string.append(",");
    }
    string.chop(1);
    return string;
}
//------------------------------------------------------
QString NCPCommandOutp::toCode() const
{
    auto code = command() + "\t";
    Data::DataType datatype;
    for (auto data : m_outDataList) {
        code.append(data.symbolStr());
        datatype = data.type();
        if (datatype == Data::Bool) {
            if(data.value() == 0)
                code.append("F");
        }
        code.append(",");
    }
    code.chop(1);
    return code;
}

NCPCommand* NCPCommandOutp::clone() const
{
    auto selfClone = new NCPCommandOutp(m_resource);
    if (selfClone != nullptr) {
        for (auto data : m_outDataList)
            selfClone->append(data);
    }
    return selfClone;
}
