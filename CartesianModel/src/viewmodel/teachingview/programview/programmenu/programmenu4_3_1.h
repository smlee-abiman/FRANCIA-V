#ifndef PROGRAMMENU4_3_1_H
#define PROGRAMMENU4_3_1_H

#include "programmenu.h"

//------------------------------------------------------
// ProgramMenu4_3_1
//------------------------------------------------------
class ProgramMenu4_3_1 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(int selectedCountType1Addr MEMBER m_selectedCountType1Addr WRITE setSelectedCountType1Addr NOTIFY selectedCountType1AddrChanged)
    Q_PROPERTY(QList<int> countType1Addr READ countType1Addr CONSTANT)
public:
    explicit ProgramMenu4_3_1(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_3_1() override;

    // gettter/setter.
    QList<int> countType1Addr() const { return m_countType1Addr; }
    int selectedCountType1Addr() const { return m_selectedCountType1Addr; }
    void setSelectedCountType1Addr(int selectedCountType1Addr);

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

signals:
    void selectedCountType1AddrChanged(int);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    void readPenSetData();

private:
    int m_selectedCountType1Addr;
    QList<int> m_countType1Addr;
};

#endif // PROGRAMMENU4_3_1_H
