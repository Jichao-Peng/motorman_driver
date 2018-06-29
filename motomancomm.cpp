#include "motomancomm.h"

MotomanComm::MotomanComm(std::condition_variable& TCPCondMsg,std::condition_variable& UDPCondMsg,const QString HostIP,quint16 TCPHostPort,quint16 UDPHostPort,QObject* parent)
    :QObject(parent)
{
    pcMotomanTCPCommCtrl = new MotomanTCPCommCtrl(TCPCondMsg,HostIP,TCPHostPort,this);
    pcMotomanUDPCommCtrl = new MotomanUDPCommCtrl(UDPCondMsg,HostIP,UDPHostPort,this);

    connect(pcMotomanTCPCommCtrl->pcMotomanTCPComm,&MotomanTCPComm::Connected,this,&MotomanComm::TcpConnectedHandle);
    connect(pcMotomanTCPCommCtrl->pcMotomanTCPComm,&MotomanTCPComm::Disconnected,this,&MotomanComm::TcpDisconnectedHandle);

    connect(pcMotomanUDPCommCtrl->pcMotomanUDPComm,&MotomanUDPComm::Connected,this,&MotomanComm::UdpConnectedHandle);
    connect(pcMotomanUDPCommCtrl->pcMotomanUDPComm,&MotomanUDPComm::Disconnected,this,&MotomanComm::UdpDisconnectedHandle);

    connect(pcMotomanUDPCommCtrl->pcMotomanUDPComm,&MotomanUDPComm::DataUpdated,this,&MotomanComm::DataUpdatedHandle);
}

MotomanComm::~MotomanComm()
{
    delete pcMotomanTCPCommCtrl;
    delete pcMotomanUDPCommCtrl;
}

void MotomanComm::Start()
{
    pcMotomanTCPCommCtrl->StartTCPComm();
    pcMotomanUDPCommCtrl->StartUDPComm();
}

void MotomanComm::Stop()
{
    pcMotomanTCPCommCtrl->StopTCPComm();
    pcMotomanUDPCommCtrl->StopUDPComm();
}

void MotomanComm::ServoON()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(SERVO_ON);
}

void MotomanComm::ServoOFF()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(SERVO_OFF);
}

void MotomanComm::ClearAlarm()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(CLEAR_ALARM);
}

void MotomanComm::ClearError()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(CLEAR_ERROR);
}

void MotomanComm::GetCurPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(GET_CUR_POS);
}

void MotomanComm::MovJAbsPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(MOVJ_ABS_POS);
}

void MotomanComm::MovJRelPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(MOVJ_REL_POS);
}

void MotomanComm::MovJInitPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(MOVJ_INIT_POS);
}

void MotomanComm::MovLAbsPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(MOVL_ABS_POS);
}

void MotomanComm::MovLRelPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(MOVL_REL_POS);
}

void MotomanComm::PulseMovJAbsPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(PULSE_MOVJ_ABS_POS);
}

void MotomanComm::PulseMovJRelPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(PULSE_MOVJ_REL_POS);
}


void MotomanComm::PulseMovLAbsPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(PULSE_MOVL_ABS_POS);
}

void MotomanComm::PulseMovLRelPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(PULSE_MOVL_REL_POS);}


void MotomanComm::StartRTCtrl()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(START_RT_CTRL);
}

void MotomanComm::StopRTCtrl()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(STOP_RT_CTRL);
}

void MotomanComm::MovRTAbsPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(MOVRT_ABS_POS);
}

void MotomanComm::MovRTRelPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(MOVRT_REL_POS);
}

void MotomanComm::PulseMovRTAbsPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(PULSE_MOVRT_ABS_POS);
}

void MotomanComm::PulseMovRTRelPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(PULSE_MOVRT_REL_POS);
}

void MotomanComm::StartGetRTPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(START_GET_RT_POS);
}

void MotomanComm::StopGetRTPos()
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(STOP_GET_RT_POS);
}

void MotomanComm::SetTarPos(std::vector<long> slTarPos)
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->slTargetPos.assign(slTarPos.begin(),slTarPos.end());
}

void MotomanComm::SetTarSpeed(long slTarSpeed)
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->slTargetSpeed = slTarSpeed;
}

void MotomanComm::SetDigitalIO(unsigned long ulIOAddr,unsigned long ulIOValue)
{
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ulIOAddr = ulIOAddr;
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ulIOValue = ulIOValue;
    pcMotomanTCPCommCtrl->pcMotomanTCPComm->ExecuteCmd(SET_IO);
}

void MotomanComm::TcpConnectedHandle()
{
    Q_EMIT TcpConnected();
}

void MotomanComm::TcpDisconnectedHandle()
{
    Q_EMIT TcpDisconnected();
}

void MotomanComm::UdpConnectedHandle()
{
    Q_EMIT UdpConnected();
}

void MotomanComm::UdpDisconnectedHandle()
{
    Q_EMIT UdpDisconnected();
}

void MotomanComm::DataUpdatedHandle(void)
{
    Q_EMIT DataUpdated();
}
