#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include "MessageWidget.h"
#include <QScreen>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();    
    void showResult(const QString& result);
    QString getUsername();
signals:
    void loginDataReady(const QJsonDocument& doc);
public slots:
    void blockBtOk();
private slots:
    void on_pushButton_clicked();

    void unlockBtOk();
private:
    Ui::LoginWindow *ui;
    QTcpSocket *_socket_;
    QString _username;
};

#endif // LOGINWINDOW_H
