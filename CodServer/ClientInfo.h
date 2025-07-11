#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class ClientInfo;
}

class ClientInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ClientInfo(QWidget *parent = nullptr);
    ~ClientInfo();

    void myShow(QListWidgetItem *currentItem);
private:
    Ui::ClientInfo *ui;
};

#endif // CLIENTINFO_H
