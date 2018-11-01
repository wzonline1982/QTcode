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
    //初始化 lineEdit
    ui->lineEdit->setText("30");
    ui->lineEdit_2->setText("255");
    //写textBrowser
    ui->textBrowser->setText("初始化");
    ui->textBrowser->append("加一行");

    this->image = new QImage();  //分配内存
    //绘制画布背景
    isDraw = 1;
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
    paintEventPix = pix.copy(0,0,pix.width(),pix.height());  //考贝图片,至画板
    isDraw = 2;
    update();            //触发画图   

    img = cv::imread(qm->urls()[0].toLocalFile().toStdString().data());  //读取图片
 //   *image = pix.toImage();   //pixmap 转成image
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

           img = cv::imread(fileName.toLatin1().data());  //读取图片
         //  *image = pix.toImage();  //pixmap 转成image
        }
}

//矩形绘图
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter;   //只能有一个绘图设备

    if(isDraw == 1)      //绘制背景色
    {
        painter.begin(this);
        QRect rect(paintx,painty,720,512);
        painter.fillRect(rect,QColor(88,88,88)); // 填充背景色
        painter.end();
    }

   if(isDraw == 3 && !pix.isNull())  //绘制选框
    {
        tempPix = pix.copy(0,0,pix.width(),pix.height());  //考贝图片至临时画布
        painter.begin(&tempPix);
        painter.setPen(Qt::green);
        painter.drawRect(lastPoint.x(),lastPoint.y(),endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());
        painter.end();
        paintEventPix = tempPix.copy(0,0,tempPix.width(),tempPix.height());  //考贝图片

        painter.begin(this);  //切换绘图设备
        painter.drawPixmap(paintx,painty,pix.width(),pix.height(),paintEventPix); //绘制载入图片
        painter.end();
    }

    if(isDraw == 2)          //绘制载入图片
    {
       painter.begin(this);
       painter.drawPixmap(paintx,painty,pix.width(),pix.height(),paintEventPix); //绘制载入图片
       painter.end();
    }

    if(isDraw == 4)          //绘制载入图片
    {
       painter.begin(this);
       painter.drawPixmap(paintx,painty,pix.width(),pix.height(),paintEventPix); //绘制载入图片
       painter.end();
    }
}

void Widget::on_pushButton_2_clicked()
{

    if(img.empty() == false) // 不是空
    {
        int thres =ui->lineEdit->text().toInt();
        int maxval=ui->lineEdit_2->text().toInt();

        qDebug() << thres <<maxval ;
        ui->textBrowser->append("二值化");

        threshold(img, thr, thres, maxval, CV_THRESH_BINARY);
        imshow("ds",thr);
        (*image)=QImage((const unsigned char*)(thr.data),thr.cols,thr.rows,QImage::Format_RGB888);

        tempPix = QPixmap::fromImage(*image);  //image转pixmap
        paintEventPix = tempPix.copy(0,0,tempPix.width(),tempPix.height());  //考贝图片
        isDraw = 2;
        update();            //触发画图
     }
}

void Widget::on_pushButton_3_clicked()
{
    if(img.empty() == false) // 不是空
    {
        int thres =ui->lineEdit->text().toInt();
        int maxval=ui->lineEdit_2->text().toInt();

        Mat imgcp;
        cv::cvtColor(img, imgcp, CV_RGB2GRAY);
        threshold(imgcp, thr, thres, maxval, CV_THRESH_BINARY);

        vector<vector<Point> > contours;
        CvPoint2D32f rectpoint[4];
        CvBox2D rect;
        float angle;
        int Max=0;
        double g_dConArea,temp;
        double Max_X=0,Min_X=5000; //Min_X 初始值足够大

        findContours(thr, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

        if( contours.size() >=1)
            {
                //查找最大轮廓
                    for(vector<Point2f>::size_type i= 0;i < contours.size()-1; i++)
                    {
                        temp = fabs(contourArea(contours[i], true));
                        g_dConArea = fabs(contourArea(contours[i+1], true));
                        Max = temp > g_dConArea ? i: (i+1);
                    }
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
                    qDebug()<< "count > = 1";

            }
            else
            {
                    Max =0;
                    g_dConArea = fabs(contourArea(contours[Max], true));
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
            }
          ui->textBrowser->append( QString("------------------------------"));
          ui->textBrowser->append( QString("countersiz  %1").arg(contours.size()));
          ui->textBrowser->append( QString("angle is %1").arg(angle));
          ui->textBrowser->append( QString("rect is %1").arg(g_dConArea));
          ui->textBrowser->append( QString("center is x = %1  y = %2").arg(rect.center.x).arg(rect.center.y));


          Mat RatationedImg(img.rows, img.cols, CV_8UC1);
          RatationedImg.setTo(0);
          Point2f center = rect.center;  //中心点
          Mat M2 = getRotationMatrix2D(center, angle, 1);//计算旋转加缩放的变换矩阵
          warpAffine(img, RatationedImg, M2, img.size(),1, 0, Scalar(0));//仿射变换
          imshow("Rat",RatationedImg); //显示旋转后图片

          // 旋转后的 轮廓
          Mat RatationedImgcp;
          cv::cvtColor(RatationedImg, RatationedImgcp, CV_RGB2GRAY);
          threshold(RatationedImgcp, RatationedImgcp, thres, maxval, CV_THRESH_BINARY);
          vector<vector<Point> > Ratcontours;
          findContours(RatationedImgcp, Ratcontours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓
          if( Ratcontours.size() >=2)
              {
                  //查找最大轮廓
                      for(vector<Point2f>::size_type i= 0;i < Ratcontours.size()-1; i++)
                      {
                          temp = fabs(contourArea(Ratcontours[i], true));
                          g_dConArea = fabs(contourArea(Ratcontours[i+1], true));
                          Max = temp > g_dConArea ? i: (i+1);
                      }
                      rect =minAreaRect(Mat(Ratcontours[Max]));  //最小矩形
                      cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                      //与水平线的角度
                      angle = rect.angle;

              }
              else
              {
                      Max =0;
                      g_dConArea = fabs(contourArea(Ratcontours[Max], true));
                      rect =minAreaRect(Mat(Ratcontours[Max]));  //最小矩形
                      cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                      //与水平线的角度
                      angle = rect.angle;
              }

          ui->textBrowser->append( QString("angle is %1").arg(angle));
          ui->textBrowser->append( QString("rect is %1").arg(g_dConArea));
          ui->textBrowser->append( QString("center is x = %1  y = %2").arg(rect.center.x).arg(rect.center.y));
          ui->textBrowser->append( QString("Point0 is x = %1  y = %2").arg(rectpoint[0].x).arg(rectpoint[0].y));
          ui->textBrowser->append( QString("Point1 is x = %1  y = %2").arg(rectpoint[1].x).arg(rectpoint[1].y));
          ui->textBrowser->append( QString("Point2 is x = %1  y = %2").arg(rectpoint[2].x).arg(rectpoint[2].y));
          ui->textBrowser->append( QString("Point3 is x = %1  y = %2").arg(rectpoint[3].x).arg(rectpoint[3].y));

          Point topl,topr;

          if(angle < 0)
          {
            topl.x = rectpoint[1].x;
            topl.y = rectpoint[1].y;

            topr.x = rectpoint[3].x;
            topr.y = rectpoint[3].y;
          }
          else if (angle > 0)
          {


          }
          else if(fabs(angle) == 0)
          {
              topl.x = rectpoint[1].x;
              topl.y = rectpoint[1].y;

              topr.x = rectpoint[3].x;
              topr.y = rectpoint[3].y;
          }





        (*image)=QImage((const unsigned char*)(RatationedImg.data),img.cols,thr.rows,QImage::Format_RGB888);

        tempPix = QPixmap::fromImage(*image);  //image转pixmap
        paintEventPix = tempPix.copy(0,0,tempPix.width(),tempPix.height());  //考贝图片
        isDraw = 2;
        update();            //触发画图

     }
}
