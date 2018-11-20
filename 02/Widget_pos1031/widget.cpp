#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
#include <QDebug>

#include<stdio.h>
#include<math.h>
#include"stdlib.h"

#define pi 3.1415926

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

    TextBrowser_2.setFixedHeight(300);
    TextBrowser_2.setFixedWidth(600);
    TextBrowser_2.show();

    //禁用radioButton
    ui->radioButton->setDisabled(true);
    ui->radioButton_2->setDisabled(true);


    //设置 半径 初始值
    ui->lineEdit_6->setText("10");

    for (int i = 0;i < 8;i++)
    {
        baseData[i] = 10.0;
    }

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
    if(x>=paintx && y>=painty && x<= (720+paintx) && y<= (640 +painty) && (m->buttons()&Qt::LeftButton))
    {
        endPoint.setX(x-paintx);
        endPoint.setY(y-painty);
        qDebug() << endPoint ;
    }

    update();
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
    if(x>=paintx && y>=painty && x<= (720+paintx) && y<= (640 +painty) && (event->button()==Qt::LeftButton))
    {
        lastPoint.setX(x-paintx);
        lastPoint.setY(y-painty);
        qDebug() << "左键按下" << lastPoint;
    }

    if(isDraw == 4 && lastPoint.x() < RatRoil.step && lastPoint.y() < RatRoil.cols \
                        && endPoint.x() < RatRoil.step && endPoint.y() < RatRoil.rows \
                        && abs(endPoint.x()-lastPoint.x()) >0 && abs(endPoint.y()-lastPoint.y())> 0 && (event->button()==Qt::LeftButton))    //旋转后的图片
    {
        int r;
        if(!ui->lineEdit_6->text().isEmpty()){
        r = ui->lineEdit_6->text().toInt();
        qDebug()<< "半径 " << r ;

        cv::Point center;
//        center.x = 100;
//        center.y = 50;

        center.x = lastPoint.x();
        center.y = lastPoint.y();

        //绘制圆形选区
        Mat result(RatRoil.rows, RatRoil.cols, CV_8UC1, cv::Scalar::all(0)); //黑图
        Mat resultcp;
        RatRoil.rowRange(0,RatRoil.rows).copyTo(result.rowRange(0,RatRoil.rows));  //区域大小要一样
        cvtColor(result,resultcp,CV_GRAY2RGB);   //转成三通道图像
        cv::circle(resultcp, center,r, cv::Scalar(0, 128, 0), -1);
        imshow("circle",resultcp);

        //计算平均值
        double temp = 0,arv = 0;
        int num = 0;
        circle(result, center, r, cv::Scalar(255, 255, 255), -1);
        imshow("result",result);

        Rect rect;
        rect.x = center.x-r-3;
        rect.y = center.y-r-3;
        qDebug() << rect.x << rect.y ;
        rect.width = 2*r+6;
        rect.height = rect.width;

        for (int i = 0; i < rect.width; i++)
        {
            for (int j = 0; j < rect.height; j++)
            {

                if (result.at<uchar>(i+rect.y,j+rect.x) == 255)
                {
                    temp += RatRoil.at<uchar>(i+rect.y,j+rect.x);
                    num++;
                }
            }
         }
        arv = temp/num;
        ui->lineEdit_7->setText(QString::number(arv, 10, 4));

//        uchar * data;
//        Rect rect(lastPoint.x(),lastPoint.y(), abs(endPoint.x()-lastPoint.x()), abs(endPoint.y()-lastPoint.y())); //左上角座标 ， 宽度， 高度。
//        data=(uchar *)malloc(rect.width*rect.height);
//        Roi_Mat(data,RatRoil,rect);
//        Mat Roil(rect.height, rect.width, CV_8UC1, (unsigned char*)data);

//        //imwrite("pic.bmp",Roil);
//        imshow("roil",Roil);
        }
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    if(x>=paintx && y>=painty && x<= (720+paintx) && y<= (640 +painty) && event->button()==Qt::LeftButton) //鼠标左键释放
    {   
        if(isDraw == 2)   //打开的图片
        {

        }

        if(isDraw == 4 && lastPoint.x() < RatRoil.step && lastPoint.y() < RatRoil.cols \
                            && endPoint.x() < RatRoil.step && endPoint.y() < RatRoil.rows \
                            && abs(endPoint.x()-lastPoint.x()) >0 && abs(endPoint.y()-lastPoint.y())> 0)    //旋转后的图片
        {
            uchar * data;
            Rect rect(lastPoint.x(),lastPoint.y(), abs(endPoint.x()-lastPoint.x()), abs(endPoint.y()-lastPoint.y())); //左上角座标 ， 宽度， 高度。
            data=(uchar *)malloc(rect.width*rect.height);

            Roi_Mat(data,RatRoil,rect);  //框选范围
            Mat Roil(rect.height, rect.width, CV_8UC1, (unsigned char*)data);

            //imwrite("pic.bmp",Roil);
            //imshow("roil",Roil); 显示矩形选区
        }


//        ui->li
//        ui->Listv->append( QString("topl is x = %1  y = %2").arg(topl.x).arg(topl.y));
//        ui->textBrowser->append( QString("topr is x = %1  y = %2").arg(topr.x).arg(topr.y));
        seriallastPoint = lastPoint; //保存鼠标上一个值
        serialendPoint = endPoint; //保存鼠标上一个值

        serialcenterPoint.setX( (endPoint.x()-lastPoint.x())/2+lastPoint.x() );
        serialcenterPoint.setY( (endPoint.y()-lastPoint.y())/2+lastPoint.y() );
        ui->lineEdit_9->setText(QString(" x = %1  y = %2").arg(lastPoint.x()).arg(lastPoint.y()));
        ui->lineEdit_10->setText(QString(" x = %1  y = %2").arg(endPoint.x()).arg(endPoint.y()));
        ui->lineEdit_8->setText(QString(" x = %1  y = %2").arg(serialcenterPoint.x()).arg(serialcenterPoint.y()));
        endPoint.setX(0);
        endPoint.setY(0);
        lastPoint.setX(0);
        lastPoint.setY(0);
        update();
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

//拖入图片------------------
void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData *qm=event->mimeData();//获取MIMEData
    filename = qm->urls()[0].toLocalFile().toStdString().data();
    pix.load(filename);  //加载图片
    paintEventPix = pix.copy(0,0,pix.width(),pix.height());  //考贝图片,至画板
    isDraw = 3;
    update();            //触发画图   

    img = cv::imread(qm->urls()[0].toLocalFile().toStdString().data(),1);  //读取图片 三通道灰度图
    Mat gray;
    cv::cvtColor(img, gray, CV_RGB2GRAY);
    Orimg = gray;//浅考贝
    ui->radioButton->setEnabled(true);
    ui->radioButton->setChecked(true);
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
           isDraw = 3;
           update();                            //触发画图

           img = cv::imread(fileName.toLatin1().data(),1);  //读取图片 三通道灰度图
           Mat gray;
           cv::cvtColor(img, gray, CV_RGB2GRAY);
           Orimg = gray;//浅考贝
           ui->radioButton->setEnabled(true);
           ui->radioButton->setChecked(true);
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
        QRect rect(paintx,painty,720+paintx,512+painty);
        painter.fillRect(rect,QColor(88,88,88)); // 填充背景色
        painter.end();
    }

   if(isDraw == 3 && !pix.isNull())  //初次加载的图
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

    if(isDraw == 2)          //二值化绘图
    {
       painter.begin(this);
       painter.drawPixmap(paintx,painty,paintEventPix.width(),paintEventPix.height(),paintEventPix); //绘制载入图片
       painter.end();
    }

    if(isDraw == 4 && !image->isNull())    //绘制较正后的图
    {
       tempPix = QPixmap::fromImage(*image);  //image转pixmap
       painter.begin(&tempPix);
       painter.setPen(Qt::green);
       painter.drawRect(lastPoint.x(),lastPoint.y(),endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());
       painter.end();
       paintEventPix = tempPix.copy(0,0,tempPix.width(),tempPix.height());  //考贝图片

       painter.begin(this);
       painter.drawPixmap(paintx,painty,paintEventPix.width(),paintEventPix.height(),paintEventPix); //绘制载入图片
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
        ui->radioButton->setEnabled(true);
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
  //      imshow("imgcp",imgcp);

        threshold(imgcp, thr, thres, maxval, CV_THRESH_BINARY);

        vector<vector<Point> > contours;
        CvPoint2D32f rectpoint[4];
        CvBox2D rect;
        float angle;
        int Max=0;
        double g_dConArea,temp = 0;
        double Max_X=0,Min_X=5000; //Min_X 初始值足够大

        findContours(thr, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

        if( contours.size() >=2)
            {
                //查找最大轮廓
                    for(vector<Point2f>::size_type i= 0;i < contours.size(); i++)
                    {
                        g_dConArea = fabs(contourArea(contours[i], true));
                        if(temp < g_dConArea)
                        {
                            temp = g_dConArea;
                            Max = i;
                        }
                    }
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
                     qDebug() << angle;
                    if(angle < -45)angle=angle+90;
            }
            else
            {
                    Max =0;
                    g_dConArea = fabs(contourArea(contours[Max], true));
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
                    qDebug() << angle;
                    if(angle < -45)angle=angle+90;
            }
          QString dataAngle = QString("%1").arg(angle);
          ui->lineEdit_3->setText(dataAngle);
          ui->textBrowser->append( QString("------------------------------"));
          ui->textBrowser->append( QString("countersiz  %1").arg(contours.size()));
          ui->textBrowser->append( QString("angle is %1").arg(angle));
          ui->textBrowser->append( QString("rect is %1").arg(g_dConArea));
          ui->textBrowser->append( QString("center is x = %1  y = %2").arg(rect.center.x).arg(rect.center.y));
          ui->textBrowser->append( QString("Point0 is x = %1  y = %2").arg(rectpoint[0].x).arg(rectpoint[0].y));
          ui->textBrowser->append( QString("Point1 is x = %1  y = %2").arg(rectpoint[1].x).arg(rectpoint[1].y));
          ui->textBrowser->append( QString("Point2 is x = %1  y = %2").arg(rectpoint[2].x).arg(rectpoint[2].y));
          ui->textBrowser->append( QString("Point3 is x = %1  y = %2").arg(rectpoint[3].x).arg(rectpoint[3].y));

          Mat RatationedImg(img.rows, img.cols, CV_8UC1);
          RatationedImg.setTo(0);
          Point2f center = rect.center;  //中心点
          Mat M2 = getRotationMatrix2D(center, angle, 1);//计算旋转加缩放的变换矩阵
          warpAffine(img, RatationedImg, M2, img.size(),1, 0, Scalar(0));//仿射变换

    //      imshow("Rat",RatationedImg); //显示旋转后图片

          // 旋转后的 轮廓
          Mat RatationedImgcp,graycp;
          cv::cvtColor(RatationedImg, graycp, CV_RGB2GRAY);
          threshold(graycp, RatationedImgcp, thres, maxval, CV_THRESH_BINARY);

          vector<vector<Point> > Ratcontours;
          findContours(RatationedImgcp, Ratcontours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

          temp = 0;
          if( Ratcontours.size() >=2)
              {
                  //查找最大轮廓
                      for(vector<Point2f>::size_type i= 0;i < Ratcontours.size(); i++)
                      {
                        /*  temp = fabs(contourArea(Ratcontours[i], true));
                          g_dConArea = fabs(contourArea(Ratcontours[i+1], true));
                          Max = temp > g_dConArea ? i: (i+1);
                          */
                          g_dConArea = fabs(contourArea(Ratcontours[i], true));
                          if(temp < g_dConArea)
                          {
                              temp = g_dConArea;
                              Max = i;
                          }
                      }
                      rect =minAreaRect(Mat(Ratcontours[Max]));  //最小矩形
                      cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                      //与水平线的角度
                      angle = rect.angle;
                      if(angle < -45)angle=angle+90;

              }
              else
              {
                      Max =0;
                      g_dConArea = fabs(contourArea(Ratcontours[Max], true));
                      rect =minAreaRect(Mat(Ratcontours[Max]));  //最小矩形
                      cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                      //与水平线的角度
                      angle = rect.angle;
                      if(angle < -45)angle=angle+90;
              }

          ui->textBrowser->append( QString("angle is %1").arg(angle));
          ui->textBrowser->append( QString("rect is %1").arg(g_dConArea));
          ui->textBrowser->append( QString("center is x = %1  y = %2").arg(rect.center.x).arg(rect.center.y));
          ui->textBrowser->append( QString("Point0 is x = %1  y = %2").arg(rectpoint[0].x).arg(rectpoint[0].y));
          ui->textBrowser->append( QString("Point1 is x = %1  y = %2").arg(rectpoint[1].x).arg(rectpoint[1].y));
          ui->textBrowser->append( QString("Point2 is x = %1  y = %2").arg(rectpoint[2].x).arg(rectpoint[2].y));
          ui->textBrowser->append( QString("Point3 is x = %1  y = %2").arg(rectpoint[3].x).arg(rectpoint[3].y));



          topl.x =800;
          topl.y =800;
          topr.x =0;
          topr.y =0;
          for (int i = 0; i < 4;i++)
          {
              topl.x = topl.x < rectpoint[i].x ? topl.x : rectpoint[i].x; // 小于不成立，就等于后面的数
          }

          for (int i = 0; i < 4;i++)
          {
              topl.y = topl.y < rectpoint[i].y ? topl.y : rectpoint[i].y;
          }
          for (int i = 0; i < 4;i++)
          {
              topr.x = topr.x > rectpoint[i].x ? topr.x : rectpoint[i].x;
          }

          for (int i = 0; i < 4;i++)
          {
              topr.y = topr.y > rectpoint[i].y ? topr.y : rectpoint[i].y;
          }
          QString Width = QString("%1").arg(topr.x-topl.x); //计算宽度
          ui->lineEdit_4->setText(Width);
          QString Height = QString("%1").arg(topr.y-topl.y);//计算高度
          ui->lineEdit_5->setText(Height);

          ui->textBrowser->append( QString("topl is x = %1  y = %2").arg(topl.x).arg(topl.y));
          ui->textBrowser->append( QString("topr is x = %1  y = %2").arg(topr.x).arg(topr.y));

     //     imshow("graycp",graycp);  //旋转后灰度图
          uchar * data;
          Rect rect5((int)topl.x,(int)topl.y, (int)topr.x-(int)topl.x, (int)(topr.y-topl.y)/4*4); //左上角座标 ， 宽度， 高度。
        // Rect rect5(0,0, 360, 149);
          data=(uchar *)malloc(rect5.width*rect5.height*3);

          Roi_Mat(data,graycp,rect5);
          Mat Roil(rect5.height, rect5.width, CV_8UC1, (unsigned char*)data);
          RatRoil = Roil; //浅考贝

          imshow("warpAffine",Roil);

       //QImage::Format_Indexed8 QImage::Format_RGB888 QImage::Format_RGB32
        (*image)=QImage((const unsigned char*)(Roil.data),Roil.cols,Roil.rows,Roil.step,QImage::Format_Indexed8);

//        (*image)=QImage((const unsigned char*)(Roil.data),Roil.cols,Roil.rows,QImage::Format_RGB888);
//        tempPix = QPixmap::fromImage(*image);  //image转pixmap
//        paintEventPix = tempPix.copy(0,0,tempPix.width(),tempPix.height());  //考贝图片
        isDraw = 4;
        update();            //触发画图

        ui->radioButton_2->setEnabled(true);
        ui->radioButton_2->setChecked(true);
     }
}


void Widget::Roi_Mat(uchar* data,Mat src, Rect rect)
{
    int step =rect.width ;
     for(int i=0;i<rect.height;i++)
     {
          for(int j=0;j<rect.width;j++){
              data[i*step+j]=src.at<uchar>(i+rect.y,j+rect.x);
          }
     }
}

void Widget::on_radioButton_clicked(bool checked)
{
    if(checked == true) isDraw = 3;
    update();
}

void Widget::on_radioButton_2_clicked(bool checked)
{
    if(checked == true) isDraw = 4;
    update();
}


void Widget::on_pushButton_4_clicked()
{
    if(!RatRoil.empty())
    {
        int feature;
        double arv[8];
        cv::Point center[8];
        int r;
        if(!ui->lineEdit_6->text().isEmpty())
        {
            r = ui->lineEdit_6->text().toInt();
        }

        //绘制圆形选区
        int width,height;
        width = 2*r+6;
        height = width;
        cv::Point centerTp;
        centerTp.x = width/2;
        centerTp.y = height/2;

        Mat backTp(height,width, CV_8UC1, cv::Scalar::all(0)); //画背景板黑色，比圆大4个像素
        circle(backTp, centerTp, r, cv::Scalar(255, 255, 255), -1);  //画白色实芯圆 -1
      //  imshow("backTp",backTp);

        //初始化采样圆心
        int Pwidth = topr.x-topl.x;
        int Pheight = topr.y-topl.y;


        for(int j = 0; j < 4; j++)
        {
            center[j].x = Pwidth/5*(j+1);  //五等分，取四个点
            center[j].y = Pheight/3; //高度三等分
            center[j+4].x = Pwidth/5*(j+1);  //五等分，取四个点
            center[j+4].y = Pheight/3*2; //高度三等分
        }

        //求样点平均值
        double temp[8];
        int num = 0;
        for (int i = 0;i<8;i++)
        {
            temp[i] = 0;
            arv[i] = 0;
        }

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {

                if (backTp.at<uchar>(i,j) == 255)
                {
                    for(int k = 0;k < 8;k++)
                    {
                       temp[k] += RatRoil.at<uchar>(i+(center[k].y-r-3),j+(center[k].x-r-3));
                    }
                    num++;
                }

            }
         }

//        arv[0] =  temp[0]/num;
//        ui->textBrowser->append( QString("num %1 ").arg(num));
//        ui->textBrowser->append( QString("centter[0] %1 is %2").arg(center[0].x).arg(center[0].y));
//        ui->textBrowser->append( QString("arg %1 is %2").arg(0).arg(arv[0]));
        double checkData[8];
        for (int i = 0;i<8;i++)
        {
            arv[i] =  temp[i]/num;
            checkData[i] = arv[i];
        }


          TextBrowser_2.append(QString("采样："));
          for (int i = 0;i<8;i++)
          {
          //  ui->textBrowser->append( QString("arg %1 is %2").arg(i).arg(arv[i]));
              TextBrowser_2.insertPlainText(QString("%1   ").arg(arv[i]));
          }
          TextBrowser_2.show();

        double rat = Correl(checkData,baseData,8);
        TextBrowser_2.append(QString("相关性 ： %1   ").arg(rat));

        for(int i =0;i < 8;i++)
        {
            baseData[i] = checkData[i];
        }

        //画采样点实心圆
        Mat RatRoilcp(RatRoil.rows,RatRoil.cols, CV_8UC1, cv::Scalar::all(0)); //画背景板黑色，比圆大4个像素
        RatRoil.rowRange(0,RatRoil.rows).copyTo(RatRoilcp.rowRange(0,RatRoil.rows));  //区域大小要一样
        for(int i = 0;i <8;i++)
        {
         circle(RatRoilcp, center[i], r, cv::Scalar(255, 255, 255), -1);  //画白色实芯圆 -1
        }
        imshow("RatRoilcp",RatRoilcp);
    }
}


double Widget::Correl(double *basedata,double *checkData,int N)
{
    double sumX = 0.0;
    double sumY = 0.0;
    double sumX_Sq = 0.0;
    double sumY_Sq = 0.0;
    double sumXY = 0.0;
    if (NULL != basedata && NULL != checkData){
        for (int i = 0; i < N; i++ ){
            if ( (NULL != basedata[i]) && (NULL != checkData[i])){
                sumX +=  basedata[i];
                sumY += checkData[i];
                sumX_Sq += pow(basedata[i],2); //求平方
                sumY_Sq += pow(checkData[i],2);
                sumXY += basedata[i]*checkData[i];
                }
            }
     }
     double numerator = sumXY - sumX * sumY / N;
     double denominator = sqrt((sumX_Sq - sumX * sumX / N) * (sumY_Sq - sumY * sumY / N));
     // 分母不能为0
     if (denominator == 0) {
                  return 0.0;
      }
     return numerator / denominator;
}

void Widget::on_pushButton_5_clicked()
{
    if(!Orimg.empty())  //原图gray浅考贝
    {
        int thres =ui->lineEdit->text().toInt();
        int maxval=ui->lineEdit_2->text().toInt();

        Mat thr;
        threshold(Orimg, thr, thres, maxval, CV_THRESH_BINARY);

        vector<vector<Point> > contours;
        CvPoint2D32f rectpoint[4];
        CvBox2D rect;
        float angle;
        int Max=0;
        double g_dConArea,temp = 0;
        double Max_X=0,Min_X=5000; //Min_X 初始值足够大

        findContours(thr, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

        if( contours.size() >=2)
            {
                //查找最大轮廓
                    for(vector<Point2f>::size_type i= 0;i < contours.size(); i++)
                    {
                        g_dConArea = fabs(contourArea(contours[i], true));
                        if(temp < g_dConArea)
                        {
                            temp = g_dConArea;
                            Max = i;
                        }
                    }
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
                     qDebug() << angle;
                    if(angle < -45)angle=angle+90;
            }
            else
            {
                    Max =0;
                    g_dConArea = fabs(contourArea(contours[Max], true));
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
                    qDebug() << angle;
                    if(angle < -45)angle=angle+90;

            }
        QString dataAngle = QString("%1").arg(angle);
        ui->lineEdit_3->setText(dataAngle);
        ui->textBrowser->append( QString("------------------------------"));
        ui->textBrowser->append( QString("countersiz  %1").arg(contours.size()));
        ui->textBrowser->append( QString("angle is %1").arg(angle));
        ui->textBrowser->append( QString("rect is %1").arg(g_dConArea));
        ui->textBrowser->append( QString("center is x = %1  y = %2").arg(rect.center.x).arg(rect.center.y));
        ui->textBrowser->append( QString("Point0 is x = %1  y = %2").arg(rectpoint[0].x).arg(rectpoint[0].y));
        ui->textBrowser->append( QString("Point1 is x = %1  y = %2").arg(rectpoint[1].x).arg(rectpoint[1].y));
        ui->textBrowser->append( QString("Point2 is x = %1  y = %2").arg(rectpoint[2].x).arg(rectpoint[2].y));
        ui->textBrowser->append( QString("Point3 is x = %1  y = %2").arg(rectpoint[3].x).arg(rectpoint[3].y));

        if(angle < 0)
        {
            topl = getRotatePoint(angle,rect.center,rectpoint[1].x ,rectpoint[1].y);
            topr = getRotatePoint(angle,rect.center,rectpoint[3].x ,rectpoint[3].y);
        }
        else if(angle > 0)
        {
            topl = getRotatePoint(angle,rect.center,rectpoint[2].x ,rectpoint[2].y);
            topr = getRotatePoint(angle,rect.center,rectpoint[0].x ,rectpoint[0].y);
        }
        else if(fabs(angle) == 0)
        {
            topl = getRotatePoint(angle,rect.center,rectpoint[2].x ,rectpoint[2].y);
            topr = getRotatePoint(angle,rect.center,rectpoint[0].x ,rectpoint[0].y);
        }
        ui->textBrowser->append( QString("topl is x = %1  y = %2").arg(topl.x).arg(topl.y));
        ui->textBrowser->append( QString("topr is x = %1  y = %2").arg(topr.x).arg(topr.y));

        cv::Point center[8];
        cv::Point centerp[8];
        int Oriwidth = topr.x-topl.x;
        int Oriheight =topr.y-topl.y;

        for(int j = 0; j < 4; j++)
        {
            centerp[j].x = topl.x+Oriwidth/5*(j+1);  //五等分，取四个点
            centerp[j].y = topl.y+Oriheight/3; //高度三等分
            centerp[j+4].x =topl.x+ Oriwidth/5*(j+1);  //五等分，取四个点
            centerp[j+4].y =topl.y+ Oriheight/3*2; //高度三等分

            center[j] = getRotatePoint(-angle,rect.center,centerp[j].x ,centerp[j].y);
            center[j+4] = getRotatePoint(-angle,rect.center,centerp[j+4].x ,centerp[j+4].y);
        }


        int r;
        if(!ui->lineEdit_6->text().isEmpty())
        {
            r = ui->lineEdit_6->text().toInt();
        }
        int width,height;
        width = 2*r+6;
        height = width;
        cv::Point centerTp;
        centerTp.x = width/2;
        centerTp.y = height/2;

        Mat backTp(height,width, CV_8UC1, cv::Scalar::all(0)); //画背景板黑色，比圆大4个像素
        circle(backTp, centerTp, r, cv::Scalar(255, 255, 255), -1);  //画白色实芯圆 -1

        //求样点平均值
        double tempc[8];
        double arv[8];
        int num = 0;
        for (int i = 0;i<8;i++)
        {
            tempc[i] = 0;
            arv[i] = 0;
        }

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {

                if (backTp.at<uchar>(i,j) == 255)
                {
                    for(int k = 0;k < 8;k++)
                    {
                       tempc[k] += Orimg.at<uchar>(i+(center[k].y-r-3),j+(center[k].x-r-3));
                    }
                    num++;
                }

            }
         }
        double checkData[8];
        for (int i = 0;i<8;i++)
        {
            arv[i] =  tempc[i]/num;
            checkData[i] = arv[i];
        }


          TextBrowser_2.append(QString("采样："));
          for (int i = 0;i<8;i++)
          {
          //  ui->textBrowser->append( QString("arg %1 is %2").arg(i).arg(arv[i]));
              TextBrowser_2.insertPlainText(QString("%1   ").arg(arv[i]));
          }
          TextBrowser_2.show();

        double rat = Correl(checkData,baseData,8);
        TextBrowser_2.append(QString("相关性 ： %1   ").arg(rat));

        for(int i =0;i < 8;i++)
        {
            baseData[i] = checkData[i];
        }


        //画采样点实芯圆
        Mat RatRoilcp(Orimg.rows,Orimg.cols, CV_8UC1, cv::Scalar::all(0)); //
        Orimg.rowRange(0,Orimg.rows).copyTo(RatRoilcp.rowRange(0,Orimg.rows));  //区域大小要一样
        for(int i = 0;i <8;i++)
        {
         circle(RatRoilcp, center[i], r, cv::Scalar(255, 255, 255), -1);  //画白色实芯圆 -1
        }
        imshow("Orimgcp",RatRoilcp);
    }
}

Point Widget::getRotatePoint(double angle,Point center,int x ,int y)  //计算旋转前的坐标
{
    Point top;
    if(fabs(angle) == 0)
    {
        top.x = x;
        top.y = y;
    }
    else
    {
        double x1,y1,x2,y2;
        x1 = x;
        y1 = 1024 - y;  // 1024 为预设值，大于图像高度就可以。
        x2 = center.x;
        y2 = 1024 - center.y;
        top.x = (x1 - x2)*cos(pi / 180.0 * angle) - (y1 - y2)*sin(pi / 180.0 * angle) + x2;
        top.y = (x1 - x2)*sin(pi / 180.0 * angle) + (y1 - y2)*cos(pi / 180.0 * angle) + y2;
        top.y = 1024 - top.y;

    }

    return top;
}


//定位序列号
void Widget::on_pushButton_6_clicked()
{
    if(!RatRoil.empty()){
    double angle = ui->lineEdit_3->text().toDouble();
    Rect rect;
    rect.x = seriallastPoint.x();
    rect.y = seriallastPoint.y();
    rect.width = serialendPoint.x()-seriallastPoint.x();
    rect.height = serialendPoint.y()-seriallastPoint.y();



    uchar *data;
    data =(uchar *)malloc(rect.width*rect.height);
    Roi_Mat(data,RatRoil, rect);
    Mat serialp(rect.height, rect.width, CV_8UC1, (unsigned char*)data);


    int thres =ui->lineEdit->text().toInt();
    int maxval=ui->lineEdit_2->text().toInt();
    Mat thr;
    threshold(serialp, thr, thres, maxval, CV_THRESH_BINARY_INV);
    imshow("serialp",thr);


   //细化

    Mat thrcp(thr.rows,thr.cols,CV_8UC1,thr.data);
//    zhang_thinimage_improve(thrcp);
    //cv::Scalar::all(0)
//    Mat thrcp(thr.rows,thr.cols,CV_8UC1,cv::Scalar::all(0));
//    imageThin(thr.data, thrcp.data,thr.cols,thr.rows );
//    Mat element = getStructuringElement(MORPH_RECT,Size(2,2));
//    dilate(thrcp,thrcp,element);
    zhang_thinimage_improve(thrcp);
    imshow("dst",thrcp);



    vector<vector<Point> > contours;
//    CvPoint2D32f rectpoint[4];
//    CvBox2D rect;
//    float angle;
//    int Max=0;
//    double g_dConArea,temp = 0;
//    double Max_X=0,Min_X=5000; //Min_X 初始值足够大

    findContours(thr, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓
    qDebug() << contours.size() ;
    double area;
    for (int i =0; i<contours.size();i++)
    {
       CvBox2D rectp;
       rectp = minAreaRect(Mat(contours[i]));
       area = contourArea(contours[i]);
       qDebug()<< rectp.center.x << rectp.angle ;
    }

    imshow("serialp2",thr);

    }
}


cv::Mat Widget::thinImage(const cv::Mat & src, const int maxIterations)
{
    assert(src.type() == CV_8UC1);
    cv::Mat dst;
    int width  = src.cols;
    int height = src.rows;
    src.copyTo(dst);
    int count = 0;  //记录迭代次数
    while (true)
    {
        count++;
        if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达
            break;
        std::vector<uchar *> mFlag; //用于标记需要删除的点
        //对点标记
        for (int i = 0; i < height ;++i)
        {
            uchar * p = dst.ptr<uchar>(i);
            for (int j = 0; j < width; ++j)
            {
                //如果满足四个条件，进行标记
                //  p9 p2 p3
                //  p8 p1 p4
                //  p7 p6 p5
                uchar p1 = p[j];
                if (p1 != 1) continue;
                uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
                uchar p8 = (j == 0) ? 0 : *(p + j - 1);
                uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
                uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
                uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
                uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
                uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
                uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
                if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
                {
                    int ap = 0;
                    if (p2 == 0 && p3 == 1) ++ap;
                    if (p3 == 0 && p4 == 1) ++ap;
                    if (p4 == 0 && p5 == 1) ++ap;
                    if (p5 == 0 && p6 == 1) ++ap;
                    if (p6 == 0 && p7 == 1) ++ap;
                    if (p7 == 0 && p8 == 1) ++ap;
                    if (p8 == 0 && p9 == 1) ++ap;
                    if (p9 == 0 && p2 == 1) ++ap;

                    if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
                    {
                        //标记
                        mFlag.push_back(p+j);
                    }
                }
            }
        }

        //将标记的点删除
        for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
        {
            **i = 0;
        }

        //直到没有点满足，算法结束
        if (mFlag.empty())
        {
            break;
        }
        else
        {
            mFlag.clear();//将mFlag清空
        }

        //对点标记
        for (int i = 0; i < height; ++i)
        {
            uchar * p = dst.ptr<uchar>(i);
            for (int j = 0; j < width; ++j)
            {
                //如果满足四个条件，进行标记
                //  p9 p2 p3
                //  p8 p1 p4
                //  p7 p6 p5
                uchar p1 = p[j];
                if (p1 != 1) continue;
                uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
                uchar p8 = (j == 0) ? 0 : *(p + j - 1);
                uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
                uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
                uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
                uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
                uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
                uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);

                if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
                {
                    int ap = 0;
                    if (p2 == 0 && p3 == 1) ++ap;
                    if (p3 == 0 && p4 == 1) ++ap;
                    if (p4 == 0 && p5 == 1) ++ap;
                    if (p5 == 0 && p6 == 1) ++ap;
                    if (p6 == 0 && p7 == 1) ++ap;
                    if (p7 == 0 && p8 == 1) ++ap;
                    if (p8 == 0 && p9 == 1) ++ap;
                    if (p9 == 0 && p2 == 1) ++ap;

                    if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0)
                    {
                        //标记
                        mFlag.push_back(p+j);
                    }
                }
            }
        }

        //将标记的点删除
        for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
        {
            **i = 0;
        }

        //直到没有点满足，算法结束
        if (mFlag.empty())
        {
            break;
        }
        else
        {
            mFlag.clear();//将mFlag清空
        }
    }
    return dst;
}


void Widget::zhang_thinimage_improve(Mat &srcimage)//单通道、二值化后的图像
{
    vector<Point> deletelist1;
    int Zhangmude[9];
    int nl = srcimage.rows;
    int nc = srcimage.cols;
    while (true)
    {
        for (int j = 1; j<(nl - 1); j++)
        {
            uchar* data_last = srcimage.ptr<uchar>(j - 1);
            uchar* data = srcimage.ptr<uchar>(j);
            uchar* data_next = srcimage.ptr<uchar>(j + 1);
            for (int i = 1; i<(nc - 1); i++)
            {
                if (data[i] == 255)
                {
                    Zhangmude[0] = 1;
                    if (data_last[i] == 255) Zhangmude[1] = 1;
                    else  Zhangmude[1] = 0;
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1;
                    else  Zhangmude[2] = 0;
                    if (data[i + 1] == 255) Zhangmude[3] = 1;
                    else  Zhangmude[3] = 0;
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;
                    else  Zhangmude[4] = 0;
                    if (data_next[i] == 255) Zhangmude[5] = 1;
                    else  Zhangmude[5] = 0;
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1;
                    else  Zhangmude[6] = 0;
                    if (data[i - 1] == 255) Zhangmude[7] = 1;
                    else  Zhangmude[7] = 0;
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;
                    else  Zhangmude[8] = 0;
                    int whitepointtotal = 0;
                    for (int k = 1; k < 9; k++)
                    {
                        //得到1的个数
                        whitepointtotal = whitepointtotal + Zhangmude[k];
                    }
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
                    {
                        //得到01的个数
                        int ap = 0;
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
            //计算bp
            int bp=0;
            bp+=Zhangmude[1];
            bp+=Zhangmude[2]<<1;
            bp+=Zhangmude[3]<<2;
            bp+=Zhangmude[4]<<3;
            bp+=Zhangmude[5]<<4;
            bp+=Zhangmude[6]<<5;
            bp+=Zhangmude[7]<<6;
            bp+=Zhangmude[8]<<7;

                        if (ap == 1||bp==65||bp==5||bp==20||bp==80||bp==13||bp==22||bp==52||bp==133||bp==141||bp==54)
                        {
                            if ((Zhangmude[1] * Zhangmude[7] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0))
                            {
                                deletelist1.push_back(Point(i, j));  //满足条件，去除该点
                            }
                        }
                    }
                }
            }
        }
        if (deletelist1.size() == 0) break;
        for (size_t i = 0; i < deletelist1.size(); i++)
        {
            Point tem;
            tem = deletelist1[i];
            uchar* data = srcimage.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deletelist1.clear();


        for (int j = 1; j<(nl - 1); j++)
        {
            uchar* data_last = srcimage.ptr<uchar>(j - 1);
            uchar* data = srcimage.ptr<uchar>(j);
            uchar* data_next = srcimage.ptr<uchar>(j + 1);
            for (int i = 1; i<(nc - 1); i++)
            {
                if (data[i] == 255)
                {
                    Zhangmude[0] = 1;
                    if (data_last[i] == 255) Zhangmude[1] = 1;
                    else  Zhangmude[1] = 0;
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1;
                    else  Zhangmude[2] = 0;
                    if (data[i + 1] == 255) Zhangmude[3] = 1;
                    else  Zhangmude[3] = 0;
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;
                    else  Zhangmude[4] = 0;
                    if (data_next[i] == 255) Zhangmude[5] = 1;
                    else  Zhangmude[5] = 0;
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1;
                    else  Zhangmude[6] = 0;
                    if (data[i - 1] == 255) Zhangmude[7] = 1;
                    else  Zhangmude[7] = 0;
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;
                    else  Zhangmude[8] = 0;
                    int whitepointtotal = 0;
                    for (int k = 1; k < 9; k++)
                    {
                        whitepointtotal = whitepointtotal + Zhangmude[k];
                    }
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
                    {
                        int ap = 0;
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
            int bp=0;
            bp+=Zhangmude[1];
            bp+=Zhangmude[2]<<1;
            bp+=Zhangmude[3]<<2;
            bp+=Zhangmude[4]<<3;
            bp+=Zhangmude[5]<<4;
            bp+=Zhangmude[6]<<5;
            bp+=Zhangmude[7]<<6;
            bp+=Zhangmude[8]<<7;
                        if (ap == 1||bp==65||bp==5||bp==20||bp==80||bp==13||bp==22||bp==52||bp==133||bp==141||bp==54)
                        {
                            if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[1] * Zhangmude[7] == 0))
                            {
                                deletelist1.push_back(Point(i, j));
                            }
                        }
                    }
                }
            }
        }
        if (deletelist1.size() == 0) break;
        for (size_t i = 0; i < deletelist1.size(); i++)
        {
            Point tem;
            tem = deletelist1[i];
            uchar* data = srcimage.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deletelist1.clear();
    }
}


/*
***************指纹图像细化：将指纹部分细化成一条条细线，即一条细线由一个个数据点连接起来***************
lpBits输入数据指针，g_lpTemp输入数据指针
*/
void Widget::imageThin(unsigned char *lpBits, unsigned char *g_lpTemp,int ImageWidth,int ImageHeight )
{
    int Width = ImageWidth;
    int Height = ImageHeight;
    /////////////////////////////////////////////////////////////////
    //	lpBits: [in, out] 要细化的图像数据缓冲区
    //	Width: [in] 要细化的图像宽度
    //	Height: [in] 要细化的图像高度
    /////////////////////////////////////////////////////////////////
    unsigned char  erasetable[256] = {
        0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
        0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
        1,1,0,0,1,1,0,0,             0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,             0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,0,0,             1,1,0,1,1,1,0,1,
        0,0,0,0,0,0,0,0,             0,0,0,0,0,0,0,0,
        0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
        0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,0,0,             0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,0,
        1,1,0,0,1,1,0,0,             1,1,0,1,1,1,0,0,
        1,1,0,0,1,1,1,0,             1,1,0,0,1,0,0,0
    };
    int		x, y;
    int      num;
    bool        Finished;
    unsigned char       nw, n, ne, w, e, sw, s, se;
    unsigned char       *lpPtr = NULL;
    unsigned char       *lpTempPtr = NULL;

    memcpy((void *)g_lpTemp, (void *)lpBits, Width*Height);

    //结束标志置成假
    Finished = FALSE;
    while (!Finished) { //还没有结束
                        //结束标志置成假
        Finished = TRUE;
        //先进行水平方向的细化
        for (y = 1; y<Height - 1; y++)
        { //注意为防止越界，y的范围从1到高度-2
          //lpPtr指向原图数据，lpTempPtr指向新图数据

            lpPtr = (unsigned char *)lpBits + y*Width;
            lpTempPtr = (unsigned char *)g_lpTemp + y*Width;

            x = 1; //注意为防止越界，x的范围从1到宽度-2

            while (x<Width - 1)
            {
                if (*(lpPtr + x) == 0)
                { //是黑点才做处理
                    w = *(lpPtr + x - 1);  //左邻点
                    e = *(lpPtr + x + 1);  //右邻点

                    if ((w == 255) || (e == 255))
                    {

                        //如果左右两个邻居中至少有一个是白点才处理

                        nw = *(lpPtr + x + Width - 1); //左上邻点

                        n = *(lpPtr + x + Width); //上邻点

                        ne = *(lpPtr + x + Width + 1); //右上邻点

                        sw = *(lpPtr + x - Width - 1); //左下邻点

                        s = *(lpPtr + x - Width); //下邻点

                        se = *(lpPtr + x - Width + 1); //右下邻点

                                                       //计算索引

                        num = nw / 255 + n / 255 * 2 + ne / 255 * 4 + w / 255 * 8 + e / 255 * 16 +
                            sw / 255 * 32 + s / 255 * 64 + se / 255 * 128;

                        if (erasetable[num] == 1)
                        { //经查表，可以删除

                                                    //在原图缓冲区中将该黑点删除

                            *(lpPtr + x) = 255;

                            //结果图中该黑点也删除

                            *(lpTempPtr + x) = 255;

                            Finished = FALSE; //有改动，结束标志置成假

                            x++; //水平方向跳过一个象素

                        }

                    }

                }

                x++; //扫描下一个象素

            }

        }

        //再进行垂直方向的细化

        for (x = 1; x<Width - 1; x++) { //注意为防止越界，x的范围从1到宽度-2

            y = 1; //注意为防止越界，y的范围从1到高度-2

            while (y<Height - 1) {

                lpPtr = lpBits + y*Width;

                lpTempPtr = g_lpTemp + y*Width;

                if (*(lpPtr + x) == 0)
                { //是黑点才做处理

                    n = *(lpPtr + x + Width);

                    s = *(lpPtr + x - Width);

                    if ((n == 255) || (s == 255))
                    {

                        //如果上下两个邻居中至少有一个是白点才处理

                        nw = *(lpPtr + x + Width - 1);

                        ne = *(lpPtr + x + Width + 1);

                        w = *(lpPtr + x - 1);

                        e = *(lpPtr + x + 1);

                        sw = *(lpPtr + x - Width - 1);

                        se = *(lpPtr + x - Width + 1);

                        //计算索引

                        num = nw / 255 + n / 255 * 2 + ne / 255 * 4 + w / 255 * 8 + e / 255 * 16 +

                            sw / 255 * 32 + s / 255 * 64 + se / 255 * 128;

                        if (erasetable[num] == 1)
                        { //经查表，可以删除

                                                    //在原图缓冲区中将该黑点删除

                            *(lpPtr + x) = 255;

                            //结果图中该黑点也删除

                            *(lpTempPtr + x) = 255;

                            Finished = FALSE; //有改动，结束标志置成假

                            y++;//垂直方向跳过一个象素

                        }

                    }

                }

                y++; //扫描下一个象素

            }

        }

    }

    memcpy((void *)lpBits, (void *)g_lpTemp, Width*Height);

    return;
}
//缩1/4计算轮廓角度
void Widget::on_pushButton_8_clicked()
{
    if(img.empty() == false) // 不是空
    {
        int thres =ui->lineEdit->text().toInt(); //取变量
        int maxval=ui->lineEdit_2->text().toInt();


        Mat imgcp;
        Mat imgcpt;
        cv::cvtColor(img, imgcpt, CV_RGB2GRAY);
        cv::resize(imgcpt, imgcp,Size(imgcpt.cols/4,imgcpt.rows/4),0,0,INTER_LINEAR);

        imshow("imgcp",imgcp);

        threshold(imgcp, thr, thres, maxval, CV_THRESH_BINARY);

        vector<vector<Point> > contours;
        CvPoint2D32f rectpoint[4];
        CvBox2D rect;
        float angle;
        int Max=0;
        double g_dConArea,temp = 0;
        double Max_X=0,Min_X=5000; //Min_X 初始值足够大

        findContours(thr, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

        if( contours.size() >=2)
            {
                //查找最大轮廓
                    for(vector<Point2f>::size_type i= 0;i < contours.size(); i++)
                    {
                        g_dConArea = fabs(contourArea(contours[i], true));
                        if(temp < g_dConArea)
                        {
                            temp = g_dConArea;
                            Max = i;
                        }
                    }
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
                     qDebug() << angle;
                    if(angle < -45)angle=angle+90;
            }
            else
            {
                    Max =0;
                    g_dConArea = fabs(contourArea(contours[Max], true));
                    rect =minAreaRect(Mat(contours[Max]));  //最小矩形
                    cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                    //与水平线的角度
                    angle = rect.angle;
                    qDebug() << angle;
                    if(angle < -45)angle=angle+90;
            }
          QString dataAngle = QString("%1").arg(angle);
          ui->lineEdit_3->setText(dataAngle);
          ui->textBrowser->append( QString("------------------------------"));
          ui->textBrowser->append( QString("countersiz  %1").arg(contours.size()));
          ui->textBrowser->append( QString("angle is %1").arg(angle));
          ui->textBrowser->append( QString("rect is %1").arg(g_dConArea));
          ui->textBrowser->append( QString("center is x = %1  y = %2").arg(rect.center.x).arg(rect.center.y));
          ui->textBrowser->append( QString("Point0 is x = %1  y = %2").arg(rectpoint[0].x).arg(rectpoint[0].y));
          ui->textBrowser->append( QString("Point1 is x = %1  y = %2").arg(rectpoint[1].x).arg(rectpoint[1].y));
          ui->textBrowser->append( QString("Point2 is x = %1  y = %2").arg(rectpoint[2].x).arg(rectpoint[2].y));
          ui->textBrowser->append( QString("Point3 is x = %1  y = %2").arg(rectpoint[3].x).arg(rectpoint[3].y));

          Mat RatationedImg(img.rows, img.cols, CV_8UC1);
          RatationedImg.setTo(0);
          Point2f center;
          center.x= rect.center.x*4;  //中心点
          center.y= rect.center.y*4;

          Mat M2 = getRotationMatrix2D(center, angle, 1);//计算旋转加缩放的变换矩阵
          warpAffine(img, RatationedImg, M2, img.size(),1, 0, Scalar(0));//仿射变换

       //   imshow("Rat",RatationedImg); //显示旋转后图片

          // 旋转后的 轮廓
          Mat RatationedImgcp,graycp;
          cv::cvtColor(RatationedImg, graycp, CV_RGB2GRAY);
          threshold(graycp, RatationedImgcp, thres, maxval, CV_THRESH_BINARY);

          vector<vector<Point> > Ratcontours;
          findContours(RatationedImgcp, Ratcontours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓

          temp = 0;
          if( Ratcontours.size() >=2)
              {
                  //查找最大轮廓
                      for(vector<Point2f>::size_type i= 0;i < Ratcontours.size(); i++)
                      {
                        /*  temp = fabs(contourArea(Ratcontours[i], true));
                          g_dConArea = fabs(contourArea(Ratcontours[i+1], true));
                          Max = temp > g_dConArea ? i: (i+1);
                          */
                          g_dConArea = fabs(contourArea(Ratcontours[i], true));
                          if(temp < g_dConArea)
                          {
                              temp = g_dConArea;
                              Max = i;
                          }
                      }
                      rect =minAreaRect(Mat(Ratcontours[Max]));  //最小矩形
                      cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                      //与水平线的角度
                      angle = rect.angle;
                      if(angle < -45)angle=angle+90;

              }
              else
              {
                      Max =0;
                      g_dConArea = fabs(contourArea(Ratcontours[Max], true));
                      rect =minAreaRect(Mat(Ratcontours[Max]));  //最小矩形
                      cvBoxPoints(rect, rectpoint); //获取4个顶点坐标
                      //与水平线的角度
                      angle = rect.angle;
                      if(angle < -45)angle=angle+90;
              }

          ui->textBrowser->append( QString("angle is %1").arg(angle));
          ui->textBrowser->append( QString("rect is %1").arg(g_dConArea));
          ui->textBrowser->append( QString("center is x = %1  y = %2").arg(rect.center.x).arg(rect.center.y));
          ui->textBrowser->append( QString("Point0 is x = %1  y = %2").arg(rectpoint[0].x).arg(rectpoint[0].y));
          ui->textBrowser->append( QString("Point1 is x = %1  y = %2").arg(rectpoint[1].x).arg(rectpoint[1].y));
          ui->textBrowser->append( QString("Point2 is x = %1  y = %2").arg(rectpoint[2].x).arg(rectpoint[2].y));
          ui->textBrowser->append( QString("Point3 is x = %1  y = %2").arg(rectpoint[3].x).arg(rectpoint[3].y));



          topl.x =800;
          topl.y =800;
          topr.x =0;
          topr.y =0;
          for (int i = 0; i < 4;i++)
          {
              topl.x = topl.x < rectpoint[i].x ? topl.x : rectpoint[i].x;
          }

          for (int i = 0; i < 4;i++)
          {
              topl.y = topl.y < rectpoint[i].y ? topl.y : rectpoint[i].y;
          }
          for (int i = 0; i < 4;i++)
          {
              topr.x = topr.x > rectpoint[i].x ? topr.x : rectpoint[i].x;
          }

          for (int i = 0; i < 4;i++)
          {
              topr.y = topr.y > rectpoint[i].y ? topr.y : rectpoint[i].y;
          }
          QString Width = QString("%1").arg(topr.x-topl.x); //计算宽度
          ui->lineEdit_4->setText(Width);
          QString Height = QString("%1").arg(topr.y-topl.y);//计算高度
          ui->lineEdit_5->setText(Height);

          ui->textBrowser->append( QString("topl is x = %1  y = %2").arg(topl.x).arg(topl.y));
          ui->textBrowser->append( QString("topr is x = %1  y = %2").arg(topr.x).arg(topr.y));

//     //     imshow("graycp",graycp);  //旋转后灰度图
//          uchar * data;
//          Rect rect5((int)topl.x,(int)topl.y, (int)topr.x-(int)topl.x, (int)(topr.y-topl.y)/4*4); //左上角座标 ， 宽度， 高度。
//        // Rect rect5(0,0, 360, 149);
//          data=(uchar *)malloc(rect5.width*rect5.height*3);

//          Roi_Mat(data,graycp,rect5);
//          Mat Roil(rect5.height, rect5.width, CV_8UC1, (unsigned char*)data);
//          RatRoil = Roil; //浅考贝

//          imshow("warpAffine",Roil);

//       //QImage::Format_Indexed8 QImage::Format_RGB888 QImage::Format_RGB32
//        (*image)=QImage((const unsigned char*)(Roil.data),Roil.cols,Roil.rows,Roil.step,QImage::Format_Indexed8);

////        (*image)=QImage((const unsigned char*)(Roil.data),Roil.cols,Roil.rows,QImage::Format_RGB888);
////        tempPix = QPixmap::fromImage(*image);  //image转pixmap
////        paintEventPix = tempPix.copy(0,0,tempPix.width(),tempPix.height());  //考贝图片
//        isDraw = 4;
//        update();            //触发画图

//        ui->radioButton_2->setEnabled(true);
//        ui->radioButton_2->setChecked(true);
     }
}
