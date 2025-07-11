#ifndef MESSAGEWIDGETME_H
#define MESSAGEWIDGETME_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class MessageWidgetMe;
}

class MessageWidgetMe : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidgetMe(const QString &text, const QString &username, QWidget *parent = nullptr);
    ~MessageWidgetMe();

private:
    Ui::MessageWidgetMe *ui;
};

#endif // MESSAGEWIDGETME_H
