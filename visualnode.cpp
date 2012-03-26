#include "visualnode.h"
#include <qmath.h>
#include <QPoint>
VisualNode::VisualNode(){}
VisualNode::VisualNode(int _x, int _y, int _r, int _z, int _id, int _state):x(_x),y(_y),r(_r),z(_z),id(_id),state(_state)
{
   // neighbours = 0;
}
bool VisualNode::Inside(const QPoint &p)
{
    return pow(p.x()-x,2)+pow(p.y()-y,2)<pow(r,2);
}
int VisualNode::getZ(){return z;}
void VisualNode::setState(int s)
{
    if(state!=6 && state!=8)
        state=s;
}
void VisualNode::setPos(const QPoint &p)
{
    x=p.x();
    y=p.y();
}
void VisualNode::Paint(QPainter &painter)
{
    painter.setPen(QPen(Qt::black,3));
    painter.setBrush(Qt::SolidPattern);
    switch (state)
    {
    case 0:
        painter.setBrush(Qt::white);//default
        break;
    case 1:
        painter.setBrush(QColor(150,150,150));//drag
        break;
    case 2:
        painter.setBrush(Qt::blue);
        break;
    case 3:
        painter.setBrush(QColor(200,200,200));//hover
        break;
    case 4:
        painter.setBrush(QColor(255,200,20));//in stack
        break;
    case 5:
        painter.setBrush(Qt::gray);
        break;
    case 6:
        painter.setBrush(Qt::red);//finish
        break;
    case 7:
        painter.setBrush(QColor(255,100,100));
        break;
    case 8:
        painter.setBrush(Qt::green);//first
        break;
    }
    painter.drawEllipse(QPoint(x,y),r,r);
    painter.drawText(x-4,y+5, QString::number(id));
}

/*void VisualNode::PaintConnections(QPainter &painter)
{
    painter.setPen(Qt::blue);
    foreach(VisualNode* vn, neighbours)
    {
        painter.drawLine(QPoint(x,y),QPoint(vn->x,vn->y));
    }
}*/
