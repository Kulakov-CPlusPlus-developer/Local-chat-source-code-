#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "MessageWidgetMe.h"
#include "MessageWidget.h"
#include "ClientManager.h"

namespace Ui {
class ClientChatWidget;
}

class ClientChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientChatWidget(ClientManager *client, QWidget *parent = nullptr);
    ~ClientChatWidget();
    void setID(const uint& _id);
    uint getID();
    bool getChatStarted();
    void setChatStarted(bool flag);

private slots:
    void on_btnSend_clicked();
    void clientDisconnected();

public slots:
    void dataReceived(const QString& data, const QString& _username_);

private:
    Ui::ClientChatWidget *ui;
    ClientManager *_client;
    uint idClient;
    bool chatStarted = false;
};

#endif // CLIENTCHATWIDGET_H
