#ifndef PROGRAMMENU1_4ITEM_H
#define PROGRAMMENU1_4ITEM_H

#include <QObject>

class ProgramMenu1_4Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool optional MEMBER m_optional NOTIFY optionalChanged)
    Q_PROPERTY(bool selectState MEMBER m_selectState NOTIFY selectStateChanged)
    Q_PROPERTY(QString mark MEMBER m_mark NOTIFY markChanged)
    Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged)
    Q_PROPERTY(int msgLabelIndex MEMBER m_msgLabelIndex NOTIFY msgLabelIndexChanged)
public:
    explicit ProgramMenu1_4Item(QObject *parent = nullptr);
    void init();
    void setOptional(bool option);
    bool optional() const { return m_optional; }
    void setSelectState(bool state);
    bool selectState() const { return m_selectState; }
    void setMark(const QString &mark);
    QString mark() const { return m_mark; }
    void setMsgLabelIndex(int index);
    void setValue(int value);
    int value() const { return m_value; }
signals:
    void optionalChanged();
    void selectStateChanged();
    void markChanged();
    void valueChanged();
    void msgLabelIndexChanged();
public slots:
private:
    bool        m_optional = false;
    bool        m_selectState = false;
    QString     m_mark = "";
    int         m_msgLabelIndex = -1;
    int         m_value = 0;
};

#endif // PROGRAMMENU1_4ITEM_H
