#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QRegExp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include "common/des.h"
#include "common/logininfoinstance.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //init
    //netrequest http
    m_manager=Common::getNetManager();
    m_mainWin=new MainWindow;
    //window icon
    this->setWindowIcon(QIcon(":/images/logo.ico"));
    m_mainWin->setWindowIcon(QIcon(":/images/logo.ico"));
    //no windowhint
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setFont(QFont("幼圆",12,QFont::Bold,false));
    //password
    ui->log_pwd->setEchoMode(QLineEdit::Password);
    ui->reg_pwd->setEchoMode(QLineEdit::Password);
    ui->reg_surepwd->setEchoMode(QLineEdit::Password);
    //current widget
    ui->stackedWidget->setCurrentIndex(0);
    ui->log_usr->setFocus();
    //data front
    ui->log_usr->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->reg_usr->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->reg_nickname->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 3~16");
    ui->log_pwd->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->reg_pwd->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    ui->reg_surepwd->setToolTip("合法字符:[a-z|A-Z|#|@|0-9|-|_|*],字符个数: 6~18");
    //read json configure file
    readCfg();
    //load picture info and initalize file list
    m_cm.getFileTypeList();
#if 1
    // test data
    ui->reg_usr->setText("kevin_666");
    ui->reg_nickname->setText("kevin@666");
    ui->reg_pwd->setText("123456");
    ui->reg_surepwd->setText("123456");
    ui->reg_phone->setText("11111111111");
    ui->reg_mail->setText("abc@qq.com");

#endif
    //register
    connect(ui->log_register_btn,&QToolButton::clicked,this,[=](){
        //switch to register page
        ui->stackedWidget->setCurrentWidget(ui->register_page);
        ui->reg_usr->setFocus();
    });
    //setting button
    connect(ui->title_widget,&TitleWidget::showSetWidget,[=](){
        //switch to setting page
        ui->stackedWidget->setCurrentWidget(ui->set_page);
        ui->address_server->setFocus();
    });
    // close
    connect(ui->title_widget, &TitleWidget::closeWindow, [=]()
    {
        // current window is register page
        if(ui->stackedWidget->currentWidget() == ui->register_page)
        {
            // clear data
            ui->reg_mail->clear();
            ui->reg_usr->clear();
            ui->reg_nickname->clear();
            ui->reg_pwd->clear();
            ui->reg_surepwd->clear();
            ui->reg_phone->clear();
            // switch widow
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            ui->log_usr->setFocus();
        }
        // or setting page
        else if(ui->stackedWidget->currentWidget() == ui->set_page)
        {
            // clear data
            ui->address_server->clear();
            ui->port_server->clear();
            // switch window
            ui->stackedWidget->setCurrentWidget(ui->login_page);
            ui->log_usr->setFocus();
        }
        // login page close
        else if(ui->stackedWidget->currentWidget() == ui->login_page)
        {
            close();
        }
    });

}

Login::~Login()
{
    delete ui;
}
//set json data login user use
QByteArray Login::setLoginJson(QString user, QString pwd)
{
    QMap<QString,QVariant> login;
    login.insert("user",user);
    login.insert("pwd",pwd);

    QJsonDocument doc =QJsonDocument::fromVariant(login);
    if(doc.isNull())
    {
        cout<<"jsondocument.isNull";
        return "";
    }
    return doc.toJson();
}
//get server response of login stat return code 000 or 001 and login section
QByteArray Login::setRegisterJson(QString userName, QString nickName, QString firstPwd, QString phone, QString email)
{
    QMap<QString, QVariant> reg;
    reg.insert("userName", userName);
    reg.insert("nickName", nickName);
    reg.insert("firstPwd", firstPwd);
    reg.insert("phone", phone);
    reg.insert("email", email);

    /*json data
         {
             userName:xxxx,
             nickName:xxx,
             firstPwd:xxx,
             phone:xxx,
             email:xxx
         }
     */
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(reg);
    if ( jsonDocument.isNull() )
    {
        cout << " jsonDocument.isNull() ";
        return "";
    }

    //cout << jsonDocument.toJson().data();

    return jsonDocument.toJson();
}

QStringList Login::getLoginStatus(QByteArray json)
{
    QJsonParseError error;
    QStringList list;
    //json to json doc;
    QJsonDocument doc =QJsonDocument::fromJson(json,&error);
    if(error.error==QJsonParseError::NoError)
    {
        if(doc.isNull()||doc.isEmpty())
        {
            cout<<"doc.isnull||doc.isempty";
            return list;
        }
        if(doc.isObject())
        {
            QJsonObject obj=doc.object();

            cout<<"server return"<<obj;
            //code
            list.append(obj.value("code").toString());
            //login token
            list.append(obj.value("token").toString());

        }
        else
        {
            cout<<"err="<<error.errorString();
        }
    }
    return list;
}

void Login::paintEvent(QPaintEvent *)
{

}

void Login::onRegisterBtnClicked()
{

}

void Login::onLoginBtnClicked()
{

}

void Login::onSetOkBtnClicked()
{

}
