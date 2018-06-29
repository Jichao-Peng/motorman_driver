#ifndef MOTOMANTCPCOMM_H
#define MOTOMANTCPCOMM_H

#include <QSemaphore>
#include <QObject>
#include <condition_variable>
#include <QThread>
#include <QTcpSocket>
#include <vector>

typedef enum
{
    SERVO_ON,
    SERVO_OFF,
    CLEAR_ALARM,
    CLEAR_ERROR,
    GET_CUR_POS,
    MOVJ_ABS_POS,
    MOVJ_REL_POS,
    MOVJ_INIT_POS,
    MOVL_ABS_POS,
    MOVL_REL_POS,
    PULSE_MOVJ_ABS_POS,
    PULSE_MOVJ_REL_POS,
    PULSE_MOVL_ABS_POS,
    PULSE_MOVL_REL_POS,
    START_RT_CTRL,
    STOP_RT_CTRL,
    MOVRT_ABS_POS,
    MOVRT_REL_POS,
    PULSE_MOVRT_ABS_POS,
    PULSE_MOVRT_REL_POS,
    START_GET_RT_POS,
    STOP_GET_RT_POS,
    SET_IO,
}MOTOMAN_CTRL_CMD;

typedef struct
{
    char scChannal;
    char scProperty;
}ST_SEND_ORDER;

typedef struct
{
    unsigned long ulAddr;
    unsigned long ulValue;
}MP_IO_DATA;

#define	MAX_NO_OF_AXES	12
typedef struct
{
    unsigned long ulCtrlGrp;
    long  slSpeed;
    short ssVType;
    short ssFrame;
    short ssToolNo;
    short ssConfig;
    long  slPos[MAX_NO_OF_AXES];
    MP_IO_DATA mpIOData;
}ST_SEND_DATA;

typedef struct
{
    ST_SEND_DATA stSendData;
    ST_SEND_ORDER stSendOrder;
}ST_SEND_MSG;//发送结构体

Q_DECLARE_METATYPE(ST_SEND_MSG)

typedef struct
{
    int bit1 : 8;
    int bit2 : 8;
    int bit3 : 8;
    int bit4 : 8;
}FOUR_BITS;

typedef struct
{
    short bit1 : 8;
    short bit2 : 8;
}TWO_BITS;

typedef union
{
    unsigned long ulData;
    long slData;
    FOUR_BITS Bits;
}FOUR_BITS_DATA;

typedef union
{
    short ssData;
    TWO_BITS Bits;
}TWO_BITS_DATA;


class MotomanTCPComm : public QObject
{
    Q_OBJECT
public:
    MotomanTCPComm(std::condition_variable& CondMsg,const QString HostIP,quint16 HostPort,QObject* parent = nullptr);
    ~MotomanTCPComm();

    void ExecuteCmd(MOTOMAN_CTRL_CMD MotomanCtrlCmd);
    void ClearCmd(void);
    void SetSendData(std::vector<long> slTarPos,long slTarSpeed);
    void SetSendIO(unsigned long ulIOAddr,unsigned long ulIOValue);

    void Start(void);
    void Stop(void);

public:
    std::vector<long> slTargetPos;
    long slTargetSpeed;
    unsigned long ulIOAddr;
    unsigned long ulIOValue;

protected:
    std::condition_variable& cvCondMsg;
    QString qRobotHostIP;
    quint16 usRobotHostPort;
    QTcpSocket* qTcpSocket;

    ST_SEND_MSG* pstSendMsg;    

private:
    unsigned long EndianTrans(unsigned long RawData);
    long EndianTrans(long RawData);
    short EndianTrans(short RawData);
    unsigned short EndianTrans(unsigned short RawData);
    void EndianTransForSend(ST_SEND_DATA* pstRawData,ST_SEND_DATA* pstSendData);

public:
    void ConnectedHandle(void);
    void DisconnectedHandle(void);


Q_SIGNALS:
    Connected();
    Disconnected();
    SendCmd();

private slots:
    void SendCmdHandle();
};




class MotomanTCPCommCtrl : public QObject
{
    Q_OBJECT
protected:
    QThread qTCPThread;

public:
    MotomanTCPComm* pcMotomanTCPComm;
public:
    MotomanTCPCommCtrl(std::condition_variable& CondMsg,const QString HostIP,quint16 HostPort,QObject* parent):QObject(parent)
    {
        pcMotomanTCPComm = new MotomanTCPComm(CondMsg,HostIP,HostPort);
        pcMotomanTCPComm->moveToThread(&qTCPThread);
        connect(&qTCPThread,&QThread::finished,pcMotomanTCPComm,&QObject::deleteLater);
        connect(this,&MotomanTCPCommCtrl::Start,pcMotomanTCPComm,&MotomanTCPComm::Start);
        connect(this,&MotomanTCPCommCtrl::Stop,pcMotomanTCPComm,&MotomanTCPComm::Stop);
        qTCPThread.start();
    }
    
    ~MotomanTCPCommCtrl()
    {
        qTCPThread.quit();
        qTCPThread.wait();
    }
    
    void StartTCPComm()
    {
        Q_EMIT Start();
    }

    void StopTCPComm()
    {
        Q_EMIT Stop();
    }

    
Q_SIGNALS:
    void Start();
    void Stop();
};
#endif // MOTOMANTCPCOMM_H
