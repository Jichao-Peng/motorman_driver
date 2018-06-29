#ifndef MOTOMANROBOTDRIVER_H
#define MOTOMANROBOTDRIVER_H

#include "motomancomm.h"
#include <string>
#include "motomanobserver.h"

class Observer;

class MotomanRobotDriver : public QObject
{
    Q_OBJECT
public:
    MotomanRobotDriver(QObject* parent);
    ~MotomanRobotDriver();

    void Start(QString sRobotHostIP,quint16 usRobotTCPHostPort,quint16 usRobotUDPHostPort);
    void Stop();

    void TcpConnectedHandle(void);
    void TcpDisconnectedHandle(void);
    void UdpConnectedHandle(void);
    void UdpDisconnectedHandle(void);
    void DataUpdatedHandle(void);

    void Attach(const std::shared_ptr<Observer>& Observer);
    void Notify(std::function<void(std::shared_ptr<Observer>& Observer)> fun);

    MotomanComm* pcMotomanComm;

protected:
    std::condition_variable cvTCPCondMsg;
    std::condition_variable cvUDPCondMsg;
    QString sRobotHostIP;
    quint16 usRobotTCPHostPort;
    quint16 usRobotUDPHostPort;

    std::vector<std::shared_ptr<Observer>> ObserverList;
};



#endif // MOTOMANROBOTDRIVER_H
