#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <cnn.h>
#include <QThread>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QThread *thread;
    DCNN *dnn;

    void dealSignal();
    void dealClose();

signals:
    void startThread(); //启动子线程的信号

private slots:
    void on_pushButton_read_clicked();

    void on_pushButton_init_clicked();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
