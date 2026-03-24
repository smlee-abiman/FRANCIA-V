#ifndef NCPCOMMANDFACTORY_H
#define NCPCOMMANDFACTORY_H

#include <QObject>

class ResourceManager;
class NCPCommand;

class NCPCommandFactory
{
public:
    NCPCommandFactory(ResourceManager *resource);
    NCPCommand *createCommand(const QString &commandString, const QString &commandLine = "");

private:
    ResourceManager *m_resource;
};

#endif // NCPCOMMANDFACTORY_H
