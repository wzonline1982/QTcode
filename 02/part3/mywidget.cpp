#include "mywidget.h"
#include "ui_mywidget.h"

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

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    setFixedSize(800,480);  //设置窗口固定大小.

    this->image = new QImage();  //分配内存
    /*
    QImage image(400,300,QImage::Format_ARGB32);

    QPalette pal(ui->subwidget->palette());   // 画笔工具
    pal.setColor(QPalette::Background, Qt::gray); //设置背景黑色
    ui->subwidget->setAutoFillBackground(true);
    ui->subwidget->setPalette(pal);
*/
    void(Subwin::*testSignal)(QString) = &Subwin::subSignal;  //重载函数转化，函数指针
    connect(&w,testSignal,this,&MyWidget::dealSlot);  //传参给dealSlot

    w.show();
    binw.show();
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{

    fileName = QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.png *.bmp *.jpg)"));
        if(fileName.isEmpty())
            return;
        else
        {
           /*打开图片，显示到子窗口*/
           w.pix.load(fileName.toLatin1().data());//加载图片
           w.paintEventPix = w.pix.copy(0,0,w.pix.width(),w.pix.height());  //考贝图片
           w.update();                            //触发画图
        }

}

void MyWidget::on_pushButton_2_clicked()
{
    if(fileName.isEmpty())
        return;
    else
    {
       /*打开图片，二值化*/
        img = cv::imread(fileName.toLatin1().data());  //读取图片

        int thres =ui->lineEdit->text().toInt();
        int maxval=ui->lineEdit_2->text().toInt();

        qDebug() << thres <<maxval ;
        ui->textBrowser->append("二值化");

        threshold(img, thr, thres, maxval, CV_THRESH_BINARY);

        (*image)=QImage((const unsigned char*)(thr.data),thr.cols,thr.rows,QImage::Format_RGB888);

    }
}

void MyWidget::dealSlot(QString str)
{

    fileName = str;
    qDebug() << fileName ;
}
