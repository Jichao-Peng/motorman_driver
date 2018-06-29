#ifndef MOTOMANCOMM_H
#define MOTOMANCOMM_H

#include "motomantcpcomm.h"
#include "motomanudpcomm.h"

class MotomanComm : public QObject
{
    Q_OBJECT
public:
    MotomanComm(std::condition_variable& TCPCondMsg,std::condition_variable& UDPCondMsg,const QString HostIP,quint16 TCPHostPort,quint16 UDPHostPort,QObject* parent = 0);
    ~MotomanComm();

    void Start(void);//启动网络连接
    void Stop(void);//关闭网络连接
    void ServoON(void);//启动伺服
    void ServoOFF(void);//关闭伺服
    void ClearAlarm(void);//清除警告
    void ClearError(void);//清除错误
    void GetCurPos(void);//得到当前位置
    void MovJAbsPos(void);//在笛卡尔坐标系下以绝对位置的形式发送MOVJ
    void MovJRelPos(void);//在笛卡尔坐标系下以相对位置的形式发送MOVJ
    void MovJInitPos(void);//以MOVJ回到启动点
    void MovLAbsPos(void);//在笛卡尔坐标系下以绝对位置的形式发送MOVL
    void MovLRelPos(void);//在笛卡尔坐标系下以相对位置的形式发送MOVL
    void PulseMovJAbsPos(void);//在关节角坐标系下以绝对位置的形式发送MOVJ
    void PulseMovJRelPos(void);//在关节角坐标系下以相对位置的形式发送MOVJ
    void PulseMovLAbsPos(void);//在关节角坐标系下以绝对位置的形式发送MOVL
    void PulseMovLRelPos(void);//在关节角坐标系下以相对位置的形式发送MOVL
    void StartRTCtrl(void);//开启实时控制
    void StopRTCtrl(void);//关闭实时控制
    void MovRTAbsPos(void);//在笛卡尔坐标系下以绝对位置的形式发送实时控制
    void MovRTRelPos(void);//在笛卡尔坐标系下以绝对位置的形式发送实时控制
    void PulseMovRTAbsPos(void);//在关节角坐标系下以绝对位置的形式发送实时控制
    void PulseMovRTRelPos(void);//在关节角坐标系下以绝对位置的形式发送实时控制
    void StartGetRTPos(void);//实时查询位置
    void StopGetRTPos(void);//实时查询位置
    void SetDigitalIO(unsigned long ulIOAddr,unsigned long ulIOValue);//设置IO

    void SetTarPos(std::vector<long> slTarPos);//设置目标位置
    void SetTarSpeed(long slTarSpeed);//设置目标速度

public:
    MotomanTCPCommCtrl* pcMotomanTCPCommCtrl;
    MotomanUDPCommCtrl* pcMotomanUDPCommCtrl;

public:
    void TcpConnectedHandle(void);
    void TcpDisconnectedHandle(void);
    void UdpConnectedHandle(void);
    void UdpDisconnectedHandle(void);
    void DataUpdatedHandle(void);

Q_SIGNALS:
    TcpConnected();
    TcpDisconnected();
    UdpConnected();
    UdpDisconnected();
    DataUpdated();
};

#endif // MOTOMANCOMM_H
