#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QGraphicsScene>
#include <QUrl>

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

    cv::Mat QImage2cvMat(QImage image);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
    QImage *image;
    QGraphicsScene *scene;
    Mat img;
    Mat thr;


protected:
    void dragEnterEvent(QDragEnterEvent *event);//拖动进入事件
    void dropEvent(QDropEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


};

#endif // WIDGET_H
