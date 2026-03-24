#ifndef NCPCOMMANDGOSUB_H
#define NCPCOMMANDGOSUB_H

#include "../ncpcommand.h"

//------------------------------------------------------
// NCPCommandGoSub.
//------------------------------------------------------
class NCPCommandGoSub : public NCPCommand
{
    Q_OBJECT
    const int PROGRAM_STANDARD_PALLETIZE_START = 161;
    const int PROGRAM_STANDARD_PALLETIZE_END = 168;
    const int PROGRAM_FREE_PALLETIZE_START = 169;
    const int PROGRAM_FREE_PALLETIZE_END = 176;
    const int PROGRAM_JIG_START = 50;
    const int PROGRAM_JIG_END = 99;

public:
    explicit NCPCommandGoSub(QObject *parent = nullptr);
    explicit NCPCommandGoSub(ResourceManager *resource, QObject *parent = nullptr);
    explicit NCPCommandGoSub(ResourceManager *resource, int programNo, QObject *parent = nullptr);

    void setProgramNo(int no);
    int programNo() const { return m_programNo; }
    QString command() const { return "GOSUB"; }
    bool isStandardPalletizeProgram() const { return ((m_programNo >= PROGRAM_STANDARD_PALLETIZE_START) && (m_programNo <= PROGRAM_STANDARD_PALLETIZE_END)); }
    bool isFreePalletizeProgram() const { return ((m_programNo >= PROGRAM_FREE_PALLETIZE_START) && (m_programNo <= PROGRAM_FREE_PALLETIZE_END)); }
    bool isJigProgram() const { return ((m_programNo >= PROGRAM_JIG_START) && (m_programNo <= PROGRAM_JIG_END)); }
    int startStandardPalletizeProgramNo() const { return PROGRAM_STANDARD_PALLETIZE_START; }
    int startFreePalletizeProgramNo() const { return PROGRAM_FREE_PALLETIZE_START; }
    int pointNo() const;
    // NCPCommand interface
    virtual int type() const override { return Type::GoSub; }
    virtual int wizardMenuIndex() const override;
    bool parse(const QString& commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;

private:
    int             m_programNo = 0;
};

#endif // NCPCOMMANDGOSUB_H
