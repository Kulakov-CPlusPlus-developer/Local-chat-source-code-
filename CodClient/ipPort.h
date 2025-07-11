#ifndef IPPORT_H
#define IPPORT_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScreen>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

namespace Ui {
class ipPort;
}

class ipPort : public QWidget
{
    Q_OBJECT

public:
    explicit ipPort(QWidget *parent = nullptr);
    ~ipPort();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ipPort *ui;
};

#endif // IPPORT_H
