#include "ClientManager.h"


ClientManager::ClientManager(QHostAddress ip, uint port, QObject *parent)
    : QObject{parent},
    _ip(ip),
    _port(port)
{
    _socket = new QTcpSocket(this);

    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connectedMessage);
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_socket, &QTcpSocket::readyRead, this, [this]() {
        QByteArray data = _socket->readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if(parseError.error != QJsonParseError::NoError)
        {
            qDebug() << "Error parse JSON:" << parseError.errorString();
            return;
        }

        QJsonObject obj = doc.object();

        if(obj["message"].toString() == "disconnect"){
            qDebug() << "New message: " << obj["message"].toString();
            disconnectClient(obj["id"].toString());
        }
        else if(obj.contains("signal")){
            emit nameForClientsOnline(obj);
        }
        else if (!isAutorized) {
            qDebug() << "Step 1";
            if (_logWin) {
                _logWin->showResult(obj["status"].toString());
                if (obj["status"].toString() == "ok") {
                    idC = obj["id"].toVariant().toUInt();
                    isAutorized = true;                    
                    userName_ = _logWin->getUsername();
                    _logWin->close();
                    _logWin = nullptr;
                    emit nameForClientsOnline(obj);                    
                    emit thisConnected();
                }
                if(obj["status"].toString() == "already_logged_in"){
                    emit clientAlreadyLogged();
                }
            }
        } else {
            qDebug() << "New message: " << obj["message"].toString();

            emit dataReceived(obj);
        }
    });
}

void ClientManager::connectToServer()
{
    if (!isAutorized) {
        _logWin = new LoginWindow;
        _logWin->setAttribute(Qt::WA_DeleteOnClose);
        _logWin->show();
        connect(_logWin, &LoginWindow::loginDataReady, this, &ClientManager::sendLoginData);
        connect(this, &ClientManager::clientAlreadyLogged, _logWin, &LoginWindow::blockBtOk);
    }
}

void ClientManager::sendMessage(QString& message, const uint& _id)
{
    QJsonObject messageToWrite;    
    messageToWrite["id"] = QString::number(_id);
    messageToWrite["message"] = message;
    messageToWrite["idSendler"] = QString::number(idC);
    messageToWrite["nameSandler"] = userName_;
    qDebug() << "id: " << messageToWrite["id"].toString();
    QJsonDocument doc(messageToWrite);
    _socket->write(doc.toJson());
}

void ClientManager::sendLoginData(const QJsonDocument& doc)
{
    _socket->connectToHost(_ip, _port);
    qDebug() << "Slot sendLoginData";
    _socket->write(doc.toJson());
}

void ClientManager::connectedMessage()
{
    qDebug() << "connected";
}

void ClientManager::setIsAutorizedTrue()
{
    isAutorized = true;
}

void ClientManager::setIsAutorizedFalse()
{
    isAutorized = false;
}

QString ClientManager::getName()
{
    return userName_;
}

uint ClientManager::getID()
{
    return idC;
}

QTcpSocket *ClientManager::getSocket()
{
    return _socket;
}



