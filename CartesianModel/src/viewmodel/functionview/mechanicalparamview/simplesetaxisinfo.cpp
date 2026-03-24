#include "simplesetaxisinfo.h"

SimpleSetAxisData::SimpleSetAxisData(QObject *parent) : QObject(parent)
{
}

bool SimpleSetAxisData::operator ==(SimpleSetAxisData &dat)
{
    (void)dat;
    return true;
}

SimpleSetAxisData &SimpleSetAxisData::operator =(SimpleSetAxisData &dat)
{
    (void)dat;
    return *this;
}

SimpleSetAxisData &SimpleSetAxisData::operator =(SimpleSetAxisData dat)
{
    (void)dat;
    return *this;
}


SimpleSetAxisInfo::SimpleSetAxisInfo(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < SimpleSetAxis::AxisNum; i++){
        m_simpleSetAxisDataList.append(new SimpleSetAxisData(this));
    }
}

int SimpleSetAxisInfo::setPage1Mode(int page1Mode)
{
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(0));
    data->setPage1Mode(page1Mode);
    return 0;
}

int SimpleSetAxisInfo::setPage1SetValue1(SimpleSetAxis axis, const QString &page1SetValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage1SetValue1(page1SetValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage1SetValue2(SimpleSetAxis axis, const QString &page1SetValue2)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage1SetValue2(page1SetValue2);
    return 0;
}

int SimpleSetAxisInfo::setPage1SetValue3(SimpleSetAxis axis, const QString &page1SetValue3)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage1SetValue3(page1SetValue3);
    return 0;
}

int SimpleSetAxisInfo::setPage1DefValue1(SimpleSetAxis axis, const QString &page1DefValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage1DefValue1(page1DefValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage1DefValue2(SimpleSetAxis axis, const QString &page1DefValue2)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage1DefValue2(page1DefValue2);
    return 0;
}

int SimpleSetAxisInfo::setPage1DefValue3(SimpleSetAxis axis, const QString &page1DefValue3)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage1DefValue3(page1DefValue3);
    return 0;
}

int SimpleSetAxisInfo::setPage2Mode(SimpleSetAxis axis, int page2Mode)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage2Mode(page2Mode);
    return 0;
}

int SimpleSetAxisInfo::setPage2AnyValue1(SimpleSetAxis axis, const QString &page2AnyValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage2AnyValue1(page2AnyValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage2AnyValue2(SimpleSetAxis axis, const QString &page2AnyValue2)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage2AnyValue2(page2AnyValue2);
    return 0;
}

int SimpleSetAxisInfo::setPage2AnyValue3(SimpleSetAxis axis, const QString &page2AnyValue3)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage2AnyValue3(page2AnyValue3);
    return 0;
}

int SimpleSetAxisInfo::setPage2AnyValue4(SimpleSetAxis axis, const QString &page2AnyValue4)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage2AnyValue4(page2AnyValue4);
    return 0;
}

int SimpleSetAxisInfo::setPage2AnyValue5(SimpleSetAxis axis, const QString &page2AnyValue5)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage2AnyValue5(page2AnyValue5);
    return 0;
}

int SimpleSetAxisInfo::setPage2AnyValue6(SimpleSetAxis axis, const QString &page2AnyValue6)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage2AnyValue6(page2AnyValue6);
    return 0;
}

int SimpleSetAxisInfo::setPage3Mode(SimpleSetAxis axis, int page3Mode)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage3Mode(page3Mode);
    return 0;
}

int SimpleSetAxisInfo::setPage3SetValue1(SimpleSetAxis axis, const QString &page3SetValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage3SetValue1(page3SetValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage3SetValue2(SimpleSetAxis axis, const QString &page3SetValue2)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage3SetValue2(page3SetValue2);
    return 0;
}

int SimpleSetAxisInfo::setPage3SetValue3(SimpleSetAxis axis, const QString &page3SetValue3)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage3SetValue3(page3SetValue3);
    return 0;
}

int SimpleSetAxisInfo::setPage3DefValue1(SimpleSetAxis axis, const QString &page3DefValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage3DefValue1(page3DefValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage3DefValue2(SimpleSetAxis axis, const QString &page3DefValue2)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage3DefValue2(page3DefValue2);
    return 0;
}

int SimpleSetAxisInfo::setPage3DefValue3(SimpleSetAxis axis, const QString &page3DefValue3)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage3DefValue3(page3DefValue3);
    return 0;
}

int SimpleSetAxisInfo::setPage4AnyValue1(SimpleSetAxis axis, const QString &page4AnyValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage4AnyValue1(page4AnyValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage4SetValue1(SimpleSetAxis axis, const QString &page4SetValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage4SetValue1(page4SetValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage4SetValue2(SimpleSetAxis axis, const QString &page4SetValue2)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage4SetValue2(page4SetValue2);
    return 0;
}

int SimpleSetAxisInfo::setPage4DefValue1(SimpleSetAxis axis, const QString &page4DefValue1)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage4DefValue1(page4DefValue1);
    return 0;
}

int SimpleSetAxisInfo::setPage4DefValue2(SimpleSetAxis axis, const QString &page4DefValue2)
{
    if (axis >= m_simpleSetAxisDataList.length())
        return -1;
    SimpleSetAxisData *data = static_cast<SimpleSetAxisData *>(m_simpleSetAxisDataList.at(axis));
    data->setPage4DefValue2(page4DefValue2);
    return 0;
}

void SimpleSetAxisInfo::setSimpleSetAxisDataList(QList<QObject *> simpleSetAxisDataList)
{
    if (m_simpleSetAxisDataList != simpleSetAxisDataList) {
        m_simpleSetAxisDataList = simpleSetAxisDataList;
        emit simpleSetAxisDataListChanged(m_simpleSetAxisDataList);
    }
}
