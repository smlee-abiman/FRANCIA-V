#include "corelibmodbuscommunicationtest.h"
#include <QDebug>
#include <QEventLoop>

#ifdef CORELIB_UNIT_TEST
#else // CORELIB_UNIT_TEST
/* table */
static bool modBusComTestWriteData[2048] = {
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true,  true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true,  true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true,  true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true,  true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true,  true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true,  true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true,  true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, true, false, true, false, true, false, true, true,
    true, false, true, false, true, false, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, true, false, true, false, true, false, true, true, true,
    false, true, false, true, false, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, true, false, true, false, true, false, true, true, true, false,
    true, false, true, false, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, true, false, true, false, true, false, true, true, true, false, true,
    false, true, false, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, true, false, true, false, true, false, true, true, true, false, true, false,
    true, false, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, true, false, true, false, true, false, true, true, true, false, true, false, true,
    false, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, true, true, false, true, false, true, false, true, true, true, false, true, false, true, false,
    true, false, true, false, true, false, false, false, true, true, false, false, true, false, true, false,
};
static bool modBusComTest012WriteData[16] = {
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true
};

static bool modBusComTest013WriteData[16] = {
    true, false, false, true, false, true, true, false, true, false, false, true, false, true, true, false 
};

static quint16 modBusComTest014WriteData[8] = {
    0x5d5d, 0x5d5d, 0x5d5d, 0x5d5d, 0x5d5d, 0x5d5d, 0x5d5d, 0x5d5d
};

static bool modBusComTest046WriteData[120] = {
    true, false, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, false, false, true, false, true, false, true,
    true, false, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true
};
static quint16 modBusComTest059WriteData[160] = {
    0x0102, 0x0203, 0x0304, 0x0405, 0x0607, 0x0708, 0x0809, 0x090A, 0x0A0B, 0x0B0C, 0x0C0D, 0x0D0E, 0x0E0F, 0x0F0E, 0x0E0D, 0x0D0C,
    0x0C0B, 0x0B0A, 0x0A09, 0x0908, 0x0807, 0x0706, 0x0605, 0x0504, 0x0403, 0x0302, 0x0201, 0x0110, 0x1020, 0x2030, 0x3040, 0x4050,
    0x5060, 0x6070, 0x7080, 0x8090, 0x90A0, 0xA0B0, 0xB0C0, 0xC0D0, 0xD0E0, 0xE0F0, 0xF0E0, 0xE0D0, 0xD0C0, 0xC0B0, 0xB0A0, 0xA090,
    0x9080, 0x8070, 0x7060, 0x6050, 0x5040, 0x4030, 0x3020, 0x2010, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF, 0x9999, 0x8888,
    0x1030, 0x1031, 0x1032, 0x1033, 0x1034, 0x1035, 0x1036, 0x1037, 0x1038, 0x1039, 0x103A, 0x103B, 0x103C, 0x103D, 0x103E, 0x103F,
    0x2030, 0x2031, 0x2032, 0x2033, 0x2034, 0x2035, 0x2036, 0x2037, 0x2038, 0x2039, 0x203A, 0x203B, 0x203C, 0x203D, 0x203E, 0x203F,
    0x3030, 0x3031, 0x3032, 0x3033, 0x3034, 0x3035, 0x3036, 0x3037, 0x3038, 0x3039, 0x303A, 0x303B, 0x303C, 0x303D, 0x303E, 0x303F,
    0x4030, 0x4031, 0x4032, 0x4033, 0x4034, 0x4035, 0x4036, 0x4037, 0x4038, 0x4039, 0x403A, 0x403B, 0x403C, 0x403D, 0x403E, 0x403F,
    0x5030, 0x5031, 0x5032, 0x5033, 0x5034, 0x5035, 0x5036, 0x5037, 0x5038, 0x5039, 0x503A, 0x503B, 0x503C, 0x503D, 0x503E, 0x503F,
    0x6030, 0x6031, 0x6032, 0x6033, 0x6034, 0x6035, 0x6036, 0x6037, 0x6038, 0x6039, 0x603A, 0x603B, 0x603C, 0x603D, 0x603E, 0x603F
};

static bool modBusComTest060WriteData[128] = {
    true, false, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, false, false, true, false, true, false, true,
    true, false, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true,
    true, true, false, true, false, true, false, true, true, true, false, true, false, true, false, true
};
static quint16 modBusComTest062WriteData[120] = {
    0x0102, 0x0203, 0x0304, 0x0405, 0x0607, 0x0708, 0x0809, 0x090A, 0x0A0B, 0x0B0C, 0x0C0D, 0x0D0E, 0x0E0F, 0x0F0E, 0x0E0D, 0x0D0C,
    0x0C0B, 0x0B0A, 0x0A09, 0x0908, 0x0807, 0x0706, 0x0605, 0x0504, 0x0403, 0x0302, 0x0201, 0x0110, 0x1020, 0x2030, 0x3040, 0x4050,
    0x5060, 0x6070, 0x7080, 0x8090, 0x90A0, 0xA0B0, 0xB0C0, 0xC0D0, 0xD0E0, 0xE0F0, 0xF0E0, 0xE0D0, 0xD0C0, 0xC0B0, 0xB0A0, 0xA090,
    0x9080, 0x8070, 0x7060, 0x6050, 0x5040, 0x4030, 0x3020, 0x2010, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF, 0x9999, 0x8888,
    0x1030, 0x1031, 0x1032, 0x1033, 0x1034, 0x1035, 0x1036, 0x1037, 0x1038, 0x1039, 0x103A, 0x103B, 0x103C, 0x103D, 0x103E, 0x103F,
    0x2030, 0x2031, 0x2032, 0x2033, 0x2034, 0x2035, 0x2036, 0x2037, 0x2038, 0x2039, 0x203A, 0x203B, 0x203C, 0x203D, 0x203E, 0x203F,
    0x3030, 0x3031, 0x3032, 0x3033, 0x3034, 0x3035, 0x3036, 0x3037, 0x3038, 0x3039, 0x303A, 0x303B, 0x303C, 0x303D, 0x303E, 0x303F,
    0x4030, 0x4031, 0x4032, 0x4033, 0x4034, 0x4035, 0x4036, 0x4037
};
#endif // CORELIB_UNIT_TEST

CoreLibModBusCommunicationTestSub::CoreLibModBusCommunicationTestSub(QObject *parent)
    : QThread(parent),
      mModBusInitInfo{"unit_test", "127.0.0.1", 502, 1, 450, 1},
      modBusState(-1)
{
    mModBusCom = new CoreLibModBusCommunication(mModBusInitInfo);
}

CoreLibModBusCommunicationTestSub::~CoreLibModBusCommunicationTestSub()
{
    if (mModBusCom){
        delete mModBusCom;
        mModBusCom = nullptr;
    }
}

void CoreLibModBusCommunicationTestSub::modBusConnectStatusChanged(int state)
{
    modBusState = state;
    emit modBusConnectStatusChangedNotify();
}

void CoreLibModBusCommunicationTestSub::modBusReadStatusChanged(int state)
{
    modBusState = state;
    emit modBusConnectStatusChangedNotify();
    disconnect(mModBusCom, SIGNAL(modBusComReadStatusChanged(int)),
        this, SLOT(modBusReadStatusChanged(int)));
}

int CoreLibModBusCommunicationTestSub::getModBusState()
{
    return modBusState;
}

void CoreLibModBusCommunicationTestSub::connetStatusChangedFunc()
{
    connect(mModBusCom, SIGNAL(modBusComConnectStatusChanged(int)),
        this, SLOT(modBusConnectStatusChanged(int)));
}

void CoreLibModBusCommunicationTestSub::connetStatusChangedEventLoop()
{
    QEventLoop loop;
    connect(this, &CoreLibModBusCommunicationTestSub::modBusConnectStatusChangedNotify, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(this, &CoreLibModBusCommunicationTestSub::modBusConnectStatusChangedNotify, &loop, &QEventLoop::quit);
}

bool CoreLibModBusCommunicationTestSub::connetReadStatusChangedFunc()
{
    bool ret = false;
    mIsBitMemory = mModBusCom->checkModBusBitMemory(mModBusReadComData);
    ret = mModBusCom->readModBusData(mModBusReadComData);
    if (!ret) {
        return ret;
    }
    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    qDebug("mIsBitMemory[%d]", mIsBitMemory);
//    if (isBitMemory){
//        connect(mModBusDataMgr, SIGNAL(modBusBitMemDataChanged(QBitArray)),
//            this, SLOT(modBusBitMemoryDataChanged(QBitArray)));
//    } else {
        connect(mModBusDataMgr, SIGNAL(modBusMemDataChanged(stModBusCommunicationData)),
            this, SLOT(modBusMemoryDataChanged(stModBusCommunicationData)));
//    }
    connect(mModBusCom, SIGNAL(modBusComReadStatusChanged(int)),
        this, SLOT(modBusReadStatusChanged(int)));

    return true;
}

bool CoreLibModBusCommunicationTestSub::connetReadWriteStatusChangedFunc()
{
    bool ret = false;
    ret = mModBusCom->readWriteModBusData(mModBusReadComData);
    if (!ret) {
        return ret;
    }
    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    connect(mModBusDataMgr, SIGNAL(modBusMemDataChanged(stModBusCommunicationData)),
        this, SLOT(modBusMemoryDataChanged(stModBusCommunicationData)));
    connect(mModBusCom, SIGNAL(modBusComReadStatusChanged(int)),
        this, SLOT(modBusReadStatusChanged(int)));
    return true;
}

void CoreLibModBusCommunicationTestSub::connetReadStatusChangedEventLoop()
{
    QEventLoop loop;
    connect(this, &CoreLibModBusCommunicationTestSub::modBusConnectStatusChangedNotify, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(this, &CoreLibModBusCommunicationTestSub::modBusConnectStatusChangedNotify, &loop, &QEventLoop::quit);
}

void CoreLibModBusCommunicationTestSub::modBusBitMemoryDataChanged(QBitArray bitMemoryData)
{
    int size = bitMemoryData.size();
    mModBusReadComData.bitReadBuf.resize(size);
    for (int i = 0; i < size; i++){
        mModBusReadComData.bitReadBuf[i] = bitMemoryData[i];
        //bool value = mModBusReadComData.bitReadBuf.at(i);
    }
    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    disconnect(mModBusDataMgr, SIGNAL(modBusBitMemDataChanged(QBitArray)),
        this, SLOT(modBusBitMemoryDataChanged(QBitArray)));
}
void CoreLibModBusCommunicationTestSub::modBusMemoryDataChanged(stModBusCommunicationData modBusMemData)
{
    int size = 0;
    if (mIsBitMemory) {
        size = (modBusMemData.comSize % 16 == 0 ? modBusMemData.comSize / 16 : modBusMemData.comSize / 16 + 1);
    } else {
        size = (int)modBusMemData.comSize;
    }
    //qDebug("size[%d] modBusMemData.comSize[%d]", size, modBusMemData.comSize);
    for (int i = 0; i < size; i++){
        getModBusReadComData()->readBuf[i] = modBusMemData.readBuf[i];
        //qDebug("readBuf[%d]=[0x%04x] getModBusReadComData()->readBuf[%d]=[0x%04x]", i, modBusMemData.readBuf[i], i, getModBusReadComData()->readBuf[i]);
    }
    CoreLibModBusDataManager *mModBusDataMgr = CoreLibModBusDataManager::get_instance();
    disconnect(mModBusDataMgr, SIGNAL(modBusMemDataChanged(stModBusCommunicationData)),
        this, SLOT(modBusMemoryDataChanged(stModBusCommunicationData)));
}
#ifdef CORELIB_UNIT_TEST
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest001</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>startModBusCommunication()</category2>
    <category3>MODBUS-Connect</category3>
    <case>
        1)QModbusTcpClient::connectDevice関数がfalseを返却するように設定しておく
        2)startModBusCommunication()を実行する
    </case>
    <check>
        1)MODBUSステータスがQModbusDevice::UnconnectedState(0)になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest001)
{
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_connect())
                    .Times(AtLeast(1))
                    .WillOnce(Return(false));
    testSub->getModBusCom()->startModBusCommunication();
    EXPECT_EQ(testSub->getModBusState(), -1);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest002</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncStarModBusCommunication()</category2>
    <category3>MODBUS-Connect</category3>
    <case>
        1)QModbusTcpClient::connectDevice関数がfalseを返却するように設定しておく
        2)syncStarModBusCommunication()を実行する
    </case>
    <check>
        1)MODBUSステータスがQModbusDevice::UnconnectedState(0)になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest002)
{
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_connect())
                    .Times(AtLeast(1))
                    .WillOnce(Return(false));
    testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(testSub->getModBusState(), -1);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest003</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Sync-Read-Request</category3>
    <case>
        1)QModbusTcpClient::connectDevice関数がtrueを返却するように設定しておく
        2)syncStartModBusCommunication()を実行する
        3)QModbusTcpClient::sendReadRequest関数がnullptrを返却するように設定しておく
        4)syncReadModBusData()を実行する
    </case>
    <check>
        1)syncReadModBusDataの戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest003)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusComData.readAddress = 0x02A0;
    modBusComData.comSize = 8;
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_connect())
                    .Times(AtLeast(1))
                    .WillOnce(Return(true));
    testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_send_read_request(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(nullptr));
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusComData);
    qDebug() << "ret:" << ret;
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest004</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Sync-Write-Request</category3>
    <case>
        1)QModbusTcpClient::connectDevice関数がtrueを返却するように設定しておく
        2)syncStartModBusCommunication()を実行する
        3)QModbusTcpClient::sendWriteRequest関数がnullptrを返却するように設定しておく
        4)syncWriteModBusData()を実行する
    </case>
    <check>
        1)syncWriteModBusDataの戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest004)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusComData.readAddress = 0x02A0;
    modBusComData.comSize = 8;
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_connect())
                    .Times(AtLeast(1))
                    .WillOnce(Return(true));
    testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_send_write_request(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(nullptr));
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
    qDebug() << "ret:" << ret;
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest005</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>readModBusData()</category2>
    <category3>MODBUS-Read-Request</category3>
    <case>
        1)QModbusTcpClient::connectDevice関数がtrueを返却するように設定しておく
        2)syncStartModBusCommunication()を実行する
        3)QModbusTcpClient::sendReadRequest関数がnullptrを返却するように設定しておく
        4)readModBusData()を実行する
    </case>
    <check>
        1)readModBusDataの戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest005)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusComData.readAddress = 0x02A0;
    modBusComData.comSize = 8;
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_connect())
                    .Times(AtLeast(1))
                    .WillOnce(Return(true));
    testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_send_read_request(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(nullptr));
    ret = testSub->getModBusCom()->readModBusData(modBusComData);
    qDebug() << "ret:" << ret;
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest006</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>writeModBusData()</category2>
    <category3>MODBUS-Write-Request</category3>
    <case>
        1)QModbusTcpClient::connectDevice関数がtrueを返却するように設定しておく
        2)syncStartModBusCommunication()を実行する
        3)QModbusTcpClient::sendWriteRequest関数がnullptrを返却するように設定しておく
        4)writeModBusData()を実行する
    </case>
    <check>
        1)writeModBusDataの戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest006)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusComData.readAddress = 0x02A0;
    modBusComData.comSize = 8;
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_connect())
                    .Times(AtLeast(1))
                    .WillOnce(Return(true));
    testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_CALL(testSub->getModBusCom()->getUnitTestMockMethod(), mock_modbus_send_write_request(_,_))
                    .Times(AtLeast(1))
                    .WillOnce(Return(nullptr));
    ret = testSub->getModBusCom()->writeModBusData(modBusComData);
    qDebug() << "ret:" << ret;
    EXPECT_EQ(ret, false);
}
#else // CORELIB_UNIT_TEST
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest007</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>startModBusCommunication()</category2>
    <category3>MODBUS-Connect</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
    </case>
    <check>
        1)MODBUSステータスがQModbusDevice::ConnectedState(2)になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest007)
{
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    // modbus connet
    testSub->getModBusCom()->startModBusCommunication();
    testSub->connetStatusChangedFunc();
    for (int i = 0; i < 100; i++){
        loop.exec();
        if (testSub->getModBusState() == QModbusDevice::ConnectedState){
            break;
        }
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest008</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>startModBusCommunication()</category2>
    <category3>MODBUS-DisConnect</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
        3)stopModBusCommunication()を実行する
    </case>
    <check>
        1)2)実行後MODBUSステータスがQModbusDevice::ConnectedState(2)になっていること
        2)3)実行後MODBUSステータスがQModbusDevice::ConnectedState(0)になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest008)
{
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    // modbus connet
    testSub->getModBusCom()->startModBusCommunication();
    testSub->connetStatusChangedFunc();
    for (int i = 0; i < 100; i++){
        loop.exec();
        if (testSub->getModBusState() == QModbusDevice::ConnectedState){
            break;
        }
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
    EXPECT_EQ(testSub->getModBusState(), 0);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest009</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>startModBusCommunication()</category2>
    <category3>MODBUS-DisConnect</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
        3)syncStopModBusCommunication()を実行する
    </case>
    <check>
        1)2)実行後MODBUSステータスがQModbusDevice::ConnectedState(2)になっていること
        2)3)実行後MODBUSステータスがQModbusDevice::ConnectedState(0)になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest009)
{
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    // modbus connet
    testSub->getModBusCom()->startModBusCommunication();
    testSub->connetStatusChangedFunc();
    for (int i = 0; i < 100; i++){
        loop.exec();
        if (testSub->getModBusState() == QModbusDevice::ConnectedState){
            break;
        }
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    // modbus disconnet
    testSub->getModBusCom()->syncStopModBusCommunication();
    EXPECT_EQ(testSub->getModBusState(), 0);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest010</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>startModBusCommunication()</category2>
    <category3>MODBUS-DisConnect</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)stopModBusCommunication()を実行する
    </case>
    <check>
        1)syncStartModBusCommunication()戻り値がtrueになっていること
        2)1)実行後MODBUSステータスがQModbusDevice::ConnectedState(2)になっていること
        3)2)実行後MODBUSステータスがQModbusDevice::ConnectedState(0)になっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest010)
{
    bool ret = false;
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->stopModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::UnconnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 0);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest011</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-SyncWrite</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x05, writeAddress=0x0000, writeBuf=modBusComTestWriteData comSize=128]
        4)syncWriteModBusData()を実行する
    </case>
    <check>
        1)4)の戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest011)
{
    bool ret = false;
    stModBusCommunicationData modBusWriteComData;
    int i = 0;
    quint16 writeData = 0;

    modBusWriteComData.funcCode = QModbusPdu::FunctionCode::WriteSingleCoil;
    modBusWriteComData.writeAddress = 0x0000;
    modBusWriteComData.comSize = 128;
    for (i = 0; i < modBusWriteComData.comSize*2*8; i++){
        if (i > 0 && i % 16) {
            writeData |= (modBusComTestWriteData[((i/16)*16+(i%16))] == true ? (1 << (15 - (i % 16))) : (0 << (15 - (i % 16))));
        } else if (i > 0 && !(i % 16)){
            modBusWriteComData.writeBuf[i/16 - 1] = writeData;
            writeData = 0;
            writeData |= (modBusComTestWriteData[((i/16)*16+(i%16))] == true ? (1 << 15) : (0 << 15));
        } else if (i == modBusWriteComData.comSize - 1){
            writeData |= (modBusComTestWriteData[((i/16)*16+(i%16))] == true ? (1 << (15 - (i % 16))) : (0 << (15 - (i % 16))));
            modBusWriteComData.writeBuf[i/16 - 1] = writeData;
            writeData = 0;
        } else {
            writeData |= (modBusComTestWriteData[((i/16)*16+(i%16))] == true ? (1 << (15 - (i % 16))) : (0 << (15 - (i % 16))));
        }
    }
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    // modbus connet
    testSub->getModBusCom()->startModBusCommunication();
    testSub->connetStatusChangedFunc();
    for (int i = 0; i < 100; i++){
        loop.exec();
        if (testSub->getModBusState() == QModbusDevice::ConnectedState){
            break;
        }
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusWriteComData);
    EXPECT_EQ(ret, true);
    // modbus disconnet
    testSub->getModBusCom()->syncStopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest012</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-SyncWrite</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x05, writeAddress=0x0000, writeBuf=modBusComTest012WriteData comSize=127]
        4)syncWriteModBusData()を実行する
        5)syncReadModBusData(funcCode=0x01, readAddress=0x0000, comSize=127)を実行する
    </case>
    <check>
        1)4)の戻り値がtrueであること
        2)5)の戻り値がtrueであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest012)
{
    bool ret = false;
    stModBusCommunicationData modBusWriteComData;
    stModBusCommunicationData modBusReadComData;
    int i = 0;
    quint16 writeData = 0;
    QBitArray readBuf;

    modBusWriteComData.funcCode = QModbusPdu::FunctionCode::WriteSingleCoil;
    modBusWriteComData.writeAddress = 0x0000;
    modBusWriteComData.comSize = 2;
    for (i = 0; i < modBusWriteComData.comSize*8; i++){
        if (i > 0 && i % 16) {
            writeData |= (modBusComTest012WriteData[((i/16)*16+(i%16))] == true ? (1 << (15 - (i % 16))) : (0 << (15 - (i % 16))));
        } else if (i > 0 && !(i % 16)){
            modBusWriteComData.writeBuf[i/16 - 1] = writeData;
            writeData = 0;
            writeData |= (modBusComTest012WriteData[((i/16)*16+(i%16))] == true ? (1 << 15) : (0 << 15));
        } else if (i == modBusWriteComData.comSize - 1){
            writeData |= (modBusComTest012WriteData[((i/16)*16+(i%16))] == true ? (1 << (15 - (i % 16))) : (0 << (15 - (i % 16))));
            modBusWriteComData.writeBuf[i/16 - 1] = writeData;
            writeData = 0;
        } else {
            writeData |= (modBusComTest012WriteData[((i/16)*16+(i%16))] == true ? (1 << (15 - (i % 16))) : (0 << (15 - (i % 16))));
        }
    }
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    // modbus connet
    testSub->getModBusCom()->startModBusCommunication();
    testSub->connetStatusChangedFunc();
    for (int i = 0; i < 100; i++){
        loop.exec();
        if (testSub->getModBusState() == QModbusDevice::ConnectedState){
            break;
        }
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusWriteComData);
    EXPECT_EQ(ret, true);

    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    modBusReadComData.readAddress = 0x0000;
    modBusReadComData.comSize = 2;

    readBuf.resize(modBusReadComData.comSize*8);
    for (i = 0; i < modBusReadComData.comSize*8; i++){
        readBuf.setBit(i, modBusComTest012WriteData[i]);
    }
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusReadComData);
    EXPECT_EQ(ret, true);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest013</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x05, writeAddress=0x0000, writeBuf=modBusComTest013WriteData comSize=1]
        4)syncWriteModBusData()を実行する
        5)readModBusData(funcCode=0x01, readAddress=0x0000, comSize=1)を実行する
    </case>
    <check>
        1)4)の戻り値がtrueであること
        2)5)実行後、modBusComTest013WriteDataのデータが読み出せること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest013)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;
    int i = 0;
    ushort bitSize = 16;

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleCoils;
    modBusComData.writeAddress = 0x02A0;
    modBusComData.comSize = 16;
    int init_size = (bitSize / 16 == 0 ? 1 : (bitSize % 16 == 0 ? bitSize / 16 : bitSize / 16 + 1));

    // init
    for (i = 0; i < init_size; i++){
        modBusComData.writeBuf[i] = 0;
    }
    for (i = 0; i < (int)bitSize; i++){
        modBusComData.writeBuf[i / 16] |= modBusComTest013WriteData[i] << (i % 16);
    }

    // modbus connet
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
    EXPECT_EQ(ret, true);
    testSub->getModBusReadComData()->funcCode = QModbusPdu::FunctionCode::ReadCoils;
    testSub->getModBusReadComData()->readAddress = 0x02A0;
    testSub->getModBusReadComData()->comSize = 16;
    // init
    for (i = 0; i < testSub->getModBusReadComData()->comSize; i++){
        testSub->getModBusReadComData()->readBuf[i] = 0;
    }
    testSub->connetReadStatusChangedFunc();
    testSub->connetReadStatusChangedEventLoop();
    EXPECT_EQ(testSub->getModBusState(), 0);
    for (i = 0; i < testSub->getModBusReadComData()->comSize; i++){
        //qDebug("readBuf[%d]=0x%04x %d", i / 16, (testSub->getModBusReadComData()->readBuf[i / 16] & (1 << i)), modBusComTest013WriteData[i]);
        EXPECT_EQ(((testSub->getModBusReadComData()->readBuf[i / 16] & (1 << i)) >> i), modBusComTest013WriteData[i]);
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest014</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x10, writeAddress=0x02A0, writeBuf=modBusComTest014WriteData comSize=8]
        4)syncWriteModBusData()を実行する
        5)readModBusData(funcCode=0x03, readAddress=0x02A0, comSize=8)を実行する
    </case>
    <check>
        1)4)の戻り値がtrueであること
        2)5)実行後、書き込んだデータが読み出せていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest014)
{
    int i = 0;
    bool ret = false;
    stModBusCommunicationData modBusWriteComData;

    modBusWriteComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusWriteComData.writeAddress = 0x02A0;
    modBusWriteComData.comSize = 8;
    for (i = 0; i < modBusWriteComData.comSize; i++){
        modBusWriteComData.writeBuf[i] = modBusComTest014WriteData[i];
    }
    // modbus connet
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusWriteComData);
    EXPECT_EQ(ret, true);

    testSub->getModBusReadComData()->funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    testSub->getModBusReadComData()->readAddress = 0x02A0;
    testSub->getModBusReadComData()->comSize = 8;
    // init
    for (i = 0; i < testSub->getModBusReadComData()->comSize; i++){
        testSub->getModBusReadComData()->readBuf[i] = 0;
    }
    testSub->connetReadStatusChangedFunc();
    testSub->connetReadStatusChangedEventLoop();
    EXPECT_EQ(testSub->getModBusState(), 0);
    for (i = 0; i < (testSub->getModBusReadComData()->comSize); i++){
        EXPECT_EQ(testSub->getModBusReadComData()->readBuf[i], modBusComTest014WriteData[i]);
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest015</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Sync-Write-Request</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncWriteModBusData()を実行する
    </case>
    <check>
        1)2)syncWriteModBusDataの戻り値がtrueになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest015)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;
    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusComData.writeAddress = 0x02A0;
    modBusComData.comSize = 8;
    for (int i = 0; i < modBusComData.comSize; i++){
        modBusComData.writeBuf[i] = 0x0505;
    }
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    // modbus connet
    testSub->getModBusCom()->startModBusCommunication();
    testSub->connetStatusChangedFunc();
    for (int i = 0; i < 100; i++){
        loop.exec();
        if (testSub->getModBusState() == QModbusDevice::ConnectedState){
            break;
        }
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
    EXPECT_EQ(ret, true);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest016</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Sync-Read-Request</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x10, writeAddress=0x0A0A, writeBuf=modBusComTest016WriteData comSize=8]
        4)syncWriteModBusData()を実行する
        5)syncReadModBusData()を実行する
    </case>
    <check>
        1)2)syncReadModBusDataの戻り値がtrueになっていること
        2)writeAddress=0x0A0Aから8word分、modBusComTest016WriteDataのデータが読み出させていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest016)
{
    bool ret = false;
    stModBusCommunicationData modBusWriteComData;
    stModBusCommunicationData modBusReadComData;
    int i = 0;
    // modbus write data table
    quint16 modBusComTest016WriteData[8] = {
        0x0102, 0x0203, 0x0304, 0x0405, 0x0506, 0x0607, 0x0708, 0x0809
    };

    modBusWriteComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusWriteComData.writeAddress = 0x0A0A;
    modBusWriteComData.comSize = 8;
    for (i = 0; i < modBusWriteComData.comSize; i++){
        modBusWriteComData.writeBuf[i] = modBusComTest016WriteData[i];
    }
    QEventLoop loop;
    QTimer::singleShot(10, &loop, SLOT(quit()));
    // modbus connet
    testSub->getModBusCom()->startModBusCommunication();
    testSub->connetStatusChangedFunc();
    for (int i = 0; i < 100; i++){
        loop.exec();
        if (testSub->getModBusState() == QModbusDevice::ConnectedState){
            break;
        }
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusWriteComData);
    EXPECT_EQ(ret, true);
    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusReadComData.readAddress = 0x0A0A;
    modBusReadComData.comSize = 8;
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusReadComData);
    EXPECT_EQ(ret, true);
    for (i = 0; i < modBusReadComData.comSize; i++){
        EXPECT_EQ(modBusReadComData.readBuf[i], modBusComTest016WriteData[i]);
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest017</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusDataUnit()</category2>
    <category3>MODBUS-Sync-Read-Request</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x10, writeAddress=0x0A0A, writeBuf=modBusComTest017WriteData comSize=8]
        4)syncWriteModBusData()を実行する
        5)syncReadModBusDataUnit()を実行する
    </case>
    <check>
        1)2)syncReadModBusDataの戻り値がtrueになっていること
        2)writeAddress=0x0A0Aから8word分、modBusComTest017WriteDataのデータが読み出させていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest017)
{
    bool ret = false;
    stModBusCommunicationData modBusWriteComData;
    stModBusCommunicationData modBusReadComData;
    QModbusDataUnit modBusDataUnit;
    int i = 0;
    // modbus write data table
    quint16 modBusComTest017WriteData[8] = {
        0x0102, 0x0203, 0x0304, 0x0405, 0x0506, 0x0607, 0x0708, 0x0809
    };

    modBusWriteComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusWriteComData.writeAddress = 0x0A0A;
    modBusWriteComData.comSize = 8;
    for (i = 0; i < modBusWriteComData.comSize; i++){
        modBusWriteComData.writeBuf[i] = modBusComTest017WriteData[i];
    }
    // modbus connet
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusWriteComData);
    EXPECT_EQ(ret, true);
    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusReadComData.readAddress = 0x0A0A;
    modBusReadComData.comSize = 8;
    ret = testSub->getModBusCom()->syncReadModBusDataUnit(&modBusReadComData, &modBusDataUnit);
    EXPECT_EQ(ret, true);
    for (int i = 0, total = (int)(modBusDataUnit.valueCount()); i < total; ++i) {
        EXPECT_EQ(modBusDataUnit.value(i), modBusComTest017WriteData[i]);
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest018</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadCoils(0x01)を設定する
        2)readAddressを0x0000に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がtrueになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest018)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    modBusComData.readAddress = 0x0000;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, true);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest019</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadCoils(0x01)を設定する
        2)readAddressを0x1600に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がtrueになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest019)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    modBusComData.readAddress = 0x1600;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, true);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest020</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadHoldingRegisters(0x03)を設定する
        2)readAddressを0x1600に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest020)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusComData.readAddress = 0x1000;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest021</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadCoils(0x01)を設定する
        2)readAddressを0x1800に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がtrueになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest021)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    modBusComData.readAddress = 0x1800;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, true);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest022</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadDiscreteInputs(0x02)を設定する
        2)readAddressを0x0000に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がtrueになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest022)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadDiscreteInputs;
    modBusComData.readAddress = 0x0000;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, true);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest023</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadDiscreteInputs(0x02)を設定する
        2)readAddressを0x0700に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がtrueになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest023)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadDiscreteInputs;
    modBusComData.readAddress = 0x0700;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, true);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest024</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadHoldingRegisters(0x03)を設定する
        2)readAddressを0x0400に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest024)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusComData.readAddress = 0x0400;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest025</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadDiscreteInputs(0x02)を設定する
        2)readAddressを0x0800に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がtrueになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest025)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadDiscreteInputs;
    modBusComData.readAddress = 0x0800;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, true);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest026</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadInputRegisters(0x04)を設定する
        2)readAddressを0x0000に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest026)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadInputRegisters;
    modBusComData.readAddress = 0x0000;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest027</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadInputRegisters(0x04)を設定する
        2)readAddressを0x0240に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest027)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadInputRegisters;
    modBusComData.readAddress = 0x0240;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest028</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadHoldingRegisters(0x03)を設定する
        2)readAddressを0x0200に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest028)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusComData.readAddress = 0x0200;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest029</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadInputRegisters(0x04)を設定する
        2)readAddressを0x0300に設定する
        3)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)3)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest029)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadInputRegisters;
    modBusComData.readAddress = 0x0300;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest030</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadExceptionStatus(0x07)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest030)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadExceptionStatus;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest031</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::Diagnostics(0x08)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest031)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::Diagnostics;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest032</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::GetCommEventCounter(0x0B)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest032)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::GetCommEventCounter;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest033</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::GetCommEventLog(0x0C)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest033)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::GetCommEventLog;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest034</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReportServerId(0x11)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest034)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReportServerId;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest035</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadFileRecord(0x14)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest035)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadFileRecord;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest036</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::WriteFileRecord(0x15)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest036)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteFileRecord;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest037</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::MaskWriteRegister(0x16)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest037)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::MaskWriteRegister;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest038</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadWriteMultipleRegisters(0x17)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest038)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadWriteMultipleRegisters;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest039</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::ReadFifoQueue(0x18)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest039)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadFifoQueue;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest040</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::EncapsulatedInterfaceTransport(0x2B)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest040)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::EncapsulatedInterfaceTransport;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest041</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>checkModBusBitMemory()</category2>
    <category3>MODBUS-MemoryType-judge</category3>
    <case>
        1)QModbusPdu::FunctionCode::UndefinedFunctionCode(0x100)を設定する
        2)checkModBusBitMemory()を実行する
    </case>
    <check>
        1)2)checkModBusBitMemory()の戻り値がfalseになっていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest041)
{
    bool ret = true;
    stModBusCommunicationData modBusComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::UndefinedFunctionCode;

    ret = testSub->getModBusCom()->checkModBusBitMemory(modBusComData);    
    EXPECT_EQ(ret, false);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest042</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[comSize=255]
        4)readModBusData(funcCode=0x03, readAddress=0x02A0, comSize=255)を実行する
    </case>
    <check>
        1)4)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest042)
{
    bool ret = false;
    stModBusCommunicationData modBusReadComData;

    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusReadComData.readAddress = 0x02A0;
    modBusReadComData.comSize = 255;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);
    // modbus read
    ret = testSub->getModBusCom()->readModBusData(modBusReadComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest043</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[comSize=128]
        4)readModBusData(funcCode=0x14, readAddress=0x02A0, comSize=128)を実行する
    </case>
    <check>
        1)4)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest043)
{
    bool ret = false;
    stModBusCommunicationData modBusReadComData;

    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadFileRecord;
    modBusReadComData.readAddress = 0x02A0;
    modBusReadComData.comSize = 127;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);
    // modbus read
    ret = testSub->getModBusCom()->readModBusData(modBusReadComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest044</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Write</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[comSize=128]
        4)writeModBusData(funcCode=0x10, writeAddress=0x0400, comSize=128)を実行する
    </case>
    <check>
        1)4)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest044)
{
    bool ret = false;
    stModBusCommunicationData modBusReadComData;

    modBusReadComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusReadComData.writeAddress = 0x0400;
    modBusReadComData.comSize = 128;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);
    // modbus write
    ret = testSub->getModBusCom()->writeModBusData(modBusReadComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>異常系</testclass>
    <testname>ModBusComTest045</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Write</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)readModBusData(funcCode=0x15, readAddress=0x1000, comSize=127)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest045)
{
    bool ret = false;
    stModBusCommunicationData modBusReadComData;

    modBusReadComData.funcCode = QModbusPdu::FunctionCode::WriteFileRecord;
    modBusReadComData.writeAddress = 0x1000;
    modBusReadComData.comSize = 127;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);
    // modbus read
    ret = testSub->getModBusCom()->readModBusData(modBusReadComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest046</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>readModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x05, writeAddress=0x02A0, writeBuf=modBusComTest046WriteData comSize=120]
        4)syncWriteModBusData()を実行する
        5)readModBusData(funcCode=0x01, readAddress=0x02A0, comSize=120)を実行する
    </case>
    <check>
        1)4)の戻り値がtrueであること
        2)5)実行後、書き込んだデータが読み出せていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest046)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;
    int i = 0, j = 0;
    ushort bitSize = 120;
    bool checkSizeRemainder = false;

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleCoils;
    modBusComData.writeAddress = 0x02A0;
    modBusComData.comSize = 120;
    int init_size = (bitSize / 16 == 0 ? 1 : (bitSize % 16 == 0 ? bitSize / 16 : bitSize / 16 + 1));

    //qDebug("init_size[%d] bitSize[%d]", init_size, bitSize);
    // init
    for (i = 0; i < init_size; i++){
        modBusComData.writeBuf[i] = 0;
        //testSub->getModBusReadComData()->readBuf[i] = 0;    
    }
    for (i = 0; i < (int)bitSize; i++){
        //modBusComData.writeBuf[i / 16] |= modBusComTest046WriteData[i] << (i % 16);
        modBusComData.writeBuf[i / 16] |= modBusComTest046WriteData[i] << (i % 16);
    }
    // modbus connet
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
    EXPECT_EQ(ret, true);
    testSub->getModBusReadComData()->funcCode = QModbusPdu::FunctionCode::ReadCoils;
    testSub->getModBusReadComData()->readAddress = 0x02A0;
    testSub->getModBusReadComData()->comSize = 120;
    // init
    for (i = 0; i < testSub->getModBusReadComData()->comSize; i++){
        testSub->getModBusReadComData()->readBuf[i] = 0;
    }
    checkSizeRemainder = (testSub->getModBusReadComData()->comSize % 16 == 0 ? false : true);
    testSub->connetReadStatusChangedFunc();
    testSub->connetReadStatusChangedEventLoop();
    EXPECT_EQ(testSub->getModBusState(), 0);
    //qDebug("init_size[%d] bitSize[%d] checkSizeRemainder[%d]", init_size, bitSize, checkSizeRemainder);
    for (i = 0; i < init_size; i++){
        if (checkSizeRemainder) {
            if (i == init_size - 1){
                for (j = 0; j < testSub->getModBusReadComData()->comSize % 16; j++){
                    //qDebug("readBuf[%d]=0x%04x %d", i % 16, (testSub->getModBusReadComData()->readBuf[i % 16] & (1 << j)), modBusComTest046WriteData[i * 16 + j]);
                    EXPECT_EQ(((testSub->getModBusReadComData()->readBuf[i % 16] & (1 << j)) >> j), modBusComTest046WriteData[i * 16 + j]);
                }
                break;
            }
            for (j = 0; j < 16; j++){
                //qDebug("readBuf[%d]=0x%04x %d", i % 16, (testSub->getModBusReadComData()->readBuf[i % 16] & (1 << j)), modBusComTest046WriteData[i * 16 + j]);
                EXPECT_EQ(((testSub->getModBusReadComData()->readBuf[i % 16] & (1 << j)) >> j), modBusComTest046WriteData[i * 16 + j]);
            }
        } else {
            for (j = 0; j < (testSub->getModBusReadComData()->comSize)/init_size; j++){
                //qDebug("readBuf[%d]=0x%04x %d", i % 16, (testSub->getModBusReadComData()->readBuf[i % 16] & (1 << j)), modBusComTest046WriteData[i * 16 + j]);
                EXPECT_EQ(((testSub->getModBusReadComData()->readBuf[i % 16] & (1 << j)) >> j), modBusComTest046WriteData[i * 16 + j]);
            }
        }
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
    EXPECT_EQ(testSub->getModBusState(), 0);
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest047</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
        3)syncReadModBusData(funcCode=0x01, readAddress=0x0000, comSize=2049)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest047)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    modBusComData.readAddress = 0x0000;
    modBusComData.comSize = 2049;
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest048</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>readModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
        3)readModBusData(funcCode=0x02, readAddress=0x1000, comSize=2049)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest048)
{
    bool ret = false;

    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);

    testSub->getModBusReadComData()->funcCode = QModbusPdu::FunctionCode::ReadDiscreteInputs;
    testSub->getModBusReadComData()->readAddress = 0x1000;
    testSub->getModBusReadComData()->comSize = 2049;
    ret = testSub->connetReadStatusChangedFunc();
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest049</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncReadModBusData(funcCode=0x03, readAddress=0x3000, comSize=129)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest049)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusComData.readAddress = 0x3000;
    modBusComData.comSize = 129;
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest050</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>readModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)readModBusData(funcCode=0x04, readAddress=0x4000, comSize=129)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest050)
{
    bool ret = false;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    testSub->getModBusReadComData()->funcCode = QModbusPdu::FunctionCode::ReadInputRegisters;
    testSub->getModBusReadComData()->readAddress = 0x4000;
    testSub->getModBusReadComData()->comSize = 129;
    ret = testSub->connetReadStatusChangedFunc();
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest051</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Write</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
        3)syncWriteModBusData(funcCode=0x05, writeAddress=0x5000, comSize=2049)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest051)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteSingleCoil;
    modBusComData.writeAddress = 0x5000;
    modBusComData.comSize = 2049;
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest052</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>writeModBusData()</category2>
    <category3>MODBUS-Write</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
        3)writeModBusData(funcCode=0x06, writeAddress=0x6000, comSize=129)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest052)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteSingleRegister;
    modBusComData.writeAddress = 0x6000;
    modBusComData.comSize = 129;
    ret = testSub->getModBusCom()->writeModBusData(modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest053</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncWriteModBusData()</category2>
    <category3>MODBUS-Write</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncWriteModBusData(funcCode=0x0F, writeAddress=0xF000, comSize=2049)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest053)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleCoils;
    modBusComData.writeAddress = 0xF000;
    modBusComData.comSize = 2049;
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest054</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>writeModBusData()</category2>
    <category3>MODBUS-Write</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)writeModBusData(funcCode=0x10, readAddress=0xF010, comSize=129)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest054)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusComData.writeAddress = 0xF010;
    modBusComData.comSize = 129;
    ret = testSub->getModBusCom()->writeModBusData(modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest055</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Read引数チェック</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncReadModBusData(modBusComData==NULL)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest055)
{
    bool ret = false;
    stModBusCommunicationData *modBusComData = NULL;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    ret = testSub->getModBusCom()->syncReadModBusData(modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest056</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusDataUnit()</category2>
    <category3>MODBUS-Read引数チェック</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncReadModBusDataUnit(modBusComData==NULL)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest056)
{
    bool ret = false;
    stModBusCommunicationData *modBusComData = NULL;
    QModbusDataUnit modBusDataUnit;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    ret = testSub->getModBusCom()->syncReadModBusDataUnit(modBusComData, &modBusDataUnit);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}
/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest057</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusDataUnit()</category2>
    <category3>MODBUS-Read引数チェック</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncReadModBusDataUnit(modBusDataUnit==NULL)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest057)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;
    QModbusDataUnit *modBusDataUnit = NULL;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    ret = testSub->getModBusCom()->syncReadModBusDataUnit(&modBusComData, modBusDataUnit);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest058</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Read引数チェック</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncWriteModBusData(modBusComData==NULL)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest058)
{
    bool ret = false;
    stModBusCommunicationData *modBusComData = NULL;

    // modbus connet
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);

    ret = testSub->getModBusCom()->syncWriteModBusData(modBusComData);
    EXPECT_EQ(ret, false);
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest059</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)startModBusCommunication()を実行する
        3)syncReadModBusData(funcCode=0x01, readAddress=0x0000, comSize=120)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest059)
{
    bool ret = false;
    stModBusCommunicationData modBusWriteComData;
    stModBusCommunicationData modBusReadComData;
    int i = 0;

    modBusWriteComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusWriteComData.writeAddress = 0x1000;
    modBusWriteComData.comSize = 120;
    for(i = 0; i < modBusWriteComData.comSize; i++){
        modBusWriteComData.writeBuf[i] = modBusComTest059WriteData[i];
    }

    testSub->getModBusReadComData()->funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    testSub->getModBusReadComData()->readAddress = 0x1000;
    testSub->getModBusReadComData()->comSize = 120;
    testSub->getModBusReadComData()->writeFuncCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    testSub->getModBusReadComData()->writeAddress = 0x2000;
    testSub->getModBusReadComData()->writeComSize = 120;
    for(i = 0; i < testSub->getModBusReadComData()->writeComSize; i++){
        testSub->getModBusReadComData()->writeBuf[i] = modBusComTest059WriteData[i];
    }

    // modbus connet
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusWriteComData);
    EXPECT_EQ(ret, true);

    ret = testSub->connetReadWriteStatusChangedFunc();
    EXPECT_EQ(ret, true);
    testSub->connetReadStatusChangedEventLoop();
    EXPECT_EQ(testSub->getModBusState(), 0);
    for (i = 0; i < (testSub->getModBusReadComData()->comSize); i++){
        //qDebug("readBuf[%d]=0x%04x", i, testSub->getModBusReadComData()->readBuf[i]);
        EXPECT_EQ(testSub->getModBusReadComData()->readBuf[i], modBusComTest059WriteData[i]);
    }
    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusReadComData.readAddress = 0x2000;
    modBusReadComData.comSize = 120;
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusReadComData);
    EXPECT_EQ(ret, true);
    for (i = 0; i < (testSub->getModBusReadComData()->comSize); i++){
        //qDebug("readBuf[%d]=0x%04x", i, modBusReadComData.readBuf[i]);
        EXPECT_EQ(modBusReadComData.readBuf[i], modBusComTest059WriteData[i]);
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest060</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Sync-Read-Request</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x05, writeAddress=0x02A0, writeBuf=modBusComTest060WriteData comSize=128]
        4)syncWriteModBusData()を実行する
        5)syncReadModBusData(funcCode=0x01, readAddress=0x02A0, comSize=128)を実行する
    </case>
    <check>
        1)2)syncReadModBusDataの戻り値がtrueになっていること
        2)writeAddress=0x02A0から128bit分、modBusComTest060WriteDataのデータが読み出させていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest060)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;
    int i = 0, j = 0;
    ushort bitSize = 128;
    bool checkSizeRemainder = false;
    stModBusCommunicationData modBusReadComData;

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleCoils;
    modBusComData.writeAddress = 0x02A0;
    modBusComData.comSize = 128;
    int init_size = (bitSize / 16 == 0 ? 1 : (bitSize % 16 == 0 ? bitSize / 16 : bitSize / 16 + 1));

    // init
    for (i = 0; i < init_size; i++){
        modBusComData.writeBuf[i] = 0;  
    }
    for (i = 0; i < (int)bitSize; i++){
        modBusComData.writeBuf[i / 16] |= modBusComTest060WriteData[i] << (i % 16);
    }
    // modbus connet
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
    EXPECT_EQ(ret, true);
    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    modBusReadComData.readAddress = 0x02A0;
    modBusReadComData.comSize = 128;
    // init
    for (i = 0; i < modBusReadComData.comSize; i++){
        modBusReadComData.readBuf[i] = 0;
    }
    checkSizeRemainder = (modBusReadComData.comSize % 16 == 0 ? false : true);
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusReadComData);
    EXPECT_EQ(ret, true);
    for (i = 0; i < init_size; i++){
        if (checkSizeRemainder) {
            if (i == init_size - 1){
                for (j = 0; j < modBusReadComData.comSize % 16; j++){
                    qDebug("readBuf[%d]=0x%04x %d", i % 16, (modBusReadComData.readBuf[i % 16] & (1 << j)), modBusComTest060WriteData[i * 16 + j]);
                    EXPECT_EQ(((modBusReadComData.readBuf[i % 16] & (1 << j)) >> j), modBusComTest060WriteData[i * 16 + j]);
                }
                break;
            }
            for (j = 0; j < 16; j++){
                qDebug("readBuf[%d]=0x%04x %d", i % 16, (modBusReadComData.readBuf[i % 16] & (1 << j)), modBusComTest060WriteData[i * 16 + j]);
                EXPECT_EQ(((modBusReadComData.readBuf[i % 16] & (1 << j)) >> j), modBusComTest060WriteData[i * 16 + j]);
            }
        } else {
            for (j = 0; j < (modBusReadComData.comSize)/init_size; j++){
                qDebug("readBuf[%d]=0x%04x %d", i % 16, (modBusReadComData.readBuf[i % 16] & (1 << j)), modBusComTest060WriteData[i * 16 + j]);
                EXPECT_EQ(((modBusReadComData.readBuf[i % 16] & (1 << j)) >> j), modBusComTest060WriteData[i * 16 + j]);
            }
        }
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
    EXPECT_EQ(testSub->getModBusState(), 0);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest061</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Sync-Read-Request</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)modbus-data[funcCode=0x05, writeAddress=0x02A0, writeBuf=modBusComTestWriteData comSize=272]
        4)syncWriteModBusData()を実行する
        5)syncReadModBusData(funcCode=0x01, readAddress=0x02A0, comSize=272)を実行する
    </case>
    <check>
        1)2)syncReadModBusDataの戻り値がtrueになっていること
        2)writeAddress=0x02A0から2000bit分、modBusComTestWriteDataのデータが読み出させていること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest061)
{
    bool ret = false;
    stModBusCommunicationData modBusComData;
    int i = 0, j = 0;
    ushort bitSize = 272;
    bool checkSizeRemainder = false;
    stModBusCommunicationData modBusReadComData;
    int writeReqCnt = 0;
    ushort firstComSize = 0, midComSize = 0, lastComSize = 0; 

    modBusComData.funcCode = QModbusPdu::FunctionCode::WriteMultipleCoils;
    modBusComData.writeAddress = 0x02A0;
    modBusComData.comSize = 272;
    int init_size = (bitSize / 16 == 0 ? 1 : (bitSize % 16 == 0 ? bitSize / 16 : bitSize / 16 + 1));

    qDebug("init_size %d", init_size);
    // init
    for (i = 0; i < init_size; i++){
        modBusComData.writeBuf[i] = 0;
    }
    for (i = 0; i < (int)bitSize; i++){
        modBusComData.writeBuf[i / 16] |= modBusComTestWriteData[i] << (i % 16);
    }
    // modbus connet
    testSub->connetStatusChangedFunc();
    testSub->getModBusCom()->startModBusCommunication();
    if (testSub->getModBusState() != QModbusDevice::ConnectedState){
        testSub->connetStatusChangedEventLoop();
    }
    EXPECT_EQ(testSub->getModBusState(), 2);
    writeReqCnt = (modBusComData.comSize <= 256 ? 1 : (modBusComData.comSize % 256 == 0 ? modBusComData.comSize / 256: modBusComData.comSize / 256 + 1));
    firstComSize = (writeReqCnt == 1 ? modBusComData.comSize : 256);
    midComSize = 256;
    lastComSize = (writeReqCnt == 1 ? 256 : (modBusComData.comSize % 256 == 0 ? 256 : modBusComData.comSize % 256));
    for (int k = 0; k < writeReqCnt; k++){
        if (k == 0){    // first
            modBusComData.comSize = firstComSize;
        } else if (k == writeReqCnt - 1){
            modBusComData.comSize = lastComSize;
            modBusComData.writeAddress += 256;
            std::memcpy(reinterpret_cast<quint16 *>(&modBusComData.writeBuf[0]), &modBusComData.writeBuf[k * 256 / 16], sizeof(quint16)*16);
        } else {
            modBusComData.comSize = midComSize;
            modBusComData.writeAddress += 256;
            std::memcpy(reinterpret_cast<quint16 *>(&modBusComData.writeBuf[0]), &modBusComData.writeBuf[k * 256 / 16], sizeof(quint16)*16);
        }
        ret = testSub->getModBusCom()->syncWriteModBusData(&modBusComData);
        EXPECT_EQ(ret, true);
    }
    modBusReadComData.funcCode = QModbusPdu::FunctionCode::ReadCoils;
    modBusReadComData.readAddress = 0x02A0;
    modBusReadComData.comSize = 272;
    //modBusReadComData.comSize = 576;
    // init
    for (i = 0; i < init_size; i++){
        modBusReadComData.readBuf[i] = 0;
    }
    checkSizeRemainder = (modBusReadComData.comSize % 16 == 0 ? false : true);
    ret = testSub->getModBusCom()->syncReadModBusData(&modBusReadComData);
    EXPECT_EQ(ret, true);
    for (i = 0; i < init_size; i++){
        if (checkSizeRemainder) {
            if (i == init_size - 1){
                for (j = 0; j < modBusReadComData.comSize % 16; j++){
                    EXPECT_EQ(((modBusReadComData.readBuf[i % 16] & (1 << j)) >> j), modBusComTestWriteData[i * 16 + j]);
                }
                break;
            }
            for (j = 0; j < 16; j++){
                EXPECT_EQ(((modBusReadComData.readBuf[i % 16] & (1 << j)) >> j), modBusComTestWriteData[i * 16 + j]);
            }
        } else {
            for (j = 0; j < (modBusReadComData.comSize)/init_size; j++){
                EXPECT_EQ(((modBusReadComData.readBuf[i] & (1 << j)) >> j), modBusComTestWriteData[i * 16 + j]);
            }
        }
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
    EXPECT_EQ(testSub->getModBusState(), 0);
}

/*
--------------------------------------------------------------------------------
<testitem>
    <testclass>正常系</testclass>
    <testname>ModBusComTest062</testname>
    <category1>CoreLibModBusCommunication</category1>
    <category2>syncReadModBusData()</category2>
    <category3>MODBUS-Read</category3>
    <case>
        1)ModBus-slaveツールにconnectしておく
        2)syncStartModBusCommunication()を実行する
        3)syncReadWriteModBusData(funcCode=0x03, readAddress=0x1000, comSize=120, funcCode=0x10, writeAddress=0x1000, writeComSize=120)を実行する
    </case>
    <check>
        1)3)の戻り値がfalseであること
    </check>
</testitem>*/
//------------------------------------------------------------------------------
TEST_F(CoreLibModBusCommunicationTestBase, ModBusComTest062)
{
    bool ret = false;
    ret = testSub->getModBusCom()->syncStartModBusCommunication();
    EXPECT_EQ(ret, true);
    stModBusCommunicationData modBusComData;
    int i = 0;
 
    //init
    for(i = 0; i < MODBUS_COMMUNICATION_DATA_MAX; i++){
        modBusComData.readBuf[i] = 0;
        modBusComData.writeBuf[i] = 0;
    }
 
    modBusComData.funcCode = QModbusPdu::FunctionCode::ReadHoldingRegisters;
    modBusComData.readAddress = 0x1000;
    modBusComData.comSize = 120;
    modBusComData.writeFuncCode = QModbusPdu::FunctionCode::WriteMultipleRegisters;
    modBusComData.writeAddress = 0x1000;
    modBusComData.writeComSize = 120;
    for(i = 0; i < modBusComData.writeComSize; i++){
        modBusComData.writeBuf[i] = modBusComTest062WriteData[i];
    }
    ret = testSub->getModBusCom()->syncReadWriteModBusData(&modBusComData);
    EXPECT_EQ(ret, true);
    for (i = 0; i < modBusComData.comSize; i++){
        //qDebug("readBuf[%d]=0x%04x", i, modBusComData.readBuf[i]);
        EXPECT_EQ(modBusComData.readBuf[i], modBusComTest062WriteData[i]);
    }
    // modbus disconnet
    testSub->getModBusCom()->stopModBusCommunication();
}

#endif // CORELIB_UNIT_TEST
