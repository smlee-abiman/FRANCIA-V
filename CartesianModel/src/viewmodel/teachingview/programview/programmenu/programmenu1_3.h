#ifndef PROGRAMMENU1_3_H
#define PROGRAMMENU1_3_H

#include "programmenu.h"
#include "programmenu1_3item.h"

class NCPProgram;
class NCPStep;

class ProgramMenu1_3 : public ProgramMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> itemList MEMBER m_itemList NOTIFY itemListChanged)
    Q_PROPERTY(int open MEMBER m_open NOTIFY openChanged)
    Q_PROPERTY(bool singleChoice MEMBER m_singleChoice NOTIFY singleChoiceChanged)

public:
    explicit ProgramMenu1_3(ResourceManager *resource,CommunicationThread *comm);
    ~ProgramMenu1_3() override;
    Q_INVOKABLE void setJigSingleChoice();
    Q_INVOKABLE void setJigMultipleChoice();
    Q_INVOKABLE int setTime(int itemIndex, int timeIndex, double time);
    Q_INVOKABLE int getKeyNo(int itemIndex, int keyIndex);
    Q_INVOKABLE void modbusGetSignal();
    Q_INVOKABLE bool isSArmClampItem(int index);

    void initData() override;
    CreateMethod commandCreateMethod() override { return CreateMethod::ComplexNcpCommand; }
    int insertCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    int replaceCustomNcpCommand(NCPFile *file, NCPProgram *program, int lineIndex) override;
    void setOpen(int open);
    int open() const { return m_open; }
    void setSingleChoice(bool choice);
    bool singleChoice() const { return m_singleChoice; }
private:
    void init();
    void readDataFormatCsv();
    void readPenSetData();
    void readJigUse();
    void readKey();
    void readDiAddress();
    void readHrAddress();
    void onActivate() override;
    void onDeactivate() override;
    void modbusSaveData();
    void modbusSaveTime(quint16 hrAddress, int time);
    void modbusGetTime(quint16 hrAddress, quint16 size);
    void updateTime(quint16 hrAddress, quint16 size);
    void updateSignal();
    void resetData();
    ProgramMenu1_3Item *item(int index);
    NCPStep *createStep(NCPCommand *command);
    NCPCommand *createGoSubCommand(int programNo);
    NCPStep *createOutCommand1();
    NCPStep *createInCommand();
    NCPStep *createOutCommand2();
    int insertCommand(NCPProgram *program, int lineIndex, NCPCommand *command);
    int procCreateSingleChoiceCommand(NCPFile *file, NCPProgram *program, int lineIndex, bool insert);
    int procCreateMultipleChoiceCommand(NCPFile *file, NCPProgram *program, int lineIndex, bool insert);
    int procCreateMainCommand(NCPProgram *program, int lineIndex, int jigProgramNo, bool insert);
    NCPProgram *procCreateSubProgram(NCPFile *file, int programNo);

signals:
    void itemListChanged();
    void openChanged();
    void singleChoiceChanged();
public slots:
    void onOneShotReadFinished(stModBusCommunicationData command, CommunicationThread::OneShotReadModBusCaller oneShotReadCaller);
    void onUpdateListReq();

private:
    static const QList<QList<QString>>  m_jigStrList;
    static const QList<QList<QString>>  m_waitSignalStrList;
    QList<QObject *>                    m_itemList;
    int                                 m_open = 0;                 // 0: close, 1: open
    bool                                m_singleChoice = false;
};

#endif // PROGRAMMENU1_3_H
