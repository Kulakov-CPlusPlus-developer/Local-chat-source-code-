#include "ipPort.h"
#include "ui_ipPort.h"

ipPort::ipPort(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ipPort)
{
    ui->setupUi(this);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x,y);

    connect(ui->lnPort, &QLineEdit::returnPressed, this, &ipPort::on_pushButton_clicked);
}

ipPort::~ipPort()
{
    delete ui;
}

void ipPort::on_pushButton_clicked()
{
    if(!ui->lnIP->text().isEmpty() && !ui->lnPort->text().isEmpty())
    {
        QString fileName = QDir(QCoreApplication::applicationDirPath()).filePath("config.json");

        QJsonObject obj;
        obj["ip"] = ui->lnIP->text();
        obj["port"] = ui->lnPort->text();
        QJsonDocument doc(obj);

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(doc.toJson());
            file.close();
        } else
        {
            qDebug() << "File error " << fileName;
        }
        ui->lnIP->clear();
        ui->lnPort->clear();
        this->close();
    }else
    {
        ui->lnIP->setPlaceholderText("Input ip-address...");
        ui->lnPort->setPlaceholderText("Input port...");
    }
}

