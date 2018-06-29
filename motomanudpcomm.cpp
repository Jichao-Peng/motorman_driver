#include "motomanudpcomm.h"

MotomanUDPComm::MotomanUDPComm(std::condition_variable& CondMsg,const QString& HostIP,quint16 HostPort,QObject* parent)
    :QObject(parent),cvCondMsg(CondMsg)
{
    qUdpSocket = new QUdpSocket(this);
    spMotomanRobotStatus = std::make_shared<MotomanRobotStatus>(cvCondMsg,this);
    qRobotHostIP = HostIP;
    usRobotHostPort = HostPort;

    connect(qUdpSocket,&QUdpSocket::connected,this,&MotomanUDPComm::ConnectedHandle);
    connect(qUdpSocket,&QUdpSocket::disconnected,this,&MotomanUDPComm::DisconnectedHandle);
    connect(qUdpSocket,&QUdpSocket::readyRead,this,&MotomanUDPComm::ReceiveData);
}

MotomanUDPComm::~MotomanUDPComm()
{
    qUdpSocket->close();
    delete qUdpSocket;
}

void MotomanUDPComm::Start(void)
{
    qUdpSocket->abort();
    qUdpSocket->bind(usRobotHostPort);
    qUdpSocket->connectToHost(qRobotHostIP,usRobotHostPort);
}

void MotomanUDPComm::Stop(void)
{
    qUdpSocket->disconnectFromHost();
}

void MotomanUDPComm::ConnectedHandle()
{
    Q_EMIT Connected();
}

void MotomanUDPComm::DisconnectedHandle()
{
    Q_EMIT Disconnected();
}

void MotomanUDPComm::ReceiveData(void)
{
    int siRecvFlag;
    ST_RECV_MSG* pstRecvMsgTmp = new ST_RECV_MSG;
    siRecvFlag = qUdpSocket->read(scRecvBuff,sizeof(scRecvBuff));
    qDebug("siRecvFlag is %d\n",siRecvFlag);
    memcpy(pstRecvMsgTmp,scRecvBuff,sizeof(ST_RECV_MSG));
    spMotomanRobotStatus->Unpack(pstRecvMsgTmp);
    Q_EMIT DataUpdated();
}

std::shared_ptr<MotomanRobotStatus> MotomanUDPComm::GetRobotStatus(void)
{
   return spMotomanRobotStatus;
}
