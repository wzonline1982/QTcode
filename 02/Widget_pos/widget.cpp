#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->subwidget->setWindowTitle("显示窗口");

    QPalette pal(ui->subwidget->palette());  //设置背景色
    pal.setColor(QPalette::Background, Qt::gray);
    ui->subwidget->setAutoFillBackground(true);
    ui->subwidget->setPalette(pal);
 //   ui->subwidget->show();

    this->setAcceptDrops(true);//设置窗口启用拖动
//    ui->subwidget->installEventFilter(this);
    ui->subwidget->setAcceptDrops(true);  //


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
    int subx =x - ui->subwidget->pos().x();
    int suby =y - ui->subwidget->pos().y();

 //   qDebug() << x <<y ;
 //   qDebug() << subx << suby ;
    //
    if(m->buttons()&Qt::LeftButton)
    {
        endPoint.setX(subx);  //设置坐标值
        endPoint.setY(suby);
   //     endPoint = m->pos();

        update(); //执行painterEvent函数

   //     qDebug() << endPoint ;
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    int subx = x-ui->subwidget->pos().x();
    int suby =y-ui->subwidget->pos().y();

    qDebug() << subx << suby ;
    if(event->button()==Qt::LeftButton)
    {
        lastPoint = event->pos();
        isDrawing = true;
        qDebug() << "左键按下" ;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) //鼠标左键释放
    {
        endPoint = event->pos();
        isDrawing = false;
        update();
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
      qDebug() << "111" ;
  }

  else
       event->ignore();//否则不接受鼠标事件
}


void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData *qm=event->mimeData();//获取MIMEData
    qDebug() << "222" ;
    QPalette palette;
    QPixmap pixmap(qm->urls()[0].toLocalFile().toStdString().data());
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    ui->subwidget->setPalette(palette);
}

//pushButton event
void Widget::on_pushButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.png *.bmp *.jpg)"));
        if(fileName.isEmpty())
            return;
        else
        {
            QPalette palette;
            QPixmap pixmap(fileName.toLatin1().data()); //路径不能包含中文
            palette.setBrush(QPalette::Window, QBrush(pixmap));
            ui->subwidget->setPalette(palette);
        }
}

//矩形绘图
void Widget::paintEvent(QPaintEvent *)
{
    int x,y,w,h;
    x = lastPoint.x();
    y = lastPoint.y();
    w = endPoint.x()-x;
    h = endPoint.y()-y;
    QPainter painter(this);

    if(isDrawing) //如果正在绘图，就在辅助画布上绘制
    {
        tempPix = pix;
        QPainter pp(&tempPix);
        pp.begin(&tempPix);
        pp.setPen(Qt::green);
        pp.drawRect(x,y,w,h);
        painter.drawPixmap(0,0,tempPix);
        pp.end();
    }
    else
    {
        QPainter pp(&pix);
        pp.begin(&pix);
        pp.setPen(Qt::green);
        pp.drawRect(x,y,w,h);
        painter.drawPixmap(0,0,pix);
        pp.end();
    }

}
