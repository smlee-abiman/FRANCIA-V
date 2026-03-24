#ifndef PROGRAMMENU4_3_2_H
#define PROGRAMMENU4_3_2_H

#include "programmenu.h"

//------------------------------------------------------
// ProgramMenu4_3_2
//------------------------------------------------------
class ProgramMenu4_3_2 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(int selectedCountType2Addr MEMBER m_selectedCountType2Addr WRITE setSelectedCountType2Addr NOTIFY selectedCountType2AddrChanged)
    Q_PROPERTY(QList<int> countType2Addr READ countType2Addr CONSTANT)

public:
    explicit ProgramMenu4_3_2(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_3_2() override;

    // gettter/setter.
    QList<int> countType2Addr() const { return m_countType2Addr; }
    int selectedCountType2Addr() const { return m_selectedCountType2Addr; }
    void setSelectedCountType2Addr(int selectedCountType2Addr);

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

signals:
    void selectedCountType2AddrChanged(int);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    void readPenSetData();

private:
    int m_selectedCountType2Addr;
    QList<int> m_countType2Addr;
};

#endif // PROGRAMMENU4_3_2_H
