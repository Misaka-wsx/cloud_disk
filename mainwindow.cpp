#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common/logininfoinstance.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 去掉边框
}

