#include "rankinglist.h"
#include "ui_rankinglist.h"
#include "common/logininfoinstance.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

RankingList::RankingList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankingList)
{
    ui->setupUi(this);
}
