#include "motomancanvas.h"

MotomanCanvas::MotomanCanvas(QWidget *parent):QWidget(parent)
{
    this->setGeometry(QRect(655,85,550,460));

    QPosX = 275;
    QPosY = 230;
    slInitPos.resize(6);
    slTarPos.resize(6);
    slCurPos.resize(6);
}

MotomanCanvas::~MotomanCanvas()
{}

void MotomanCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::white,3));
    painter.drawLine(QPoint(0,230),QPoint(550,230));
    painter.drawLine(QPoint(275,0),QPoint(275,460));

    painter.setPen(QPen(Qt::white,11));
    painter.drawEllipse(QPosX,QPosY,10,10);


    CurPosX = (slCurPos[0]-slInitPos[0])/200;
    CurPosY = (slCurPos[1]-slInitPos[1])/200;
    QCurPosX = CurPosX+275;
    QCurPosY = -CurPosY+230;
    painter.setPen(QPen(Qt::white,2));
    painter.drawEllipse(QCurPosX,QCurPosY,10,10);
}

void MotomanCanvas::mouseMoveEvent(QMouseEvent *event)
{
    QPosX = event->pos().x();
    QPosY = event->pos().y();

    PosX = (QPosX-275);
    PosY = -(QPosY-230);

    slTarPos[0] = slInitPos[0]+PosX*200;
    slTarPos[1] = slInitPos[1]+PosY*200;
}
