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

    //action 1 add to menu 1
    m_menu->addAction(m_downloadAction);
    m_menu->addAction(m_shareAction);
    m_menu->addAction(m_delAction);
    m_menu->addAction(m_propertyAction);
    //mene 2
    m_menuEmpty = new MyMenu( this );
    // action2
    m_pvAscendingAction = new QAction("按下载量升序", this);
    m_pvDescendingAction = new QAction("按下载量降序", this);
    m_refreshAction = new QAction("刷新", this);
    m_uploadAction = new QAction("上传", this);

    // action2 add to menue 2
    m_menuEmpty->addAction(m_pvAscendingAction);
    m_menuEmpty->addAction(m_pvDescendingAction);
    m_menuEmpty->addAction(m_refreshAction);
    m_menuEmpty->addAction(m_uploadAction);
    //signal and slot
    //download
    connect(m_downloadAction,&QAction::triggered,[=]()
    {
        cout<<"下载动作";
        addDownloadFiles();
    });
    // share
        connect(m_shareAction, &QAction::triggered, [=]()
        {
            cout << "分享动作";
            dealSelectdFile("分享"); //处理选中的文件
        });

        // delete
        connect(m_delAction, &QAction::triggered, [=]()
        {
            cout << "删除动作";
            dealSelectdFile("删除"); //处理选中的文件
        });

        // attribute
        connect(m_propertyAction, &QAction::triggered, [=]()
        {
            cout << "属性动作";
            dealSelectdFile("属性");
        });

        // sort top order
        connect(m_pvAscendingAction, &QAction::triggered, [=]()
        {
            cout << "按下载量升序";
            refreshFiles(PvAsc);
        });

        // sort down order
        connect(m_pvDescendingAction, &QAction::triggered, [=]()
        {
            cout << "按下载量降序";
            refreshFiles(PvDesc);
        });

        //flush
        connect(m_refreshAction, &QAction::triggered, [=]()
        {
            cout << "刷新动作";

            refreshFiles();
        });

        //uoload
        connect(m_uploadAction, &QAction::triggered, [=]()
        {
            cout << "上传动作";

            addUploadFiles();
        });
}

void MyFileWg::addUploadFiles()
{
    // switch to upload page
    emit gotoTransfer(TransferStatus::Uplaod);
    //get upload list instance
    UploadTask *uploadList = UploadTask::getInstance();
    if(uploadList == nullptr)
    {
        cout << "UploadTask::getInstance() == NULL";
        return;
    }

    QStringList list = QFileDialog::getOpenFileNames();
    for(int i = 0; i < list.size(); ++i)
    {
        //cout << "所选文件为："<<list.at(i);
        //  -1: 文件大于30m
        //  -2：上传的文件是否已经在上传队列中
        //  -3: 打开文件失败
        //  -4: 获取布局失败
        int res = uploadList->appendUploadList(list.at(i));
        if(res == -1)
        {
            QMessageBox::warning(this, "文件太大", "文件大小不能超过30M！！！");
        }
        else if(res == -2)
        {
            QMessageBox::warning(this, "添加失败", "上传的文件是否已经在上传队列中！！！");
        }
        else if(res == -3)
        {
            cout << "打开文件失败";
        }
        else if(res == -4)
        {
            cout << "获取布局失败";
        }
    }
}

QByteArray MyFileWg::setMd5Json(QString user, QString token, QString md5, QString fileName)
{
    QMap<QString, QVariant> tmp;
    tmp.insert("user", user);
    tmp.insert("token", token);
    tmp.insert("md5", md5);
    tmp.insert("fileName", fileName);

    /*json数据如下
       {
           user:xxxx,
           token:xxxx,
           md5:xxx,
           fileName: xxx
       }
       */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(tmp);
    if ( jsonDocument.isNull() )
    {
        cout << " jsonDocument.isNull() ";
        return "";
    }

    return jsonDocument.toJson();
}

void MyFileWg::uploadFilesAction()
{ // 获取上传列表实例
    UploadTask *uploadList = UploadTask::getInstance();
    if(uploadList == nullptr)
    {
        cout << "UploadTask::getInstance() == NULL";
        return;
    }

    // 如果队列为空，无上传任务，终止程序
    if( uploadList->isEmpty() )
    {
        return;
    }

    // 查看是否有上传任务，单任务上传，当前有任务，不能上传
    if( uploadList->isUpload() )
    {
        return;
    }

    // 获取登陆信息实例
    LoginInfoInstance *login = LoginInfoInstance::getInstance(); //获取单例

    // url
    QNetworkRequest request;
    QString url = QString("http://%1:%2/md5").arg( login->getIp() ).arg( login->getPort() );
    request.setUrl( QUrl( url )); //设置url
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 取出第0个上传任务，如果任务队列没有任务在上传，设置第0个任务上传
    UploadFileInfo *info = uploadList->takeTask();

    // post数据包
    QByteArray array = setMd5Json(login->getUser(), login->getToken(), info->md5, info->fileName);

    // 发送post请求
    QNetworkReply *reply = m_manager->post(request, array);
    if(reply == nullptr )
    {
        cout << "reply is NULL";
        return;
    }

    // 信号和槽连接
    connect(reply, &QNetworkReply::finished, [=]()
    {
        if (reply->error() != QNetworkReply::NoError) //有错误
        {
            cout << reply->errorString();
            reply->deleteLater(); //释放资源
            return;
        }

        QByteArray array = reply->readAll();
        //cout << array.data();

        /*
        秒传文件：
            文件已存在：{"code":"005"}
            秒传成功：  {"code":"006"}
            秒传失败：  {"code":"007"}
        token验证失败：{"code":"111"}

        */
        if("006" == m_cm.getCode(array) ) //common.h
        {
            //秒传文件成功
            //cout << info->fileName.toUtf8().data() << "-->秒传成功";
            m_cm.writeRecord(login->getUser(), info->fileName, "006");

            //删除已经完成的上传任务
            uploadList->dealUploadTask();

        }
        else if("007" == m_cm.getCode(array) )
        {
            // 说明后台服务器没有此文件，需要真正的文件上传
            uploadFile(info);
        }
        else if("005" == m_cm.getCode(array) )// "005", 上传的文件已存在
        {
            //QMessageBox::information(this, "文件已存在", QString("%1 已存在").arg(info->fileName))
            //cout << QString("%1 已存在").arg(info->fileName).toUtf8().data();
            m_cm.writeRecord(login->getUser(), info->fileName, "005");

            //删除已经完成的上传任务
            uploadList->dealUploadTask();
        }
        else if("111" == m_cm.getCode(array)) //token验证失败
        {
            QMessageBox::warning(this, "账户异常", "请重新登陆！！！");
            emit loginAgainSignal(); //发送重新登陆信号
            return;
        }
        reply->deleteLater();
    });

}

void MyFileWg::uploadFile(UploadFileInfo *info)
{
    //取出上传任务
    QFile *file = info->file;           //文件指针
    QString fileName = info->fileName;  //文件名字
    QString md5 = info->md5;            //文件md5码
    qint64 size = info->size;           //文件大小
    DataProgress *dp = info->dp;        //进度条控件
    QString boundary = m_cm.getBoundary();   //产生分隔线

    //获取登陆信息实例
    LoginInfoInstance *login = LoginInfoInstance::getInstance(); //获取单例

    QByteArray data;

    /*
       ------WebKitFormBoundary88asdgewtgewx\r\n
       Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
       Content-Type: application/octet-stream\r\n
       \r\n
       真正的文件内容\r\n
       ------WebKitFormBoundary88asdgewtgewx
       */

    //第1行，分隔线
    data.append(boundary);
    data.append("\r\n");

    //上传文件信息
    data.append("Content-Disposition: form-data; ");
    data.append( QString("user=\"%1\" ").arg( login->getUser() ) ); //上传用户
    data.append( QString("filename=\"%1\" ").arg(fileName) ); //文件名字
    data.append( QString("md5=\"%1\" ").arg(md5) ); //文件md5码
    data.append( QString("size=%1").arg(size)  );   //文件大小
    data.append("\r\n");

    data.append("Content-Type: application/octet-stream");
    data.append("\r\n");
    data.append("\r\n");

    // 上传文件内容
    data.append( file->readAll() ); //文件内容
    data.append("\r\n");

    // 结束分隔线
    data.append(boundary);

    QNetworkRequest request; //请求对象
    //http://127.0.0.1:80/upload
    QString url = QString("http://%1:%2/upload").arg(login->getIp()).arg(login->getPort());
    request.setUrl(QUrl( url )); //设置url

    // qt默认的请求头
    //request.setRawHeader("Content-Type","text/html");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");

    // 发送post请求
    QNetworkReply * reply = m_manager->post( request, data );
    if(reply == nullptr)
    {
        cout << "reply == NULL";
        return;
    }

    // 有可用数据更新时
    connect(reply, &QNetworkReply::uploadProgress, [=](qint64 bytesRead, qint64 totalBytes)
    {
        if(totalBytes != 0) //这个条件很重要
        {
            //cout << bytesRead/1024 << ", " << totalBytes/1024;
            dp->setProgress(bytesRead/1024, totalBytes/1024); //设置进度条
        }
    });

    // 获取请求的数据完成时，就会发送信号SIGNAL(finished())
    connect(reply, &QNetworkReply::finished, [=]()
    {
        if (reply->error() != QNetworkReply::NoError) //有错误
        {
            cout << reply->errorString();
            reply->deleteLater(); //释放资源
            m_cm.writeRecord(login->getUser(), info->fileName, "009");
            UploadTask *uploadList = UploadTask::getInstance();
            uploadList->dealUploadTask(); //删除已经完成的上传任务
            return;
        }

        QByteArray array = reply->readAll();

        reply->deleteLater();

        /*
               上传文件：
                   成功：{"code":"008"}
                   失败：{"code":"009"}
               */
        if("008" == m_cm.getCode(array) ) //common.h
        {
            //cout << fileName.toUtf8().data() <<" ---> 上传完成";
            m_cm.writeRecord(login->getUser(), info->fileName, "008");
        }
        else if("009" == m_cm.getCode(array) )
        {
            //cout << fileName.toUtf8().data() << " ---> 上传失败";
            m_cm.writeRecord(login->getUser(), info->fileName, "009");
        }


        //获取上传列表实例
        UploadTask *uploadList = UploadTask::getInstance();
        if(uploadList == nullptr)
        {
            cout << "UploadTask::getInstance() == NULL";
            return;
        }

        uploadList->dealUploadTask(); //删除已经完成的上传任务
    }
    );
}

void MyFileWg::clearFileList()
{
    int n = m_fileList.size();
    for(int i = 0; i < n; ++i)
    {
        FileInfo *tmp = m_fileList.takeFirst();
        delete tmp;
    }
}

void MyFileWg::clearItems()
{
    //使用QListWidget::count()来统计ListWidget中总共的item数目
    int n = ui->listWidget->count();
    for(int i = 0; i < n; ++i)
    {
        QListWidgetItem *item = ui->listWidget->takeItem(0); //这里是0，不是i
        delete item;
    }
}

void MyFileWg::addUploadItem(QString iconPath, QString name)
{
    ui->listWidget->addItem(new QListWidgetItem(QIcon(iconPath), name));
}

void MyFileWg::refreshFileItems()
{
    //清空所有item项目
    clearItems();

    //如果文件列表不为空，显示文件列表
    if(m_fileList.isEmpty() == false)
    {
        int n = m_fileList.size(); //元素个数
        for(int i = 0; i < n; ++i)
        {
            FileInfo *tmp = m_fileList.at(i);
            QListWidgetItem *item = tmp->item;
            //list widget add item
            ui->listWidget->addItem(item);
        }
    }

    //添加上传文件item
    this->addUploadItem();
}

/******************* 显示用户的文件列表 *******************/
// 得到服务器json文件
QStringList MyFileWg::getCountStatus(QByteArray json)
{
    QJsonParseError error;
    QStringList list;

    //cout << "json = " << json.data();

    //将来源数据json转化为JsonDocument
    //由QByteArray对象构造一个QJsonDocument对象，用于我们的读写操作
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error == QJsonParseError::NoError) //没有出错
    {
        if (doc.isNull() || doc.isEmpty())
        {
            cout << "doc.isNull() || doc.isEmpty()";
            return list;
        }

        if( doc.isObject() )
        {
            QJsonObject obj = doc.object();//取得最外层这个大对象
            list.append( obj.value( "token" ).toString() ); //登陆token
            list.append( obj.value( "num" ).toString() ); //文件个数
        }
    }
    else
    {
        cout << "err = " << error.errorString();
    }

    return list;
}
// 显示用户的文件列表
// cmd取值，Normal：普通用户列表，PvAsc：按下载量升序， PvDesc：按下载量降序
void MyFileWg::refreshFiles(MyFileWg::Display cmd)
{
    //=========================>先获取用户文件数目<=========================
    m_userFilesCount = 0;

    QNetworkRequest request; //请求对象

    // 获取登陆信息实例
    LoginInfoInstance *login = LoginInfoInstance::getInstance(); //获取单例

    // 127.0.0.1:80/myfiles&cmd=count		//获取用户文件个数
    QString url = QString("http://%1:%2/myfiles?cmd=count").arg(login->getIp()).arg(login->getPort());
    request.setUrl(QUrl( url )); //设置url

    // qt默认的请求头
    // request.setRawHeader("Content-Type","text/html");
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QByteArray data = setGetCountJson( login->getUser(), login->getToken());

    // 发送post请求
    QNetworkReply * reply = m_manager->post( request, data );
    if(reply == nullptr)
    {
        cout << "reply == NULL";
        return;
    }

    // 获取请求的数据完成时，就会发送信号SIGNAL(finished())
    connect(reply, &QNetworkReply::finished, [=]()
    {
        if (reply->error() != QNetworkReply::NoError) //有错误
        {
            cout << reply->errorString();
            reply->deleteLater(); //释放资源
            return;
        }

        // 服务器返回数据
        QByteArray array = reply->readAll();
        cout << "server return file ...: " << array;

        reply->deleteLater(); //释放

        // 得到服务器json文件
        QStringList list = getCountStatus(array);

        // token验证失败
        if( list.at(0) == "111" )
        {
            QMessageBox::warning(this, "账户异常", "请重新登陆！！！");

            emit loginAgainSignal(); //发送重新登陆信号

            return; //中断
        }

        // 转换为long
        m_userFilesCount = list.at(1).toLong();
        cout << "userFilesCount = " << m_userFilesCount;

        // 清空文件列表信息
        clearFileList();

        if(m_userFilesCount > 0)
        {
            // 说明用户有文件，获取用户文件列表
            m_start = 0;  //从0开始
            m_count = 10; //每次请求10个

            // 获取新的文件列表信息
            getUserFilesList(cmd);
        }
        else //没有文件
        {
            refreshFileItems(); //更新item
        }
    });
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



































