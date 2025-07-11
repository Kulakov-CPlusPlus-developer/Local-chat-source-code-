#include "ClientInfo.h"
#include "ui_ClientInfo.h"

ClientInfo::ClientInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientInfo)
{
    ui->setupUi(this);
}

ClientInfo::~ClientInfo()
{
    delete ui;
}

void ClientInfo::myShow(QListWidgetItem *currentItem)
{
    ui->lbValID->setText(currentItem->data(Qt::UserRole).toString());
    ui->lbValName->setText(currentItem->data(Qt::UserRole+1).toString());
    ui->lbValSurname->setText(currentItem->data(Qt::UserRole+2).toString());
    ui->lbValAge->setText(currentItem->data(Qt::UserRole+3).toString());
    ui->lbValPost->setText(currentItem->data(Qt::UserRole+4).toString());
    this->show();
}
