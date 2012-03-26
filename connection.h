#ifndef CONNECTION_H
#define CONNECTION_H
#include <QPoint>
#include <QPainter>
#include "visualnode.h"
class VisualNode;
class Connection
{
public:
    Connection(VisualNode* _n1, VisualNode* _n2, int _state, int _weight);
    bool Inside(const QPoint &p);
    void Paint(QPainter& m);
    VisualNode* OtherOne(VisualNode* n);
    VisualNode* n1;
    VisualNode* n2;
    int state;
    int weight;
};

#endif // CONNECTION_H
