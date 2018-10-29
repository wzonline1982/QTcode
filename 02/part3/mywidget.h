#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <subwin.h>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

private:
    Ui::MyWidget *ui;
    Subwin w;
};

#endif // MYWIDGET_H
