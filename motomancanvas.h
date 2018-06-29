#ifndef MOTOMANCANVAS_H
#define MOTOMANCANVAS_H

#include <QDialog>
#include <QMouseEvent>
#include <QToolTip>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QDebug>

class MotomanCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit MotomanCanvas(QWidget *parent);
    ~MotomanCanvas();
    std::vector<long> slInitPos;
    std::vector<long> slTarPos;
    std::vector<long> slCurPos;
    int PosX;
    int PosY;

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *evnet);

    int QPosX;
    int QPosY;
    int CurPosX;
    int CurPosY;
    int QCurPosX;
    int QCurPosY;
};


#endif // MOTOMANCANVAS_H
