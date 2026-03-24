#ifndef NCPLABEL_H
#define NCPLABEL_H

#include <QObject>

//------------------------------------------------------
// NCPLabel.
//------------------------------------------------------
class NCPLabel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int no READ no CONSTANT)
    Q_PROPERTY(int programNo READ programNo CONSTANT)
public:
    explicit NCPLabel(int labelNo, QObject *parent = 0);

    // getter/setter.
    int no() const { return mNo; }
    bool inUse() const { return mProgramNo != 0; }
    int programNo() const { return mProgramNo; }
    void setProgramNo(int programNo) { mProgramNo = programNo; }

private:
    int mNo;
    int mProgramNo;
};

#endif // NCPLABEL_H
