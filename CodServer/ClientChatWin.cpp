#include "ClientChatWin.h"
#include "QSqlQuery"

ClientChatWin::ClientChatWin() {}

void ClientChatWin::inputData(const uint& _id)
{
    databaseMan_ = new DatabaseManager(this);
    databaseMan_->setConnectionParams("db", "user_information", "postgres", "8426");

    if(databaseMan_->open()) {
        QSqlQuery query(databaseMan_->database());

        query.prepare("SELECT username, surname, age, post FROM users WHERE id = ?");
        query.addBindValue(_id);

        if(query.exec() && query.next()) {
            id = _id;
            username = query.value(0).toString();
            surname = query.value(1).toString();
            age = query.value(2).toUInt();
            post = query.value(3).toString();
            isAuthorized  = true;
        } else {

            qDebug() << "User with id" << id << "not found";
        }
    } else {

        qDebug() << "Failed to connect to database";
    }
    databaseMan_->close();
}

QString ClientChatWin::getUsername()
{
    return username;
}

QString ClientChatWin::getSurname()
{
    return surname;
}

uint ClientChatWin::getAge()
{
    return age;
}

QString ClientChatWin::getPost()
{
    return post;
}

QTcpSocket* ClientChatWin::getSocket() const
{
    return socket;
}

bool ClientChatWin::getIsAuthorized()
{
    return isAuthorized;
}

uint ClientChatWin::getId()
{
    return id;
}

void ClientChatWin::inputIsAutorized(bool n)
{
    isAuthorized = n;
}

void ClientChatWin::inputSocket(QTcpSocket* sock)
{
    socket = sock;
}
