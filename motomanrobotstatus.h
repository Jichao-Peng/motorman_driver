#ifndef MOTOMANROBOTSTATUS_H
#define MOTOMANROBOTSTATUS_H

#include "motomanudpcomm.h"
#include "motomantcpcomm.h"
typedef struct
{
    char scChannal;
    char scProperty;
}ST_RECV_ORDER;

typedef struct
{
    unsigned short usErrNo;
    char scReserved[2];
}MP_STD_RSP_DATA;

#define MAX_CART_AXES			(6)
typedef struct
{
    long	lPos[MAX_CART_AXES];
    short	sConfig;
    char	reserved[2];
} MP_CART_POS_RSP_DATA;

typedef struct
{
    MP_STD_RSP_DATA mpStdRspData;
    MP_CART_POS_RSP_DATA mpCartPosRstData;
}ST_RECV_DATA;

typedef struct
{
    ST_RECV_DATA stRecvData;
    ST_RECV_ORDER stRecvOrder;
}ST_RECV_MSG;//接收结构体


class MotomanRobotStatus : public QObject
{
    Q_OBJECT
public:
    MotomanRobotStatus(std::condition_variable& CondMsg,QObject* parent);
    ~MotomanRobotStatus();

    void Unpack(ST_RECV_MSG* pstRecvMsgTmp);

    std::vector<long> slCurPos;
    unsigned short usErrCode;
    MOTOMAN_CTRL_CMD emCtrlCmdType;

protected:
    std::condition_variable& cvCondMsg;

    unsigned long EndianTrans(unsigned long RawData);
    long EndianTrans(long RawData);
    short EndianTrans(short RawData);
    unsigned short EndianTrans(unsigned short RawData);
};


#endif // MOTOMANROBOTSTATUS_H
