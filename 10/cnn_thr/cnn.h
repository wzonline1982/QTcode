#ifndef CNN_H
#define CNN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <random>
#include <time.h>
#include <assert.h>

#include <QObject>
#include <QThread>
#include <QDebug>

#define full 0
#define same 1
#define _valid 2

#define CNNINIT  1





//图像数组
typedef struct MinstImg{
    int c;           // 图像宽
    int r;           // 图像高
    float** ImgData; // 图像数据二维动态数组
}MinstImg;

typedef struct MinstImgArr{
    int ImgNum;        // 存储图像的数目
    MinstImg* ImgPtr;  // 存储图像数组指针
}*ImgArr;              // 存储图像数据的数组

//标记数组
typedef struct MinstLabel{
    int l;            // 输出标记的长
    float* LabelData; // 输出标记数据
}MinstLabel;

typedef struct MinstLabelArr{
    int LabelNum;
    MinstLabel* LabelPtr;
}*LabelArr;              // 存储图像标记的数组

//尺寸
typedef struct Mat2DSize{
    int c; // 列（宽）
    int r; // 行（高）
}nSize;

namespace Ui {
class Widget;
}


/*
    CNN 数据结构
*/

#define AvePool 0
#define MaxPool 1
#define MinPool 2

// 卷积层
typedef struct convolutional_layer{
    int inputWidth;   //输入图像的宽
    int inputHeight;  //输入图像的长
    int mapSize;      //特征模板的大小，模板一般都是正方形

    int inChannels;   //输入图像的数目
    int outChannels;  //输出图像的数目

    // 关于特征模板的权重分布，这里是一个四维数组
    // 其大小为inChannels*outChannels*mapSize*mapSize大小
    // 这里用四维数组，主要是为了表现全连接的形式，实际上卷积层并没有用到全连接的形式
    // 这里的例子是DeapLearningToolboox里的CNN例子，其用到就是全连接
    float**** mapData;     //存放特征模块的数据
    float**** dmapData;    //存放特征模块的数据的局部梯度

    float* basicData;   //偏置，偏置的大小，为outChannels
    bool isFullConnect; //是否为全连接
    bool* connectModel; //连接模式（默认为全连接）

    // 下面三者的大小同输出的维度相同
    float*** v; // 进入激活函数的输入值
    float*** y; // 激活函数后神经元的输出

    // 输出像素的局部梯度
    float*** d; // 网络的局部梯度,δ值
}CovLayer;

// 采样层 pooling
typedef struct pooling_layer{
    int inputWidth;   //输入图像的宽
    int inputHeight;  //输入图像的长
    int mapSize;      //特征模板的大小

    int inChannels;   //输入图像的数目
    int outChannels;  //输出图像的数目

    int poolType;     //Pooling的方法
    float* basicData;   //偏置

    float*** y; // 采样函数后神经元的输出,无激活函数
    float*** d; // 网络的局部梯度,δ值
}PoolLayer;

// 输出层 全连接的神经网络
typedef struct nn_layer{
    int inputNum;   //输入数据的数目
    int outputNum;  //输出数据的数目

    float** wData; // 权重数据，为一个inputNum*outputNum大小
    float* basicData;   //偏置，大小为outputNum大小

    // 下面三者的大小同输出的维度相同
    float* v; // 进入激活函数的输入值
    float* y; // 激活函数后神经元的输出
    float* d; // 网络的局部梯度,δ值

    bool isFullConnect; //是否为全连接
}OutLayer;

typedef struct cnn_network{
    int layerNum;
    CovLayer* C1;
    PoolLayer* S2;
    CovLayer* C3;
    PoolLayer* S4;
    OutLayer* O5;

    float* e; // 训练误差
    float* L; // 瞬时误差能量
}CNN;

typedef struct train_opts{
    int numepochs; // 训练的迭代次数
    float alpha; // 学习速率
}CNNOpts;


//定义类
class DCNN : public QObject
{
    Q_OBJECT
        //只有加入了Q_OBJECT，你才能使用QT中的signal和slot机制。
private:
    bool isStop;

public:
    explicit DCNN(QObject *parent = 0);  //explicit explicit关键字只能用于修饰只有一个参数的类构造函数
    ~DCNN();

    CNNOpts opts;
    int trainNum;
    int MODEL;

    CNN* cnn;
    LabelArr trainLabel;
    ImgArr trainImg;
    LabelArr testLabel;
    ImgArr testImg;

    nSize inputSize;
    int outSize;

    void setModel(int model);
    void setFlag(bool flag = true);
    void setInputSize(nSize inputSize);
    void setOutSize(int);
    void DCNNThread();

    //Minst 数据集
    int ReverseInt(int i);
    LabelArr read_Lable(const char* filename); // 读入图像标记
    ImgArr read_Img(const char* filename); // 读入图像
    void save_Img(ImgArr imgarr,char* filedir); // 将图像数据保存成文件


    //CNN 函数
    void cnnsetup(CNN* cnn,nSize inputSize,int outputSize);
    // 初始化卷积层
    CovLayer* initCovLayer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels);
    void CovLayerConnect(CovLayer* covL,bool* connectModel);
    // 初始化采样层
    PoolLayer* initPoolLayer(int inputWidth,int inputHeigh,int mapSize,int inChannels,int outChannels,int poolType);
   // void PoolLayerConnect(PoolLayer* poolL,bool* connectModel);
    // 初始化输出层
    OutLayer* initOutLayer(int inputNum,int outputNum);

    void avgPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize); // 求平均值
    float activation_Sigma(float input,float bas);
    //前向计算，识别输出
    void nnff(float* output,float* input,float** wdata,float* bas,nSize nnSize); // 单层全连接神经网络的前向传播
    float vecMulti(float* vec1,float* vec2,int vecL);
    float sigma_derivation(float y);

    void cnnff(CNN* cnn,float** inputData);
    void cnnbp(CNN* cnn,float* outputData);
    void cnnapplygrads(CNN* cnn,CNNOpts opts,float** inputData); // 更新权重
    void cnnclear(CNN* cnn);  //drop out 清除部份数据
    void cnntrain(CNN* cnn,	ImgArr inputData,LabelArr outputData,CNNOpts opts,int trainNum);


    //mat 函数
    float** rotate180(float** mat, nSize matSize);// 矩阵翻转180度
    void addmat(float** res, float** mat1, nSize matSize1, float** mat2, nSize matSize2);// 矩阵相加
    float** correlation(float** map,nSize mapSize,float** inputData,nSize inSize,int type);// 互相关
    float** cov(float** map,nSize mapSize,float** inputData,nSize inSize,int type); // 卷积操作
    // 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
    float** UpSample(float** mat,nSize matSize,int upc,int upr);
    // 给二维矩阵边缘扩大，增加addw大小的0值边
    float** matEdgeExpand(float** mat,nSize matSize,int addc,int addr);
    // 给二维矩阵边缘缩小，擦除shrinkc大小的边
    float** matEdgeShrink(float** mat,nSize matSize,int shrinkc,int shrinkr);
    void savemat(float** mat,nSize matSize,const char* filename);// 保存矩阵数据
    void multifactor(float** res, float** mat, nSize matSize, float factor);// 矩阵乘以系数
    float summat(float** mat,nSize matSize);// 矩阵各元素的和
    char * combine_strings(char *a, char *b);
    char* intTochar(int i);

    //测试函数
    int vecmaxIndex(float* vec, int veclength);// 返回向量最大数的序号
    float cnntest(CNN* cnn, ImgArr inputData,LabelArr outputData,int testNum);

    void savecnndata(CNN* cnn,const char* filename,float** inputdata);
    void savecnn(CNN* cnn, const char* filename);
    void importcnn(CNN* cnn, const char* filename);

    void show();

signals:
    void mySignal();

public slots:

};

#endif // CNN_H
