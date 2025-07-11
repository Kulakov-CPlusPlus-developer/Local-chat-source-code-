#include "ServerMan.h"

ServerMan::ServerMan(uint port, QObject *parent)
    : QObject{parent}
{
    dbWorker = new DatabaseManager();
    dbThread = new QThread(this);
    dbWorker->moveToThread(dbThread);
    connect(dbWorker, &DatabaseManager::verificationResult, this, &ServerMan::onVerificationResult);
    dbThread->start();
    setupServer(port);
}

ServerMan::~ServerMan()
{
    dbThread->quit();
    dbThread->wait();
    delete dbWorker;
    delete dbThread;
    for(auto& userInfo: _usersInfo){
        userInfo->getSocket()->close();
        userInfo->deleteLater();
    }
    _usersInfo.clear();
    server->close();
    delete server;
}
QJsonObject ServerMan::ItemClientsOnline(QJsonObject& response, QList<ClientChatWin *>::iterator it)
{
    QJsonArray arr;

    QJsonObject clientInfo;

    for(int i = 0; i < _usersInfo.size(); i++)
    {
        if(_usersInfo[i]->getUsername() != (*it)->getUsername())
        {
            clientInfo["id"] = QString::number(_usersInfo[i]->getId());
            clientInfo["username"] = _usersInfo[i]->getUsername();
            clientInfo["surname"] = _usersInfo[i]->getSurname();
            clientInfo["age"] = QString::number(_usersInfo[i]->getAge());
            clientInfo["post"] = _usersInfo[i]->getPost();

            arr.append(clientInfo);            
        }
    }
    response["arr"] = arr;
    return response;
}

QJsonObject ServerMan::notificationClients(QJsonObject &response)
{
    QJsonArray arr;    
    QJsonObject clientInfo;

    for(int i = 0; i < _usersInfo.size(); i++)
    {
        clientInfo["id"] = QString::number(_usersInfo[i]->getId());
        clientInfo["username"] = _usersInfo[i]->getUsername();
        clientInfo["surname"] = _usersInfo[i]->getSurname();
        clientInfo["age"] = QString::number(_usersInfo[i]->getAge());
        clientInfo["post"] = _usersInfo[i]->getPost();

        arr.append(clientInfo);        
    }
    response["arr"] = arr;
    return response;
}

void ServerMan::newClientConnectionReceived()
{
    auto client = server->nextPendingConnection();
    ClientChatWin* info = new ClientChatWin();
    info->inputSocket(client);
    info->inputIsAutorized(false);
    _usersInfo << info;

    connect(client, &QTcpSocket::disconnected, this, &ServerMan::onClientDisconnected);
    connect(client, &QTcpSocket::readyRead, this, &ServerMan::onClientReadyRead);    
}
void ServerMan::onClientDisconnected()
{
    auto socket_ = qobject_cast<QTcpSocket*>(sender());
    if (!socket_) return;


    auto it = std::find_if(_usersInfo.begin(), _usersInfo.end(), [socket_](ClientChatWin* info){
        return info->getSocket() == socket_;
    });

    if (it != _usersInfo.end())
    {
        ClientChatWin* client = *it;
        _usersInfo.removeOne(client);
        emit clientDisconnected(client);
        client->deleteLater();
    }
}

void ServerMan::onClientReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if(!client) return;

    QByteArray data = client->readAll();
    QString name;

    auto it = std::find_if(_usersInfo.begin(), _usersInfo.end(), [client](ClientChatWin *info){
        return info->getSocket() == client;
    });


    if(it == _usersInfo.end())
    {
        qDebug() << "Unknown client!";
        return;
    }
    name = (*it)->getUsername();

    if(!(*it)->getIsAuthorized())
    {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if(parseError.error != QJsonParseError::NoError)
        {
            qDebug() << "Error parse JSON:" << parseError.errorString();
            return;
        }

        QJsonObject obj = doc.object();

        QString username = obj["username"].toString();
        QString password = obj["password"].toString();

        pendingAuthClient = client;
        pendingAuthIt = it;
        QMetaObject::invokeMethod(dbWorker, "verifyUser", Qt::QueuedConnection,
                                  Q_ARG(QString, username), Q_ARG(QString, password));
        return;
    }
    else
    {
        qDebug() << "is Autorized";
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if(parseError.error != QJsonParseError::NoError)
        {
            return;
        }

        QJsonObject obj = doc.object();
        QString id = obj["id"].toString();

        auto recipient = std::find_if(_usersInfo.begin(), _usersInfo.end(), [id](ClientChatWin *info){

            return QString::number(info->getId()) == id;
        });

        if(recipient == _usersInfo.end()){
            qDebug() << "Unknown client!";
            return;
        }

        if(obj["message"].toString() != "disconnect"){

            (*recipient)->getSocket()->write(doc.toJson());
        }
        else {

            (*recipient)->getSocket()->disconnectFromHost();
            for(auto user : _usersInfo){
                if(user->getSocket() != client){
                    user->getSocket()->write(doc.toJson());
                }
            }
        }

    }
}

void ServerMan::setupServer(uint port)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerMan::newClientConnectionReceived);
    connect(server, &QTcpServer::newConnection, this, &ServerMan::newConection);

    if(server->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server listen";
    }
}

void ServerMan::onVerificationResult(bool success, uint id)
{
    if (!pendingAuthClient || pendingAuthIt == _usersInfo.end())
    {
        return;
    }
    QTcpSocket* client = pendingAuthClient;
    auto it = pendingAuthIt;
    pendingAuthClient = nullptr;    

    if (success)
    {
        bool alreadyAuthorized = std::any_of(_usersInfo.begin(), _usersInfo.end(), [id](ClientChatWin* user){
            return user->getId() == id && user->getIsAuthorized();
        });
        if(alreadyAuthorized)
        {
            QJsonObject response;
            response["status"] = "already_logged_in";
            QJsonDocument doc(response);
            client->write(doc.toJson());
            client->disconnectFromHost();
            return;
        }
        (*it)->inputData(id);
        QJsonObject response;
        response["status"] = "ok";
        response["id"] = QString::number(id);
        QJsonObject signForAll;
        signForAll["signal"] = "notification";
        QJsonDocument doc(ItemClientsOnline(response, it));
        QJsonDocument doc2(notificationClients(signForAll));
        for(auto user : _usersInfo)
        {
            if(user->getSocket() != client)
            {
                user->getSocket()->write(doc2.toJson());
            }
        }
        client->write(doc.toJson());
        emit newClientIsAutorized(*it);
    } else
    {
        QJsonObject response;
        response["status"] = "Incorrect password";
        QJsonDocument doc(response);
        client->write(doc.toJson());
    }
}
