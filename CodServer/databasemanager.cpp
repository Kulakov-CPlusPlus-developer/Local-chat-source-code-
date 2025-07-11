#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent)
    :QObject(parent)
{
    m_connectionName =
        QString("PosgresConnection_%1").arg(reinterpret_cast<quintptr>(this));

    m_db = QSqlDatabase::addDatabase("QPSQL",m_connectionName);
}

DatabaseManager::~DatabaseManager()
{
    close();
    QSqlDatabase::removeDatabase(m_connectionName);   
}

void DatabaseManager::setConnectionParams(const QString &host, const QString &databaseName, const QString &user, const QString &password, uint port)
{
    m_db.setHostName(host);
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);
}
bool DatabaseManager::open()
{
    if (!m_db.open())
    {
        qDebug() << "Database connection error:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Connection to the database is successful!";
    return true;
}

void DatabaseManager::close()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

QSqlDatabase& DatabaseManager::database()
{
    return m_db;
}

void DatabaseManager::verifyUser(const QString &username, const QString &password)
{
    setConnectionParams("db", "user_information", "postgres", "8426");
    bool ok = false;
    uint id = 0;
    if (open()) {
        QSqlQuery query(database());
        query.prepare("SELECT id, (password_hash = crypt(:entered_password, password_hash)) AS password_match FROM users WHERE username = :username");
        query.bindValue(":entered_password", password);
        query.bindValue(":username", username);
        if (query.exec() && query.next()) {
            bool match = query.value("password_match").toBool();
            if (match) {
                id = query.value("id").toUInt();
                ok = true;
            }
        }
        close();
    }
    emit verificationResult(ok, id);
}
