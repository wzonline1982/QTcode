#include <iostream>
#include "cv.h"
#include "cxcore.h"
#include "highgui.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>

using namespace cv;
using namespace std;


using namespace std;

int main()
{

    Mat img=imread("C:/Users/admin/Desktop/1.BMP"); //读入一张图片

    cvNamedWindow("秦惠文王"); //创建一个名为"秦惠文王"的显示窗口

    imshow("pic", img);   //在窗口中显示图像

    waitKey(10000); //延时10000ms后关闭窗口
    cout << "Hello World!" << endl;
    return 0;
}
