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
