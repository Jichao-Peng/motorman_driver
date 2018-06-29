#include "motomanrobotstatus.h"
#include <QDebug>
MotomanRobotStatus::MotomanRobotStatus(std::condition_variable& CondMsg,QObject* parent)
    :QObject(parent),cvCondMsg(CondMsg)
{
}

MotomanRobotStatus::~MotomanRobotStatus()
{
}

unsigned short MotomanRobotStatus::EndianTrans(unsigned short RawData)
{
    TWO_BITS_DATA stRawData;
    TWO_BITS_DATA stTranData;
    unsigned short TranData;
    stRawData.ssData = RawData;
    stTranData.Bits.bit2 = stRawData.Bits.bit1;
    stTranData.Bits.bit1 = stRawData.Bits.bit2;
    TranData = stTranData.ssData;
    return TranData;
}
unsigned long MotomanRobotStatus::EndianTrans(unsigned long RawData)
{
    FOUR_BITS_DATA stRawData;
    FOUR_BITS_DATA stTranData;
    unsigned long TranData;
    stRawData.ulData = RawData;
    stTranData.Bits.bit4 = stRawData.Bits.bit1;
    stTranData.Bits.bit3 = stRawData.Bits.bit2;
    stTranData.Bits.bit2 = stRawData.Bits.bit3;
    stTranData.Bits.bit1 = stRawData.Bits.bit4;
    TranData = stTranData.ulData;
    return TranData;
}

long MotomanRobotStatus::EndianTrans(long RawData)
{
    FOUR_BITS_DATA stRawData;
    FOUR_BITS_DATA stTranData;
    long TranData;
    stRawData.slData = RawData;
    stTranData.Bits.bit4 = stRawData.Bits.bit1;
    stTranData.Bits.bit3 = stRawData.Bits.bit2;
    stTranData.Bits.bit2 = stRawData.Bits.bit3;
    stTranData.Bits.bit1 = stRawData.Bits.bit4;
    TranData = stTranData.slData;
    return TranData;
}

short MotomanRobotStatus::EndianTrans(short RawData)
{
    TWO_BITS_DATA stRawData;
    TWO_BITS_DATA stTranData;
    short TranData;
    stRawData.ssData = RawData;
    stTranData.Bits.bit2 = stRawData.Bits.bit1;
    stTranData.Bits.bit1 = stRawData.Bits.bit2;
    TranData = stTranData.ssData;
    return TranData;
}
void MotomanRobotStatus::Unpack(ST_RECV_MSG* pstRecvMsg)
{
    switch(pstRecvMsg->stRecvOrder.scChannal)
    {
    case 0x01:
        switch(pstRecvMsg->stRecvOrder.scProperty)
        {
        case 0x01:
            emCtrlCmdType = SERVO_ON;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x02:
            emCtrlCmdType = SERVO_OFF;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x03:
            emCtrlCmdType = CLEAR_ALARM;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x04:
            emCtrlCmdType = CLEAR_ERROR;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        default:
            break;
        }
        break;
    case 0x02:
        switch(pstRecvMsg->stRecvOrder.scProperty)
        {
        case 0x01:
            emCtrlCmdType = GET_CUR_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            slCurPos.clear();
            for (size_t i = 0;i < 6;i++)//大小端不一样采样移位操作读取数据
            {
                slCurPos.push_back(EndianTrans(pstRecvMsg->stRecvData.mpCartPosRstData.lPos[i]));
            }
            break;
        default:
            break;
        }
        break;
    case 0x03:
        switch(pstRecvMsg->stRecvOrder.scProperty)
        {
        case 0x01:
            emCtrlCmdType = MOVJ_ABS_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x02:
            emCtrlCmdType = MOVJ_REL_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x03:
            emCtrlCmdType = MOVJ_INIT_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x04:
            emCtrlCmdType = MOVL_ABS_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x05:
            emCtrlCmdType = MOVL_REL_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        default:
            break;
        }
        break;
    case 0x04:
        switch(pstRecvMsg->stRecvOrder.scProperty)
        {
        case 0x01:
            emCtrlCmdType = START_RT_CTRL;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x02:
            emCtrlCmdType = STOP_RT_CTRL;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x03:
            emCtrlCmdType = MOVRT_ABS_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x04:
            emCtrlCmdType = MOVRT_REL_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x05:
            emCtrlCmdType = START_GET_RT_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        case 0x06:
            emCtrlCmdType = STOP_GET_RT_POS;
            usErrCode = EndianTrans(pstRecvMsg->stRecvData.mpStdRspData.usErrNo);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
