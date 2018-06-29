#ifndef MOTOMANUDPCOMM_H
#define MOTOMANUDPCOMM_H

#include <QSemaphore>
#include <QObject>
#include <condition_variable>
#include <QThread>
#include <QUdpSocket>
#include <motomanrobotstatus.h>

class MotomanRobotStatus;

class MotomanUDPComm : public QObject
{
    Q_OBJECT
public:
    MotomanUDPComm(std::condition_variable& CondMsg,const QString& HostIP,quint16 HostPort,QObject* parent = nullptr);
    ~MotomanUDPComm();

    void Start(void);
    void Stop(void);
    std::shared_ptr<MotomanRobotStatus> GetRobotStatus(void);

protected:
    std::condition_variable& cvCondMsg;
    QString qRobotHostIP;
    quint16 usRobotHostPort;
    QUdpSocket* qUdpSocket;

    char scRecvBuff[256];
    std::shared_ptr<MotomanRobotStatus> spMotomanRobotStatus;

public:
    void ConnectedHandle(void);
    void DisconnectedHandle(void);

Q_SIGNALS:
    Connected();
    Disconnected();
    DataUpdated();

private Q_SLOTS:
    void ReceiveData(void);
};

class MotomanUDPCommCtrl : public QObject
{
    Q_OBJECT
protected:
    QThread qUDPThread;
public:
    MotomanUDPComm* pcMotomanUDPComm;
public:
    MotomanUDPCommCtrl(std::condition_variable &CondMsg,const QString& HostIP,quint16 HostPort,QObject* parent):QObject(parent)
    {
        pcMotomanUDPComm = new MotomanUDPComm(CondMsg,HostIP,HostPort);
        pcMotomanUDPComm->moveToThread(&qUDPThread);
        connect(&qUDPThread,&QThread::finished,pcMotomanUDPComm,&QObject::deleteLater);
        connect(this,&MotomanUDPCommCtrl::Start,pcMotomanUDPComm,&MotomanUDPComm::Start);
        connect(this,&MotomanUDPCommCtrl::Stop,pcMotomanUDPComm,&MotomanUDPComm::Stop);//
        qUDPThread.start();
    }
    ~MotomanUDPCommCtrl()
    {
        qUDPThread.quit();
        qUDPThread.wait();
    }

    void StartUDPComm()
    {
        Q_EMIT Start();
    }

    void StopUDPComm()
    {
        Q_EMIT Stop();
    }

Q_SIGNALS:
    void Start();
    void Stop();
};


#endif // MOTOMANUDPCOMM_H
