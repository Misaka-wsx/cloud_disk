#include "transfer.h"
#include "ui_transfer.h"
#include "common/downloadlayout.h"
#include "common/uploadlayout.h"
#include "common/logininfoinstance.h"
#include <QFile>

Transfer::Transfer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Transfer)
{
    ui->setupUi(this);

}
