#include "MessageWidgetMe.h"
#include "ui_MessageWidgetMe.h"

MessageWidgetMe::MessageWidgetMe(const QString &text, const QString &username, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MessageWidgetMe)
{
    ui->setupUi(this);
    ui->lbMessage->setText(text);    

    ui->lbMessage->adjustSize();
    ui->frame->adjustSize();
    this->adjustSize();

    ui->lbTime->setText(QDateTime::currentDateTime().toString("HH:mm"));

    ui->lbUsName->setText(username);
}

MessageWidgetMe::~MessageWidgetMe()
{
    delete ui;
}
