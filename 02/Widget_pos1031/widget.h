#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QPushButton>
#include <QUrl>

#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>

#include <QListView>
#include <QTextBrowser>
#include <QDialog>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QScreen>
#include <QGuiApplication>

#include "cv.h"
#include "cxcore.h"
#include "highgui.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void Roi_Mat(uchar* data,Mat src, Rect rect);
    double Correl(double *basedata,double *checkData,int N);
    Point getRotatePoint(double angle,Point center,int x ,int y);
    Mat thinImage(const cv::Mat & src, const int maxIterations = -1);
    void zhang_thinimage_improve(Mat &srcimage);
    void imageThin(unsigned char *lpBits, unsigned char *g_lpTemp,int ImageWidth,int ImageHeight );


private:
    Ui::Widget *ui;

    double baseData[8];
    int paintx=20;
    int painty=50;
    QPixmap pix; //全局变量保存绘制的图片
    QPixmap tempPix; //辅助画布
    QPixmap paintEventPix; //辅助画布

    QPoint lastPoint; //保存鼠标上一个值
    QPoint endPoint; //保存鼠标上一个值
    QPoint seriallastPoint; //保存鼠标上一个值
    QPoint serialendPoint; //保存鼠标上一个值
    QPoint serialcenterPoint; //保存鼠标上一个值

    QImage *image;
    Mat img;
    Mat thr;
    Mat Orimg;
    Mat RatRoil; //较正后的图

    QString filename;

    int isDraw; //绘图模式

    Point topl,topr;

    QTextBrowser TextBrowser_2;
    QListView    listView;

protected:
    /**********重写事件响应的虚函数****************/
    void mouseMoveEvent(QMouseEvent *m);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //拖动进入事件
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_radioButton_clicked(bool checked);
    void on_radioButton_2_clicked(bool checked);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();
    void on_pushButton_8_clicked();
};

#endif // WIDGET_H
