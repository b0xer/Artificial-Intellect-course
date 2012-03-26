#include "connection.h"
#include "visualnode.h"
#include <qmath.h>
Connection::Connection(VisualNode* _n1, VisualNode* _n2, int _state, int _weight):n1(_n1), n2(_n2),state(_state), weight(_weight){}
void Connection::Paint(QPainter &painter)
{
    switch(state)
    {
    case 0:
        painter.setPen(QPen(QBrush(QColor(0,0,0)),8));
        painter.drawLine(n1->x,n1->y, n2->x,n2->y);
        painter.setPen(QPen(QBrush(QColor(255,255,255)),4));
        break;
    case 1:
         painter.setPen(QPen(QBrush(QColor(155,100,20)),5));
        break;
    case 2:
        painter.setPen(QPen(QBrush(QColor(200,200,200)),5));
        break;
    case 3:
        painter.setPen(QPen(QBrush(QColor(255,100,100)),7));
        break;
    }
    painter.drawLine(n1->x,n1->y, n2->x,n2->y);
    painter.setPen(QPen(QColor(240,240,240,0),1));
    for(int i=0;i<10;i++)
    {
        painter.setBrush(QBrush(QColor(240,240,240,25*i)));
        painter.drawEllipse(QPoint((n1->x+n2->x)/2,(n1->y+n2->y)/2),35-i*3.5,35-i*3.5);
    }
    painter.setPen(QPen(Qt::black,3));
    painter.drawText((n1->x+n2->x)/2-4-(QString::number(weight).length()-1)*4,(n1->y+n2->y)/2+5, QString::number(weight));
}

VisualNode* Connection::OtherOne(VisualNode *n)
{
    return n==n1?n2:n1;
}

bool Connection::Inside(const QPoint &p)
{
    int x=(n1->x+n2->x)/2;
    int y=(n1->y+n2->y)/2;
    return pow(p.x()-x,2)+pow(p.y()-y,2)<pow(15,2);
}

