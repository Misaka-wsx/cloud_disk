#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common/logininfoinstance.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // no windowhint
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    ui->btn_group->setParent(this);
    ui->stackedWidget->setCurrentWidget(ui->myfiles_page);
    connect(this,&MainWindow::setUser,ui->btn_group,&ButtonGroup::changeCurrUser);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMainWindow()
{
    m_common.moveToCenter(this);
    ui->stackedWidget->setCurrentWidget(ui->myfiles_page);
    //flash myfile list
    ui->myfiles_page->refreshFiles();
    //send signal
    LoginInfoInstance* login=LoginInfoInstance::getInstance();
    emit setUser(login->getUser());

}

void MainWindow::managerSignals()
{
    // close
    connect(ui->btn_group, &ButtonGroup::closeWindow, this, &MainWindow::close);
    // max
    connect(ui->btn_group, &ButtonGroup::maxWindow, [=]()
    {
        static bool flag = false;
        if(!flag)
        {
            this->showMaximized();
            flag = true;
        }
        else
        {
            this->showNormal();
            flag = false;
        }
    });
    // minimize
    connect(ui->btn_group, &ButtonGroup::minWindow, this, &MainWindow::showMinimized);
    // mufile
    connect(ui->btn_group, &ButtonGroup::sigMyFile, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->myfiles_page);
        // flash myfile list
        ui->myfiles_page->refreshFiles();
    });
    // share list
    connect(ui->btn_group, &ButtonGroup::sigShareList, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->sharefile_page);
        // flash
        ui->sharefile_page->refreshFiles();
    });
    // download top list
    connect(ui->btn_group, &ButtonGroup::sigDownload, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->ranking_page);
        // flush
        ui->ranking_page->refreshFiles();
    });
    // trans list
    connect(ui->btn_group, &ButtonGroup::sigTransform, [=]()
    {
        ui->stackedWidget->setCurrentWidget(ui->transfer_page);
    });
    // switch user
    connect(ui->btn_group, &ButtonGroup::sigSwitchUser, [=]()
    {
        qDebug() << "bye bye...";
        loginAgain();
    });
    // stack window switch
    connect(ui->myfiles_page, &MyFileWg::gotoTransfer, [=](TransferStatus status)
    {
        ui->btn_group->slotButtonClick(Page::TRANSFER);
        if(status == TransferStatus::Uplaod)
        {
            ui->transfer_page->showUpload();
        }
        else if(status == TransferStatus::Download)
        {
            ui->transfer_page->showDownload();
        }
    });
    // signal delay
    connect(ui->sharefile_page, &ShareList::gotoTransfer, ui->myfiles_page, &MyFileWg::gotoTransfer);
}

void MainWindow::loginAgain()
{
    // send switch user
    emit changeUser();
    //clear last user mission
    ui->myfiles_page->clearAllTask();

    // clear file  message of last user
    ui->myfiles_page->clearFileList();
    ui->myfiles_page->clearItems();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap bk(":/images/title_bk3.jpg");
    painter.drawPixmap(0,0,width(),height(),bk);
}

















































