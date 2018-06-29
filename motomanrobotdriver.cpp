#include "motomanrobotdriver.h"

MotomanRobotDriver::MotomanRobotDriver(QObject* parent) : QObject(parent)
{

}

MotomanRobotDriver::~MotomanRobotDriver()
{
    delete pcMotomanComm;
}

void MotomanRobotDriver::Start(QString sRobotHostIP,quint16 usRobotTCPHostPort,quint16 usRobotUDPHostPort)
{
    pcMotomanComm = new MotomanComm(cvTCPCondMsg,cvUDPCondMsg,sRobotHostIP,usRobotTCPHostPort,usRobotUDPHostPort,this);
    connect(pcMotomanComm,&MotomanComm::TcpConnected,this,&MotomanRobotDriver::TcpConnectedHandle);
    connect(pcMotomanComm,&MotomanComm::TcpDisconnected,this,&MotomanRobotDriver::TcpDisconnectedHandle);
    connect(pcMotomanComm,&MotomanComm::UdpConnected,this,&MotomanRobotDriver::UdpConnectedHandle);
    connect(pcMotomanComm,&MotomanComm::UdpDisconnected,this,&MotomanRobotDriver::UdpDisconnectedHandle);
    connect(pcMotomanComm,&MotomanComm::DataUpdated,this,&MotomanRobotDriver::DataUpdatedHandle);
    pcMotomanComm->Start();
}

void MotomanRobotDriver::Stop()
{
    pcMotomanComm->Stop();
}

void MotomanRobotDriver::Notify(std::function<void(std::shared_ptr<Observer>& Observer)> fun)
{
    //多线程需要在这里加互斥锁
    std::vector<std::shared_ptr<Observer>> ObserverListTmp;
    ObserverListTmp = ObserverList;


    for(auto& Observer:ObserverList)
    {
        fun(Observer);
    }
}

void MotomanRobotDriver::Attach(const std::shared_ptr<Observer>& Observer)
{
    for(auto& iter:ObserverList)
    {
        if(iter.get() == Observer.get())
            return;
    }

    if(Observer)
    {
        ObserverList.push_back(Observer);
    }
}

void MotomanRobotDriver::DataUpdatedHandle()
{
    auto auMotomanStatus = pcMotomanComm->pcMotomanUDPCommCtrl->pcMotomanUDPComm->GetRobotStatus();

    Notify([=](std::shared_ptr<Observer>& Observer){Observer->Update(auMotomanStatus->slCurPos,auMotomanStatus->usErrCode,auMotomanStatus->emCtrlCmdType);});
}

void MotomanRobotDriver::TcpConnectedHandle()
{
    Notify([=](std::shared_ptr<Observer>& Observer){Observer->TcpConnected();});
}

void MotomanRobotDriver::TcpDisconnectedHandle()
{
    Notify([=](std::shared_ptr<Observer>& Observer){Observer->TcpDisconnected();});
}

void MotomanRobotDriver::UdpConnectedHandle()
{
    Notify([=](std::shared_ptr<Observer>& Observer){Observer->UdpConnected();});
}

void MotomanRobotDriver::UdpDisconnectedHandle()
{
    Notify([=](std::shared_ptr<Observer>& Observer){Observer->UdpDisconnected();});
}
