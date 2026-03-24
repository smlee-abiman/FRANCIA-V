#ifndef PROGRAMMENU4_3_4_H
#define PROGRAMMENU4_3_4_H

#include "programmenu.h"

//------------------------------------------------------
// ProgramMenu4_3_4
//------------------------------------------------------
class ProgramMenu4_3_4 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(int selectedCountType4Addr MEMBER m_selectedCountType4Addr WRITE setSelectedCountType4Addr NOTIFY selectedCountType4AddrChanged)
    Q_PROPERTY(QList<int> countType4Addr READ countType4Addr CONSTANT)

public:
    explicit ProgramMenu4_3_4(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_3_4() override;

    // gettter/setter.
    QList<int> countType4Addr() const { return m_countType4Addr; }
    int selectedCountType4Addr() const { return m_selectedCountType4Addr; }
    void setSelectedCountType4Addr(int selectedCountType4Addr);

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

signals:
    void selectedCountType4AddrChanged(int);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    void readPenSetData();

private:
    int m_selectedCountType4Addr;
    QList<int> m_countType4Addr;
};

#endif // PROGRAMMENU4_3_4_H
