#ifndef PROGRAMMENU4_2_4_H
#define PROGRAMMENU4_2_4_H

#include "programmenu.h"

//------------------------------------------------------
// ProgramMenu4_2_4
//------------------------------------------------------
class ProgramMenu4_2_4 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(bool use MEMBER mUse WRITE setUse NOTIFY useChanged)
public:
    explicit ProgramMenu4_2_4(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu4_2_4() override;

    // gettter/setter.
    void setUse(bool use) {
        if (mUse == use) { return; }
        mUse = use;
        emit useChanged(mUse);
    }

    // ProgramMenu interface.
    virtual void show(NCPCommand *command) override;
    virtual NCPCommand *createNcpCommand() override;

signals:
    void useChanged(bool);

protected:
    // ViewModelBase interface
    virtual void onActivate() override;
    virtual void onDeactivate() override;

private:
    bool mUse;
};

#endif // PROGRAMMENU4_2_4_H
