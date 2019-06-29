#include "buttongroup.h"
#include "ui_buttongroup.h"
#include <QToolButton>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup()
{

}

// 当前登录用户初始化
void ButtonGroup::changeCurrUser(QString user)
{

}

void ButtonGroup::slotButtonClick(Page cur)
{

}

void ButtonGroup::slotButtonClick(QString text)
{

}

void ButtonGroup::setParent(QWidget *parent)
{

}

void ButtonGroup::paintEvent(QPaintEvent *event)
{

}

void ButtonGroup::mousePressEvent(QMouseEvent *event)
{
    // 如果是左键, 计算窗口左上角, 和当前按钮位置的距离

}

void ButtonGroup::mouseMoveEvent(QMouseEvent *event)
{
    // 移动是持续的状态, 需要使用buttons

}
