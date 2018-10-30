#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setAcceptDrops(true);//设置窗口启用拖动

//    QPalette pal(ui->subwidget->palette());  //设置背景色
//    pal.setColor(QPalette::Background, Qt::gray);
//    ui->subwidget->setAutoFillBackground(true);
//    ui->subwidget->setPalette(pal);
//  ui->subwidget->show();


//  ui->subwidget->installEventFilter(this);
//    ui->subwidget->setAcceptDrops(true);  //

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
  /*  int subx =x - ui->subwidget->pos().x();
    int suby =y - ui->subwidget->pos().y();
*/
    qDebug() << x-20 <<y-50 ;
 //   qDebug() << subx << suby ;


    if(m->buttons()&Qt::LeftButton)
    {
//        endPoint.setX(subx);  //设置坐标值
//        endPoint.setY(suby);
//   //     endPoint = m->pos();

//        update(); //执行painterEvent函数
      qDebug() << endPoint ;
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
/*    int subx = x-ui->subwidget->pos().x();
    int suby =y-ui->subwidget->pos().y();

    qDebug() << subx << suby ;
    */
    if(event->button()==Qt::LeftButton)
    {
//        lastPoint = event->pos();
//        isDrawing = true;
        qDebug() << "左键按下" << x << y ;
    }

}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) //鼠标左键释放
    {
//        endPoint = event->pos();
//        isDrawing = false;
        qDebug() << "左键释放" ;
        update();
    }
}

void Widget::dragEnterEvent(QDragEnterEvent *event){
//如果类型是jpg或者png,bmp才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
  if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("png")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("bmp"))
  {
      event->acceptProposedAction();
//      qDebug() << "111" ;
  }

  else
       event->ignore();//否则不接受鼠标事件
}


void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData *qm=event->mimeData();//获取MIMEData
    filename = qm->urls()[0].toLocalFile().toStdString().data();
    pix.load(filename);  //加载图片
    update();            //触发画图
//    qDebug() << "222" ;
}

//pushButton event
void Widget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.png *.bmp *.jpg)"));
        if(fileName.isEmpty())
            return;
        else
        {
           pix.load(fileName.toLatin1().data());//加载图片
           update();                            //触发画图
        }
}

//矩形绘图
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.drawPixmap(20,50,pix.width()+10,pix.height()+10,pix);

/*    // 设置画笔颜色
    painter.setPen(QColor(0, 160, 230));

    int x,y,w,h;
    x = lastPoint.x();
    y = lastPoint.y();
    w = endPoint.x()-x;
    h = endPoint.y()-y;
    painter.setPen(QColor(0, 160, 230));
*/
        // 设置字体：微软雅黑、点大小50、斜体
/*        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(50);
        font.setItalic(true);
        painter.setFont(font);

        // 绘制文本
        painter.drawText(rect(), Qt::AlignCenter, "Qt");
*/
/*
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
*/
}
