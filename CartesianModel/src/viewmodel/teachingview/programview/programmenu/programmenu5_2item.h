#ifndef PROGRAMMENU5_2ITEM_H
#define PROGRAMMENU5_2ITEM_H

#include <QObject>

class ProgramMenu5_2Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool optional MEMBER m_optional NOTIFY optionalChanged)
    Q_PROPERTY(bool selectState MEMBER m_selectState NOTIFY selectStateChanged)
    Q_PROPERTY(QString mark MEMBER m_mark NOTIFY markChanged)
    Q_PROPERTY(QString opeStr MEMBER m_opeStr NOTIFY opeStrChanged)
    Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged)
    Q_PROPERTY(int varIndex MEMBER m_varIndex NOTIFY varIndexChanged)
    Q_PROPERTY(int msgLabelIndex MEMBER m_msgLabelIndex NOTIFY msgLabelIndexChanged)
public:
    explicit ProgramMenu5_2Item(QObject *parent = nullptr);
    void init();
    void setOptional(bool option);
    bool optional() const { return m_optional; }
    void setSelectState(bool state);
    bool selectState() const { return m_selectState; }
    void setMark(const QString &mark);
    QString mark() const { return m_mark; }
    QString opeStr() const { return m_opeStr; }
    QString programOpeStr() const { return (m_opeStr == "≠") ? "#" : m_opeStr; }
    void setOpeStr(const QString &constOpeStr);
    void setValue(int value);
    int value() const { return m_value; }
    void setVarIndex(int varIndex);
    int varIndex() const { return m_varIndex; }
    void setMsgLabelIndex(int index);
signals:
    void optionalChanged();
    void selectStateChanged();
    void markChanged();
    void opeStrChanged();
    void valueChanged();
    void varIndexChanged();
    void msgLabelIndexChanged();
public slots:
private:
    bool        m_optional = false;
    bool        m_selectState = false;
    QString     m_mark = "";
    int         m_msgLabelIndex = -1;
    QString     m_opeStr = "";
    int         m_value = 0;
    int         m_varIndex = 0;
};

#endif // PROGRAMMENU5_2ITEM_H
