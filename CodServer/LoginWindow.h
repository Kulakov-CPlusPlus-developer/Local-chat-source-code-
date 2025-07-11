#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "databasemanager.h"
#include <QPair>
#include <QScreen>
#include <memory>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
public slots:
    void LogIn();
public:
    QPair<QString, QString> InputProcessing();
    //bool DataVerificationUsers (const QString& usName, const QString& pass);
    bool DataVerificationAdmins ();
private:
    Ui::LoginWindow *ui;    
    std::unique_ptr<DatabaseManager> databaseMan;
};

#endif // LOGINWINDOW_H
