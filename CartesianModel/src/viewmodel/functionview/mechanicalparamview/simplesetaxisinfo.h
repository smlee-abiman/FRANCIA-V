#ifndef SIMPLESETAXISINFO_H
#define SIMPLESETAXISINFO_H

#include <QObject>
#include <QDebug>
#include "resourcemanager.h"

class SimpleSetAxisData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int page1Mode READ page1Mode WRITE setPage1Mode NOTIFY page1ModeChanged)
    Q_PROPERTY(QString page1SetValue1 READ page1SetValue1 WRITE setPage1SetValue1 NOTIFY page1SetValue1Changed)
    Q_PROPERTY(QString page1SetValue2 READ page1SetValue2 WRITE setPage1SetValue2 NOTIFY page1SetValue2Changed)
    Q_PROPERTY(QString page1SetValue3 READ page1SetValue3 WRITE setPage1SetValue3 NOTIFY page1SetValue3Changed)
    Q_PROPERTY(QString page1DefValue1 READ page1DefValue1 WRITE setPage1DefValue1 NOTIFY page1DefValue1Changed)
    Q_PROPERTY(QString page1DefValue2 READ page1DefValue2 WRITE setPage1DefValue2 NOTIFY page1DefValue2Changed)
    Q_PROPERTY(QString page1DefValue3 READ page1DefValue3 WRITE setPage1DefValue3 NOTIFY page1DefValue3Changed)
    Q_PROPERTY(QList<QString> page1Items1 READ page1Items1 WRITE setPage1Items1 NOTIFY page1Items1Changed)
    Q_PROPERTY(int page2Mode READ page2Mode WRITE setPage2Mode NOTIFY page2ModeChanged)
    Q_PROPERTY(QString page2AnyValue1 READ page2AnyValue1 WRITE setPage2AnyValue1 NOTIFY page2AnyValue1Changed)
    Q_PROPERTY(QString page2AnyValue2 READ page2AnyValue2 WRITE setPage2AnyValue2 NOTIFY page2AnyValue2Changed)
    Q_PROPERTY(QString page2AnyValue3 READ page2AnyValue3 WRITE setPage2AnyValue3 NOTIFY page2AnyValue3Changed)
    Q_PROPERTY(QString page2AnyValue4 READ page2AnyValue4 WRITE setPage2AnyValue4 NOTIFY page2AnyValue4Changed)
    Q_PROPERTY(QString page2AnyValue5 READ page2AnyValue5 WRITE setPage2AnyValue5 NOTIFY page2AnyValue5Changed)
    Q_PROPERTY(QString page2AnyValue6 READ page2AnyValue6 WRITE setPage2AnyValue6 NOTIFY page2AnyValue6Changed)
    Q_PROPERTY(int page3Mode READ page3Mode WRITE setPage3Mode NOTIFY page3ModeChanged)
    Q_PROPERTY(QString page3SetValue1 READ page3SetValue1 WRITE setPage3SetValue1 NOTIFY page3SetValue1Changed)
    Q_PROPERTY(QString page3SetValue2 READ page3SetValue2 WRITE setPage3SetValue2 NOTIFY page3SetValue2Changed)
    Q_PROPERTY(QString page3SetValue3 READ page3SetValue3 WRITE setPage3SetValue3 NOTIFY page3SetValue3Changed)
    Q_PROPERTY(QString page3DefValue1 READ page3DefValue1 WRITE setPage3DefValue1 NOTIFY page3DefValue1Changed)
    Q_PROPERTY(QString page3DefValue2 READ page3DefValue2 WRITE setPage3DefValue2 NOTIFY page3DefValue2Changed)
    Q_PROPERTY(QString page3DefValue3 READ page3DefValue3 WRITE setPage3DefValue3 NOTIFY page3DefValue3Changed)
    Q_PROPERTY(QString page4AnyValue1 READ page4AnyValue1 WRITE setPage4AnyValue1 NOTIFY page4AnyValue1Changed)
    Q_PROPERTY(QString page4SetValue1 READ page4SetValue1 WRITE setPage4SetValue1 NOTIFY page4SetValue1Changed)
    Q_PROPERTY(QString page4SetValue2 READ page4SetValue2 WRITE setPage4SetValue2 NOTIFY page4SetValue2Changed)
    Q_PROPERTY(QString page4DefValue1 READ page4DefValue1 WRITE setPage4DefValue1 NOTIFY page4DefValue1Changed)
    Q_PROPERTY(QString page4DefValue2 READ page4DefValue2 WRITE setPage4DefValue2 NOTIFY page4DefValue2Changed)

public:
    explicit SimpleSetAxisData(QObject *parent = nullptr);

    bool operator == (SimpleSetAxisData &dat);
    SimpleSetAxisData &operator =(SimpleSetAxisData &dat);
    SimpleSetAxisData &operator =(SimpleSetAxisData dat);

    int page1Mode() const { return m_page1Mode;}
    QString page1SetValue1() const { return m_page1SetValue1;}
    QString page1SetValue2() const { return m_page1SetValue2;}
    QString page1SetValue3() const { return m_page1SetValue3;}
    QString page1DefValue1() const { return m_page1DefValue1;}
    QString page1DefValue2() const { return m_page1DefValue2;}
    QString page1DefValue3() const { return m_page1DefValue3;}
    QList<QString> page1Items1() const { return m_page1Items1;}
    int page2Mode() const { return m_page2Mode;}
    QString page2AnyValue1() const { return m_page2AnyValue1;}
    QString page2AnyValue2() const { return m_page2AnyValue2;}
    QString page2AnyValue3() const { return m_page2AnyValue3;}
    QString page2AnyValue4() const { return m_page2AnyValue4;}
    QString page2AnyValue5() const { return m_page2AnyValue5;}
    QString page2AnyValue6() const { return m_page2AnyValue6;}
    int page3Mode() const { return m_page3Mode;}
    QString page3SetValue1() const { return m_page3SetValue1;}
    QString page3SetValue2() const { return m_page3SetValue2;}
    QString page3SetValue3() const { return m_page3SetValue3;}
    QString page3DefValue1() const { return m_page3DefValue1;}
    QString page3DefValue2() const { return m_page3DefValue2;}
    QString page3DefValue3() const { return m_page3DefValue3;}
    QString page4AnyValue1() const { return m_page4AnyValue1;}
    QString page4SetValue1() const { return m_page4SetValue1;}
    QString page4SetValue2() const { return m_page4SetValue2;}
    QString page4DefValue1() const { return m_page4DefValue1;}
    QString page4DefValue2() const { return m_page4DefValue2;}

private:
    int m_page1Mode = 0;
    QString m_page1SetValue1 = "";
    QString m_page1SetValue2 = "";
    QString m_page1SetValue3 = "";
    QString m_page1DefValue1 = "";
    QString m_page1DefValue2 = "";
    QString m_page1DefValue3 = "";
    QList<QString> m_page1Items1;
    int m_page2Mode = 0;
    QString m_page2AnyValue1 = "";
    QString m_page2AnyValue2 = "";
    QString m_page2AnyValue3 = "";
    QString m_page2AnyValue4 = "";
    QString m_page2AnyValue5 = "";
    QString m_page2AnyValue6 = "";
    int m_page3Mode = 0;
    QString m_page3SetValue1 = "";
    QString m_page3SetValue2 = "";
    QString m_page3SetValue3 = "";
    QString m_page3DefValue1 = "";
    QString m_page3DefValue2 = "";
    QString m_page3DefValue3 = "";
    QString m_page4AnyValue1 = "";
    QString m_page4SetValue1 = "";
    QString m_page4SetValue2 = "";
    QString m_page4DefValue1 = "";
    QString m_page4DefValue2 = "";

public slots:
    void setPage1Mode(int page1Mode)
    {
        if (m_page1Mode == page1Mode)
            return;

        m_page1Mode = page1Mode;
        emit page1ModeChanged(m_page1Mode);
    }
    void setPage1SetValue1(QString page1SetValue1)
    {
        if (m_page1SetValue1 == page1SetValue1)
            return;

        m_page1SetValue1 = page1SetValue1;
        emit page1SetValue1Changed(m_page1SetValue1);
    }
    void setPage1SetValue2(QString page1SetValue2)
    {
        if (m_page1SetValue2 == page1SetValue2)
            return;

        m_page1SetValue2 = page1SetValue2;
        emit page1SetValue2Changed(m_page1SetValue2);
    }
    void setPage1SetValue3(QString page1SetValue3)
    {
        if (m_page1SetValue3 == page1SetValue3)
            return;

        m_page1SetValue3 = page1SetValue3;
        emit page1SetValue3Changed(m_page1SetValue3);
    }
    void setPage1DefValue1(QString page1DefValue1)
    {
        if (m_page1DefValue1 == page1DefValue1)
            return;

        m_page1DefValue1 = page1DefValue1;
        emit page1DefValue1Changed(m_page1DefValue1);
    }
    void setPage1DefValue2(QString page1DefValue2)
    {
        if (m_page1DefValue2 == page1DefValue2)
            return;

        m_page1DefValue2 = page1DefValue2;
        emit page1DefValue2Changed(m_page1DefValue2);
    }
    void setPage1DefValue3(QString page1DefValue3)
    {
        if (m_page1DefValue3 == page1DefValue3)
            return;

        m_page1DefValue3 = page1DefValue3;
        emit page1DefValue3Changed(m_page1DefValue3);
    }
    void setPage1Items1(QList<QString> page1Items1)
    {
        if (m_page1Items1 == page1Items1)
            return;

        m_page1Items1 = page1Items1;
        emit page1Items1Changed(m_page1Items1);
    }
    void setPage2Mode(int page2Mode)
    {
        if (m_page2Mode == page2Mode)
            return;

        m_page2Mode = page2Mode;
        emit page2ModeChanged(m_page2Mode);
    }
    void setPage2AnyValue1(QString page2AnyValue1)
    {
        if (m_page2AnyValue1 == page2AnyValue1)
            return;

        m_page2AnyValue1 = page2AnyValue1;
        emit page2AnyValue1Changed(m_page2AnyValue1);
    }
    void setPage2AnyValue2(QString page2AnyValue2)
    {
        if (m_page2AnyValue2 == page2AnyValue2)
            return;

        m_page2AnyValue2 = page2AnyValue2;
        emit page2AnyValue2Changed(m_page2AnyValue2);
    }
    void setPage2AnyValue3(QString page2AnyValue3)
    {
        if (m_page2AnyValue3 == page2AnyValue3)
            return;

        m_page2AnyValue3 = page2AnyValue3;
        emit page2AnyValue3Changed(m_page2AnyValue3);
    }
    void setPage2AnyValue4(QString page2AnyValue4)
    {
        if (m_page2AnyValue4 == page2AnyValue4)
            return;

        m_page2AnyValue4 = page2AnyValue4;
        emit page2AnyValue4Changed(m_page2AnyValue4);
    }
    void setPage2AnyValue5(QString page2AnyValue5)
    {
        if (m_page2AnyValue5 == page2AnyValue5)
            return;

        m_page2AnyValue5 = page2AnyValue5;
        emit page2AnyValue5Changed(m_page2AnyValue5);
    }
    void setPage2AnyValue6(QString page2AnyValue6)
    {
        if (m_page2AnyValue6 == page2AnyValue6)
            return;

        m_page2AnyValue6 = page2AnyValue6;
        emit page2AnyValue6Changed(m_page2AnyValue6);
    }
    void setPage3Mode(int page3Mode)
    {
        if (m_page3Mode == page3Mode)
            return;

        m_page3Mode = page3Mode;
        emit page3ModeChanged(m_page3Mode);
    }
    void setPage3SetValue1(QString page3SetValue1)
    {
        if (m_page3SetValue1 == page3SetValue1)
            return;

        m_page3SetValue1 = page3SetValue1;
        emit page3SetValue1Changed(m_page3SetValue1);
    }
    void setPage3SetValue2(QString page3SetValue2)
    {
        if (m_page3SetValue2 == page3SetValue2)
            return;

        m_page3SetValue2 = page3SetValue2;
        emit page3SetValue2Changed(m_page3SetValue2);
    }
    void setPage3SetValue3(QString page3SetValue3)
    {
        if (m_page3SetValue3 == page3SetValue3)
            return;

        m_page3SetValue3 = page3SetValue3;
        emit page3SetValue3Changed(m_page3SetValue3);
    }
    void setPage3DefValue1(QString page3DefValue1)
    {
        if (m_page3DefValue1 == page3DefValue1)
            return;

        m_page3DefValue1 = page3DefValue1;
        emit page3DefValue1Changed(m_page3DefValue1);
    }
    void setPage3DefValue2(QString page3DefValue2)
    {
        if (m_page3DefValue2 == page3DefValue2)
            return;

        m_page3DefValue2 = page3DefValue2;
        emit page3DefValue2Changed(m_page3DefValue2);
    }
    void setPage3DefValue3(QString page3DefValue3)
    {
        if (m_page3DefValue3 == page3DefValue3)
            return;

        m_page3DefValue3 = page3DefValue3;
        emit page3DefValue3Changed(m_page3DefValue3);
    }
    void setPage4AnyValue1(QString page4AnyValue1)
    {
        if (m_page4AnyValue1 == page4AnyValue1)
            return;

        m_page4AnyValue1 = page4AnyValue1;
        emit page4AnyValue1Changed(m_page4AnyValue1);
    }
    void setPage4SetValue1(QString page4SetValue1)
    {
        if (m_page4SetValue1 == page4SetValue1)
            return;

        m_page4SetValue1 = page4SetValue1;
        emit page4SetValue1Changed(m_page4SetValue1);
    }
    void setPage4SetValue2(QString page4SetValue2)
    {
        if (m_page4SetValue2 == page4SetValue2)
            return;

        m_page4SetValue2 = page4SetValue2;
        emit page4SetValue2Changed(m_page4SetValue2);
    }
    void setPage4DefValue1(QString page4DefValue1)
    {
        if (m_page4DefValue1 == page4DefValue1)
            return;

        m_page4DefValue1 = page4DefValue1;
        emit page4DefValue1Changed(m_page4DefValue1);
    }
    void setPage4DefValue2(QString page4DefValue2)
    {
        if (m_page4DefValue2 == page4DefValue2)
            return;

        m_page4DefValue2 = page4DefValue2;
        emit page4DefValue2Changed(m_page4DefValue2);
    }
signals:
    void page1ModeChanged(int page1Mode);
    void page1SetValue1Changed(QString page1SetValue1);
    void page1SetValue2Changed(QString page1SetValue2);
    void page1SetValue3Changed(QString page1SetValue3);
    void page1DefValue1Changed(QString page1DefValue1);
    void page1DefValue2Changed(QString page1DefValue2);
    void page1DefValue3Changed(QString page1DefValue3);
    void page1Items1Changed(QList<QString> page1Items1);
    void page2ModeChanged(int page2Mode);
    void page2AnyValue1Changed(QString page2AnyValue1);
    void page2AnyValue2Changed(QString page2AnyValue2);
    void page2AnyValue3Changed(QString page2AnyValue3);
    void page2AnyValue4Changed(QString page2AnyValue4);
    void page2AnyValue5Changed(QString page2AnyValue5);
    void page2AnyValue6Changed(QString page2AnyValue6);
    void page3ModeChanged(int page3Mode);
    void page3SetValue1Changed(QString page3SetValue1);
    void page3SetValue2Changed(QString page3SetValue2);
    void page3SetValue3Changed(QString page3SetValue3);
    void page3DefValue1Changed(QString page3DefValue1);
    void page3DefValue2Changed(QString page3DefValue2);
    void page3DefValue3Changed(QString page3DefValue3);
    void page4AnyValue1Changed(QString page4AnyValue1);
    void page4SetValue1Changed(QString page4SetValue1);
    void page4SetValue2Changed(QString page4SetValue2);
    void page4DefValue1Changed(QString page4DefValue1);
    void page4DefValue2Changed(QString page4DefValue2);
};

class SimpleSetAxisInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> simpleSetAxisDataList READ simpleSetAxisDataList WRITE setSimpleSetAxisDataList NOTIFY simpleSetAxisDataListChanged)

public:
    enum SimpleSetAxis {
       AxisY = 0,
       AxisMX,
       AxisMZ,
       AxisSX,
       AxisSZ,
       Axis6,
       Axis7,
       Axis8,
       AxisNum
   };

    explicit SimpleSetAxisInfo(QObject *parent = nullptr);

    int setPage1Mode(int page1Mode);
    int setPage1SetValue1(SimpleSetAxis axis, const QString &page1SetValue1);
    int setPage1SetValue2(SimpleSetAxis axis, const QString &page1SetValue2);
    int setPage1SetValue3(SimpleSetAxis axis, const QString &page1SetValue3);
    int setPage1DefValue1(SimpleSetAxis axis, const QString &page1DefValue1);
    int setPage1DefValue2(SimpleSetAxis axis, const QString &page1DefValue2);
    int setPage1DefValue3(SimpleSetAxis axis, const QString &page1DefValue3);
    int setPage2Mode(SimpleSetAxis axis, int page2Mode);
    int setPage2AnyValue1(SimpleSetAxis axis, const QString &page2AnyValue1);
    int setPage2AnyValue2(SimpleSetAxis axis, const QString &page2AnyValue2);
    int setPage2AnyValue3(SimpleSetAxis axis, const QString &page2AnyValue3);
    int setPage2AnyValue4(SimpleSetAxis axis, const QString &page2AnyValue4);
    int setPage2AnyValue5(SimpleSetAxis axis, const QString &page2AnyValue5);
    int setPage2AnyValue6(SimpleSetAxis axis, const QString &page2AnyValue6);
    int setPage3Mode(SimpleSetAxis axis, int page3Mode);
    int setPage3SetValue1(SimpleSetAxis axis, const QString &page3SetValue1);
    int setPage3SetValue2(SimpleSetAxis axis, const QString &page3SetValue2);
    int setPage3SetValue3(SimpleSetAxis axis, const QString &page3SetValue3);
    int setPage3DefValue1(SimpleSetAxis axis, const QString &page3DefValue1);
    int setPage3DefValue2(SimpleSetAxis axis, const QString &page3DefValue2);
    int setPage3DefValue3(SimpleSetAxis axis, const QString &page3DefValue3);
    int setPage4AnyValue1(SimpleSetAxis axis, const QString &page4AnyValue1);
    int setPage4SetValue1(SimpleSetAxis axis, const QString &page4SetValue1);
    int setPage4SetValue2(SimpleSetAxis axis, const QString &page4SetValue2);
    int setPage4DefValue1(SimpleSetAxis axis, const QString &page4DefValue1);
    int setPage4DefValue2(SimpleSetAxis axis, const QString &page4DefValue2);

    int numAxis() const { return m_simpleSetAxisDataList.length(); }

    void setSimpleSetAxisDataList(QList<QObject *> simpleSetAxisDataList);
    QList<QObject *> simpleSetAxisDataList() const { return m_simpleSetAxisDataList; }

private:
    QList<QObject *>   m_simpleSetAxisDataList;

signals:
    void simpleSetAxisDataListChanged(QList<QObject *> simpleSetAxisDataList);
};

#endif // SIMPLESETAXISINFO_H
