#include "controller.h"
#include "visualnode.h"
#include "QPainter"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qwidget.h"
#include <QFile>
#include <QTextStream>

Controller::Controller()
{
    curr_active = 0;
    last_one = 0;
    first_one = 0;
    connection_active = 0;
}
void Controller::StartDrag(const QPoint &p)
{
    for(int i=0;i<viewElements.count();i++)
    {
        if((curr_active==0) && (viewElements[i]->Inside(p)))
        {
            curr_active=viewElements[i];
        }
    }
    if(curr_active!=0)
    {
        curr_active->setState(1);
    }
    else
    {
        for(int i=0;i<connections.count();i++)
        {
            if((connection_active==0) && (connections[i]->Inside(p)))
            {
                connection_active=connections[i];
            }
        }
        startPos = new QPoint(p.x(),p.y());
    }
}

void Controller::Hover(const QPoint &p)
{
    for(int i=0;i<viewElements.count();i++)
    {
        if(viewElements[i]->Inside(p))
        {
           if(curr_active==0)
            viewElements[i]->setState(3);

        }
        else
        {
            if(curr_active==0 && viewElements[i]->state==3)
            viewElements[i]->setState(0);
        }
    }

}

void Controller::Drag(const QPoint &p)
{
    if(curr_active!=0)
    {
        curr_active->setPos(p);
    }
    else
    {
        if(connection_active!=0)
        {
            connection_active->weight = ((startPos->y()-p.y())/3);
            if ((startPos->y()-p.y())/3>100)
                connection_active->weight = 99;
            if ((startPos->y()-p.y())/3<-100)
                connection_active->weight = -99;
        }
    }
}
void Controller::FinishDrag()
{
    if(curr_active!=0)
    {
        curr_active->setState(0);
        curr_active=0;
    }
    if(connection_active!=0)
    {
        connection_active = 0;
    }
}
void Controller::AddNewMember(const QPoint &p)
{
    VisualNode *tmp = new VisualNode(p.x(),p.y(),20,1,viewElements.count()+1,0);
    viewElements.append(tmp);
    if (first_one==0)
    {
        first_one = tmp;
        first_one->setState(8);
    }
    InitLogic();
}
void Controller::PaintElements(QPainter &painter)
{
    painter.setFont(QFont("arial",14,5));
    foreach(Connection* vn, connections)
    {
        vn->Paint(painter);
    }
    for(int i=0;i<viewElements.count();i++)
    {
        viewElements[i]->Paint(painter);
    }
}

bool Controller::ConnectionExista(VisualNode* n1, VisualNode* n2)
{
    for(int i=0;i<connections.count();i++)
    {
        if((connections[i]->n1==n1 && connections[i]->n2==n2)||(connections[i]->n2==n1 && connections[i]->n1==n2))
        {
            return true;
        }
    }
    return false;
}

void Controller::Connect(const QPoint &p)
{
    VisualNode *tmp = 0;
    for(int i=0;i<viewElements.count();i++)
    {
        if((tmp==0) && (viewElements[i]->Inside(p)))
        {
            tmp=viewElements[i];
        }
    }
    if(tmp!=curr_active)
    {
        if(tmp!=0 )
        {
            if(curr_active!=0)
            {
                if(!ConnectionExista(curr_active,tmp))
                {
                    Connection *tmp_con = new Connection(curr_active,tmp,0,0);
                    connections.append(tmp_con);
                    curr_active->connections.append(tmp_con);
                    tmp->connections.append(tmp_con);
                }
                curr_active->setState(0);
                curr_active=0;
            }
            else
            {
                curr_active=tmp;
                curr_active->setState(2);
            }
        }
    }
    else
    {
        if(curr_active!=0 )
        {
            curr_active->setState(0);
            curr_active=0;
        }
    }
    InitLogic();
}


void Controller::SelectFirstAndLast(const QPoint &p, int button)
{
    VisualNode *tmp = 0;
    for(int i=0;i<viewElements.count();i++)
    {
        if((tmp==0) && (viewElements[i]->Inside(p)))
        {
            tmp=viewElements[i];
        }
    }
    if (button==2)
    {
        last_one = tmp;
        for(int i=0;i<viewElements.count();i++)
        {
            if (viewElements[i]->state==6)
                viewElements[i]->state = 0;
        }
        last_one->setState(6);
    }
    else
    {
        first_one = tmp;
        for(int i=0;i<viewElements.count();i++)
        {
            if (viewElements[i]->state==8)
                viewElements[i]->state = 0;
        }
        first_one->setState(8);
    }
}


void Controller::InitLogic(bool reset_stats)
{
    stack.clear();
    marked.clear();
    if(reset_stats)
        for(int i=0;i<viewElements.count();i++)
            viewElements[i]->setState(0);
        for(int i=0;i<connections.count();i++)
            connections[i]->state = 0;
    stack.append(first_one);
    marked.append(first_one);
}

bool Controller::MakeStepBS()
{
    if(stack.empty())
            return false;
    VisualNode *tmp = stack.first();
    tmp->setState(5);
    stack.pop_front();
    for(int i=0;i<tmp->connections.count();i++)
    {
        if(!marked.contains(tmp->connections[i]->OtherOne(tmp)))//if marked list does not contain connection which has othe end of tmp
        {
            tmp->connections[i]->state = 1;
            marked.append(tmp->connections[i]->OtherOne(tmp));
            tmp->connections[i]->OtherOne(tmp)->setState(4);
            stack.append(tmp->connections[i]->OtherOne(tmp));
            if(tmp->connections[i]->OtherOne(tmp)==last_one)
            {
                stack.clear();
                highlightPath(marked);
                return false;
            }
        }
        else
        {
            tmp->connections[i]->state = 2;
        }
    }

    return true;
}
void Controller::Sort(QList<Connection *>& stack)
{
    bool go=true;
    while(go)
    {
        go=false;
        for(int i=0;i<stack.count()-1;i++)
        {
            if(stack[i]->weight<stack[i+1]->weight)
            {
                stack.swap(i,i+1);
                go=true;
            }
        }
    }
}

void Controller::BFSgo(MainWindow* const mv)
{
    MakeStepBFS(first_one, mv);
}

bool Controller::MakeStepBFS(VisualNode* from, MainWindow* const mv)
{
    QList<Connection*> new_stack;
    //VisualNode* from =
    from->setState(5);
    marked.append(from);
    for(int i=0;i<from->connections.count();i++)
    {
        VisualNode* tmp = from->connections[i]->OtherOne(from);
        if(!marked.contains(tmp))//if marked list does not contain connection which has other end of tmp
        {
            new_stack.append(from->connections[i]);
        }
    }
    Sort(new_stack);
    for(int i=0;i<new_stack.count();i++)
    {
        new_stack[i]->state = 2;
        if((new_stack[i]->OtherOne(from)==last_one)||
        !MakeStepBFS(new_stack[i]->OtherOne(from),mv))
        {
            new_stack[i]->OtherOne(from)->setState(7);
            new_stack[i]->state = 3;
            return false;
        }
    }
    return true;
}
QString Controller::highlightPath(QList<VisualNode*>& marked)
{
    QString res="";
    VisualNode* curr = marked.last();
    for(int i=marked.size()-2;i>=0;i--)
    {
        if(Parent(curr,marked[i]))
        {
            res+=QString::number(marked[i]->id);
            curr=marked[i];
        }
    }
    return res;
}

bool Controller::Parent(VisualNode *a1, VisualNode *a2)
{
    for(int i=0;i<connections.count();i++)
    {
        if((connections[i]->n1==a1 && connections[i]->n2==a2)|| (connections[i]->n1==a2 && connections[i]->n2==a1))
        {
            connections[i]->state = 3;
            a2->setState(7);
            return true;
        }
    }
    return false;
}

void Controller::save()
{
    QFile file("prev_state");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for(int i=0;i<viewElements.count();i++)
    {
        out << viewElements[i]->x<<" ";
        out << viewElements[i]->y<<" ";
        out << viewElements[i]->r<<" ";
        out << viewElements[i]->id<<" ";
        out << viewElements[i]->state<<" ";
        out << viewElements[i]->z<<"\n";
    }
    out << "*\n";
    for(int i=0;i<connections.count();i++)
    {
        out << connections[i]->n1->id<<" ";
        out << connections[i]->n2->id<<" ";
        out << connections[i]->state<<" ";
        out << connections[i]->weight<<"\n";
    }
    file.close();
}

VisualNode* Controller::NodeById(int _id)
{
    for(int i=0;i<viewElements.count();i++)
    {
        if(viewElements[i]->id==_id)
        {
            return viewElements[i];
        }
    }
}

void Controller::load()
{
    QFile file("prev_state");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream ( &file );
    QString line;
    int x;
    int y;
    int r;
    int id;
    int state;
    int z;
    VisualNode* n1;
    VisualNode* n2;
    while( !stream.atEnd() )
    {
         line = stream.readLine();
         if(line=="*")
             break;
         QStringList mas = line.split(' ');
         x = (mas[0]).toInt();
         y = (mas[1]).toInt();
         r = (mas[2]).toInt();
         id = (mas[3]).toInt();
         state = (mas[4]).toInt();
         z = (mas[5]).toInt();
         VisualNode *tmp = new VisualNode(x,y,r,1,id,state);
         viewElements.append(tmp);
         if (tmp->state == 8)
         {
             first_one = tmp;
         }
         if (tmp->state == 6)
         {
             last_one = tmp;
         }
    }
    while( !stream.atEnd() )
    {
         line = stream.readLine();
         QStringList mas = line.split(' ');
         n1 = NodeById((mas[0]).toInt());
         n2 = NodeById((mas[1]).toInt());
         state = mas[2].toInt();
         r = (mas[3]).toInt();;
         Connection *tmp_con = new Connection(n1, n2, state, r);
         connections.append(tmp_con);
         n1->connections.append(tmp_con);
         n2->connections.append(tmp_con);
    }
    file.close();
    InitLogic(true);
}
