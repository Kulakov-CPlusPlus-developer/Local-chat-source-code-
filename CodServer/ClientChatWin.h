#ifndef CLIENTCHATWIN_H
#define CLIENTCHATWIN_H

#include <QObject>
#include "databasemanager.h"
#include <QTcpSocket>

class ClientChatWin : public QObject
{
    Q_OBJECT
private://attributes

    uint id;
    QString username;
    QString surname;
    uint age;
    QString post;
    bool isAuthorized = false;
    QTcpSocket* socket;
    DatabaseManager *databaseMan_;

public:
    ClientChatWin();

    QString getUsername();
    QString getSurname();
    uint getAge();
    QString getPost();
    QTcpSocket* getSocket () const;
    bool getIsAuthorized();
    uint getId();


    void inputData(const uint& id);
    void inputIsAutorized(bool n);
    void inputSocket(QTcpSocket* sock);
};

#endif // CLIENTCHATWIN_H
