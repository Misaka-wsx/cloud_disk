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
    QPainter painter(this);
    QPixmap pixmap(":/images/login_bk.jpg");
    painter.drawPixmap(0,0,width(),height(),pixmap);
}

void Login::onRegisterBtnClicked()
{
    //get usr data
    QString userName = ui->reg_usr->text();
    QString nickName = ui->reg_nickname->text();
    QString firstPwd = ui->reg_pwd->text();
    QString surePwd = ui->reg_surepwd->text();
    QString phone = ui->reg_phone->text();
    QString email = ui->reg_mail->text();
#if 0
    QRegExp regexp(USER_REG);
    if(!regexp.exactMatch(userName))
    {
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        ui->reg_usr->clear();
        ui->reg_usr->setFocus();
        return;
    }
    if(!regexp.exactMatch(nickName))
    {
        QMessageBox::warning(this, "警告", "昵称格式不正确");
        ui->reg_nickname->clear();
        ui->reg_nickname->setFocus();
        return;
    }
    regexp.setPattern(PASSWD_REG);
    if(!regexp.exactMatch(firstPwd))
    {
        QMessageBox::warning(this, "警告", "密码格式不正确");
        ui->reg_pwd->clear();
        ui->reg_pwd->setFocus();
        return;
    }
    if(surePwd != firstPwd)
    {
        QMessageBox::warning(this, "警告", "两次输入的密码不匹配, 请重新输入");
        ui->reg_surepwd->clear();
        ui->reg_surepwd->setFocus();
        return;
    }
    regexp.setPattern(PHONE_REG);
    if(!regexp.exactMatch(phone))
    {
        QMessageBox::warning(this, "警告", "手机号码格式不正确");
        ui->reg_phone->clear();
        ui->reg_phone->setFocus();
        return;
    }
    regexp.setPattern(EMAIL_REG);
    if(!regexp.exactMatch(email))
    {
        QMessageBox::warning(this, "警告", "邮箱码格式不正确");
        ui->reg_mail->clear();
        ui->reg_mail->setFocus();
        return;
    }
#endif
    //pack register infomation to json
    QByteArray array=setRegisterJson(userName,nickName,m_cm.getStrMd5(firstPwd),phone,email);

    QNetworkRequest request;
    //url
    QString url=QString("http://%1:%2/reg").arg(ui->address_server->text().arg(ui->port_server->text()));
    request.setUrl(QUrl(url));
    //header
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setHeader(QNetworkRequest::ContentLengthHeader,QVariant(array.size()));
    //send data
    QNetworkReply* reply=m_manager->post(request,array);
    //check if dealed
    connect(reply,&QNetworkReply::readyRead,[=](){
        //server return code
        QByteArray json=reply->readAll();
        //success code 002
        //user exist code 003
        //fail code 004
        if("002"==m_cm.getCode(json))
        {
            //register success
            QMessageBox::information(this, "注册成功", "注册成功，请登录");

            //clear
            ui->reg_usr->clear();
            ui->reg_nickname->clear();
            ui->reg_pwd->clear();
            ui->reg_surepwd->clear();
            ui->reg_phone->clear();
            ui->reg_mail->clear();

            //set login page info
            ui->log_usr->setText(userName);
            ui->log_pwd->setText(firstPwd);
            ui->rember_pwd->setChecked(true);

            //switch to login page
            ui->stackedWidget->setCurrentWidget(ui->login_page);
        }
        else if("003" == m_cm.getCode(json))
        {
            QMessageBox::warning(this, "注册失败", QString("[%1]该用户已经存在!!!").arg(userName));
        }
        else if("004" == m_cm.getCode(json))
        {
            QMessageBox::warning(this, "注册失败", "注册失败！！！");
        }
        // release resorcs
        delete reply;
        });
}

void Login::onLoginBtnClicked()
{

}

void Login::onSetOkBtnClicked()
{

}

void Login::readCfg()
 {
    QString user=m_cm.getCfgValue("login","user");
    QString pwd=m_cm.getCfgValue("login","pwd");
    QString remeber=m_cm.getCfgValue("login","remember");
    int ret=0;
    if(remeber=="yes")
    {
        //decode pwd
        unsigned char encPwd[521]={0};
        int encPwdLen=0;
        QByteArray tmp=QByteArray::fromBase64(pwd.toLocal8Bit());
        ret=DesDec((unsigned char* )tmp.data(),tmp.size(),encPwd,&encPwdLen);
        if(ret!=0)
        {
            cout<<"desdec";
            return ;
        }
#ifdef _WIN32
        ui->log_pwd->setText(QString::fromLocal8Bit((const char*)encPwd,encPwdLen));
#else
        ui->log_pwd->setText((const char *)encpwd);
#endif
        ui->rember_pwd->setChecked(true);
    }
    else
    {
        ui->log_pwd->setText("");
        ui->rember_pwd->setChecked(false);
    }
    //user decode
    unsigned char encUsr[512]={0};
    int encUsrLen=0;
    //toLocal8Bit()
    QByteArray tmp = QByteArray::fromBase64( user.toLocal8Bit());
    ret = DesDec( (unsigned char *)tmp.data(), tmp.size(), encUsr, &encUsrLen);
    if(ret != 0)
    {
        cout << "DesDec";
        return;
    }

#ifdef _WIN32
    //fromLocal8Bit()
    ui->log_usr->setText( QString::fromLocal8Bit( (const char *)encUsr, encUsrLen ) );
#else
    ui->log_usr->setText( (const char *)encUsr );
#endif

    QString ip = m_cm.getCfgValue("web_server", "ip");
    QString port = m_cm.getCfgValue("web_server", "port");
    ui->address_server->setText(ip);
    ui->port_server->setText(port);
}
