#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"

#include <windows.h>
#include <stdio.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    struct hid_device_info *devs, *cur_dev;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
