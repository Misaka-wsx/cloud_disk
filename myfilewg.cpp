#include "myfilewg.h"
#include "ui_myfilewg.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QListWidgetItem>
#include "common/logininfoinstance.h"
#include "selfwidget/filepropertyinfo.h"
#include "common/downloadtask.h"

MyFileWg::MyFileWg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyFileWg)
{
    ui->setupUi(this);
    //init listwidget
    initListWidget();
    //add action menu
    addActionMenu();
    //http manage
    m_manager=Common::getNetManager();
    //check task list
    checkTaskList();

}

MyFileWg::~MyFileWg()
{
    delete ui;
}

void MyFileWg::initListWidget()
{
    ui->listWidget->setViewMode(QListView::IconMode);//icon mode
    ui->listWidget->setIconSize(QSize(80,80));//icon size
    ui->listWidget->setGridSize(QSize(100,120));//item size

    //set qlistview size fixed to adjust
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setMovement(QListView::Static);
    //set icon size
    ui->listWidget->setSpacing(30);
    //listwidget
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget,&QListWidget::itemPressed,this,[=](QListWidgetItem* item)
    {
        QString str=item->text();
        if(str=="上传文件")
        {
            addUploadFiles();
        }
    });
}

void MyFileWg::addActionMenu()
{
    m_menu=new MyMenu(this);
    //init menu list
    m_downloadAction=new QAction("下载",this);
    m_shareAction=new QAction("分享",this);
    m_delAction = new QAction("删除", this);
    m_propertyAction = new QAction("属性", this);
}

void MyFileWg::addUploadFiles()
{

}

QByteArray MyFileWg::setMd5Json(QString user, QString token, QString md5, QString fileName)
{

}

void MyFileWg::uploadFilesAction()
{

}

void MyFileWg::uploadFile(UploadFileInfo *info)
{

}

void MyFileWg::clearFileList()
{

}

void MyFileWg::clearItems()
{

}

void MyFileWg::addUploadItem(QString iconPath, QString name)
{

}

void MyFileWg::refreshFileItems()
{

}

QStringList MyFileWg::getCountStatus(QByteArray json)
{

}

void MyFileWg::refreshFiles(MyFileWg::Display cmd)
{

}

QByteArray MyFileWg::setGetCountJson(QString user, QString token)
{

}

QByteArray MyFileWg::setFilesListJson(QString user, QString token, int start, int count)
{

}

void MyFileWg::getUserFilesList(MyFileWg::Display cmd)
{

}

void MyFileWg::getFileJsonInfo(QByteArray data)
{

}

void MyFileWg::dealSelectdFile(QString cmd)
{

}

QByteArray MyFileWg::setDealFileJson(QString user, QString token, QString md5, QString filename)
{

}

void MyFileWg::shareFile(FileInfo *info)
{

}

void MyFileWg::delFile(FileInfo *info)
{

}

void MyFileWg::getFileProperty(FileInfo *info)
{

}

void MyFileWg::addDownloadFiles()
{

}

void MyFileWg::downloadFilesAction()
{

}

void MyFileWg::dealFilePv(QString md5, QString filename)
{

}

void MyFileWg::clearAllTask()
{

}

void MyFileWg::checkTaskList()
{

}

void MyFileWg::rightMenu(const QPoint &pos)
{

}

void MyFileWg::contextMenuEvent(QContextMenuEvent *event)
{

}



































