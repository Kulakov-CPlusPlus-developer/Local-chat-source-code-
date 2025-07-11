#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupServer();

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x,y);
}

MainWindow::~MainWindow()
{
    delete server;
    delete ui;
    infoWidgets.clear();
}

void MainWindow::newConnection()
{
    ui->lbMessage->setText("new connection");

    QTimer::singleShot(10000, this, [this]() {
        ui->lbMessage->clear();
    });
}

void MainWindow::newLstClientItem(ClientChatWin *info)
{
    QString str = info->getUsername() + " " + info->getSurname() + " " + QString::number(info->getId());
    if(ui->lstClients->findItems(str, Qt::MatchExactly).isEmpty())
    {
    QListWidgetItem* item = new QListWidgetItem(info->getUsername() + " " + info->getSurname() + " " + QString::number(info->getId()));
    item->setData(Qt::UserRole, info->getId());
    item->setData(Qt::UserRole+1, info->getUsername());
    item->setData(Qt::UserRole+2, info->getSurname());
    item->setData(Qt::UserRole+3, info->getAge());
    item->setData(Qt::UserRole+4, info->getPost());

    ui->lstClients->addItem(item);
    }
}
void MainWindow::clientDisconnected(ClientChatWin *info)
{
    QString buf = info->getUsername() + " " + info->getSurname() + " " + QString::number(info->getId());    
    for(int i = 0; i < ui->lstClients->count(); i++)
    {
        if(ui->lstClients->item(i)->text() == buf)
        {
            delete ui->lstClients->takeItem(i);
            break;
        }
    }
}
void MainWindow::setupServer()
{
    server = new ServerMan();
    connect(server, &ServerMan::newClientIsAutorized, this,  &MainWindow::newLstClientItem);    
    connect(server, &ServerMan::clientDisconnected, this, &MainWindow::clientDisconnected);
    connect(server, &ServerMan::newConection, this, &MainWindow::newConnection);
}
void MainWindow::on_lstClients_itemDoubleClicked(QListWidgetItem *item)
{
    auto info_ = std::make_unique<ClientInfo>();
    info_->myShow(item);
    infoWidgets.push_back(std::move(info_));
}
