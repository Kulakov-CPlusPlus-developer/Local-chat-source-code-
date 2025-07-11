#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    void setConnectionParams(const QString &host,
                             const QString &databaseName,
                             const QString &user,
                             const QString &password,
                             uint port = 5432);

    bool open();

    void close();

    QSqlDatabase& database();

signals:
    void verificationResult(bool success, uint id);//добавил

public slots:
    void verifyUser(const QString &username, const QString &password);//добавил
private:
    QSqlDatabase m_db;
    QString m_connectionName;
};

#endif // DATABASEMANAGER_H
