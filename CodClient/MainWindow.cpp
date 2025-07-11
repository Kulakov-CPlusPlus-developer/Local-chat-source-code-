#include "MainWindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);
    ui->actionExit->setEnabled(false);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x,y);
}

MainWindow::~MainWindow()
{
    if (_client) delete _client;
    if(ipPort_) delete ipPort_;
    delete ui;
}

void MainWindow::setState(const QString &idSendler, ClientChatWidget*& chatWin)
{
    if(temporaryStorage.contains(idSendler)){
        for(auto sendler : temporaryStorage[idSendler]){
            chatWin->dataReceived(sendler["message"].toString(),
                                  sendler["nameSandler"].toString());
        }
        temporaryStorage[idSendler].clear();
    }else return;
}

void MainWindow::on_actionConnected_triggered()
{
    QString fileName = QDir(QCoreApplication::applicationDirPath()).filePath("config.json");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "File not open: " << fileName;
        ui->lbErrorMessage->setText("Config file not open!");
        return;
    }
    if (file.size() == 0) {
        qDebug() << "File is empty: " << fileName;
        file.close();
        ui->lbErrorMessage->setText("Config file is empty!");
        return;
    }
    QByteArray dataServer = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(dataServer, &parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Error parse JSON:" << parseError.errorString();
        ui->lbErrorMessage->setText("Problem with the config file!");
        return;
    }

    QJsonObject obj = doc.object();

    QString ip_ = obj["ip"].toString();
    uint port = obj["port"].toVariant().toUInt();

    QHostAddress ip(ip_);
    _client = new ClientManager(ip, port);
    connect(_client, &ClientManager::connected, [this](){
        ui->centralwidget->setEnabled(true);
    });
    connect(_client, &ClientManager::disconnected, [this](){
        ui->centralwidget->setEnabled(false);
        _client->setIsAutorizedFalse();
    });

    connect(_client, &ClientManager::nameForClientsOnline, this, &MainWindow::addItemClientsOnline);

    connect(_client, &ClientManager::dataReceived, this, &MainWindow::dataReceived);
    connect(_client, &ClientManager::thisConnected, this, &MainWindow::actionConnectedBlock);    
    connect(_client, &ClientManager::disconnectClient, this, &MainWindow::anotherClientDisconnected);

    _client->connectToServer();
}

void MainWindow::dataReceived(const QJsonObject& obj)
{
    bool find = false;
    for(int i = 0; i < ui->tbChats->count(); i++){
        QWidget *tab = ui->tbChats->widget(i);
        if (!tab) {
            qDebug() << "tabError";
            continue;
        }
        ClientChatWidget *chat = qobject_cast<ClientChatWidget*>(tab);
        if (!chat) {
            qDebug() << "chatError";
            continue;
        }
        //qDebug() << obj["idSendler"].toString() << " " << QString::number(chat->getID());
        if(obj["idSendler"].toString() == QString::number(chat->getID())){
            //emit dataReceivedReady(obj["message"].toString(), username_);
            chat->dataReceived(obj["message"].toString(), obj["nameSandler"].toString());
        }
    }
    if(!find){
        temporaryStorage[obj["idSendler"].toString()].push_back(obj);
    }
}

void MainWindow::addItemClientsOnline(QJsonObject &obj)
{
    QJsonArray arr = obj["arr"].toArray();
    //QJsonArray arrID = obj["arrID"].toArray();
    for(int i = 0; i < arr.size(); i++)
    {
        //qDebug() << arr[i] << " " << arrID[i] << " " << _client->getID();
        QJsonObject clientInfo = arr[i].toObject();
        QString nameClientItem = clientInfo["username"].toString()+" "+clientInfo["surname"].toString()+" "+clientInfo["id"].toString();

        if(ui->lstClients->findItems(nameClientItem, Qt::MatchExactly).isEmpty())
        {
            if(clientInfo["id"].toVariant().toUInt() != _client->getID())
            {
                QListWidgetItem* item = new QListWidgetItem(nameClientItem);
                item->setData(Qt::UserRole, clientInfo["id"].toString());
                item->setData(Qt::UserRole+1, clientInfo["username"].toString());
                item->setData(Qt::UserRole+2, clientInfo["surname"].toString());
                item->setData(Qt::UserRole+3, clientInfo["age"].toString());
                item->setData(Qt::UserRole+4, clientInfo["post"].toString());
                item->setData(Qt::UserRole+5, false);
                ui->lstClients->addItem(item);
            }
        }
    }
}

void MainWindow::showContextMenu(QListWidgetItem *item)
{
    QMenu contextMenu(tr("choose"), this);

    contextMenu.setStyleSheet(
        "QMenu { background-color: rgb(54, 54, 54); }"
        "QMenu::item { color: white; }"
        "QMenu::item:selected { background-color: white; color: rgb(54, 54, 54); }"
        "QMenu::item:disabled { color: gray; }"
        );

    QAction *action1 = contextMenu.addAction("Start chat");
    QAction *action2 = contextMenu.addAction("Close chat");
    QAction *action3 = contextMenu.addAction("Info");

    if(item->data(Qt::UserRole+5).toBool())
    {
        action1->setEnabled(false);
        action2->setEnabled(true);
    }
    else
    {
        action1->setEnabled(true);
        action2->setEnabled(false);
    }

    QPoint globalPos = ui->lstClients->viewport()->mapToGlobal(
        ui->lstClients->visualItemRect(item).center()
        );

    QAction *selectedAction = contextMenu.exec(globalPos);

    QString name = item->data(Qt::UserRole + 1).toString()+" "+ item->data(Qt::UserRole + 2).toString()+" "+item->data(Qt::UserRole).toString();

    if (selectedAction == action1)
    {
        item->setData(Qt::UserRole+5, true);

        auto chatWidget = new ClientChatWidget(_client);        
        bool have = false;

        chatWidget->setID(item->data(Qt::UserRole).toUInt());

        for(int i =0; i < ui->tbChats->count(); i++)
        {
            if(ui->tbChats->tabText(i) == name)
            {
                have = true;
                break;
            }
        }
        if(!have)
        {
            ui->tbChats->addTab(chatWidget, name);
            setState(QString::number(chatWidget->getID()), chatWidget);

        }
    }
    else if (selectedAction == action2)
    {
        item->setData(Qt::UserRole+5, false);
        for(int i =0; i < ui->tbChats->count(); i++)
        {
            if(ui->tbChats->tabText(i) == name)
            {
                ui->tbChats->removeTab(i);
                break;
            }
        }
    }
    else if (selectedAction == action3)
    {
        auto clientInformatoin = new ClientInfo;
        clientInformatoin->myShow(item);
    }
}

void MainWindow::actionConnectedBlock()
{
    ui->actionConnected->setEnabled(false);
    ui->actionServer->setEnabled(false);
    ui->actionExit->setEnabled(true);
}

void MainWindow::anotherClientDisconnected(const QString &id_)
{
    for(int i = 0; i < ui->tbChats->count(); i++){
        QWidget *tab = ui->tbChats->widget(i);
        if (!tab) {
            qDebug() << "tabError";
            continue;
        }
        ClientChatWidget *chat = qobject_cast<ClientChatWidget*>(tab);
        if (!chat) {
            qDebug() << "chatError";
            continue;
        }
        if(id_ == QString::number(chat->getID()))
        {
            delete chat;
            ui->tbChats->removeTab(i);
            break;
        }
    }

    for(int i = 0; i < ui->lstClients->count(); i++){
        QListWidgetItem* item = ui->lstClients->item(i);
        if(item->data(Qt::UserRole) == id_){
            delete ui->lstClients->takeItem(i);
            item->setData(Qt::UserRole+5, false);
            break;
        }
    }
}

void MainWindow::on_actionExit_triggered()
{    
    QString dis = "disconnect";    
    ui->tbChats->clear();
    ui->lstClients->clear();
    _client->sendMessage(dis, _client->getID());
    _client->getSocket()->disconnectFromHost();
    ui->actionConnected->setEnabled(true);
    ui->actionExit->setEnabled(false);
    ui->actionServer->setEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!_isDisconnecting)
    {
        event->ignore();
        _isDisconnecting = true;
        on_actionExit_triggered();

        connect(_client, &ClientManager::disconnected, this, [this]() {
            this->close();
        });
    }
    else
    {
        event->accept();
    }
}


void MainWindow::on_actionServer_triggered()
{
    ipPort_ = new ipPort;
    ipPort_->show();
}

