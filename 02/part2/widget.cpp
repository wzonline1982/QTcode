#include "widget.h"
#include "ui_widget.h"
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <QDebug>

#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>


#include "cv.h"
#include "cxcore.h"
#include "highgui.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace cv;
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->lineEdit->setText("30");
    ui->lineEdit_2->setText("255");

    ui->textBrowser->setText("初始化");
//    ui->textBrowser->append("信息输出2");
//    ui->textBrowser->insertPlainText("信息输出3");

    scene = new QGraphicsScene;

    this->setAcceptDrops(true);//设置窗口启用拖动
  //  ui->graphicsView->installEventFilter(this);
   // ui->graphicsView->setAcceptDrops(true);  //graphicsviiew使能拖入


    this->image = new QImage();  //分配内存


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.png *.bmp *.jpg)"));
        if(fileName.isEmpty())
            return;
        else
        {

            if(image->load(fileName))
            {
              img = cv::imread(fileName.toLatin1().data());  //读取图片
              (*image)=QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);//转换格式
              scene->QGraphicsScene::clear();
              scene->setSceneRect(0,0, 480, 400);
              scene->addPixmap(QPixmap::fromImage(*image));
              ui->graphicsView->setScene(scene);
              ui->graphicsView->resize(480,400);
              ui->graphicsView->show();
             }
        }

  //  Mat img=imread("C:/Users/admin/Desktop/1.BMP"); //读入一张图片
  //  imshow("pic", img);   //在窗口中显示图像
}

void Widget::dragEnterEvent(QDragEnterEvent *event){
//如果类型是jpg或者png,bmp才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
   if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("png")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("BMP"))

       event->acceptProposedAction();

    else
       event->ignore();//否则不接受鼠标事件
   qDebug() << "111" ;

}


void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData *qm=event->mimeData();//获取MIMEData

    if(image->load(qm->urls()[0].toLocalFile()))
    {
       img = cv::imread(qm->urls()[0].toLocalFile().toStdString());  //读取图片
       (*image)=QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);
       scene->QGraphicsScene::clear();
       scene->setSceneRect(0,0, 480, 400);
       scene->addPixmap(QPixmap::fromImage(*image));
       ui->graphicsView->setScene(scene);
//       ui->graphicsView->resize(image->width() + 10, image->height() + 10);
       ui->graphicsView->resize(480, 400);
       ui->graphicsView->show();
     }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
  //  if(event->button()==Qt::LeftButton)//鼠标左击
  //     {
            qDebug()<<"左"<<event->pos();

  //     }

    /*const QMimeData *qm=event->mimeData();//获取MIMEData

    if(image->load(qm->urls()[0].toLocalFile()))
    {
       scene->addPixmap(QPixmap::fromImage(*image));
       ui->graphicsView->setScene(scene);
       ui->graphicsView->resize(image->width() + 10, image->height() + 10);
       ui->graphicsView->show();
     }
     */
}


void Widget::on_pushButton_2_clicked()   //二值化
{
    if(img.empty() == false) // 不是空
    {
    int thres =ui->lineEdit->text().toInt();
    int maxval=ui->lineEdit_2->text().toInt();

    qDebug() << thres <<maxval ;
    ui->textBrowser->append("二值化");

    threshold(img, thr, thres, maxval, CV_THRESH_BINARY);
    (*image)=QImage((const unsigned char*)(thr.data),thr.cols,thr.rows,QImage::Format_RGB888);
    scene->QGraphicsScene::clear();
    scene->setSceneRect(0,0, 480, 400);
    scene->addPixmap(QPixmap::fromImage(*image));
    ui->graphicsView->setScene(scene);
//       ui->graphicsView->resize(image->width() + 10, image->height() + 10);
    ui->graphicsView->resize(480, 400);
    ui->graphicsView->show();
    }
}

cv::Mat Widget::QImage2cvMat(QImage image)
{
    cv::Mat mat;
        qDebug() << image.format();
        switch(image.format())
        {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, CV_BGR2RGB);
            break;
        case QImage::Format_Indexed8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
        }
        return mat;
}

void Widget::on_pushButton_3_clicked()
{
    if(thr.empty() == false) // 不是空
    {
 /*   int thres =ui->lineEdit->text().toInt();
    int maxval=ui->lineEdit_2->text().toInt();

    qDebug() << thres <<maxval ;
    ui->textBrowser->append("二值化");

    threshold(img, thr, thres, maxval, CV_THRESH_BINARY);
    (*image)=QImage((const unsigned char*)(thr.data),thr.cols,thr.rows,QImage::Format_RGB888);
    scene->QGraphicsScene::clear();
    scene->setSceneRect(0,0, 480, 400);
    scene->addPixmap(QPixmap::fromImage(*image));
    ui->graphicsView->setScene(scene);
//       ui->graphicsView->resize(image->width() + 10, image->height() + 10);
    ui->graphicsView->resize(480, 400);
    ui->graphicsView->show();
    */
    }
}

void graphicsView()
{

}
