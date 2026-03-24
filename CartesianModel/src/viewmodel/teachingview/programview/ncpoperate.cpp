#include "ncpoperate.h"
#include <QStringList>

Q_LOGGING_CATEGORY(ncpoperate, "ncpoperate")

namespace{
QString detectLineEnding(const QString &data)
{
    int index = data.indexOf('\n');
    if (index <= 0) {
        return QStringLiteral("\n");
    } else if (data.at(index - 1) == '\r') {
        return QStringLiteral("\r\n");
    } else {
        return QStringLiteral("\n");
    }
}//检测每行行尾
QString readfile(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qCWarning(ncpoperate) << "read error file name:"<<filename<<"error info:"<<file.errorString();
        return{};
    }//读取文件只读

    QByteArray tst = file.read(2);
    file.seek(0);
    bool isUTF16 = (tst == QByteArrayLiteral("\xff\xfe"));
    if (isUTF16) {
        QByteArray ba = file.readAll();
        return QTextCodec::codecForMib(1015)->toUnicode(ba);
    } else {
        return QString::fromUtf8(file.readAll());
    }
}//读文件
}

NcpOperate::NcpOperate(QObject *parent) : QObject(parent)
{
    parseProg();
    ProgInit();
}

void NcpOperate::parse(QString &data,const QString &delimiter)
{
    QStringList datalist = data.split(detectLineEnding(data));//分成一行一行
    QJsonArray progRows;
    QString currentGroupIndex;//当前组索引
    for (const auto &line : datalist) {
        auto row = line.split(delimiter);
        if(row.size() >= 2 && row.at(1) == "PROG"){
            if(!currentGroupIndex.isNull()){
                m_jsonObject.insert(currentGroupIndex,progRows);
                QString dd = currentGroupIndex.remove("#");
                m_progIndex.append(dd.toInt());
                progRows = {};
            }

            QString currentGroupIndex1 = row.at(2);
            currentGroupIndex = currentGroupIndex1.remove(" ");
            continue;
        }
        if(currentGroupIndex.isNull())
            continue;
        progRows.append(QJsonArray::fromStringList(row));
    }

    if (!currentGroupIndex.isNull()) {
        m_jsonObject.insert(currentGroupIndex, progRows);
        QString qq = currentGroupIndex.remove("#");
        m_progIndex.append(qq.toInt());
    }
}//将NCP分成一个prog一个prog的

void NcpOperate::parseProg()
{
    QString data = readfile(AppEnvironment::getNcpFolderPath() + QStringLiteral("/VGR5STDA00.NCP")); // MODIFIED : change ncp file
    parse(data,"\t");
}

QJsonArray NcpOperate::RowArray(const QString &prog, int row) const
{
    return m_jsonObject.value(prog).toArray().at(row).toArray();
}

QString NcpOperate::stringValue(const QString &prog, int row, int column)
{
    return RowArray(prog,row).at(column).toString();
}

void NcpOperate::setAutoProgNum()
{
    for(int i = 0; i < m_progIndex.size(); i++) {
        if(m_progIndex.at(i) > 0 && m_progIndex.at(i) < 100){
            m_autoProgNum++;
            m_autoprogIndex.append(m_progIndex.at(i));
        }
    }
}

void NcpOperate::setManualProgNum()
{
    for(int i = 0; i < m_progIndex.size(); i++) {
        if(m_progIndex.at(i) >= 178 && m_progIndex.at(i) <= 249){
            m_manualProgNum++;
            m_manualprogIndex.append(m_progIndex.at(i));
        }
    }
}

void NcpOperate::setSystemProgNum()
{
    for(int i = 0; i < m_progIndex.size(); i++) {
        if((m_progIndex.at(i) >= 100 && m_progIndex.at(i) <= 177)||(m_progIndex.at(i)>=251 && m_progIndex.at(i)<=255)){
            m_systemProgNum++;
            m_systemprogIndex.append(m_progIndex.at(i));
        }
    }
}

void NcpOperate::ProgInit()
{
    setAutoProgNum();
    setManualProgNum();
    setSystemProgNum();
    setCurrentProgArray();
}



