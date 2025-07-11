#include "MessageWidget.h"
#include "ui_MessageWidget.h"

MessageWidget::MessageWidget(const QString &text, const QString &username, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MessageWidget)
{
    ui->setupUi(this);
    ui->lbMessage->setText(text);

    ui->lbMessage->adjustSize();
    ui->frame->adjustSize();
    this->adjustSize();

    ui->lbTime->setText(QDateTime::currentDateTime().toString("HH:mm"));

    ui->lbUsName->setText(username);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}




