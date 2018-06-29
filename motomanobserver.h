#ifndef MOTOMANOBSERVER_H
#define MOTOMANOBSERVER_H

#include <vector>
#include "motomanrobotdriver.h"
#include <QObject>

class Observer : public QObject
{
    Q_OBJECT
public:
    Observer(QObject* parent);
    ~Observer();

public:
    void Update(std::vector<long> CurPos,unsigned short ErrCode,MOTOMAN_CTRL_CMD CtrlCmdType);
    void UdpConnected(void);
    void TcpConnected(void);
    void UdpDisconnected(void);
    void TcpDisconnected(void);

    std::vector<long> slCurPos;
    unsigned short usErrCode;
    MOTOMAN_CTRL_CMD emCtrlCmdType;

Q_SIGNALS:
    tcpconnected();
    tcpdisconnected();
    udpconnected();
    udpdisconnected();
    update();
};

#endif // MOTOMANOBSERVER_H
