#include "motomanobserver.h"

Observer::Observer(QObject* parent):QObject(parent)
{}

Observer::~Observer()
{}

void Observer::TcpConnected()
{
    Q_EMIT tcpconnected();
}

void Observer::TcpDisconnected()
{
    Q_EMIT tcpdisconnected();
}

void Observer::UdpConnected()
{
    Q_EMIT udpconnected();
}

void Observer::UdpDisconnected()
{
    Q_EMIT udpdisconnected();
}

void Observer::Update(std::vector<long> CurPos, unsigned short ErrCode, MOTOMAN_CTRL_CMD CtrlCmdType)
{
    slCurPos.assign(CurPos.begin(),CurPos.end());
    usErrCode = ErrCode;
    emCtrlCmdType = CtrlCmdType;
    Q_EMIT update();
}
