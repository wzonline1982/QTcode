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


private:
    Ui::Widget *ui;

    int paintx=20;
    int painty=50;
    QPixmap pix; //全局变量保存绘制的图片
    QPixmap tempPix; //辅助画布
    QPixmap paintEventPix; //辅助画布

    QPoint lastPoint; //保存鼠标上一个值
    QPoint endPoint; //保存鼠标上一个值

    QImage *image;
    Mat img;
    Mat thr;

    QString filename;

    int isDraw;




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
};

#endif // WIDGET_H
