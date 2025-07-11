#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ClientManager.h"
#include <QJsonObject>
#include <QJsonArray>
#include "MessageWidgetMe.h"
#include "ClientChatWidget.h"
#include <QListWidgetItem>

#include <QScreen>
#include <QMap>
#include <QVector>
#include "ClientInfo.h"
#include <QCloseEvent>
#include "ipPort.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setState(const QString& idSendler, ClientChatWidget*& chatWin);
private slots:
    void on_actionConnected_triggered();
    void dataReceived(const QJsonObject& data);   
    void addItemClientsOnline(QJsonObject& obj);
    void showContextMenu(QListWidgetItem *item);
    void actionConnectedBlock();
    void anotherClientDisconnected(const QString& id_);
    void on_actionExit_triggered();
    void on_actionServer_triggered();

protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::MainWindow *ui;
    ClientManager *_client;
    int numberForClients = 1;
    QMap<QString, QVector<QJsonObject>> temporaryStorage;
    bool _isDisconnecting = false;
    std::vector<std::unique_ptr<ClientInfo>> infoWidgets;
    ipPort *ipPort_;
};
#endif // MAINWINDOW_H
