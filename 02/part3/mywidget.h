#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <subwin.h>
#include <binwin.h>

#include "cv.h"
#include "cxcore.h"
#include "highgui.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void dealSlot(QString);

private:
    Ui::MyWidget *ui;

    Subwin w;
    binwin binw;

    Mat img;
    Mat thr;
    QImage *image;
    QString fileName;

};

#endif // MYWIDGET_H
