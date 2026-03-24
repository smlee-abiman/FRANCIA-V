#ifndef PROGRAMMENU4_3_3_H
#define PROGRAMMENU4_3_3_H

#include "programmenu.h"

//------------------------------------------------------
// ProgramMenu4_3_3
//------------------------------------------------------
class ProgramMenu4_3_3 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(int selectedCountType3Addr MEMBER m_selectedCountType3Addr WRITE setSelectedCountType3Addr NOTIFY selectedCountType3AddrChanged)
    Q_PROPERTY(QList<int> countType3Addr READ countType3Addr CONSTANT)

public:
    explicit ProgramMenu4_3_3(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_3_3() override;

    // gettter/setter.
    QList<int> countType3Addr() const { return m_countType3Addr; }
    int selectedCountType3Addr() const { return m_selectedCountType3Addr; }
    void setSelectedCountType3Addr(int selectedCountType3Addr);

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

signals:
    void selectedCountType3AddrChanged(int);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    void readPenSetData();

private:
    int m_selectedCountType3Addr;
    QList<int> m_countType3Addr;
};

#endif // PROGRAMMENU4_3_3_H
