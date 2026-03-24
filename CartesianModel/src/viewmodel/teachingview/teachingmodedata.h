#ifndef TEACHINGMODEDATA_H
#define TEACHINGMODEDATA_H

#include <QObject>
#include "label.h"

class TeachingModeData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Label *label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(Label *label1 READ label1 WRITE setLabel1 NOTIFY label1Changed)
    Q_PROPERTY(Label *description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(int keyNo READ keyNo WRITE setKeyNo NOTIFY keyNoChanged)
    Q_PROPERTY(int di READ di CONSTANT)
    Q_PROPERTY(int key_on READ key_on WRITE setKey_on NOTIFY key_onChanged)
    Q_PROPERTY(int key_off READ key_off WRITE setKey_off NOTIFY key_offChanged)

public:
    explicit TeachingModeData(QObject *parent = nullptr);

    void setLabelName(const QString &name);
    void setLabelData(const QString &name, const QString &color);
    void setLabel1Data(const QString &name, const QString &color);
    void setDescriptionName(const QString &name);
    void setDescriptionData(const QString &name, const QString &color);

    void setLabel(Label *label);
    void setLabel1(Label *label1);
    void setDescription(Label *label);
    void setStatus(int status);
    void setKeyNo(int no);
    void setDi(int di);
    void setKey_on(int key_on);
    void setKey_off(int key_off);

    Label *label() const { return m_label; }
    Label *label1() const { return m_label1; }
    Label *description() const { return m_description; }
    int status() const { return m_status; }
    int keyNo() const { return m_keyNo; }
    int di() const { return m_di; }
    int key_on() const { return m_key_on; }
    int key_off() const { return m_key_off; }

signals:
    void labelChanged(Label *label);
    void label1Changed(Label *label1);
    void descriptionChanged(Label *label);
    void statusChanged(int status);
    void keyNoChanged(int status);
    void key_onChanged(int key_on);
    void key_offChanged(int key_off);

private:
    Label           *m_label = nullptr;
    Label           *m_label1 = nullptr;
    Label           *m_description = nullptr;
    int             m_status = 0;
    int             m_keyNo = 0;
    int             m_di = 0;
    int             m_key_on;
    int             m_key_off;
};

#endif // TEACHINGMODEDATA_H
