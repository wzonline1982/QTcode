#include "widget.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>

using namespace std;
using namespace cv;


Widget::Widget(QWidget *parent) //widget 继承 基类 widget
    : QWidget(parent)
{


    b1.setParent(this);   //指定父对象
    b1.setText("close");  //给按钮设置内容
    b1.move(100,100);

    b2 = new QPushButton(this);
    b2->setText("abc");  //给按钮设置内容

    connect(&b1,&QPushButton::pressed,this,&Widget::close);
    /*&b1: 信号发出者，指针类型
     *&QPushButton::pressed： 处理的信号， &发送者的类名：：信号名
     *this : 信号接收者
     * &&widget::close: 槽函数，信号处理函灵敏 &接收的类名：：槽函数名
     */
    connect(b2,&QPushButton::released,this,&Widget::mySlot);
  //  connect(b2,&QPushButton::released,&b1,&QPushButton::hide);

    b3.setParent(this);   //指定父对象
    b3.setText("切换到子窗口");  //给按钮设置内容
    b3.move(50,50);

    connect(&b3,&QPushButton::released,this,&Widget::changeWin);
  //  w.show();
    //处理子窗口的信号
    void(SubWidget::*funSignal)() = &SubWidget::mySignal;  //重载函数转化，函数指针
    connect(&w,funSignal,this,&Widget::dealSub);

    void(SubWidget::*testSignal)(int,QString) = &SubWidget::mySignal;  //重载函数转化，函数指针
    connect(&w,testSignal,this,&Widget::dealSlot);  //传参给dealSlot

    //QT4信号连接
    //QT4槽函数必须有slots 关键字来修饰
    //connect(&w,SIGNAL(mySignal()),this,SLOT(dealSub()));  //传参给dealSlot

    //Lambda表达式 匿名函数对象
    //c++11增加的新特性，项目文件：CONFIG +=C++11
    QPushButton *b4 = new QPushButton(this);
     b4->setText("Lambda表达式");
     b4->move(150,150);
     int a = 10;
     connect(b4,&QPushButton::released,
             [=]() mutable  // = : 把外部所有局部变量，类中所有成员以值传递方式 mutable 可改变值
                            //& :把外部所有局部变量，引用符号
             {
                b4->setText("123");
                qDebug() << "11111";
                qDebug() << a;   //传入为只读
             }
             );

    resize(400,300);


//    Mat img=imread("C:/Users/admin/Desktop/1.BMP"); //读入一张图片
//    cvNamedWindow("秦惠文王"); //创建一个名为"秦惠文王"的显示窗口
//    imshow("pic", img);   //在窗口中显示图像
//    waitKey(10000); //延时10000ms后关闭窗口

}

Widget::~Widget()
{

}

void Widget::mySlot()
{
    b2->setText("123");
}

void Widget::changeWin()
{
   //子窗口显示
    //本窗口隐藏
    w.show();
    this->hide();

}

void Widget::dealSub()
{
    w.hide();
    show();
}

void Widget::dealSlot(int a, QString str)
{

    qDebug() << a << str.toUtf8().data();
}
