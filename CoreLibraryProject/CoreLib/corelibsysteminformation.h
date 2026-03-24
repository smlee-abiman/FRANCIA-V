#ifndef CORELIBSYSTEMINFORMATION_H
#define CORELIBSYSTEMINFORMATION_H

#include <QString>

class CorelibSystemInformation
{
public:
    CorelibSystemInformation();

    inline static QString getCoreAppVersion(){
        return m_coreAppVersion;
    }

private:
    const static QString m_coreAppVersion;
};

#endif // CORELIBSYSTEMINFORMATION_H
