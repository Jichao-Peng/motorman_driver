#ifndef MOTOMANDRIVER_H
#define MOTOMANDRIVER_H

#include <QDialog>
#include <string>
#include "motomanrobotdriver.h"
#include "motomanobserver.h"
#include "motomancanvas.h"
#include <QTimer>

namespace Ui {
class MotomanDriver;
}

class MotomanDriver : public QDialog
{
    Q_OBJECT

public:
    explicit MotomanDriver(QWidget *parent = 0);
    ~MotomanDriver();

    MotomanRobotDriver* pcMotomanRobotDriver;
    MotomanCanvas* pcMotomanCanvas;
    QTimer* timer;
    std::shared_ptr<Observer> spObserver;

private:
    Ui::MotomanDriver* ui;
    char scTCPConnectedFlag;
    char scUDPConnectedFlag;

    char scStartBoardCtrlFlag;

protected:
    std::vector<long> slTarPos;
    long slTarSpeed;


protected:
    void GetTarInfo(void);

private slots:
    void TcpConnectedHandle();
    void TcpDisconnectedHandle();
    void UdpConnectedHandle();
    void UdpDisconnectedHandle();
    void UpdateHandle();
    void LineEditUpdate();
    void SetLineEditSliderValue();
    void MovRTAbsPosbySlider();
    void SetLineEditBoardValue();
    void MovRTAbsPosbyBoard();

    void on_ConnectButton_clicked();
    void on_DisconnectButton_clicked();
    void on_ServoONButton_clicked();
    void on_ServoOFFButton_clicked();
    void on_GetCurPosButton_clicked();
    void on_GetInitPosButton_clicked();
    void on_ClearErrorButton_clicked();
    void on_ClearAlarmButton_clicked();
    void on_SendAbsPosButtonMovL_clicked();
    void on_SendRelPosButtonMovL_clicked();
    void on_SendAbsPosButtonMovJ_clicked();
    void on_SendRelPosButtonMovJ_clicked();
    void on_StartRealTimeCtrlButton_clicked();
    void on_StopRealTimeCtrlButton_clicked();
    void on_SendAbsPosButtonRT_clicked();
    void on_SendRelPosButtonRT_clicked();
    void on_StartGetRTPosButton_clicked();
    void on_StopGetRTPosButton_clicked();
    void on_StartSlideSCtrlButton_clicked();
    void on_StopSlideCtrlButton_clicked();
    void on_StartBoardButton_clicked();
    void on_StopBoardCtrlButton_clicked();
};

#endif // MOTOMANDRIVER_H
