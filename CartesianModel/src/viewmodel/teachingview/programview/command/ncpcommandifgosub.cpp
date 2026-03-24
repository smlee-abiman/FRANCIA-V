#include "ncpcommandifgosub.h"
#include <QRegularExpression>
#include <QString>

//------------------------------------------------------
// NCPCommandIfGoSub.
//------------------------------------------------------
NCPCommandIfGoSub::NCPCommandIfGoSub(QObject *parent)
    : NCPCommandIn(parent)
    , m_programNo(0)
{
}
//------------------------------------------------------
NCPCommandIfGoSub::NCPCommandIfGoSub(ResourceManager *resource, QObject *parent)
    : NCPCommandIn(resource, parent)
    , m_programNo(0)
{
}
//------------------------------------------------------
NCPCommandIfGoSub::NCPCommandIfGoSub(ResourceManager *resource, int programNo, QObject *parent)
    : NCPCommandIn(resource, parent)
{
    setProgramNo(programNo);
}
//------------------------------------------------------
void NCPCommandIfGoSub::setProgramNo(int no)
{
    const int minProgramNo = 1;
    const int maxProgramNo = 255;
    if ((no >= minProgramNo) && (no <= maxProgramNo)) {
        m_programNo = no;
    }
}
//------------------------------------------------------
bool NCPCommandIfGoSub::parse(const QString &constCommandLine)
{
    auto commandLine(constCommandLine);
    commandLine.replace(",", " , ");
    commandLine.replace("or", " or ");
    commandLine.replace("and", " and ");
    commandLine.replace("=", " = ");
    commandLine.replace("<", " < ");
    commandLine.replace(">", " > ");
    commandLine.replace("#", " # ");

    QRegularExpression commandRe("(\\s+|\\t+)");
    auto splitList = commandLine.split(commandRe);
    if (splitList.size() < 6) {
        qDebug() << "Syntax error commandLine:[" << commandLine << "]";
        return false;
    }
    // parse program.
    auto relation = QString(","); // "and", "or"
    auto symbol = QString("");
    auto symbol1 = QString("");
    auto opeStr = QString("");    // "=", "#", "<", ">"
    auto value = -1;              // "ON", "OFF", number.
    auto loop = splitList.size();
    m_programNo = 0;
    for (int i = 1; i < loop; i++) {    // [0] = "IF", so skip it.
        auto word = splitList[i];
        if (word.compare("gosub", Qt::CaseInsensitive) == 0) {
            // parse GOSUB #XXX.
            auto symbolIndex = i + 1;   // "#"
            auto noIndex     = i + 2;
            if (m_programNo != 0) { goto lError; }
            else if (loop <= noIndex) { goto lError; }
            else if (splitList[symbolIndex] != "#") { goto lError; }
            auto ret = parseProgramNo(splitList[noIndex]);
            if (!ret) { goto lError; }
            break;
        }
        auto isNumber = false;
        auto number = word.toInt(&isNumber);
        if ((word.compare("and", Qt::CaseInsensitive) == 0) ||
            (word == ",")) {
            if (!relation.isEmpty()) { goto lError; }
            relation = ",";
        } else if ((word.compare("or", Qt::CaseInsensitive) == 0)) {
            if (!relation.isEmpty()) { goto lError; }
            relation = "or";
        } else if (word == "ON") {
            if (value != -1) { goto lError; }
            value = 1;
        } else if (word == "OFF") {
            if (value != -1) { goto lError; }
            value = 0;
        } else if (isNumber) {
            if (value != -1) { goto lError; }
            value = number;
        } else if (word == "=" || word == "#" || word == "<" || word == ">") {
            if (!opeStr.isEmpty()) { goto lError; }
            opeStr = word;
        } else {
//            if (!symbol.isEmpty()) { goto lError; }
            if (!symbol.isEmpty()) {
                symbol1 = word;
            }else {
                symbol = word;
            }
        }
        if (value == -1 && symbol1 == "") {
            continue;
        }

        if (relation == "" || symbol == "") {
            goto lError;
        }
        auto ioAddress = m_resource->indexAt(symbol);
        auto var = new NCPVariable(relation,
                                   symbol,
                                   symbol1,
                                   opeStr,
                                   value,
                                   ioAddress
                                   );
        m_variableList.append(var);

        // clear variable.
        relation = "";
        symbol = "";
        opeStr = "";
        value = -1;
    }
    return true;

lError:
    qDebug() << "Syntax error commandLine:[" << commandLine << "]";
    return false;
}
//------------------------------------------------------
bool NCPCommandIfGoSub::parseProgramNo(const QString &programNoStr)
{
    QRegularExpression programNoRe("(\\d{3})");
    auto match = programNoRe.match(programNoStr);
    if (!match.hasMatch()) {
        return false;
    }
    auto ok = false;
    auto value = match.captured(1).toInt(&ok);
    if (ok) {
        setProgramNo(value);
    }
    return ok;
}
//------------------------------------------------------
QString NCPCommandIfGoSub::nameString() const
{
    return QString("%1%2").arg(m_resource->label(41, 534),
                               m_resource->label(41, 536));
}
//------------------------------------------------------
NCPCommand* NCPCommandIfGoSub::clone() const
{
    auto selfClone = new NCPCommandIfGoSub(m_resource, m_programNo);
    if (selfClone != nullptr) {
        for (auto& var : m_variableList) {
            auto newVar = new NCPVariable(var->relation(),
                                          var->symbol(),
                                          var->symbol1(),
                                          var->opeStr(),
                                          var->value(),
                                          var->ioAddress()
                                          );
            selfClone->append(newVar);
        }
    }
    return selfClone;
}
//------------------------------------------------------
QString NCPCommandIfGoSub::toAdditionalString() const
{
    return QString("#%1%2 → ").arg(m_programNo, 3, 10, QChar('0'))
                              .arg(m_resource->label(41, 37 + (m_programNo - 1)));
}
//------------------------------------------------------
QString NCPCommandIfGoSub::toAdditionalCode() const
{
    return QString("GOSUB #%1").arg(m_programNo, 3, 10, QChar('0'));
}
