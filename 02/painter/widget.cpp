#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);//设置窗口启用拖动
}

Widget::~Widget()
{
    delete ui;
}


void Widget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void Widget::dragEnterEvent(QDragEnterEvent *event){
//如果类型是jpg或者png,bmp才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
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


void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData *qm=event->mimeData();//获取MIMEData
    filename = qm->urls()[0].toLocalFile().toStdString().data();
    isDrag = true;
 //   update();
}


void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if(isDrag == true)
    {
        // 设置画笔颜色
        painter.setPen(QColor(0, 160, 230));

        // 设置字体：微软雅黑、点大小50、斜体
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(50);
        font.setItalic(true);
        painter.setFont(font);

        // 绘制文本
        painter.drawText(rect(), Qt::AlignCenter, "QT");
    }
}
