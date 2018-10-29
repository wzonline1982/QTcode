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
    setFixedSize(640,480);  //设置窗口固定大小

    QImage image(400,300,QImage::Format_ARGB32);
    w.show();


}

MyWidget::~MyWidget()
{
    delete ui;
}
