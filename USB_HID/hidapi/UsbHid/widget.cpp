#include "widget.h"
#include "ui_widget.h"

#include <windows.h>
#include "hidapi.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    int res;
    unsigned char buf[65];
    wchar_t wstr[255];
    hid_device *handle;
    int i;


}

Widget::~Widget()
{
    delete ui;
}
