#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setAcceptDrops(true);//设置窗口启用拖动

    isDraw = 1; //绘制画布背景
    update();
}

Widget::~Widget()
{
    delete ui;
}


/**********重写事件响应的虚函数****************/
void Widget::mouseMoveEvent(QMouseEvent *m)
{
    int x = m->pos().x();
    int y = m->pos().y();
    if(x>=paintx && y>=painty && x<=720 && y<= 640 && (m->buttons()&Qt::LeftButton))
    {
        endPoint.setX(x-paintx);
        endPoint.setY(y-painty);

        qDebug() << endPoint ;
        isDraw = 3;
        update();
    }

//    if(m->buttons()&Qt::LeftButton)
//    {
//      update(); //执行painterEvent函数
//      qDebug() << endPoint ;
//    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    if(x>=paintx && y>=painty && x<=720 && y<= 640 && (event->button()==Qt::LeftButton))
    {
        lastPoint.setX(x-paintx);
        lastPoint.setY(y-painty);
        qDebug() << "左键按下" << lastPoint;
    }

    if(event->button()==Qt::LeftButton)
    {
       // qDebug() << "左键按下" << lastPoint;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    if(x>=paintx && y>=painty && x<=720 && y<= 640 && event->button()==Qt::LeftButton) //鼠标左键释放
    {   
        qDebug() << "左键释放" ;
    }
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

//拖入图片
void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData *qm=event->mimeData();//获取MIMEData
    filename = qm->urls()[0].toLocalFile().toStdString().data();
    pix.load(filename);  //加载图片
    paintEventPix = pix.copy(0,0,pix.width(),pix.height());  //考贝图片
    isDraw = 2;
    update();            //触发画图
}

//pushButton event 打开图片
void Widget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.png *.bmp *.jpg)"));
        if(fileName.isEmpty())
            return;
        else
        {
           pix.load(fileName.toLatin1().data());//加载图片
           paintEventPix = pix.copy(0,0,pix.width(),pix.height());  //考贝图片
           isDraw = 2;
           update();                            //触发画图
        }
}

//矩形绘图
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter;   //只能有一个绘图设备

    if(isDraw == 1)
    {
        painter.begin(this);
        QRect rect(paintx,painty,720,512);
        painter.fillRect(rect,QColor(88,88,88)); // 填充背景色
        painter.end();
    }

   if(isDraw == 3 && !pix.isNull())
    {
        tempPix = pix.copy(0,0,pix.width(),pix.height());  //考贝图片至画布
        painter.begin(&tempPix);
        painter.setPen(Qt::green);
        painter.drawRect(lastPoint.x(),lastPoint.y(),endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());
        painter.end();
        paintEventPix = tempPix.copy(0,0,tempPix.width(),tempPix.height());  //考贝图片
      //  painter.drawPixmap(paintx,painty,pix.width(),pix.height(),paintEventPix); //绘制载入图片

        painter.begin(this);  //切换绘图设备
        painter.drawPixmap(paintx,painty,pix.width(),pix.height(),paintEventPix); //绘制载入图片
        painter.end();
    }

    if(isDraw == 2)
    {
       painter.begin(this);
       painter.drawPixmap(paintx,painty,pix.width(),pix.height(),paintEventPix); //绘制载入图片
       painter.end();
    }
}
