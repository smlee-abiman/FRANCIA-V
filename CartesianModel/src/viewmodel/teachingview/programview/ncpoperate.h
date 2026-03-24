#ifndef NCPOPERATE_H
#define NCPOPERATE_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QLoggingCategory>
#include <QTimer>
#include <QDateTime>
#include <QVector>
#include <QDebug>
#include <QTextCodec>
#include <algorithm>
#include "../../../appenvironment.h"

Q_DECLARE_LOGGING_CATEGORY(ncpoperate)


class NcpOperate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int autoProgNum READ autoProgNum CONSTANT)
    Q_PROPERTY(int manualProgNum READ manualProgNum CONSTANT)
    Q_PROPERTY(int systemProgNum READ systemProgNum CONSTANT)
    Q_PROPERTY(QList<int> autoprogIndex READ autoprogIndex CONSTANT)
    Q_PROPERTY(QList<int> manualprogIndex READ manualprogIndex CONSTANT)
    Q_PROPERTY(QList<int> systemprogIndex READ systemprogIndex CONSTANT)
    Q_PROPERTY(int currentProgIndex READ currentProgIndex WRITE setCurrentProgIndex NOTIFY currentProgIndexChanged)
    Q_PROPERTY(int currentProgrows READ currentProgrows WRITE setCurrentProgrows NOTIFY currentProgrowsChanged)
    Q_PROPERTY(int progviewPage READ progviewPage WRITE setProgviewPage NOTIFY progviewPageChanged)
public:
    explicit NcpOperate(QObject *parent = nullptr);

    //接口
    int autoProgNum() const {return m_autoProgNum;}
    int manualProgNum() const {return m_manualProgNum;}
    int systemProgNum() const {return m_systemProgNum;}
    QList<int> autoprogIndex() {return m_autoprogIndex;}
    QList<int> manualprogIndex() {return m_manualprogIndex;}
    QList<int> systemprogIndex() {return m_systemprogIndex;}
    int currentProgIndex() {return m_currentProgIndex;}
    int currentProgrows() {return m_currentProgrows;}
    int progviewPage() {return m_progviewPage;}

    //函数
    Q_INVOKABLE QList<int> getProgIndex(){
        return m_progIndex;
    }//程序号存放

    QJsonArray RowArray(const QString &prog, int row) const;//返回一个prog中指定一行的数据
    Q_INVOKABLE QString stringValue(const QString &prog, int row, int column);//返回指定prog中指定行与列的数据

    void ProgInit();
    void setAutoProgNum();
    void setManualProgNum();
    void setSystemProgNum();
    Q_INVOKABLE void setCurrentProg(int index){
        setCurrentProgIndex(index);
        setCurrentProgArray();
    }
    Q_INVOKABLE void setCurrentProgArray(){
        QString s;
        if(m_currentProgIndex>0&&m_currentProgIndex<10){
            s = "#00" + QString::number(m_currentProgIndex);
        }else if(m_currentProgIndex>=10&&m_currentProgIndex<100) {
            s = "#0" + QString::number(m_currentProgIndex);
        }else {
            s = "#" + QString::number(m_currentProgIndex);
        }
        m_currentProgArray = m_jsonObject.value(s).toArray();
        setCurrentProgrows(m_currentProgArray.size());
    }
    Q_INVOKABLE QString getCurrentProgString(int row,int column)
    {
        return m_currentProgArray.at(row).toArray().at(column).toString();
    }//获取某一行某一列的字符串
    //获取指令程序列字符串
    Q_INVOKABLE QString getCurrentProgString_1(int row){
        QString xx = getCurrentProgString(row,1);
        if(xx == "OUT"){
            return "内部输出:";
        }else if (xx == "OUTP") {
            return "外部输出:";
        }else if(xx == "IF"){
            return "条件跳转:";
        }else if(xx == "IN") {
            return "条件等待:";
        }else if(xx == "GOTO"){
            return "跳转(Jump):";
        }else if(xx == "GOSUB"){
            return "程序调用:";
        }else if(xx == "MOVE"){
            return "轴移动:";
        }else if(xx =="WAIT"){
            return "计时:";
        }else if(xx =="RET"){
            return "返回";
        }else{
            return xx;
        }
    }

signals:
    void currentProgIndexChanged(int currentProgIndex);
    void currentProgrowsChanged(int currentProgrows);
    void progviewPageChanged(int progviewPage);
public slots:
    void setCurrentProgIndex(int currentProgIndex)
    {
        if(m_currentProgIndex == currentProgIndex)
            return;
        m_currentProgIndex = currentProgIndex;
        emit currentProgIndexChanged(currentProgIndex);
    }
    void setCurrentProgrows(int currentProgrows)
    {
        if(m_currentProgrows == currentProgrows)
            return;
        m_currentProgrows = currentProgrows;
        emit currentProgrowsChanged(currentProgrows);
    }
    void setProgviewPage(int progviewPage)
    {
        if(m_progviewPage == progviewPage)
            return;
        m_progviewPage = progviewPage;
        emit progviewPageChanged(progviewPage);
    }

private:
    QJsonObject m_jsonObject;
    QList<int> m_progIndex;
    QList<int> m_autoprogIndex;
    QList<int> m_manualprogIndex;
    QList<int> m_systemprogIndex;
    int m_autoProgNum = 0;
    int m_manualProgNum = 0;
    int m_systemProgNum = 0;
    int m_currentProgIndex = 101;
    QJsonArray m_currentProgArray;
    int m_currentProgrows;
    int m_progviewPage = 0;

    void parse(QString &data,const QString &delimiter);
    void parseProg();
};

#endif // NCPOPERATE_H
