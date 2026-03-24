#ifndef NCPCOMMANDOUT_H
#define NCPCOMMANDOUT_H

#include "../ncpcommand.h"

#define FIXTURE_COVERAGE_PROGRAM_NUMBER_LOWER       50
#define FIXTURE_COVERAGE_PROGRAM_NUMBER_HIGHER      99
//------------------------------------------------------
// NCPCommandOut.
//------------------------------------------------------
class NCPCommandOut : public NCPCommand
{
    Q_OBJECT
public:
    class Data {
    public:
        typedef enum {
            Int,
            Bool,
        } DataType;
        Data() {}
        Data(DataType type, const QString & symbolStr, int value) {
            m_type = type;
            m_symbolStr = symbolStr;
            m_value = value;
        }
        DataType type() const { return  m_type; }
        const QString &symbolStr() const { return  m_symbolStr; }
        int value() const { return  m_value; }
    private:
        DataType    m_type;
        QString     m_symbolStr = "";
        int         m_value = 0;
    };
    typedef enum {
        Open,
        Close,
        Detect,
        Wait,
        Other,
    } IoType;
    explicit NCPCommandOut(QObject *parent = nullptr);
    explicit NCPCommandOut(ResourceManager *resource, QObject *parent = nullptr);
    virtual QString command() const { return "OUT"; }

    // NCPCommand interface
    virtual int type() const override { return Type::Out; }
    virtual int wizardMenuIndex() const override { return WizardMenuIndex::Menu5; }
    bool parse(const QString& commandLine) override;
    virtual QString nameString() const override;
    QString toString() const override;
    QString toCode() const override;
    NCPCommand* clone() const override;
    QString toInternalOutputString() const;
    QString toFixtureString() const;
    QString toJigOpenCloseString() const;
    QString toJigString() const;
    bool isJigType() const;

    void append(Data& data);
    QList<Data> getDataList() { return m_outDataList; }
protected:
    QList<Data>      m_outDataList;
    int              mSeparatedCommaNum = 0;

private:
    void parseInternalOutputCmd(QStringList splitList);
};

#endif // NCPCOMMANDOUT_H
