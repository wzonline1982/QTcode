#include "widget.h"
#include "ui_widget.h"

#include <windows.h>
#include "hidapi.h"

#include "QDebug"

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


    if (hid_init())
                    qDebug() << "init err" ;
    else  qDebug() << "init secess" ;

        devs = hid_enumerate(0x0, 0x0);
        cur_dev = devs;
        while (cur_dev) {
            printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
            printf("\n");
            printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
            printf("  Product:      %ls\n", cur_dev->product_string);
            printf("  Release:      %hx\n", cur_dev->release_number);
            printf("  Interface:    %d\n",  cur_dev->interface_number);
            printf("\n");
            cur_dev = cur_dev->next;
        }

        qDebug() << "running" ;
}

Widget::~Widget()
{
    delete ui;
}
