#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"

ClientChatWidget::ClientChatWidget(ClientManager *client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientChatWidget),
    _client(client)
{
    ui->setupUi(this);    

    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(ui->lnMessage, &QLineEdit::returnPressed, this, &ClientChatWidget::on_btnSend_clicked);
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}

void ClientChatWidget::setID( const uint&_id)
{
    idClient = _id;
}

uint ClientChatWidget::getID()
{
    return idClient;
}

bool ClientChatWidget::getChatStarted()
{
    return chatStarted;
}

void ClientChatWidget::setChatStarted(bool flag)
{
    chatStarted = flag;
}

void ClientChatWidget::on_btnSend_clicked()
{
    if(!ui->lnMessage->text().isEmpty()){
        auto message = ui->lnMessage->text().trimmed();
        qDebug() << "idClient: " << idClient;
        _client->sendMessage(message, idClient);

        auto _messWidg = new MessageWidgetMe(message, "Me:");

        QListWidgetItem *item = new QListWidgetItem(ui->lstMessage);

        item->setSizeHint(QSize(0, _messWidg->sizeHint().height()));
        ui->lstMessage->addItem(item);
        ui->lstMessage->setItemWidget(item, _messWidg);

        ui->lnMessage->setText("");
    } else {
        ui->lnMessage->setPlaceholderText("Input message...");
    }
}

void ClientChatWidget::clientDisconnected()
{
    ui->wdgSend->setEnabled(false);
}

void ClientChatWidget::dataReceived(const QString &data, const QString &_username_)
{
    auto messWidg = new MessageWidget(data, _username_ + ":");

    QListWidgetItem *item = new QListWidgetItem(ui->lstMessage);

    item->setSizeHint(messWidg->sizeHint());
    ui->lstMessage->addItem(item);
    ui->lstMessage->setItemWidget(item, messWidg);
}

