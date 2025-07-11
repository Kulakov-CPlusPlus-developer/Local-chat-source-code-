#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "QSqlQuery"
#include "mainwindow.h"
#include <QCryptographicHash>
#include <QCryptographicHash>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
    , databaseMan(nullptr)
{
    ui->setupUi(this);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x,y);
}

LoginWindow::~LoginWindow()
{    
    delete ui;
}

QPair<QString, QString> LoginWindow::InputProcessing()
{
    QPair<QString, QString> logPass;
    QString loginText = ui->lneLogin->text();
    QString passwordText = ui->lnePassword->text();
    logPass.first = loginText;
    logPass.second = passwordText;
    return logPass;
}
bool LoginWindow::DataVerificationAdmins()
{    
    databaseMan = std::make_unique<DatabaseManager>();
    databaseMan->setConnectionParams("db", "user_information", "postgres", "8426");

    if(databaseMan->open()){
        QSqlQuery query(databaseMan->database());
        if(query.exec("SELECT * FROM admins")) {
            while (query.next()){
                QString usName = InputProcessing().first;
                QString pass = InputProcessing().second;
                query.prepare("SELECT (password_hash = crypt(:entered_password, password_hash)) AS password_match "
                              "FROM admins WHERE username = :username");
                query.bindValue(":entered_password", pass);
                query.bindValue(":username", usName);

                if (!query.exec()) {
                    qWarning() << "Error:" << query.lastError().text();
                    return false;
                }

                if (query.next()) {
                    bool passwordMatch = query.value("password_match").toBool();
                    if (!passwordMatch) {
                        ui->lbError->setText("Incorrect password!");
                        return false;
                    }else return true;
                    //return query.value("password_match").toBool();
                }

                return false;
            }
        }
    }
    else{
        qDebug() << "Database not open!";
        ui->lbError->setText("Database not open!");
        return false;
    }
}

void LoginWindow::LogIn()
{
    if(DataVerificationAdmins()){
        MainWindow *mainWin = new MainWindow;
        mainWin->setAttribute(Qt::WA_DeleteOnClose);
        mainWin->show();
        this->close();
    }else {
        //ui->lbError->setText("Error!Incorrect data!");
    }
}
