#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ServerMan.h"
#include <QListWidgetItem>
#include <QScreen>
#include "ClientInfo.h"
#include <memory>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void newConnection();
    void newLstClientItem(ClientChatWin *info);
    void clientDisconnected(ClientChatWin *info);
    void on_lstClients_itemDoubleClicked(QListWidgetItem *item);
private:
    Ui::MainWindow *ui;
    ServerMan *server;
    //QList<std::unique_ptr<ClientInfo>> infoWidgets;
    std::vector<std::unique_ptr<ClientInfo>> infoWidgets;
private://methods
    void setupServer();
};
#endif // MAINWINDOW_H
