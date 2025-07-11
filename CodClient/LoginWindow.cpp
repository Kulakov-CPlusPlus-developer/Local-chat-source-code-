#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QJsonObject>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x,y);

    connect(ui->lnPassword, &QLineEdit::returnPressed, this, &LoginWindow::on_pushButton_clicked);
    connect(ui->lnPassword, &QLineEdit::textChanged, this, &LoginWindow::unlockBtOk);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::showResult(const QString &result)
{
    ui->lbErrorMess->setText(result);
}

QString LoginWindow::getUsername()
{
    return _username;
}

void LoginWindow::on_pushButton_clicked()
{
    if(!ui->lnLogin->text().isEmpty() && !ui->lnPassword->text().isEmpty())
    {
        QJsonObject obj;
        obj["username"] = ui->lnLogin->text();
        obj["password"] = ui->lnPassword->text();
        _username = ui->lnLogin->text();;
        QJsonDocument doc(obj);
        qDebug() << "Button clicked";
        emit loginDataReady(doc);
    }else
    {
        ui->lbErrorMess->setText("Input login and pass!");
    }
}

void LoginWindow::blockBtOk()
{
    ui->pushButton->setEnabled(false);
}

void LoginWindow::unlockBtOk()
{
    ui->pushButton->setEnabled(true);
}

