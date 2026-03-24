#ifndef LABEL_H
#define LABEL_H

#include <QObject>

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

#endif // LABEL_H
