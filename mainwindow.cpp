#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qwidget.h"
#include <QMouseEvent>
#include <QHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = Controller();
    controller.load();
}

MainWindow::~MainWindow()
{
    controller.save();
    delete ui;
}

bool MainWindow::event(QEvent *event)
{
    if ( event->type() == QEvent::HoverMove)
    {
        QHoverEvent *ke = static_cast<QHoverEvent *>(event);
            controller.Hover(ke->pos());
            this->repaint();
            ui->label->setText(QString::number(ke->pos().x())+" "+QString::number(ke->pos().y()));
    }
    if ( event->type() == QEvent::MouseMove)
    {
        QMouseEvent *ke = static_cast<QMouseEvent *>(event);
        if ( ui->radioButton -> isChecked() )
        {
           controller.Drag(ke->pos());
           this->repaint();
        }
    }
    if ( event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *ke = static_cast<QMouseEvent *>(event);
        if ( ui->radioButton -> isChecked() )
        {
            controller.StartDrag(ke->pos());
        }
    }
    if ( event->type() == QEvent::MouseButtonRelease)
    {
        if ( ui->radioButton -> isChecked() )
        {
            controller.FinishDrag();
            this->repaint();
        }
        if ( ui->radioButton_2 -> isChecked() )
        {
            QMouseEvent *ke = static_cast<QMouseEvent *>(event);
            controller.AddNewMember(ke->pos());
            this->repaint();
        }
        if ( ui->radioButton_3 -> isChecked() )
        {
            QMouseEvent *ke = static_cast<QMouseEvent *>(event);
            controller.Connect(ke->pos());
            this->repaint();
        }
        if ( ui->radioButton_4 -> isChecked() )
        {
            QMouseEvent *ke = static_cast<QMouseEvent *>(event);
            controller.SelectFirstAndLast(ke->pos(), ke->button());
            this->repaint();
        }
    }
    return QWidget::event(event);
}

void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    controller.PaintElements(painter);
}

void MainWindow::on_pushButton_clicked()
{
    controller.InitLogic(true);
    this->repaint();
}

void MainWindow::on_pushButton_2_clicked()
{
    controller.MakeStepBS();
    this->repaint();
}

void MainWindow::on_pushButton_3_clicked()
{
    controller.BFSgo(this);
    this->repaint();
}
