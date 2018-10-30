#include "subwin.h"
#include <QDebug>

Subwin::Subwin(QWidget *parent) : QWidget(parent)
{
    this->setAcceptDrops(true);//设置窗口启用拖动
    this->setWindowTitle("显示图片");
    setFixedSize(720,512);  //设置窗口固定大小
}

void Subwin::dragEnterEvent(QDragEnterEvent *event)
{
    if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
             ||!event->mimeData()->urls()[0].fileName().right(3).compare("png")
             ||!event->mimeData()->urls()[0].fileName().right(3).compare("bmp")
            ||!event->mimeData()->urls()[0].fileName().right(3).compare("BMP"))
    {
        event->acceptProposedAction();
    }

    else
         event->ignore();//否则不接受鼠标事件
}
void Subwin::dropEvent(QDropEvent *event)
{
    const QMimeData *qm=event->mimeData();//获取MIMEData
    filename = qm->urls()[0].toLocalFile().toStdString().data();
    pix.load(filename);  //加载图片
    paintEventPix = pix.copy(0,0,pix.width(),pix.height());  //考贝图片
    update();            //触发画图
    emit subSignal(filename);
}

void Subwin::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,pix.width(),pix.height(),paintEventPix); //绘制载入图片
}
