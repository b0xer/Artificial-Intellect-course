#ifndef CONTROLLER_H
#define CONTROLLER_H
#include<QList>
#include "visualnode.h"
#include <QPoint>
#include <QPaintEvent>
#include <QTextEdit>
#include "connection.h"
class MainWindow;
class Controller
{
public:
    QList<VisualNode*> viewElements;
    QList<VisualNode*> stack;
    QList<VisualNode*> marked;
    QList<Connection*> connections;
    Controller();
    void StartDrag(const QPoint &p);
    void Hover(const QPoint &p);
    void Drag(const QPoint &p);
    void AddNewMember(const QPoint &p);
    void FinishDrag();
    void PaintElements(QPainter& m);
    void Connect(const QPoint &p);
    void SelectFirstAndLast(const QPoint &p, int button);
    bool MakeStepBS();
    bool MakeStepBFS(VisualNode* from, MainWindow* const mv);
    void Sort(QList<Connection*>& stack);
    void BFSgo(MainWindow* const mv);
    void InitLogic(bool reset_stats=false);
    bool ConnectionExista(VisualNode* n1, VisualNode* n2);
    QString highlightPath(QList<VisualNode*>& marked);
    bool Parent(VisualNode* a1, VisualNode* a2);
    void save();
    void load();
    VisualNode* NodeById(int _id);
private:
    VisualNode *curr_active;
    VisualNode *last_one;
    VisualNode *first_one;
    Connection *connection_active;
    QPoint *startPos;
};

#endif // CONTROLLER_H
