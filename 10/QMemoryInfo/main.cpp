#include "qmemoryinfo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMemoryInfo w;
    w.show();

    return a.exec();
}
