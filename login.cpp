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

}
