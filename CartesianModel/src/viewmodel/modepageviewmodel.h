#ifndef MODEPAGEVIEWMODEL_H
#define MODEPAGEVIEWMODEL_H

#include <QHash>
#include "viewmodelbase.h"

class Label : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
public:
    explicit Label(QObject *parent = nullptr);
    void setName(const QString &name);
    void setColor(const QString &color);
    QString name() const { return m_name; }
    QString color() const { return m_color; }

signals:
    void nameChanged(const QString &name);
    void colorChanged(const QString &color);
private:
    QString     m_name = "";
    QString     m_color = "";
};

class ModeData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Label *label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QList<Label *> choices READ choices WRITE setChoices NOTIFY choicesChanged)
    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)
public:
    explicit ModeData(QObject *parent = nullptr);
    explicit ModeData(int choiceNum, QObject *parent = nullptr);

    void setLabelName(const QString &name);
    void setLabelData(const QString &name, const QString &color);
    int setChoiceName(int index, const QString &name);
    int setChoiceData(int index, const QString &name, const QString &color);

    void setLabel(Label *label);
    void setChoices(const QList<Label *> &choices);
    void setStatus(int status);
    void setKeyNo(int no);
    void setDi(int di);

    Label *label() const { return m_label; }
    QList<Label *> choices() { return m_choices; }
    int status() const { return m_status; }
    int keyNo() const { return m_keyNo; }
    int di() const { return m_di; }

signals:
    void labelChanged(Label *label);
    void choicesChanged(const QList<Label *> &choices);
    void statusChanged(int status);

private:
    Label           *m_label = nullptr;
    QList<Label *>  m_choices;
    int             m_status = 0;
    int             m_keyNo = 0;
    int             m_di = 0;
};

class ModePageViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QList<ModeData *> currentModeList READ currentModeList WRITE setCurrentModeList NOTIFY currentModeListChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
public:
    const int PEN_MSG_GROUP_MODE_PAGE = 34;
    const int PEN_SET_START_ID = 1;
    const QString PEN_SET_GROUP_MODE_PAGE = "10";
    const int PEN_SET_START_ROW = 3;
    const int PEN_SET_COLUMN_KEY_NO = 1;
    const int PEN_SET_COLUMN_DI = 2;
    const int MODE_PAGE_MAX = 8;
    const int MODE_NUM_PER_PAGE = 8;
    const int MODE_NUM = MODE_PAGE_MAX * MODE_NUM_PER_PAGE;
    const int MODE_CHOICE_NUM = 2;
    const int MODE_DI_START = 1536;

    explicit ModePageViewModel(ResourceManager *resource, CommunicationThread * comm);
    ~ModePageViewModel();
    Q_INVOKABLE void activateView();
    Q_INVOKABLE void deactivateView();
    Q_INVOKABLE int setMode(int page, int index, int status);

    void setCurrentModeList(const QList<ModeData *>& list);
    void setCurrentPage(int page);
    QList<ModeData *> currentModeList() const { return m_currentModeList; }
    int currentPage() const { return m_currentPage; }

signals:
    void currentPageChanged(int page);
    void currentModeListChanged(const QList<ModeData *>& list);

public slots:
    void onFeed(stModBusCommunicationData command);
    void onLanguageChanged(int language);

private:
    void init();
    void initList();
    void readDataFormatCsv();
    void readPenMsgData();
    void readPenMsgNameData();
    void readPenSetData();
    void upateCurrentModeList();
    void receiveModeData(quint16 *readBuf);
private:
    QList<ModeData *>   m_currentModeList;
    QList<ModeData *>   m_modeDataList;
    QHash<int, int>     m_diModeIndexConversionMap;
    int                 m_currentPage = 0;
};

#endif // MODEPAGEVIEWMODEL_H
