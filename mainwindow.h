#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

public slots:
    void SaveState();
private:
    Ui::MainWindow *ui;
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *);
    Controller controller;
};



#endif // MAINWINDOW_H
