#include "subwin.h"

Subwin::Subwin(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("显示图片");
    setFixedSize(480,320);  //设置窗口固定大小
}
