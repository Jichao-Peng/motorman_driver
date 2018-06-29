#include <QObject>
#include "motomantcpcomm.h"
#include <QDebug>

MotomanTCPComm::MotomanTCPComm(std::condition_variable& CondMsg,const QString HostIP,quint16 HostPort,QObject* parent)
    :QObject(parent),cvCondMsg(CondMsg)
{
    qTcpSocket = new QTcpSocket(this);
    pstSendMsg = new ST_SEND_MSG;

    qRobotHostIP = HostIP;
    usRobotHostPort = HostPort;

    connect(qTcpSocket,&QTcpSocket::connected,this,&MotomanTCPComm::ConnectedHandle);
    connect(qTcpSocket,&QTcpSocket::disconnected,this,&MotomanTCPComm::DisconnectedHandle);
    connect(this,&MotomanTCPComm::SendCmd,this,&MotomanTCPComm::SendCmdHandle);
}

MotomanTCPComm::~MotomanTCPComm()
{
    qTcpSocket->close();
    delete qTcpSocket;
    delete pstSendMsg;
}

void MotomanTCPComm::Start(void)
{
    qTcpSocket->connectToHost(qRobotHostIP,usRobotHostPort);
}

void MotomanTCPComm::Stop(void)
{
    qTcpSocket->disconnectFromHost();
}

void MotomanTCPComm::ExecuteCmd(MOTOMAN_CTRL_CMD MotomanCtrlCmd)
{
    switch(MotomanCtrlCmd)
    {
    case SERVO_ON:
        pstSendMsg->stSendOrder.scChannal = 0x01;
        pstSendMsg->stSendOrder.scProperty = 0x01;
        break;
    case SERVO_OFF:
        pstSendMsg->stSendOrder.scChannal = 0x01;
        pstSendMsg->stSendOrder.scProperty = 0x02;
        break;
    case CLEAR_ALARM:
        pstSendMsg->stSendOrder.scChannal = 0x01;
        pstSendMsg->stSendOrder.scProperty = 0x03;
        break;
    case CLEAR_ERROR:
        pstSendMsg->stSendOrder.scChannal = 0x01;
        pstSendMsg->stSendOrder.scProperty = 0x04;
        break;
    case GET_CUR_POS:
        pstSendMsg->stSendOrder.scChannal = 0x02;
        pstSendMsg->stSendOrder.scProperty = 0x01;
        break;
    case MOVJ_ABS_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x01;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case MOVJ_REL_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x02;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case MOVJ_INIT_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x03;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case MOVL_ABS_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x04;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case MOVL_REL_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x05;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case PULSE_MOVJ_ABS_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x06;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case PULSE_MOVJ_REL_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x07;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case PULSE_MOVL_ABS_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x08;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case PULSE_MOVL_REL_POS:
        pstSendMsg->stSendOrder.scChannal = 0x03;
        pstSendMsg->stSendOrder.scProperty = 0x09;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case START_RT_CTRL:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x01;
        break;
    case STOP_RT_CTRL:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x02;
        break;
    case MOVRT_ABS_POS:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x03;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case MOVRT_REL_POS:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x04;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case PULSE_MOVRT_ABS_POS:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x05;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case PULSE_MOVRT_REL_POS:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x06;
        SetSendData(slTargetPos,slTargetSpeed);
        break;
    case START_GET_RT_POS:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x07;
        break;
    case STOP_GET_RT_POS:
        pstSendMsg->stSendOrder.scChannal = 0x04;
        pstSendMsg->stSendOrder.scProperty = 0x08;
        break;
    case SET_IO:
        pstSendMsg->stSendOrder.scChannal = 0x05;
        pstSendMsg->stSendOrder.scProperty = 0x01;
        SetSendIO(ulIOAddr,ulIOValue);
    default:
        break;
    }
    Q_EMIT SendCmd();
}


void MotomanTCPComm::SendCmdHandle()
{
    int siSendFlag;
    ST_SEND_MSG stSendMsg;
    stSendMsg = *pstSendMsg;
    siSendFlag = qTcpSocket->write((char*)&stSendMsg,sizeof(stSendMsg));
    qDebug("siSendFlag is : %d,The channel is %d,The property is %d.\n",siSendFlag,stSendMsg.stSendOrder.scChannal,stSendMsg.stSendOrder.scProperty);

    ClearCmd();
}

void MotomanTCPComm::SetSendData(std::vector<long> slTarPos,long slTarSpeed)
{
    ST_SEND_DATA* pstRawData = new ST_SEND_DATA;
    if(slTarPos.size() == 6)
    {
        for(size_t i=0;i<slTarPos.size();i++)
        {
            pstRawData->slPos[i] = slTarPos[i];
        }
    }
    pstRawData->slSpeed = slTarSpeed;

    pstRawData->ssConfig = 0;
    pstRawData->ssFrame = 1;
    pstRawData->ulCtrlGrp = 0;
    pstRawData->ssToolNo = 0;

    EndianTransForSend(pstRawData,&pstSendMsg->stSendData);
    delete pstRawData;
}

void MotomanTCPComm::SetSendIO(unsigned long ulIOAddr, unsigned long ulIOValue)
{
    ST_SEND_DATA* pstRawData = new ST_SEND_DATA;
    pstRawData->mpIOData.ulAddr = ulIOAddr;
    pstRawData->mpIOData.ulValue = ulIOValue;

    EndianTransForSend(pstRawData,&pstSendMsg->stSendData);
    delete pstRawData;
}

void MotomanTCPComm::ClearCmd(void)
{
    memset(pstSendMsg,0,sizeof(*pstSendMsg));
}

unsigned long MotomanTCPComm::EndianTrans(unsigned long RawData)
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

long MotomanTCPComm::EndianTrans(long RawData)
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

short MotomanTCPComm::EndianTrans(short RawData)
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

unsigned short MotomanTCPComm::EndianTrans(unsigned short RawData)
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

void MotomanTCPComm::EndianTransForSend(ST_SEND_DATA* pstRawData,ST_SEND_DATA* pstSendData)
{
    pstSendData->ssConfig = EndianTrans(pstRawData->ssConfig);
    pstSendData->ssFrame = EndianTrans(pstRawData->ssFrame);
    pstSendData->ulCtrlGrp = EndianTrans(pstRawData->ulCtrlGrp);
    pstSendData->ssToolNo = EndianTrans(pstRawData->ssToolNo);
    pstSendData->slSpeed = EndianTrans(pstRawData->slSpeed);
    for(size_t i=0;i<6;i++)
    {
        pstSendData->slPos[i] = EndianTrans(pstRawData->slPos[i]);
    }
    pstSendData->mpIOData.ulAddr = EndianTrans(pstRawData->mpIOData.ulAddr);
    pstSendData->mpIOData.ulValue = EndianTrans(pstRawData->mpIOData.ulValue);
}

void MotomanTCPComm::ConnectedHandle(void)
{
    Q_EMIT Connected();
}

void MotomanTCPComm::DisconnectedHandle(void)
{
    Q_EMIT Disconnected();
}
