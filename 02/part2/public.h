#ifndef PUBLIC_H
#define PUBLIC_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QGraphicsScene>
#include <QUrl>
#include <QDebug>

#include "cv.h"
#include "cxcore.h"
#include "highgui.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


QImage cvMat2QImage(const cv::Mat& mat);
cv::Mat QImage2cvMat(QImage image);

#endif // PUBLIC_H
