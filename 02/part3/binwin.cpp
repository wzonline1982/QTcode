#include "binwin.h"

binwin::binwin(QWidget *parent) : QWidget(parent)
{
    this->setAcceptDrops(true);//设置窗口启用拖动
    this->setWindowTitle("二值化");
    setFixedSize(720,512);  //设置窗口固定大小
}
