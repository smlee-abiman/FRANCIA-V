#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

#include "ncpstep.h"
#include "command/ncpcommandunknown.h"
#include "command/ncpcommandprog.h"
#include "command/ncpcommandgosub.h"
#include "command/ncpcommandon.h"
#include "command/ncpcommandout.h"
#include "ncpcommandfactory.h"

//------------------------------------------------------
// NCPStep.
//------------------------------------------------------
NCPStep::NCPStep(NCPCommand *command, QObject *parent)
    : QObject(parent)
    , mLabelNo(-1)
    , mCommand(command)
{

}
//------------------------------------------------------
NCPStep::NCPStep(QObject *parent)
    : QObject(parent)
    , mLabelNo(-1)
    , mCommand(nullptr)
{
}
//------------------------------------------------------
NCPStep::~NCPStep()
{
    mLabelNo = -1;
    if (mCommand) {
// qDebug() << "delete step," << mCommand->toCode();
        delete mCommand;
        mCommand = nullptr;
    }
}
//------------------------------------------------------
bool NCPStep::parse(NCPCommandFactory *factory, const QString& stepLine)
{
// qDebug() << "parse " << stepLine;
    QRegularExpression stepRe("(\\s+|\\t+)");
    auto splitList = stepLine.split(stepRe);

    // todo:
    if (0 < splitList.size()) {
        auto labelLine = splitList.at(0);
        parseLabel(labelLine);
    }
    if (stepLine.isEmpty() ||
        stepLine.simplified().isEmpty() ||
        splitList.size() < 2 ) {
        qInfo() << "Syntax error step:" << stepLine;
        mCommand = new NCPCommandUnknown();
    } else {
        auto commandString = splitList.at(1);
        auto commandLine = stepLine.mid(stepLine.indexOf(commandString)).trimmed();
        mCommand = factory->createCommand(commandString, commandLine);
        auto ret = mCommand->parse(commandLine);
        if (!ret) {
            qInfo() << "Syntax error step:" << stepLine;
        }
    }

    return true;    // todo: need the parsing results?
}
//------------------------------------------------------
bool NCPStep::parseLabel(const QString& labelLine)
{
// qDebug() << "parse " << labelLine;
    if (labelLine.isEmpty()) {
        return true;
    }
    QRegularExpression labelRe("L(\\d{3})");
    auto match = labelRe.match(labelLine);
    if (!match.hasMatch()) {
        qInfo() << "Syntax error label:" << labelLine;
        return false;
    }
    // qDebug() << "captured(0)" << match.captured(0);
    // qDebug() << "captured(1)" << match.captured(1);
    auto ok = false;
    auto value = match.captured(1).toInt(&ok);
    if (!ok) {
        qInfo() << "Syntax error label:" << labelLine;
        return false;
    }
    setLabelNo(value);

    return true;
}
//------------------------------------------------------
// memo: The caller needs to delete the command instance.
NCPCommand* NCPStep::parseCommand(NCPCommandFactory *factory, const QString& commandLine)
{
    QRegularExpression re("(\\s+|\\t+)");
    auto splitList = commandLine.split(re);
    auto commandString = splitList.at(0);
    NCPCommand* command = factory->createCommand(commandString, commandLine.simplified());
    command->parse(commandLine.simplified());
    return command;
}
//------------------------------------------------------
QString NCPStep::commandName() const
{
    return (!mCommand) ? "" : mCommand->nameString();
}
//------------------------------------------------------
QString NCPStep::toTranslatedString(int /*language*/) const
{
    return (!mCommand) ? "" : mCommand->toString();
}
//------------------------------------------------------
QString NCPStep::toCode() const
{
    QString labelLine   = (mLabelNo == -1) ? "" : QString("L%1").arg(mLabelNo, 3, 10, QChar('0'));
    QString commandLine = (!mCommand)      ? "" : mCommand->toCode();
    return QString("%1\t%2").arg(labelLine).arg(commandLine);
}
//------------------------------------------------------
// const bool copyLabelNo = false. (default)
NCPStep* NCPStep::clone(const bool copyLabelNo) const
{
    auto cloneStep = new NCPStep();
    if (copyLabelNo) {
        // note:
        // In normal duplication(e.g.:copy & paste), mLabelNo should not be copied. (Label No cannot be duplicated.)
        cloneStep->mLabelNo = mLabelNo;
    }
    NCPCommand* cloneCommand = nullptr;
    if (mCommand) {
        cloneCommand = mCommand->clone();
        if (!cloneCommand) {
            // FIXME:
            // todo: clone() method is not implemented !!!
            qWarning() << QString("%1(%2): clone command failed, type:").arg(__PRETTY_FUNCTION__).arg(__LINE__) << mCommand->type() << "code:" << mCommand->toCode();
            cloneCommand = new NCPCommandUnknown(mCommand->toCode());
        }
    }
    if (!cloneCommand) {
        cloneCommand = new NCPCommandUnknown();
    }
    cloneStep->setCommand(cloneCommand);
    return cloneStep;
}
