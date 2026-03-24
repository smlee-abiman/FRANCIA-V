#include <QRegularExpression>
#include <QString>
#include "ncpcommandifgoto.h"

//------------------------------------------------------
// NCPCommandIfGoTo.
//------------------------------------------------------
NCPCommandIfGoTo::NCPCommandIfGoTo(QObject *parent)
    : NCPCommandIn(parent)
{
}
//------------------------------------------------------
NCPCommandIfGoTo::NCPCommandIfGoTo(ResourceManager *resource, QObject *parent)
    : NCPCommandIn(resource, parent)
{
}
//------------------------------------------------------
NCPCommandIfGoTo::NCPCommandIfGoTo(ResourceManager *resource, int labelNo, QObject *parent)
    : NCPCommandIn(resource, parent)
{
    setLabelNo(labelNo);
}
//------------------------------------------------------
void NCPCommandIfGoTo::setLabelNo(int labelNo)
{
    m_labelNo = labelNo;
}
//------------------------------------------------------
bool NCPCommandIfGoTo::parse(const QString &constCommandLine)
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
    if (splitList.size() < 5) {
        qDebug() << "Syntax error commandLine:[" << commandLine << "]";
        return false;
    }
    // parse program.
    auto relation = QString(","); // "and", "or"
    auto symbol = QString("");
    auto symbol1 = QString("");
    auto opeStr = QString("");    // "=", "#", "<", ">"
    auto value = -1;              // "ON", "OFF", number.    
    m_labelNo = 0;

    auto loop = splitList.size();
    for (int i = 1; i < loop; i++) {    // [0] = "IF", so skip it.
        auto word = splitList[i];
        if (word.compare("goto", Qt::CaseInsensitive) == 0) {
            // parse GOTO LXXX.
            auto noIndex = i + 1;
            if (m_labelNo != 0) { goto lError; }
            else if (loop <= noIndex) { goto lError; }
            auto ret = parseLabelNo(splitList[noIndex]);
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
bool NCPCommandIfGoTo::parseLabelNo(const QString &labelNoStr)
{
    QRegularExpression labelNoRe("L(\\d{3})");
    auto match = labelNoRe.match(labelNoStr);
    if (!match.hasMatch()) {
        return false;
    }
    auto ok = false;
    auto value = match.captured(1).toInt(&ok);
    if (ok) {
        setLabelNo(value);
    }
    return ok;
}
//------------------------------------------------------
QString NCPCommandIfGoTo::nameString() const
{
    return QString("%1%2").arg(m_resource->label(41, 534, m_resource->language()),
                               m_resource->label(41, 537, m_resource->language()));
}
//------------------------------------------------------
NCPCommand* NCPCommandIfGoTo::clone() const
{
    auto selfClone = new NCPCommandIfGoTo(m_resource, m_labelNo);
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
QString NCPCommandIfGoTo::toAdditionalString() const
{
    return QString("L%1 → ").arg(m_labelNo, 3, 10, QChar('0'));
}
//------------------------------------------------------
QString NCPCommandIfGoTo::toAdditionalCode() const
{
    return QString("GOTO L%1").arg(m_labelNo, 3, 10, QChar('0'));
}
