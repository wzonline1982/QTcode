#include "widget.h"
#include "ui_widget.h"
#include "cnn.h"
#include <QDebug>
#include <QThread>

#include <QString>
#include <QtConcurrent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    nSize inputSize={8,8};
    int outSize=8;

    dnn = new DCNN;
    //创建子线程
    thread = new QThread(this);
    //把自定义线程加入到子线程中
    dnn->moveToThread(thread);

    connect(dnn, &DCNN::mySignal, this, &Widget::dealSignal);

    qDebug() << "主线程号：" << QThread::currentThread();

    connect(this,&Widget::startThread,dnn,&DCNN::DCNNThread);  //主窗口接收自己发出的信号，调用子线程函数
//    dnn.cnnsetup(cnn,inputSize,outSize);
//    CNNOpts opts;
//    opts.numepochs=1;
//    opts.alpha=1.0;
//    int trainNum=55000;
   // cnntrain(cnn,trainImg,trainLabel,opts,trainNum);
//    printf("train finished!!\n");
//    const char* filename = "123";
//    QFuture<LabelArr> future = QtConcurrent::run(&CNN.read_Lable,filename);

    connect(this,&Widget::destroyed,this,&Widget::dealClose);
}


void Widget::dealSignal()
{

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_read_clicked()
{
    char*  ch;
    QDir dir = QCoreApplication::applicationDirPath()+"/data/";   //当前程序路径
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
              dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);
      if(fileInfo.fileName() == "train-labels.idx1-ubyte")
      {
         QByteArray ba = fileInfo.filePath().toLatin1();
         ch=ba.data();
         dnn->trainLabel=dnn->read_Lable(ba.data());
       qDebug()<<"train-labels.idx1-ubyte";
      }
      if(fileInfo.fileName() == "train-images.idx3-ubyte")
      {
         QByteArray ba = fileInfo.filePath().toLatin1();
         ch=ba.data();
         dnn->trainImg=dnn->read_Img(ba.data());
       qDebug()<<"train-images.idx3-ubyte";
      }
      if(fileInfo.fileName() == "t10k-labels.idx1-ubyte")
      {
         QByteArray ba = fileInfo.filePath().toLatin1();
         ch=ba.data();
         dnn->testLabel=dnn->read_Lable(ba.data());
       qDebug()<<"t10k-labels.idx1-ubyte";
      }
      if(fileInfo.fileName() == "t10k-images.idx3-ubyte")
      {
         QByteArray ba = fileInfo.filePath().toLatin1();
         ch=ba.data();
         dnn->testImg=dnn->read_Img(ba.data());
         qDebug()<<"t10k-images.idx3-ubyte";
      }
          //qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
         // qDebug()<<fileInfo.fileName();
    }
}


void Widget::on_pushButton_init_clicked()
{
    if(thread->isRunning() == true)
    {
       return;
    }

    dnn->setModel(CNNINIT);  //设置CNN运行代码段

    //启动线程，但是没有启动线程处理函数
    thread->start();
    dnn->setFlag(false);
    emit startThread();
}

void Widget::on_pushButton_clicked()
{
    dnn->setFlag(true);
    thread->quit(); //等待线程结束关闭
    thread->wait();
}

void Widget::dealClose()
{
    dnn->setFlag(true);
    thread->quit(); //等待线程结束关闭
    thread->wait();
}
