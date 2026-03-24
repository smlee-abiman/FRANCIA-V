#ifndef PROGRAMMENU4_1ITEM_H
#define PROGRAMMENU4_1ITEM_H

#include <QObject>

//------------------------------------------------------
// ProgramMenu4_1Item.
//------------------------------------------------------
class ProgramMenu4_1Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool optional MEMBER m_optional NOTIFY optionalChanged)
    Q_PROPERTY(bool selectState MEMBER m_selectState NOTIFY selectStateChanged)
    Q_PROPERTY(QString relation MEMBER m_relation NOTIFY relationChanged)
    Q_PROPERTY(QString symbol MEMBER m_symbol NOTIFY symbolChanged)
    Q_PROPERTY(QString symbol1 MEMBER m_symbol1 NOTIFY symbol1Changed)
    Q_PROPERTY(QString opeStr MEMBER m_opeStr NOTIFY opeStrChanged)
    Q_PROPERTY(int value MEMBER m_value NOTIFY valueChanged)
    Q_PROPERTY(int ioAddress MEMBER m_ioAddress NOTIFY ioAddressChanged)
public:
    explicit ProgramMenu4_1Item(QObject *parent = nullptr);
    void init();
    bool optional() const { return m_optional; }
    void setOptional(bool option);
    bool selectState() const { return m_selectState; }
    void setSelectState(bool state);
    QString relation() const { return m_relation; }
    QString programRelation() const { return (m_relation == "and") ? "," : m_relation; }
    void setRelation(const QString &constRelation);
    QString symbol() const { return m_symbol; }
    QString symbol1() const { return m_symbol1; }
    void setSymbol(const QString &symbol);
    void setSymbol1(const QString &symbol1);
    QString opeStr() const { return m_opeStr; }
    QString programOpeStr() const { return (m_opeStr == "≠") ? "#" : m_opeStr; }
    void setOpeStr(const QString &constOpeStr);
    int value() const { return m_value; }
    void setValue(int value);
    int ioAddress() const { return m_ioAddress; }
    void setIoAddress(int ioAddress);

signals:
    void optionalChanged();
    void selectStateChanged();
    void relationChanged();
    void symbolChanged();
    void symbol1Changed();
    void msgChanged();
    void opeStrChanged();
    void valueChanged();
    void ioAddressChanged();

private:
    bool        m_optional = false;
    bool        m_selectState = false;
    QString     m_relation = "and";
    QString     m_symbol = "";
    QString     m_symbol1 = "";
    QString     m_opeStr = "";
    int         m_value = 0;
    int         m_ioAddress = -1;
};

#endif // PROGRAMMENU4_1ITEM_H
