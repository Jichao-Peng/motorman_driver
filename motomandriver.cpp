#include "motomandriver.h"
#include "motomanobserver.h"
#include "ui_motomandriver.h"
#include <QDebug>
#include "motomancanvas.h"
#include <QTime>
class Observer;

MotomanDriver::MotomanDriver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotomanDriver)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    timer->start(4);

    pcMotomanRobotDriver = new MotomanRobotDriver(this);
    pcMotomanCanvas = new MotomanCanvas(this);
    spObserver = std::make_shared<Observer>(this);
    pcMotomanRobotDriver->Attach(spObserver);
    Observer *rawObserver = spObserver.get();

    connect(rawObserver,SIGNAL(tcpconnected()),this,SLOT(TcpConnectedHandle()));
    connect(rawObserver,SIGNAL(tcpdisconnected()),this,SLOT(TcpDisconnectedHandle()));
    connect(rawObserver,SIGNAL(udpconnected()),this,SLOT(UdpConnectedHandle()));
    connect(rawObserver,SIGNAL(udpdisconnected()),this,SLOT(UdpDisconnectedHandle()));
    connect(rawObserver,SIGNAL(update()),this,SLOT(UpdateHandle()));    
    connect(timer,SIGNAL(timeout()),pcMotomanCanvas,SLOT(update()));
    connect(timer,SIGNAL(timeout()),this,SLOT(LineEditUpdate()));
    connect(timer,SIGNAL(timeout()),this,SLOT(MovRTAbsPosbyBoard()));
    connect(timer,SIGNAL(timeout()),this,SLOT(SetLineEditBoardValue()));

    scTCPConnectedFlag = 0;
    scUDPConnectedFlag = 0;

    ui->IPLineEdit->setText("192.168.1.198");
    ui->TCPPortLineEdit->setText("11000");
    ui->UDPPortLineEdit->setText("12000");
    ui->InformationLabel->setText(tr("开始运行，请点击网络连接按钮"));

    ui->DisconnectButton->setEnabled(false);
    ui->GetCurPosButton->setEnabled(false);
    ui->GetInitPosButton->setEnabled(false);
    ui->SendAbsPosButtonMovJ->setEnabled(false);
    ui->SendRelPosButtonMovJ->setEnabled(false);
    ui->SendAbsPosButtonMovL->setEnabled(false);
    ui->SendRelPosButtonMovL->setEnabled(false);
    ui->SendAbsPosButtonRT->setEnabled(false);
    ui->SendRelPosButtonRT->setEnabled(false);
    ui->StartRealTimeCtrlButton->setEnabled(false);
    ui->StopRealTimeCtrlButton->setEnabled(false);
    ui->ClearAlarmButton->setEnabled(false);
    ui->ClearErrorButton->setEnabled(false);
    ui->ServoOFFButton->setEnabled(false);
    ui->ServoONButton->setEnabled(false);
    ui->StartGetRTPosButton->setEnabled(false);
    ui->StartGetRTPosButton->setEnabled(false);
//    ui->StartBoardButton->setEnabled(false);
//    ui->StopBoardCtrlButton->setEnabled(false);
    ui->StartSlideSCtrlButton->setEnabled(false);
    ui->StopSlideCtrlButton->setEnabled(false);
}

MotomanDriver::~MotomanDriver()
{
    delete pcMotomanRobotDriver;
}

void MotomanDriver::GetTarInfo()
{
    slTarPos.clear();
    slTarPos.push_back(ui->TarXLineEdit->text().toLong());
    slTarPos.push_back(ui->TarYLineEdit->text().toLong());
    slTarPos.push_back(ui->TarZLineEdit->text().toLong());
    slTarPos.push_back(ui->TarRxLineEdit->text().toLong());
    slTarPos.push_back(ui->TarRyLineEdit->text().toLong());
    slTarPos.push_back(ui->TarRzLineEdit->text().toLong());
    slTarSpeed = ui->TarVLineEdit->text().toLong();
}

void MotomanDriver::TcpConnectedHandle()
{
    scTCPConnectedFlag = 1;
    if(scUDPConnectedFlag)
    {
        ui->InformationLabel->setText(tr("Tcp和Udp均连接成功"));

        ui->DisconnectButton->setEnabled(true);
        ui->GetCurPosButton->setEnabled(true);
        ui->GetInitPosButton->setEnabled(true);
        ui->SendAbsPosButtonMovJ->setEnabled(true);
        ui->SendRelPosButtonMovJ->setEnabled(true);
        ui->SendAbsPosButtonMovL->setEnabled(true);
        ui->SendRelPosButtonMovL->setEnabled(true);
        ui->ClearAlarmButton->setEnabled(true);
        ui->ClearErrorButton->setEnabled(true);
        ui->ServoOFFButton->setEnabled(true);
        ui->ServoONButton->setEnabled(true);
    }
    else
    {
         ui->InformationLabel->setText(tr("仅Tcp连接成功"));
    }
}

void MotomanDriver::TcpDisconnectedHandle()
{
    scTCPConnectedFlag = 0;
    if(scTCPConnectedFlag)
    {
        ui->InformationLabel->setText(tr("仅Tcp连接失败"));
    }
    else
    {
         ui->InformationLabel->setText(tr("Tcp和Udp均连接失败"));
    }
}

void MotomanDriver::UdpConnectedHandle()
{
    scUDPConnectedFlag = 1;
    if(scTCPConnectedFlag)
    {
        ui->InformationLabel->setText(tr("Tcp和Udp均连接成功"));
        ui->DisconnectButton->setEnabled(false);
        ui->GetCurPosButton->setEnabled(false);
        ui->GetInitPosButton->setEnabled(false);
        ui->SendAbsPosButtonMovJ->setEnabled(false);
        ui->SendRelPosButtonMovJ->setEnabled(false);
        ui->SendAbsPosButtonMovL->setEnabled(false);
        ui->SendRelPosButtonMovL->setEnabled(false);
        ui->SendAbsPosButtonRT->setEnabled(false);
        ui->SendRelPosButtonRT->setEnabled(false);
        ui->StartRealTimeCtrlButton->setEnabled(false);
        ui->StopRealTimeCtrlButton->setEnabled(false);
        ui->ClearAlarmButton->setEnabled(false);
        ui->ClearErrorButton->setEnabled(false);
        ui->ServoOFFButton->setEnabled(false);
        ui->ServoONButton->setEnabled(false);
        ui->StartGetRTPosButton->setEnabled(false);
        ui->StartGetRTPosButton->setEnabled(false);
        ui->StartBoardButton->setEnabled(false);
        ui->StopBoardCtrlButton->setEnabled(false);
        ui->StartSlideSCtrlButton->setEnabled(false);
        ui->StopSlideCtrlButton->setEnabled(false);
    }
    else
    {
        ui->InformationLabel->setText(tr("仅Udp连接成功"));
    }
}

void MotomanDriver::UdpDisconnectedHandle()
{
    scUDPConnectedFlag = 0;
    if(scTCPConnectedFlag)
    {
        ui->InformationLabel->setText(tr("仅Udp连接失败"));
    }
    else
    {
        ui->InformationLabel->setText(tr("Tcp和Udp均连接失败"));
    }
}

void MotomanDriver::UpdateHandle()
{
    if(spObserver->slCurPos.size() == 6)
    {
        ui->CurXLabel->setText(tr("%1").arg(spObserver->slCurPos[0]));
        ui->CurYLabel->setText(tr("%1").arg(spObserver->slCurPos[1]));
        ui->CurZLabel->setText(tr("%1").arg(spObserver->slCurPos[2]));
        ui->CurRxLabel->setText(tr("%1").arg(spObserver->slCurPos[3]));
        ui->CurRyLabel->setText(tr("%1").arg(spObserver->slCurPos[4]));
        ui->CurRzLabel->setText(tr("%1").arg(spObserver->slCurPos[5]));
    }
    ui->InformationLabel->setText(tr("执行的命令是 %1，错误代码为 %2").arg(spObserver->emCtrlCmdType).arg(spObserver->usErrCode));
}

void MotomanDriver::SetLineEditSliderValue()
{
    ui->TarXLineEdit->setText(tr("%1").arg(ui->TarXSlider->value()));
    ui->TarYLineEdit->setText(tr("%1").arg(ui->TarYSlider->value()));
    ui->TarZLineEdit->setText(tr("%1").arg(ui->TarZSlider->value()));
    ui->TarRxLineEdit->setText(tr("%1").arg(ui->TarRxSlider->value()));
    ui->TarRyLineEdit->setText(tr("%1").arg(ui->TarRySlider->value()));
    ui->TarRzLineEdit->setText(tr("%1").arg(ui->TarRzSlider->value()));
    ui->TarVLineEdit->setText(tr("%1").arg(ui->TarVSlider->value()));
}

void MotomanDriver::MovRTAbsPosbySlider()
{
    GetTarInfo();
    pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
    pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
    pcMotomanRobotDriver->pcMotomanComm->MovRTAbsPos();
}

void MotomanDriver::SetLineEditBoardValue()
{
    if(scStartBoardCtrlFlag)
    {
        ui->TarXLineEdit->setText(tr("%1").arg(pcMotomanCanvas->slTarPos[0]));
        ui->TarYLineEdit->setText(tr("%1").arg(pcMotomanCanvas->slTarPos[1]));
        ui->TarZLineEdit->setText(tr("%1").arg(pcMotomanCanvas->slTarPos[2]));
        ui->TarRxLineEdit->setText(tr("%1").arg(pcMotomanCanvas->slTarPos[3]));
        ui->TarRyLineEdit->setText(tr("%1").arg(pcMotomanCanvas->slTarPos[4]));
        ui->TarRzLineEdit->setText(tr("%1").arg(pcMotomanCanvas->slTarPos[5]));

        pcMotomanCanvas->slCurPos[0] = spObserver->slCurPos[0];
        pcMotomanCanvas->slCurPos[1] = spObserver->slCurPos[1];
    }
}

void MotomanDriver::MovRTAbsPosbyBoard()
{
    if(scStartBoardCtrlFlag)
    {
        GetTarInfo();
        pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
        pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
        pcMotomanRobotDriver->pcMotomanComm->MovRTAbsPos();
    }
}

void MotomanDriver::LineEditUpdate()
{
    ui->PosX->setText(tr("%1").arg(pcMotomanCanvas->PosX));
    ui->PosY->setText(tr("%1").arg(pcMotomanCanvas->PosY));
}
//###########################UI界面槽函数###########################
void MotomanDriver::on_ConnectButton_clicked()
{
    ui->InformationLabel->setText(tr("开始连接..."));
    pcMotomanRobotDriver->Start(ui->IPLineEdit->text(),ui->TCPPortLineEdit->text().toInt(),ui->UDPPortLineEdit->text().toInt());
}

void MotomanDriver::on_DisconnectButton_clicked()
{
    ui->InformationLabel->setText(tr("停止连接"));
    pcMotomanRobotDriver->Stop();
}

void MotomanDriver::on_ServoONButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->ServoON();

    ui->SendAbsPosButtonRT->setEnabled(true);
    ui->SendRelPosButtonRT->setEnabled(true);
    ui->StartRealTimeCtrlButton->setEnabled(true);
    ui->StopRealTimeCtrlButton->setEnabled(true);
    ui->StartGetRTPosButton->setEnabled(true);
    ui->StartGetRTPosButton->setEnabled(true);
    ui->StartBoardButton->setEnabled(true);
    ui->StopBoardCtrlButton->setEnabled(true);
    ui->StartSlideSCtrlButton->setEnabled(true);
    ui->StopSlideCtrlButton->setEnabled(true);
}

void MotomanDriver::on_ServoOFFButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->ServoOFF();

    ui->SendAbsPosButtonRT->setEnabled(false);
    ui->SendRelPosButtonRT->setEnabled(false);
    ui->StartRealTimeCtrlButton->setEnabled(false);
    ui->StopRealTimeCtrlButton->setEnabled(false);
    ui->StartGetRTPosButton->setEnabled(false);
    ui->StartGetRTPosButton->setEnabled(false);
    ui->StartBoardButton->setEnabled(false);
    ui->StopBoardCtrlButton->setEnabled(false);
    ui->StartSlideSCtrlButton->setEnabled(false);
    ui->StopSlideCtrlButton->setEnabled(false);
}

void MotomanDriver::on_GetCurPosButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->GetCurPos();
}

void MotomanDriver::on_GetInitPosButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->MovJInitPos();
}

void MotomanDriver::on_ClearErrorButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->ClearError();
}

void MotomanDriver::on_ClearAlarmButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->ClearAlarm();
}

void MotomanDriver::on_SendAbsPosButtonMovL_clicked()
{
    GetTarInfo();
    pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
    pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
    pcMotomanRobotDriver->pcMotomanComm->MovLAbsPos();
}

void MotomanDriver::on_SendRelPosButtonMovL_clicked()
{
    GetTarInfo();
    pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
    pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
    pcMotomanRobotDriver->pcMotomanComm->MovLRelPos();
}

void MotomanDriver::on_SendAbsPosButtonMovJ_clicked()
{
    GetTarInfo();
    pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
    pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
    pcMotomanRobotDriver->pcMotomanComm->MovJAbsPos();
}

void MotomanDriver::on_SendRelPosButtonMovJ_clicked()
{
    GetTarInfo();
    pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
    pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
    pcMotomanRobotDriver->pcMotomanComm->MovJRelPos();
}

void MotomanDriver::on_StartRealTimeCtrlButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->StartRTCtrl();
}

void MotomanDriver::on_StopRealTimeCtrlButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->StopRTCtrl();
}

void MotomanDriver::on_SendAbsPosButtonRT_clicked()
{
    GetTarInfo();
    pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
    pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
    pcMotomanRobotDriver->pcMotomanComm->MovRTAbsPos();
}

void MotomanDriver::on_SendRelPosButtonRT_clicked()
{
    GetTarInfo();
    pcMotomanRobotDriver->pcMotomanComm->SetTarPos(slTarPos);
    pcMotomanRobotDriver->pcMotomanComm->SetTarSpeed(slTarSpeed);
    pcMotomanRobotDriver->pcMotomanComm->MovRTRelPos();
}
void MotomanDriver::on_StartGetRTPosButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->StartGetRTPos();
}

void MotomanDriver::on_StopGetRTPosButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->StopGetRTPos();
}

void MotomanDriver::on_StartSlideSCtrlButton_clicked()
{
    pcMotomanRobotDriver->pcMotomanComm->StartGetRTPos();

    ui->TarXSlider->setMinimum(-100000);
    ui->TarXSlider->setMaximum(100000);
    ui->TarXSlider->setSingleStep(200000);
    ui->TarXSlider->setValue(spObserver->slCurPos[0]);
    ui->TarXLineEdit->setText(tr("%1").arg(ui->TarXSlider->value()));

    ui->TarYSlider->setMinimum(-100000);
    ui->TarYSlider->setMaximum(100000);
    ui->TarYSlider->setSingleStep(200000);
    ui->TarYSlider->setValue(spObserver->slCurPos[1]);
    ui->TarYLineEdit->setText(tr("%1").arg(ui->TarYSlider->value()));

    ui->TarZSlider->setMinimum(400000);
    ui->TarZSlider->setMaximum(500000);
    ui->TarZSlider->setSingleStep(100000);
    ui->TarZSlider->setValue(spObserver->slCurPos[2]);
    ui->TarZLineEdit->setText(tr("%1").arg(ui->TarZSlider->value()));

    ui->TarRxSlider->setMinimum(-1000000);
    ui->TarRxSlider->setMaximum(1000000);
    ui->TarRxSlider->setSingleStep(200000);
    ui->TarRxSlider->setValue(spObserver->slCurPos[3]);
    ui->TarRxLineEdit->setText(tr("%1").arg(ui->TarRxSlider->value()));

    ui->TarRySlider->setMinimum(-100000);
    ui->TarRySlider->setMaximum(100000);
    ui->TarRySlider->setSingleStep(200000);
    ui->TarRySlider->setValue(spObserver->slCurPos[4]);
    ui->TarRyLineEdit->setText(tr("%1").arg(ui->TarRySlider->value()));

    ui->TarRzSlider->setMinimum(-100000);
    ui->TarRzSlider->setMaximum(100000);
    ui->TarRzSlider->setSingleStep(200000);
    ui->TarRzSlider->setValue(spObserver->slCurPos[5]);
    ui->TarRzLineEdit->setText(tr("%1").arg(ui->TarRzSlider->value()));

    ui->TarVSlider->setMinimum(0);
    ui->TarVSlider->setMaximum(4000);
    ui->TarVSlider->setSingleStep(2000);
    ui->TarVSlider->setValue(100);
    ui->TarVLineEdit->setText(tr("%1").arg(100));

    connect(ui->TarXSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
    connect(ui->TarYSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
    connect(ui->TarZSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
    connect(ui->TarRxSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
    connect(ui->TarRySlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
    connect(ui->TarRzSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
    connect(ui->TarVSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));

    connect(ui->TarXSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
    connect(ui->TarYSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
    connect(ui->TarZSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
    connect(ui->TarRxSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
    connect(ui->TarRySlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
    connect(ui->TarRzSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
    connect(ui->TarVSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
}

void MotomanDriver::on_StopSlideCtrlButton_clicked()
{
     pcMotomanRobotDriver->pcMotomanComm->StopGetRTPos();

     disconnect(ui->TarXSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
     disconnect(ui->TarYSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
     disconnect(ui->TarZSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
     disconnect(ui->TarRxSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
     disconnect(ui->TarRySlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
     disconnect(ui->TarRzSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));
     disconnect(ui->TarVSlider,SIGNAL(valueChanged(int)),this,SLOT(SetLineEditSliderValue()));

     disconnect(ui->TarXSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
     disconnect(ui->TarYSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
     disconnect(ui->TarZSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
     disconnect(ui->TarRxSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
     disconnect(ui->TarRySlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
     disconnect(ui->TarRzSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
     disconnect(ui->TarVSlider,SIGNAL(valueChanged(int)),this,SLOT(MovRTAbsPosbySlider()));
}

void MotomanDriver::on_StartBoardButton_clicked()
{    
//    pcMotomanRobotDriver->pcMotomanComm->StartGetRTPos();

    pcMotomanCanvas->slInitPos.assign(spObserver->slCurPos.begin(),spObserver->slCurPos.end());
    pcMotomanCanvas->slTarPos.assign(spObserver->slCurPos.begin(),spObserver->slCurPos.end());

    scStartBoardCtrlFlag = 1;

    ui->TarVLineEdit->setText("100");

}

void MotomanDriver::on_StopBoardCtrlButton_clicked()
{  
     scStartBoardCtrlFlag = 0;
}
