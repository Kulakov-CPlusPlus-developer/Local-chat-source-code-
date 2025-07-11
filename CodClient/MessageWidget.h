#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(const QString &text, const QString &username, QWidget *parent = nullptr);
    ~MessageWidget();

    //void setMessage(bool isMyMess);
private:
    Ui::MessageWidget *ui;
};

#endif // MESSAGEWIDGET_H
