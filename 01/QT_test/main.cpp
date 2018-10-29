#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    //有且只有一个应用程序类的对象
    QApplication a(argc, argv);
    Widget w;    //执行构造函数
    w.setWindowTitle(QString("标题")); //设置标题

    w.show();

    return a.exec();
}
