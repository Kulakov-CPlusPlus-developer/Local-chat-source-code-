#ifndef SERVERMAN_H
#define SERVERMAN_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include "ClientChatWin.h"
#include "databasemanager.h"
#include <QThread>

class ServerMan : public QObject
{
    Q_OBJECT
public:
    explicit ServerMan(uint port = 8080, QObject *parent = nullptr);
    ~ServerMan();

    QJsonObject ItemClientsOnline(QJsonObject& response, QList<ClientChatWin *>::iterator it);
    QJsonObject notificationClients(QJsonObject& response);
private slots:
    void newClientConnectionReceived();
    void onClientDisconnected();
    void onClientReadyRead();
    void onVerificationResult(bool success, uint id);

signals:
    void newConection();
    void clientDisconnected(ClientChatWin *client);
    void newClientIsAutorized(ClientChatWin *info);
    void isAutorizerForClientWidg(const QByteArray& data, const QString& name);

private:
    QTcpServer *server;    
    QList<ClientChatWin *> _usersInfo;
    DatabaseManager* dbWorker = nullptr;
    QThread* dbThread = nullptr;
    QTcpSocket* pendingAuthClient = nullptr;
    QList<ClientChatWin*>::iterator pendingAuthIt;
private:
    void setupServer(uint port);
};

#endif // SERVERMAN_H
