#include "sharelist.h"
#include "ui_sharelist.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileDialog>
#include "common/logininfoinstance.h"
#include "common/downloadtask.h"
#include "selfwidget/filepropertyinfo.h"

ShareList::ShareList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareList)
{
    ui->setupUi(this);
}
