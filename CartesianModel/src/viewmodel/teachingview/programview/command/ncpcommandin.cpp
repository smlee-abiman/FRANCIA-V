#include "ncpcommandin.h"
#include <QRegularExpression>
#include <QString>

//------------------------------------------------------
// NCPCommandIn.
//------------------------------------------------------
NCPCommandIn::NCPCommandIn(QObject *parent)
    : NCPCommand(parent)
{
}
//------------------------------------------------------
NCPCommandIn::NCPCommandIn(ResourceManager *resource, QObject *parent)
    : NCPCommand(resource, parent)
{
}
//------------------------------------------------------
NCPCommandIn::~NCPCommandIn()
{
    qDeleteAll(m_variableList);
    m_variableList.clear();
}
//------------------------------------------------------
bool NCPCommandIn::parse(const QString& constCommandLine)
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
    if (splitList.size() < 2) {
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
    for (int i = 1; i < loop; i++) {    // [0] = "IN", so skip it.
        auto word = splitList[i];
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
QString NCPCommandIn::nameString() const
{
    return m_resource->label(41, 23, m_resource->language());
}
//------------------------------------------------------
QString NCPCommandIn::toString() const
{
    auto additionalString = toAdditionalString();
    auto variablesString = toVariablesString();
    return QString("%1%2").arg(additionalString, variablesString);
}
//------------------------------------------------------
QString NCPCommandIn::toVariablesString() const
{
    auto string = QString("");
    auto loop = m_variableList.size();
    for (int i = 0; i < loop; i++) {
        auto& var = m_variableList.at(i);
        auto relation = (i==0) ? "" : var->relation();
        auto ioAddress = var->ioAddress();
        if (ioAddress < 0) {
            ioAddress = m_resource->indexAt(var->symbol());
        }
        QString symbolName("(");
        if (0 <= ioAddress) {
            auto name = m_resource->getMessageStringValue("1", ioAddress, m_resource->language());
            symbolName.append(name);
        }
        symbolName.append(")");
        auto opeStr = (var->opeStr() == "#") ? QString("≠") : var->opeStr();

        string.append(relation);
        string.append(var->symbol());
        string.append(symbolName);
        string.append(opeStr);

        if(var->value() == -1){
          QString symbol1Name("(");
//          auto ioAddress1 = var->ioAddress();
//          if (ioAddress1 < 0) {
//              ioAddress1 = m_resource->indexAt(var->symbol1());
//          }
//          if (0 <= ioAddress1) {
//              auto name1 = m_resource->getMessageStringValue("1", ioAddress1, m_resource->language());
//              symbolName.append(name1);
//          }
          symbolName.append(")");//TODO
            string.append(var->symbol1());
            string.append(symbol1Name);
        }else {
            QString valueStr("");
            if (var->opeStr() == "") {
                valueStr = (var->value() == 0) ? "OFF" : "ON";
            } else {
                valueStr = QString("%1").arg(var->value());
            }
            string.append(valueStr);
        }
    }
    return string.trimmed();
}
//------------------------------------------------------
QString NCPCommandIn::toCode() const
{
    auto variableCodes = toVariablesCode();
    auto additionalCode = toAdditionalCode();
    return QString("%1\t%2 %3").arg(command(), variableCodes, additionalCode).trimmed();
}
//------------------------------------------------------
NCPCommand* NCPCommandIn::clone() const
{
    auto selfClone = new NCPCommandIn(m_resource);
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
void NCPCommandIn::append(NCPVariable* var)
{
    m_variableList.append(var);
}
//------------------------------------------------------
NCPVariable* NCPCommandIn::variableAt(int index) const
{
    if ((index < 0) || (m_variableList.size() <= index)) {
        return nullptr;
    }
    return m_variableList.at(index);
}
//------------------------------------------------------
QString NCPCommandIn::toVariablesCode() const
{
    QString code("");
    auto loop = m_variableList.size();
    for (int i = 0; i < loop; i++) {
        auto& var = m_variableList.at(i);

        if (i != 0) {
            code.append(var->relation());
        }
        if (var->opeStr() == "") {
            // value is logical(ON/OFF).
            auto valueStr = (var->value() == 0) ? "OFF" : "ON";
            auto varString = QString("%1 %2").arg(var->symbol(), valueStr);
            code.append(varString);
        } else if(var->value() == -1){
            // value is a number.
            auto varString = QString("%1%2%3")
                                .arg(var->symbol(), var->opeStr(), var->symbol1());
            code.append(varString);
        }else {
            // value is a number.
            auto varString = QString("%1%2%3")
                    .arg(var->symbol(), var->opeStr())
                    .arg(var->value(), 3, 10, QChar('0'));
            code.append(varString);
        }

//        else {
//            // value is a number.
//            auto varString = QString("%1%2%3")
//                    .arg(var->symbol(), var->opeStr())
//                    .arg(var->value(), 3, 10, QChar('0'));
//            code.append(varString);
//        }
    }
    return code.trimmed();
}
