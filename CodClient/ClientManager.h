#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "LoginWindow.h"
#include <QJsonDocument>
#include <QJsonObject>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip, uint port, QObject *parent = nullptr);
    void connectToServer();
    void sendMessage(QString& message, const uint& _id);

    void setIsAutorizedTrue();
    void setIsAutorizedFalse();
    QString getName();
    uint getID();
    QTcpSocket* getSocket();

signals:
    void connected();
    void disconnected();
    void dataReceived(const QJsonObject& data);
    void nameForClientsOnline(QJsonObject& obj);
    void thisConnected();
    void disconnectClient(const QString& id_);
    void clientAlreadyLogged();
private slots:
    //void readyRead();
    void sendLoginData(const QJsonDocument& doc);
    void connectedMessage();
private:
    QTcpSocket *_socket;
    QHostAddress _ip;
    uint _port;
    LoginWindow *_logWin;
    bool isAutorized = false;
    QString userName_;
    uint idC;
};

#endif // CLIENTMANAGER_H
