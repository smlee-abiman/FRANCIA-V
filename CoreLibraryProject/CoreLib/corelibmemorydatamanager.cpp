#include "corelibmemorydatamanager.h"

CoreLibMemoryDataManager::CoreLibMemoryDataManager(QObject *parent)
    : QObject(parent)
{
    // binaryデータ読込

    uchar group_block[4];
    group_block[0] = GET_POINT_MEMORY_INFO(0, getWordData(0x6440));
    group_block[1] = GET_POINT_MEMORY_INFO(1, getWordData(0x6440));
    group_block[2] = GET_POINT_MEMORY_INFO(2, getWordData(0x6440));
    group_block[3] = GET_POINT_MEMORY_INFO(3, getWordData(0x6440));

    for(int group = 0; group < 4; group++){
        if(0 < group_block[group]){
            int g_offset = 0x6442;
            int g_offset_mb = 0x3321;
            switch(group){
            case 0:
                g_offset = 0x6442;
                g_offset_mb = 0x3321;
                break;
            case 1:
                g_offset = (group_block[0]) * 0x10000;
                g_offset_mb = 0x4FE0;
                break;
            case 2:
                g_offset = (group_block[0]+group_block[1]) * 0x10000;
                g_offset_mb = 0x5FE0;
                break;
            case 3:
                g_offset = (group_block[0]+group_block[1]+group_block[2]) * 0x10000;
                g_offset_mb = 0x6FE0;
                break;
            }

            // added_files
            ushort added_files = getWordData(g_offset);
            addrTableList.append({g_offset, point_type, 2, -1, -1, g_offset_mb, g_offset_mb, -1, g_offset_mb, -1, g_offset_mb, -1, -1});
            g_offset += 2;
            g_offset_mb += 1;

            // file_comment
            for(int i = 0; i < added_files; i++){
                addrTableList.append({g_offset+(i*32), point_type, 32, -1, -1, g_offset_mb+(i*32), g_offset_mb+(i*32), -1, g_offset_mb+(i*32), -1, g_offset_mb+(i*32), -1, -1});
            }
            g_offset += added_files*32;
            g_offset_mb += added_files*16;

            // pattern
            ushort pattern = getWordData(g_offset);
            for(int i = 0; i < pattern; i++){
                int offset = g_offset+i*32;
                int offset_mb = g_offset_mb+i*16;
                addrTableList.append({offset, point_type, 2, -1, -1, offset_mb, offset_mb, -1, offset_mb, -1, offset_mb, -1, -1});
                addrTableList.append({offset+2, point_type, 16, -1, -1, offset_mb+1, offset_mb+1, -1, offset_mb+1, -1, offset_mb+1, -1, -1});
                addrTableList.append({offset+18, point_type, 2, -1, -1, offset_mb+9, offset_mb+9, -1, offset_mb+9, -1, offset_mb+9, -1, -1});
                addrTableList.append({offset+20, point_type, 4, -1, -1, offset_mb+10, offset_mb+10, -1, offset_mb+10, -1, offset_mb+10, -1, -1});
                addrTableList.append({offset+24, point_type, 4, -1, -1, offset_mb+12, offset_mb+12, -1, offset_mb+12, -1, offset_mb+12, -1, -1});
                addrTableList.append({offset+28, point_type, 4, -1, -1, offset_mb+14, offset_mb+14, -1, offset_mb+14, -1, offset_mb+14, -1, -1});
            }
            g_offset += pattern*32;
            g_offset_mb += pattern*16;


            // ポイント数
            ushort point_num = getWordData(g_offset);
            addrTableList.append({g_offset, point_type, 2, -1, -1, g_offset_mb, g_offset_mb, -1, g_offset_mb, -1, g_offset_mb, -1, -1});
            g_offset += 2;
            g_offset_mb += 1;

            // ポイントアドレス拡張
            ushort addr_ex1 = getWordData(g_offset);
            ushort addr_ex2 = getWordData(g_offset+2);
            ushort addr_ex3 = getWordData(g_offset+4);
            addrTableList.append({g_offset, point_type, 2, -1, -1, g_offset_mb, g_offset_mb, -1, g_offset_mb, -1, g_offset_mb, -1, -1});
            addrTableList.append({g_offset+2, point_type, 2, -1, -1, g_offset_mb+1, g_offset_mb+1, -1, g_offset_mb+1, -1, g_offset_mb+1, -1, -1});
            addrTableList.append({g_offset+4, point_type, 2, -1, -1, g_offset_mb+2, g_offset_mb+2, -1, g_offset_mb+2, -1, g_offset_mb+2, -1, -1});
            g_offset += 6;
            g_offset_mb += 3;

            // ポイントアドレス
            int topAddr = 0;
            switch(group){
            case 0:
                topAddr = 0x8000;
                break;
            case 1:
                topAddr = group_block[0] * 0x10000;
                break;
            case 2:
                topAddr = (group_block[0]+group_block[1]) * 0x10000;
                break;
            case 3:
                topAddr = (group_block[0]+group_block[1]+group_block[2]) * 0x10000;
                break;
            }

            QVector<stModBusAddressTable> pointDataList;
            ushort addr = g_offset + point_num*2;
            ushort addr_next = getWordData(g_offset);
            for(int p = 0; p < point_num; p++){
                ushort size = addr_next - addr;
                addrTableList.append({g_offset, point_type, 2, -1, -1, g_offset_mb, g_offset_mb, -1, g_offset_mb, -1, g_offset_mb, -1, -1});
                if(addr_ex1 <= p){
                    int modbusAddr = topAddr+0x10000+p;
                    pointDataList.append({addr, point_type, size, -1, -1, modbusAddr, modbusAddr, -1, modbusAddr, -1, modbusAddr, -1, -1});
                }
                else if(addr_ex2 <= p){
                    int modbusAddr = topAddr+0x20000+p;
                    pointDataList.append({addr, point_type, size, -1, -1, modbusAddr, modbusAddr, -1, modbusAddr, -1, modbusAddr, -1, -1});
                }
                else if(addr_ex3 <= p){
                    int modbusAddr = topAddr+0x30000+p;
                    pointDataList.append({addr, point_type, size, -1, -1, modbusAddr, modbusAddr, -1, modbusAddr, -1, modbusAddr, -1, -1});
                }
                else{
                    int modbusAddr = topAddr+p;
                    pointDataList.append({addr, point_type, size, -1, -1, modbusAddr, modbusAddr, -1, modbusAddr, -1, modbusAddr, -1, -1});
                }
                g_offset += 2;
                g_offset_mb += 1;

                addr = addr_next;
                addr_next = getWordData(g_offset);
            }

            addrTableList.append(pointDataList);
        }
    }

    // アドレスとメモリの参照マップ作成
    foreach (stModBusAddressTable addrTable, addrTableList) {
        mMapRealAddrToTable[addrTable.realAddr] = &addrTable;
    }
}

CoreLibMemoryDataManager::~CoreLibMemoryDataManager()
{

}

uchar CoreLibMemoryDataManager::getBitData(int index, int bit_idx){
    uchar uc_byte = mModBusData[index];
    uchar ret = (uc_byte >> (7 - bit_idx)) & 1;

    return ret;
}

bool CoreLibMemoryDataManager::setBitData(int index, int bit_idx, uchar data){
    uchar uc_byte = mModBusData[index];
    uchar bit = (uc_byte >> (7 - bit_idx)) & 1;

    if(data == bit){
        return true;
    }

    uc_byte ^= (1 << (7 - bit_idx));

    memcpy(&mModBusData[index], &uc_byte, 1);

    return true;
}

uchar CoreLibMemoryDataManager::getByteData(int index){
    uchar ret = mModBusData[index];

    return ret;
}

bool CoreLibMemoryDataManager::setByteData(int index, uchar data){
    memcpy(&mModBusData[index], &data, 1);

    return true;
}

ushort CoreLibMemoryDataManager::getWordData(int index){
    ushort ret = 0;
    ret += mModBusData[index] << 8;
    ret += mModBusData[index+1];

    return ret;
}

ulong CoreLibMemoryDataManager::getLongData(int index){
    ulong ret = 0;
    ret += mModBusData[index] << 24;
    ret += mModBusData[index+1] << 16;
    ret += mModBusData[index+2] << 8;
    ret += mModBusData[index+3];

    return ret;
}

QByteArray CoreLibMemoryDataManager::getModBusBitData(QModbusPdu::FunctionCode funcCode, int startIndex, int size)
{
    QByteArray byteAry;

    QVector<stModBusIndexToAddressMap> mapList;
    switch(funcCode){
    case QModbusPdu::FunctionCode::ReadCoils:
        mapList = DIList;
        break;
    case QModbusPdu::FunctionCode::ReadDiscreteInputs:
        mapList = DOList;
        break;
    default:
        return byteAry;
    }

    int mapIndex = 0;
    stModBusIndexToAddressMap map =  mapList.at(mapIndex);
    int i = 0;
    while(i < size){
        int index = startIndex + i;

        if(map.startIndex - 1 <= index && index <= map.endIndex - 1){
            int diffStartIndex = index - (map.startIndex - 1);
            int addr = map.startAddress + (diffStartIndex/8);

            uchar data = getByteData(addr);
            byteAry.append(data);

            i+=8;
        }
        else{
            mapIndex++;
            if(mapIndex < mapList.count()){
                map =  mapList.at(mapIndex);
            }
            else{
                break;
            }
        }
    }

    return byteAry;
}

bool CoreLibMemoryDataManager::setModBusBitData(int startIndex, int size, QByteArray byteAry)
{
    QVector<stModBusIndexToAddressMap> mapList = DIList;

    int mapIndex = 0;
    stModBusIndexToAddressMap map =  mapList.at(mapIndex);
    int i = 0;
    while(i < size){
        int index = startIndex + i;

        if(map.startIndex - 1 <= index && index <= map.endIndex - 1){
            int diffStartIndex = startIndex - (map.startIndex - 1);
            int addr = map.startAddress + (diffStartIndex/8) + i;

            uchar data = byteAry.at(i);
            setByteData(addr, data);

            i++;
        }
        else{
            mapIndex++;
            if(mapIndex < mapList.count()){
                map =  mapList.at(mapIndex);
            }
            else{
                break;
            }
        }
    }

    emit updateAllValue();

    return true;
}

QByteArray CoreLibMemoryDataManager::getModBusData(QModbusPdu::FunctionCode funcCode, int startIndex, int size)
{
    QByteArray byteAry;

    QVector<stModBusIndexToAddressMap> mapList;
    switch(funcCode){
    case QModbusPdu::FunctionCode::ReadInputRegisters:
        mapList = RGList;
        break;
    case QModbusPdu::FunctionCode::ReadHoldingRegisters:
        mapList = HRList;
        break;
    default:
        return byteAry;
    }

    int mapIndex = 0;
    stModBusIndexToAddressMap map =  mapList.at(mapIndex);
    int i = 0;
    while(i < size){
        int index = startIndex + i;

        if(map.startIndex-1 <= index && index <= map.endIndex-1){
            int diffStartIndex = index - (map.startIndex - 1);
            int addr = map.startAddress + (diffStartIndex*2);

            uchar data1 = getByteData(addr);
            uchar data2 = getByteData(addr+1);
            if(map.isLong){
                uchar data3 = getByteData(addr+2);
                uchar data4 = getByteData(addr+3);
                byteAry.append(data3);
                byteAry.append(data4);
                byteAry.append(data1);
                byteAry.append(data2);

                i+=2;
            }
            else{
                byteAry.append(data1);
                byteAry.append(data2);

                i++;
            }
        }
        else{
            mapIndex++;
            if(mapIndex < mapList.count()){
                map =  mapList.at(mapIndex);
            }
            else{
                break;
            }
        }
    }

    return byteAry;
}

bool CoreLibMemoryDataManager::setModBusData(int startIndex, int size, QByteArray byteAry)
{
    QVector<stModBusIndexToAddressMap> mapList = HRList;

    int mapIndex = 0;
    stModBusIndexToAddressMap map =  mapList.at(mapIndex);
    int i = 0;
    while(i < size){
        int index = startIndex + i;

        if(map.startIndex-1 <= index && index <= map.endIndex-1){
            int diffStartIndex = index - (map.startIndex - 1);
            int addr = map.startAddress + (diffStartIndex*2);

            uchar data1 = byteAry.at(i*2);
            uchar data2 = byteAry.at((i*2)+1);
            if(map.isLong){
                uchar data3 = byteAry.at((i*2)+2);
                uchar data4 = byteAry.at((i*2)+3);
                setByteData(addr, data3);
                setByteData(addr+1, data4);
                setByteData(addr+2, data1);
                setByteData(addr+3, data2);

                i+=2;
            }
            else{
                setByteData(addr, data1);
                setByteData(addr+1, data2);

                i++;
            }
        }
        else{
            mapIndex++;
            if(mapIndex < mapList.count()){
                map =  mapList.at(mapIndex);
            }
            else{
                break;
            }
        }
    }

    emit updateAllValue();

    return true;
}

