#ifndef NCPARBITRARYPOINT_H
#define NCPARBITRARYPOINT_H

class NCPArbitraryPoint
{
public:
    explicit NCPArbitraryPoint(int no);

    int no() const { return m_no; }
    bool inUse() const { return m_inUse; }
    void setInUse(bool inUse) { m_inUse = inUse; }

private:
    int     m_no = 0;
    bool    m_inUse = false;
};

#endif // NCPARBITRARYPOINT_H
