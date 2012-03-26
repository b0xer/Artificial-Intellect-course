#ifndef VISUALNODE_H
#define VISUALNODE_H
#include<QPoint>
#include<QPainter>
#include<QList>
#include<connection.h>
class Connection;
class VisualNode
{
public:
    int x;
    int y;
    int r;
    int id;
    VisualNode();
    VisualNode(int _x, int _y, int _r, int _z, int _id, int _state);
    bool Inside(const QPoint &p);
    int getZ();
    void setState(int s);
    void setPos(const QPoint &p);
    void Paint(QPainter& m);
    //void PaintConnections(QPainter& m);
    QList<Connection*> connections;
    int state;
    int z;
};

#endif // VISUALNODE_H
