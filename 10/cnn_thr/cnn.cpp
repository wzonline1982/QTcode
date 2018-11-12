#include "cnn.h"

DCNN::DCNN(QObject *parent) : QObject(parent)
{
    cnn=(CNN*)malloc(sizeof(CNN)); //声明结构体
}
DCNN::~DCNN()
{

}

void DCNN::cnnsetup(CNN* cnn,nSize inputSize,int outputSize)
{
    cnn->layerNum=5;

    nSize inSize;
    int mapSize=5;
    inSize.c=inputSize.c;
    inSize.r=inputSize.r;
    cnn->C1=initCovLayer(inSize.c,inSize.r,5,1,6);
    inSize.c=inSize.c-mapSize+1;
    inSize.r=inSize.r-mapSize+1;
    cnn->S2=initPoolLayer(inSize.c,inSize.r,2,6,6,AvePool);
    inSize.c=inSize.c/2;
    inSize.r=inSize.r/2;
    cnn->C3=initCovLayer(inSize.c,inSize.r,5,6,12);
    inSize.c=inSize.c-mapSize+1;
    inSize.r=inSize.r-mapSize+1;
    cnn->S4=initPoolLayer(inSize.c,inSize.r,2,12,12,AvePool);
    inSize.c=inSize.c/2;
    inSize.r=inSize.r/2;
    cnn->O5=initOutLayer(inSize.c*inSize.r*12,outputSize);

    cnn->e=(float*)calloc(cnn->O5->outputNum,sizeof(float));
}

CovLayer* DCNN::initCovLayer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels)
{
    CovLayer* covL=(CovLayer*)malloc(sizeof(CovLayer));

    covL->inputHeight=inputHeight;
    covL->inputWidth=inputWidth;
    covL->mapSize=mapSize;

    covL->inChannels=inChannels;
    covL->outChannels=outChannels;

    covL->isFullConnect=true; // 默认为全连接

    // 权重空间的初始化，先行再列调用，[r][c]
    int i,j,c,r;
    srand((unsigned)time(NULL));
    covL->mapData=(float****)malloc(inChannels*sizeof(float***));
    for(i=0;i<inChannels;i++){
        covL->mapData[i]=(float***)malloc(outChannels*sizeof(float**));
        for(j=0;j<outChannels;j++){
            covL->mapData[i][j]=(float**)malloc(mapSize*sizeof(float*));
            for(r=0;r<mapSize;r++){
                covL->mapData[i][j][r]=(float*)malloc(mapSize*sizeof(float));
                for(c=0;c<mapSize;c++){
                    float randnum=(((float)rand()/(float)RAND_MAX)-0.5)*2;
                    covL->mapData[i][j][r][c]=randnum*sqrt((float)6.0/(float)(mapSize*mapSize*(inChannels+outChannels)));
                }
            }
        }
    }
    // 权重梯度变化
    covL->dmapData=(float****)malloc(inChannels*sizeof(float***));
    for(i=0;i<inChannels;i++){
        covL->dmapData[i]=(float***)malloc(outChannels*sizeof(float**));
        for(j=0;j<outChannels;j++){
            covL->dmapData[i][j]=(float**)malloc(mapSize*sizeof(float*));
            for(r=0;r<mapSize;r++){
                covL->dmapData[i][j][r]=(float*)calloc(mapSize,sizeof(float));
            }
        }
    }

    covL->basicData=(float*)calloc(outChannels,sizeof(float));

    int outW=inputWidth-mapSize+1;
    int outH=inputHeight-mapSize+1;


    covL->d=(float***)malloc(outChannels*sizeof(float**));
    covL->v=(float***)malloc(outChannels*sizeof(float**));
    covL->y=(float***)malloc(outChannels*sizeof(float**));
    for(j=0;j<outChannels;j++){
        covL->d[j]=(float**)malloc(outH*sizeof(float*));
        covL->v[j]=(float**)malloc(outH*sizeof(float*));
        covL->y[j]=(float**)malloc(outH*sizeof(float*));
        for(r=0;r<outH;r++){
            covL->d[j][r]=(float*)calloc(outW,sizeof(float));
            covL->v[j][r]=(float*)calloc(outW,sizeof(float));
            covL->y[j][r]=(float*)calloc(outW,sizeof(float));
        }
    }

    return covL;
}

PoolLayer* DCNN::initPoolLayer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels,int poolType)
{
    PoolLayer* poolL=(PoolLayer*)malloc(sizeof(PoolLayer));

    poolL->inputHeight=inputHeight;
    poolL->inputWidth=inputWidth;
    poolL->mapSize=mapSize;
    poolL->inChannels=inChannels;
    poolL->outChannels=outChannels;
    poolL->poolType=poolType;

    poolL->basicData=(float*)calloc(outChannels,sizeof(float));

    int outW=inputWidth/mapSize;
    int outH=inputHeight/mapSize;

    int j,r;
    poolL->d=(float***)malloc(outChannels*sizeof(float**));
    poolL->y=(float***)malloc(outChannels*sizeof(float**));
    for(j=0;j<outChannels;j++){
        poolL->d[j]=(float**)malloc(outH*sizeof(float*));
        poolL->y[j]=(float**)malloc(outH*sizeof(float*));
        for(r=0;r<outH;r++){
            poolL->d[j][r]=(float*)calloc(outW,sizeof(float));
            poolL->y[j][r]=(float*)calloc(outW,sizeof(float));
        }
    }

    return poolL;
}

OutLayer* DCNN::initOutLayer(int inputNum,int outputNum)
{
    OutLayer* outL=(OutLayer*)malloc(sizeof(OutLayer));

    outL->inputNum=inputNum;
    outL->outputNum=outputNum;


    outL->basicData=(float*)calloc(outputNum,sizeof(float));

    outL->d=(float*)calloc(outputNum,sizeof(float));
    outL->v=(float*)calloc(outputNum,sizeof(float));
    outL->y=(float*)calloc(outputNum,sizeof(float));

    // 权重的初始化
    outL->wData=(float**)malloc(outputNum*sizeof(float*)); // 输入行，输出列
    int i,j;
    srand((unsigned)time(NULL));
    for(i=0;i<outputNum;i++){
        outL->wData[i]=(float*)malloc(inputNum*sizeof(float));
        for(j=0;j<inputNum;j++){
            float randnum=(((float)rand()/(float)RAND_MAX)-0.5)*2; // 产生一个-1到1的随机数
            outL->wData[i][j]=randnum*sqrt((float)6.0/(float)(inputNum+outputNum));
        }
    }

    outL->isFullConnect=true;

    return outL;
}

int DCNN::vecmaxIndex(float* vec, int veclength)// 返回向量最大数的序号
{
    int i;
    float maxnum=-1.0;
    int maxIndex=0;
    for(i=0;i<veclength;i++){
        if(maxnum<vec[i]){
            maxnum=vec[i];
            maxIndex=i;
        }
    }
    return maxIndex;
}

// 测试cnn函数
float DCNN::cnntest(CNN* cnn, ImgArr inputData,LabelArr outputData,int testNum)
{
    int n=0;
    int incorrectnum=0;  //错误预测的数目
    for(n=0;n<testNum;n++){
        cnnff(cnn,inputData->ImgPtr[n].ImgData);
        if(vecmaxIndex(cnn->O5->y,cnn->O5->outputNum)!=vecmaxIndex(outputData->LabelPtr[n].LabelData,cnn->O5->outputNum))
            incorrectnum++;
        cnnclear(cnn);
    }
    return (float)incorrectnum/(float)testNum;
}

// 保存cnn
void DCNN::savecnn(CNN* cnn, const char* filename)
{
    FILE  *fp=NULL;
    fp=fopen(filename,"wb");
    if(fp==NULL)
        printf("write file failed\n");

    int i,j,r;
    // C1的数据
    for(i=0;i<cnn->C1->inChannels;i++)
        for(j=0;j<cnn->C1->outChannels;j++)
            for(r=0;r<cnn->C1->mapSize;r++)
                fwrite(cnn->C1->mapData[i][j][r],sizeof(float),cnn->C1->mapSize,fp);

    fwrite(cnn->C1->basicData,sizeof(float),cnn->C1->outChannels,fp);

    // C3网络
    for(i=0;i<cnn->C3->inChannels;i++)
        for(j=0;j<cnn->C3->outChannels;j++)
            for(r=0;r<cnn->C3->mapSize;r++)
                fwrite(cnn->C3->mapData[i][j][r],sizeof(float),cnn->C3->mapSize,fp);

    fwrite(cnn->C3->basicData,sizeof(float),cnn->C3->outChannels,fp);

    // O5输出层
    for(i=0;i<cnn->O5->outputNum;i++)
        fwrite(cnn->O5->wData[i],sizeof(float),cnn->O5->inputNum,fp);
    fwrite(cnn->O5->basicData,sizeof(float),cnn->O5->outputNum,fp);

    fclose(fp);
}
// 导入cnn的数据
void DCNN::importcnn(CNN* cnn, const char* filename)
{
    FILE  *fp=NULL;
    fp=fopen(filename,"rb");
    if(fp==NULL)
        printf("write file failed\n");

    int i,j,c,r;
    // C1的数据
    for(i=0;i<cnn->C1->inChannels;i++)
        for(j=0;j<cnn->C1->outChannels;j++)
            for(r=0;r<cnn->C1->mapSize;r++)
                for(c=0;c<cnn->C1->mapSize;c++){
                    float* in=(float*)malloc(sizeof(float));
                    fread(in,sizeof(float),1,fp);
                    cnn->C1->mapData[i][j][r][c]=*in;
                }

    for(i=0;i<cnn->C1->outChannels;i++)
        fread(&cnn->C1->basicData[i],sizeof(float),1,fp);

    // C3网络
    for(i=0;i<cnn->C3->inChannels;i++)
        for(j=0;j<cnn->C3->outChannels;j++)
            for(r=0;r<cnn->C3->mapSize;r++)
                for(c=0;c<cnn->C3->mapSize;c++)
                fread(&cnn->C3->mapData[i][j][r][c],sizeof(float),1,fp);

    for(i=0;i<cnn->C3->outChannels;i++)
        fread(&cnn->C3->basicData[i],sizeof(float),1,fp);

    // O5输出层
    for(i=0;i<cnn->O5->outputNum;i++)
        for(j=0;j<cnn->O5->inputNum;j++)
            fread(&cnn->O5->wData[i][j],sizeof(float),1,fp);

    for(i=0;i<cnn->O5->outputNum;i++)
        fread(&cnn->O5->basicData[i],sizeof(float),1,fp);

    fclose(fp);
}

void DCNN::cnntrain(CNN* cnn,	ImgArr inputData,LabelArr outputData,CNNOpts opts,int trainNum)
{
    // 学习训练误差曲线
    cnn->L=(float*)malloc(trainNum*sizeof(float));
    int e;
    for(e=0;e<opts.numepochs;e++){
        int n=0;
        for(n=0;n<trainNum;n++){
            //printf("%d\n",n);
       //     qDebug() << n;
            cnnff(cnn,inputData->ImgPtr[n].ImgData);  // 前向传播，这里主要计算各
            cnnbp(cnn,outputData->LabelPtr[n].LabelData); // 后向传播，这里主要计算各神经元的误差梯度


       //     char* filedir="E:\\Code\\Matlab\\PicTrans\\CNNData\\";
       //     const char* filename=combine_strings(filedir,combine_strings(intTochar(n),".cnn"));
      //      savecnndata(cnn,filename,inputData->ImgPtr[n].ImgData);
            cnnapplygrads(cnn,opts,inputData->ImgPtr[n].ImgData); // 更新权重

            cnnclear(cnn);
            // 计算并保存误差能量
            float l=0.0;
            int i;
            for(i=0;i<cnn->O5->outputNum;i++)
                l=l+cnn->e[i]*cnn->e[i];
            if(n==0)
                cnn->L[n]=l/(float)2.0;
            else
                cnn->L[n]=cnn->L[n-1]*0.99+0.01*l/(float)2.0;

            qDebug() << n <<  cnn->L[n];
            if(isStop == true)break; //结束线程标志
        }
    }
}

// 这里InputData是图像数据，inputData[r][c],r行c列，这里根各权重模板是一致的
void DCNN::cnnff(CNN* cnn,float** inputData)
{
    int outSizeW=cnn->S2->inputWidth;
    int outSizeH=cnn->S2->inputHeight;
    // 第一层的传播
    int i,j,r,c;
    // 第一层输出数据
    nSize mapSize={cnn->C1->mapSize,cnn->C1->mapSize};
    nSize inSize={cnn->C1->inputWidth,cnn->C1->inputHeight};
    nSize outSize={cnn->S2->inputWidth,cnn->S2->inputHeight};
    for(i=0;i<(cnn->C1->outChannels);i++){
        for(j=0;j<(cnn->C1->inChannels);j++){
            float** mapout=cov(cnn->C1->mapData[j][i],mapSize,inputData,inSize,_valid);
            addmat(cnn->C1->v[i],cnn->C1->v[i],outSize,mapout,outSize);
            for(r=0;r<outSize.r;r++)
                free(mapout[r]);
            free(mapout);
        }
        for(r=0;r<outSize.r;r++)
            for(c=0;c<outSize.c;c++)
                cnn->C1->y[i][r][c]=activation_Sigma(cnn->C1->v[i][r][c],cnn->C1->basicData[i]);
    }

    // 第二层的输出传播S2，采样层
    outSize.c=cnn->C3->inputWidth;
    outSize.r=cnn->C3->inputHeight;
    inSize.c=cnn->S2->inputWidth;
    inSize.r=cnn->S2->inputHeight;
    for(i=0;i<(cnn->S2->outChannels);i++){
        if(cnn->S2->poolType==AvePool)
            avgPooling(cnn->S2->y[i],outSize,cnn->C1->y[i],inSize,cnn->S2->mapSize);
    }

    // 第三层输出传播,这里是全连接
    outSize.c=cnn->S4->inputWidth;
    outSize.r=cnn->S4->inputHeight;
    inSize.c=cnn->C3->inputWidth;
    inSize.r=cnn->C3->inputHeight;
    mapSize.c=cnn->C3->mapSize;
    mapSize.r=cnn->C3->mapSize;
    for(i=0;i<(cnn->C3->outChannels);i++){
        for(j=0;j<(cnn->C3->inChannels);j++){
            float** mapout=cov(cnn->C3->mapData[j][i],mapSize,cnn->S2->y[j],inSize,_valid);
            addmat(cnn->C3->v[i],cnn->C3->v[i],outSize,mapout,outSize);
            for(r=0;r<outSize.r;r++)
                free(mapout[r]);
            free(mapout);
        }
        for(r=0;r<outSize.r;r++)
            for(c=0;c<outSize.c;c++)
                cnn->C3->y[i][r][c]=activation_Sigma(cnn->C3->v[i][r][c],cnn->C3->basicData[i]);
    }

    // 第四层的输出传播
    inSize.c=cnn->S4->inputWidth;
    inSize.r=cnn->S4->inputHeight;
    outSize.c=inSize.c/cnn->S4->mapSize;
    outSize.r=inSize.r/cnn->S4->mapSize;
    for(i=0;i<(cnn->S4->outChannels);i++){
        if(cnn->S4->poolType==AvePool)
            avgPooling(cnn->S4->y[i],outSize,cnn->C3->y[i],inSize,cnn->S4->mapSize);
    }

    // 输出层O5的处理
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData=(float*)malloc((cnn->O5->inputNum)*sizeof(float));
    for(i=0;i<(cnn->S4->outChannels);i++)
        for(r=0;r<outSize.r;r++)
            for(c=0;c<outSize.c;c++)
                O5inData[i*outSize.r*outSize.c+r*outSize.c+c]=cnn->S4->y[i][r][c];

    nSize nnSize={cnn->O5->inputNum,cnn->O5->outputNum};
    nnff(cnn->O5->v,O5inData,cnn->O5->wData,cnn->O5->basicData,nnSize);
    for(i=0;i<cnn->O5->outputNum;i++)
        cnn->O5->y[i]=activation_Sigma(cnn->O5->v[i],cnn->O5->basicData[i]);
    free(O5inData);
}

// 激活函数 input是数据，inputNum说明数据数目，bas表明偏置
float DCNN::activation_Sigma(float input,float bas) // sigma激活函数
{
    float temp=input+bas;
    return (float)1.0/((float)(1.0+exp(-temp)));
}

void DCNN::avgPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize) // 求平均值
{
    int outputW=inputSize.c/mapSize;
    int outputH=inputSize.r/mapSize;
    if(outputSize.c!=outputW||outputSize.r!=outputH)
        printf("ERROR: output size is wrong!!");

    int i,j,m,n;
    for(i=0;i<outputH;i++)
        for(j=0;j<outputW;j++)
        {
            float sum=0.0;
            for(m=i*mapSize;m<i*mapSize+mapSize;m++)
                for(n=j*mapSize;n<j*mapSize+mapSize;n++)
                    sum=sum+input[m][n];

            output[i][j]=sum/(float)(mapSize*mapSize);
        }
}

// 单层全连接神经网络的前向传播
float DCNN::vecMulti(float* vec1,float* vec2,int vecL)// 两向量相乘
{
    int i;
    float m=0;
    for(i=0;i<vecL;i++)
        m=m+vec1[i]*vec2[i];
    return m;
}

void DCNN::nnff(float* output,float* input,float** wdata,float* bas,nSize nnSize)
{
    int w=nnSize.c;
    int h=nnSize.r;

    int i;
    for(i=0;i<h;i++)
        output[i]=vecMulti(input,wdata[i],w)+bas[i];
}

float DCNN::sigma_derivation(float y){ // Logic激活函数的自变量微分
    return y*(1-y); // 这里y是指经过激活函数的输出值，而不是自变量
}

void DCNN::cnnbp(CNN* cnn,float* outputData) // 网络的后向传播
{
    int i,j,c,r; // 将误差保存到网络中
    for(i=0;i<cnn->O5->outputNum;i++)
        cnn->e[i]=cnn->O5->y[i]-outputData[i];

    /*从后向前反向计算*/
    // 输出层O5
    for(i=0;i<cnn->O5->outputNum;i++)
        cnn->O5->d[i]=cnn->e[i]*sigma_derivation(cnn->O5->y[i]);

    // S4层，传递到S4层的误差
    // 这里没有激活函数
    nSize outSize={cnn->S4->inputWidth/cnn->S4->mapSize,cnn->S4->inputHeight/cnn->S4->mapSize};
    for(i=0;i<cnn->S4->outChannels;i++)
        for(r=0;r<outSize.r;r++)
            for(c=0;c<outSize.c;c++)
                for(j=0;j<cnn->O5->outputNum;j++){
                    int wInt=i*outSize.c*outSize.r+r*outSize.c+c;
                    cnn->S4->d[i][r][c]=cnn->S4->d[i][r][c]+cnn->O5->d[j]*cnn->O5->wData[j][wInt];
                }

    // C3层
    // 由S4层传递的各反向误差,这里只是在S4的梯度上扩充一倍
    int mapdata=cnn->S4->mapSize;
    nSize S4dSize={cnn->S4->inputWidth/cnn->S4->mapSize,cnn->S4->inputHeight/cnn->S4->mapSize};
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for(i=0;i<cnn->C3->outChannels;i++){
        float** C3e=UpSample(cnn->S4->d[i],S4dSize,cnn->S4->mapSize,cnn->S4->mapSize);
        for(r=0;r<cnn->S4->inputHeight;r++)
            for(c=0;c<cnn->S4->inputWidth;c++)
                cnn->C3->d[i][r][c]=C3e[r][c]*sigma_derivation(cnn->C3->y[i][r][c])/(float)(cnn->S4->mapSize*cnn->S4->mapSize);
        for(r=0;r<cnn->S4->inputHeight;r++)
            free(C3e[r]);
        free(C3e);
    }

    // S2层，S2层没有激活函数，这里只有卷积层有激活函数部分
    // 由卷积层传递给采样层的误差梯度，这里卷积层共有6*12个卷积模板
    outSize.c=cnn->C3->inputWidth;
    outSize.r=cnn->C3->inputHeight;
    nSize inSize={cnn->S4->inputWidth,cnn->S4->inputHeight};
    nSize mapSize={cnn->C3->mapSize,cnn->C3->mapSize};
    for(i=0;i<cnn->S2->outChannels;i++){
        for(j=0;j<cnn->C3->outChannels;j++){
            float** corr=correlation(cnn->C3->mapData[i][j],mapSize,cnn->C3->d[j],inSize,full);
            addmat(cnn->S2->d[i],cnn->S2->d[i],outSize,corr,outSize);
            for(r=0;r<outSize.r;r++)
                free(corr[r]);
            free(corr);
        }
        /*
        for(r=0;r<cnn->C3->inputHeight;r++)
            for(c=0;c<cnn->C3->inputWidth;c++)
                // 这里本来用于采样的激活
        */
    }

    // C1层，卷积层
    mapdata=cnn->S2->mapSize;
    nSize S2dSize={cnn->S2->inputWidth/cnn->S2->mapSize,cnn->S2->inputHeight/cnn->S2->mapSize};
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for(i=0;i<cnn->C1->outChannels;i++){
        float** C1e=UpSample(cnn->S2->d[i],S2dSize,cnn->S2->mapSize,cnn->S2->mapSize);
        for(r=0;r<cnn->S2->inputHeight;r++)
            for(c=0;c<cnn->S2->inputWidth;c++)
                cnn->C1->d[i][r][c]=C1e[r][c]*sigma_derivation(cnn->C1->y[i][r][c])/(float)(cnn->S2->mapSize*cnn->S2->mapSize);
        for(r=0;r<cnn->S2->inputHeight;r++)
            free(C1e[r]);
        free(C1e);
    }
}

void DCNN::cnnapplygrads(CNN* cnn,CNNOpts opts,float** inputData) // 更新权重
{
    // 这里存在权重的主要是卷积层和输出层
    // 更新这两个地方的权重就可以了
    int i,j,r,c;

    // C1层的权重更新
    nSize dSize={cnn->S2->inputHeight,cnn->S2->inputWidth};
    nSize ySize={cnn->C1->inputHeight,cnn->C1->inputWidth};
    nSize mapSize={cnn->C1->mapSize,cnn->C1->mapSize};

    for(i=0;i<cnn->C1->outChannels;i++){
        for(j=0;j<cnn->C1->inChannels;j++){
            float** flipinputData=rotate180(inputData,ySize);
            float** C1dk=cov(cnn->C1->d[i],dSize,flipinputData,ySize,_valid);
            multifactor(C1dk,C1dk,mapSize,-1*opts.alpha);
            addmat(cnn->C1->mapData[j][i],cnn->C1->mapData[j][i],mapSize,C1dk,mapSize);
            for(r=0;r<(dSize.r-(ySize.r-1));r++)
                free(C1dk[r]);
            free(C1dk);
            for(r=0;r<ySize.r;r++)
                free(flipinputData[r]);
            free(flipinputData);
        }
        cnn->C1->basicData[i]=cnn->C1->basicData[i]-opts.alpha*summat(cnn->C1->d[i],dSize);
    }

    // C3层的权重更新
    dSize.c=cnn->S4->inputWidth;
    dSize.r=cnn->S4->inputHeight;
    ySize.c=cnn->C3->inputWidth;
    ySize.r=cnn->C3->inputHeight;
    mapSize.c=cnn->C3->mapSize;
    mapSize.r=cnn->C3->mapSize;
    for(i=0;i<cnn->C3->outChannels;i++){
        for(j=0;j<cnn->C3->inChannels;j++){
            float** flipinputData=rotate180(cnn->S2->y[j],ySize);
            float** C3dk=cov(cnn->C3->d[i],dSize,flipinputData,ySize,_valid);
            multifactor(C3dk,C3dk,mapSize,-1.0*opts.alpha);
            addmat(cnn->C3->mapData[j][i],cnn->C3->mapData[j][i],mapSize,C3dk,mapSize);
            for(r=0;r<(dSize.r-(ySize.r-1));r++)
                free(C3dk[r]);
            free(C3dk);
            for(r=0;r<ySize.r;r++)
                free(flipinputData[r]);
            free(flipinputData);
        }
        cnn->C3->basicData[i]=cnn->C3->basicData[i]-opts.alpha*summat(cnn->C3->d[i],dSize);
    }

    // 输出层
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData=(float*)malloc((cnn->O5->inputNum)*sizeof(float));
    nSize outSize={cnn->S4->inputWidth/cnn->S4->mapSize,cnn->S4->inputHeight/cnn->S4->mapSize};
    for(i=0;i<(cnn->S4->outChannels);i++)
        for(r=0;r<outSize.r;r++)
            for(c=0;c<outSize.c;c++)
                O5inData[i*outSize.r*outSize.c+r*outSize.c+c]=cnn->S4->y[i][r][c];

    for(j=0;j<cnn->O5->outputNum;j++){
        for(i=0;i<cnn->O5->inputNum;i++)
            cnn->O5->wData[j][i]=cnn->O5->wData[j][i]-opts.alpha*cnn->O5->d[j]*O5inData[i];
        cnn->O5->basicData[j]=cnn->O5->basicData[j]-opts.alpha*cnn->O5->d[j];
    }
    free(O5inData);
}

void DCNN::cnnclear(CNN* cnn)
{
    // 将神经元的部分数据清除
    int j,c,r;
    // C1网络
    for(j=0;j<cnn->C1->outChannels;j++){
        for(r=0;r<cnn->S2->inputHeight;r++){
            for(c=0;c<cnn->S2->inputWidth;c++){
                cnn->C1->d[j][r][c]=(float)0.0;
                cnn->C1->v[j][r][c]=(float)0.0;
                cnn->C1->y[j][r][c]=(float)0.0;
            }
        }
    }
    // S2网络
    for(j=0;j<cnn->S2->outChannels;j++){
        for(r=0;r<cnn->C3->inputHeight;r++){
            for(c=0;c<cnn->C3->inputWidth;c++){
                cnn->S2->d[j][r][c]=(float)0.0;
                cnn->S2->y[j][r][c]=(float)0.0;
            }
        }
    }
    // C3网络
    for(j=0;j<cnn->C3->outChannels;j++){
        for(r=0;r<cnn->S4->inputHeight;r++){
            for(c=0;c<cnn->S4->inputWidth;c++){
                cnn->C3->d[j][r][c]=(float)0.0;
                cnn->C3->v[j][r][c]=(float)0.0;
                cnn->C3->y[j][r][c]=(float)0.0;
            }
        }
    }
    // S4网络
    for(j=0;j<cnn->S4->outChannels;j++){
        for(r=0;r<cnn->S4->inputHeight/cnn->S4->mapSize;r++){
            for(c=0;c<cnn->S4->inputWidth/cnn->S4->mapSize;c++){
                cnn->S4->d[j][r][c]=(float)0.0;
                cnn->S4->y[j][r][c]=(float)0.0;
            }
        }
    }
    // O5输出
    for(j=0;j<cnn->O5->outputNum;j++){
        cnn->O5->d[j]=(float)0.0;
        cnn->O5->v[j]=(float)0.0;
        cnn->O5->y[j]=(float)0.0;
    }
}

// 这是用于测试的函数
void DCNN::savecnndata(CNN* cnn,const char* filename,float** inputdata) // 保存CNN网络中的相关数据
{
    FILE  *fp=NULL;
    fp=fopen(filename,"wb");
    if(fp==NULL)
        printf("write file failed\n");

    // C1的数据
    int i,j,r;
    // C1网络
    for(i=0;i<cnn->C1->inputHeight;i++)
        fwrite(inputdata[i],sizeof(float),cnn->C1->inputWidth,fp);
    for(i=0;i<cnn->C1->inChannels;i++)
        for(j=0;j<cnn->C1->outChannels;j++)
            for(r=0;r<cnn->C1->mapSize;r++)
                fwrite(cnn->C1->mapData[i][j][r],sizeof(float),cnn->C1->mapSize,fp);

    fwrite(cnn->C1->basicData,sizeof(float),cnn->C1->outChannels,fp);

    for(j=0;j<cnn->C1->outChannels;j++){
        for(r=0;r<cnn->S2->inputHeight;r++){
            fwrite(cnn->C1->v[j][r],sizeof(float),cnn->S2->inputWidth,fp);
        }
        for(r=0;r<cnn->S2->inputHeight;r++){
            fwrite(cnn->C1->d[j][r],sizeof(float),cnn->S2->inputWidth,fp);
        }
        for(r=0;r<cnn->S2->inputHeight;r++){
            fwrite(cnn->C1->y[j][r],sizeof(float),cnn->S2->inputWidth,fp);
        }
    }

    // S2网络
    for(j=0;j<cnn->S2->outChannels;j++){
        for(r=0;r<cnn->C3->inputHeight;r++){
            fwrite(cnn->S2->d[j][r],sizeof(float),cnn->C3->inputWidth,fp);
        }
        for(r=0;r<cnn->C3->inputHeight;r++){
            fwrite(cnn->S2->y[j][r],sizeof(float),cnn->C3->inputWidth,fp);
        }
    }
    // C3网络
    for(i=0;i<cnn->C3->inChannels;i++)
        for(j=0;j<cnn->C3->outChannels;j++)
            for(r=0;r<cnn->C3->mapSize;r++)
                fwrite(cnn->C3->mapData[i][j][r],sizeof(float),cnn->C3->mapSize,fp);

    fwrite(cnn->C3->basicData,sizeof(float),cnn->C3->outChannels,fp);

    for(j=0;j<cnn->C3->outChannels;j++){
        for(r=0;r<cnn->S4->inputHeight;r++){
            fwrite(cnn->C3->v[j][r],sizeof(float),cnn->S4->inputWidth,fp);
        }
        for(r=0;r<cnn->S4->inputHeight;r++){
            fwrite(cnn->C3->d[j][r],sizeof(float),cnn->S4->inputWidth,fp);
        }
        for(r=0;r<cnn->S4->inputHeight;r++){
            fwrite(cnn->C3->y[j][r],sizeof(float),cnn->S4->inputWidth,fp);
        }
    }

    // S4网络
    for(j=0;j<cnn->S4->outChannels;j++){
        for(r=0;r<cnn->S4->inputHeight/cnn->S4->mapSize;r++){
            fwrite(cnn->S4->d[j][r],sizeof(float),cnn->S4->inputWidth/cnn->S4->mapSize,fp);
        }
        for(r=0;r<cnn->S4->inputHeight/cnn->S4->mapSize;r++){
            fwrite(cnn->S4->y[j][r],sizeof(float),cnn->S4->inputWidth/cnn->S4->mapSize,fp);
        }
    }

    // O5输出层
    for(i=0;i<cnn->O5->outputNum;i++)
        fwrite(cnn->O5->wData[i],sizeof(float),cnn->O5->inputNum,fp);
    fwrite(cnn->O5->basicData,sizeof(float),cnn->O5->outputNum,fp);
    fwrite(cnn->O5->v,sizeof(float),cnn->O5->outputNum,fp);
    fwrite(cnn->O5->d,sizeof(float),cnn->O5->outputNum,fp);
    fwrite(cnn->O5->y,sizeof(float),cnn->O5->outputNum,fp);

    fclose(fp);
}


// MAT
float** DCNN::rotate180(float** mat, nSize matSize)// 矩阵翻转180度
{
    int i,c,r;
    int outSizeW=matSize.c;
    int outSizeH=matSize.r;
    float** outputData=(float**)malloc(outSizeH*sizeof(float*));
    for(i=0;i<outSizeH;i++)
        outputData[i]=(float*)malloc(outSizeW*sizeof(float));

    for(r=0;r<outSizeH;r++)
        for(c=0;c<outSizeW;c++)
            outputData[r][c]=mat[outSizeH-r-1][outSizeW-c-1];

    return outputData;
}

// 关于卷积和相关操作的输出选项
// 这里共有三种选择：full、same、valid，分别表示
// full指完全，操作后结果的大小为inSize+(mapSize-1)
// same指同输入相同大小
// valid指完全操作后的大小，一般为inSize-(mapSize-1)大小，其不需要将输入添0扩大。

float** DCNN::correlation(float** map,nSize mapSize,float** inputData,nSize inSize,int type)// 互相关
{
    // 这里的互相关是在后向传播时调用，类似于将Map反转180度再卷积
    // 为了方便计算，这里先将图像扩充一圈
    // 这里的卷积要分成两拨，偶数模板同奇数模板
    int i,j,c,r;
    int halfmapsizew;
    int halfmapsizeh;
    if(mapSize.r%2==0&&mapSize.c%2==0){ // 模板大小为偶数
        halfmapsizew=(mapSize.c)/2; // 卷积模块的半瓣大小
        halfmapsizeh=(mapSize.r)/2;
    }else{
        halfmapsizew=(mapSize.c-1)/2; // 卷积模块的半瓣大小
        halfmapsizeh=(mapSize.r-1)/2;
    }

    // 这里先默认进行full模式的操作，full模式的输出大小为inSize+(mapSize-1)
    int outSizeW=inSize.c+(mapSize.c-1); // 这里的输出扩大一部分
    int outSizeH=inSize.r+(mapSize.r-1);
    float** outputData=(float**)malloc(outSizeH*sizeof(float*)); // 互相关的结果扩大了
    for(i=0;i<outSizeH;i++)
        outputData[i]=(float*)calloc(outSizeW,sizeof(float));

    // 为了方便计算，将inputData扩大一圈
    float** exInputData=matEdgeExpand(inputData,inSize,mapSize.c-1,mapSize.r-1);

    for(j=0;j<outSizeH;j++)
        for(i=0;i<outSizeW;i++)
            for(r=0;r<mapSize.r;r++)
                for(c=0;c<mapSize.c;c++){
                    outputData[j][i]=outputData[j][i]+map[r][c]*exInputData[j+r][i+c];
                }

    for(i=0;i<inSize.r+2*(mapSize.r-1);i++)
        free(exInputData[i]);
    free(exInputData);

    nSize outSize={outSizeW,outSizeH};
    switch(type){ // 根据不同的情况，返回不同的结果
    case full: // 完全大小的情况
        return outputData;
    case same:{
        float** sameres=matEdgeShrink(outputData,outSize,halfmapsizew,halfmapsizeh);
        for(i=0;i<outSize.r;i++)
            free(outputData[i]);
        free(outputData);
        return sameres;
        }
    case _valid:{  //valid 值 为2
        float** validres;
        if(mapSize.r%2==0&&mapSize.c%2==0)
            validres=matEdgeShrink(outputData,outSize,halfmapsizew*2-1,halfmapsizeh*2-1);
        else
            validres=matEdgeShrink(outputData,outSize,halfmapsizew*2,halfmapsizeh*2);
        for(i=0;i<outSize.r;i++)
            free(outputData[i]);
        free(outputData);
        return validres;
        }
    default:
        return outputData;
    }
}

float** DCNN::cov(float** map,nSize mapSize,float** inputData,nSize inSize,int type) // 卷积操作
{
    // 卷积操作可以用旋转180度的特征模板相关来求
    float** flipmap=rotate180(map,mapSize); //旋转180度的特征模板
    float** res=correlation(flipmap,mapSize,inputData,inSize,type);
    int i;
    for(i=0;i<mapSize.r;i++)
        free(flipmap[i]);
    free(flipmap);
    return res;
}

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
float** DCNN::UpSample(float** mat,nSize matSize,int upc,int upr)
{
    int i,j,m,n;
    int c=matSize.c;
    int r=matSize.r;
    float** res=(float**)malloc((r*upr)*sizeof(float*)); // 结果的初始化
    for(i=0;i<(r*upr);i++)
        res[i]=(float*)malloc((c*upc)*sizeof(float));

    for(j=0;j<r*upr;j=j+upr){
        for(i=0;i<c*upc;i=i+upc)// 宽的扩充
            for(m=0;m<upc;m++)
                res[j][i+m]=mat[j/upr][i/upc];

        for(n=1;n<upr;n++)      //  高的扩充
            for(i=0;i<c*upc;i++)
                res[j+n][i]=res[j][i];
    }
    return res;
}

// 给二维矩阵边缘扩大，增加addw大小的0值边
float** DCNN::matEdgeExpand(float** mat,nSize matSize,int addc,int addr)
{ // 向量边缘扩大
    int i,j;
    int c=matSize.c;
    int r=matSize.r;
    float** res=(float**)malloc((r+2*addr)*sizeof(float*)); // 结果的初始化
    for(i=0;i<(r+2*addr);i++)
        res[i]=(float*)malloc((c+2*addc)*sizeof(float));

    for(j=0;j<r+2*addr;j++){
        for(i=0;i<c+2*addc;i++){
            if(j<addr||i<addc||j>=(r+addr)||i>=(c+addc))
                res[j][i]=(float)0.0;
            else
                res[j][i]=mat[j-addr][i-addc]; // 复制原向量的数据
        }
    }
    return res;
}

// 给二维矩阵边缘缩小，擦除shrinkc大小的边
float** DCNN::matEdgeShrink(float** mat,nSize matSize,int shrinkc,int shrinkr)
{ // 向量的缩小，宽缩小addw，高缩小addh
    int i,j;
    int c=matSize.c;
    int r=matSize.r;
    float** res=(float**)malloc((r-2*shrinkr)*sizeof(float*)); // 结果矩阵的初始化
    for(i=0;i<(r-2*shrinkr);i++)
        res[i]=(float*)malloc((c-2*shrinkc)*sizeof(float));


    for(j=0;j<r;j++){
        for(i=0;i<c;i++){
            if(j>=shrinkr&&i>=shrinkc&&j<(r-shrinkr)&&i<(c-shrinkc))
                res[j-shrinkr][i-shrinkc]=mat[j][i]; // 复制原向量的数据
        }
    }
    return res;
}

void DCNN::savemat(float** mat,nSize matSize,const char* filename)
{
    FILE  *fp=NULL;
    fp=fopen(filename,"wb");
    if(fp==NULL)
        printf("write file failed\n");

    int i;
    for(i=0;i<matSize.r;i++)
        fwrite(mat[i],sizeof(float),matSize.c,fp);
    fclose(fp);
}

void DCNN::addmat(float** res, float** mat1, nSize matSize1, float** mat2, nSize matSize2)// 矩阵相加
{
    int i,j;
    if(matSize1.c!=matSize2.c||matSize1.r!=matSize2.r)
        printf("ERROR: Size is not same!");

    for(i=0;i<matSize1.r;i++)
        for(j=0;j<matSize1.c;j++)
            res[i][j]=mat1[i][j]+mat2[i][j];
}

void DCNN::multifactor(float** res, float** mat, nSize matSize, float factor)// 矩阵乘以系数
{
    int i,j;
    for(i=0;i<matSize.r;i++)
        for(j=0;j<matSize.c;j++)
            res[i][j]=mat[i][j]*factor;
}

float DCNN::summat(float** mat,nSize matSize) // 矩阵各元素的和
{
    float sum=0.0;
    int i,j;
    for(i=0;i<matSize.r;i++)
        for(j=0;j<matSize.c;j++)
            sum=sum+mat[i][j];
    return sum;
}


void DCNN::save_Img(ImgArr imgarr,char* filedir) // 将图像数据保存成文件
{
    int img_number=imgarr->ImgNum;

    int i,r;
    for(i=0;i<img_number;i++){
        const char* filename=combine_strings(filedir,combine_strings(intTochar(i),".gray"));
        FILE  *fp=NULL;
        fp=fopen(filename,"wb");
        if(fp==NULL)
            printf("write file failed\n");
        assert(fp);

        for(r=0;r<imgarr->ImgPtr[i].r;r++)
            fwrite(imgarr->ImgPtr[i].ImgData[r],sizeof(float),imgarr->ImgPtr[i].c,fp);

        fclose(fp);
    }
}


char* DCNN::intTochar(int i)// 将数字转换成字符串
{
    int itemp=i;
    int w=0;
    while(itemp>=10){
        itemp=itemp/10;
        w++;
    }
    char* ptr=(char*)malloc((w+2)*sizeof(char));
    ptr[w+1]='\0';
    int r; // 余数
    while(i>=10){
        r=i%10;
        i=i/10;
        ptr[w]=(char)(r+48);
        w--;
    }
    ptr[w]=(char)(i+48);
    return ptr;
}

char * DCNN::combine_strings(char *a, char *b) // 将两个字符串相连
{
    char *ptr;
    int lena=strlen(a),lenb=strlen(b);
    int i,l=0;
    ptr = (char *)malloc((lena+lenb+1) * sizeof(char));
    for(i=0;i<lena;i++)
        ptr[l++]=a[i];
    for(i=0;i<lenb;i++)
        ptr[l++]=b[i];
    ptr[l]='\0';
    return(ptr);
}

//英特尔处理器和其他低端机用户必须翻转头字节。
int DCNN::ReverseInt(int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

ImgArr DCNN::read_Img(const char* filename) // 读入图像
{
    FILE  *fp=NULL;
    fp=fopen(filename,"rb");
    if(fp==NULL)
        printf("open file failed\n");
    assert(fp);

    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;
    //从文件中读取sizeof(magic_number) 个字符到 &magic_number
    fread((char*)&magic_number,sizeof(magic_number),1,fp);
    magic_number = ReverseInt(magic_number);
    //获取训练或测试image的个数number_of_images
    fread((char*)&number_of_images,sizeof(number_of_images),1,fp);
    number_of_images = ReverseInt(number_of_images);
    //获取训练或测试图像的高度Heigh
    fread((char*)&n_rows,sizeof(n_rows),1,fp);
    n_rows = ReverseInt(n_rows);
    //获取训练或测试图像的宽度Width
    fread((char*)&n_cols,sizeof(n_cols),1,fp);
    n_cols = ReverseInt(n_cols);
    //获取第i幅图像，保存到vec中
    int i,r,c;

    // 图像数组的初始化
    ImgArr imgarr=(ImgArr)malloc(sizeof(MinstImgArr));
    imgarr->ImgNum=number_of_images;
    imgarr->ImgPtr=(MinstImg*)malloc(number_of_images*sizeof(MinstImg));

    for(i = 0; i < number_of_images; ++i)
    {
        imgarr->ImgPtr[i].r=n_rows;
        imgarr->ImgPtr[i].c=n_cols;
        imgarr->ImgPtr[i].ImgData=(float**)malloc(n_rows*sizeof(float*));
        for(r = 0; r < n_rows; ++r)
        {
            imgarr->ImgPtr[i].ImgData[r]=(float*)malloc(n_cols*sizeof(float));
            for(c = 0; c < n_cols; ++c)
            {
                unsigned char temp = 0;
                fread((char*) &temp, sizeof(temp),1,fp);
                imgarr->ImgPtr[i].ImgData[r][c]=(float)temp/255.0;
            }
        }
    }

    fclose(fp);
    return imgarr;
}

LabelArr DCNN::read_Lable(const char* filename)// 读入图像
{
    FILE  *fp=NULL;
    fp=fopen(filename,"rb");
    if(fp==NULL)
        printf("open file failed\n");
    assert(fp);

    int magic_number = 0;
    int number_of_labels = 0;
    int label_long = 10;

    //从文件中读取sizeof(magic_number) 个字符到 &magic_number
    fread((char*)&magic_number,sizeof(magic_number),1,fp);
    magic_number = ReverseInt(magic_number);
    //获取训练或测试image的个数number_of_images
    fread((char*)&number_of_labels,sizeof(number_of_labels),1,fp);
    number_of_labels = ReverseInt(number_of_labels);

    int i,l;

    // 图像标记数组的初始化
    LabelArr labarr=(LabelArr)malloc(sizeof(MinstLabelArr));
    labarr->LabelNum=number_of_labels;
    labarr->LabelPtr=(MinstLabel*)malloc(number_of_labels*sizeof(MinstLabel));

    for(i = 0; i < number_of_labels; ++i)
    {
        labarr->LabelPtr[i].l=10;
        labarr->LabelPtr[i].LabelData=(float*)calloc(label_long,sizeof(float));
        unsigned char temp = 0;
        fread((char*) &temp, sizeof(temp),1,fp);
        labarr->LabelPtr[i].LabelData[(int)temp]=1.0;
    }

    fclose(fp);
    return labarr;
}
void DCNN::show()
{

}

void DCNN::setFlag(bool flag)
{
    isStop = flag;
}

void DCNN::setInputSize(nSize input)
{
   inputSize.c = input.c;
   inputSize.r = input.r;
}

void DCNN::setOutSize(int outsize)
{
     outSize = outsize;
}

void DCNN::setModel(int model)
{
    MODEL = model;
}

void DCNN::DCNNThread()
{

      //  QThread::sleep(1);
        //QMessageBox::aboutQt(NULL);

        switch (MODEL) {
        case CNNINIT:
            if(MODEL == CNNINIT){
                cnn=(CNN*)malloc(sizeof(CNN));
                nSize input = {testImg->ImgPtr[0].c,testImg->ImgPtr[0].r};
                setInputSize(input);
                setOutSize(testLabel->LabelPtr[0].l);
                cnnsetup(cnn,inputSize,outSize);

                opts.numepochs=1;
                opts.alpha=1.0;
                trainNum=1000;

                qDebug() << inputSize.c << inputSize.r << outSize;
                qDebug() << "初始化CNN" << testImg->ImgPtr[0].c <<testLabel->LabelPtr[0].l<<
                          opts.numepochs<< opts.alpha << trainNum << QThread::currentThread();
             }
            break;
         case CNNTRAIN:
                if(MODEL == CNNTRAIN)
                { qDebug() << "训练CNN";
                   cnntrain(cnn,trainImg,trainLabel,opts,trainNum);
                  //   cnntrain(cnn,testImg,testLabel,opts,trainNum);
                }
            break;
         case CNNTEST:
            if(CNNTEST == MODEL)
            {
                int testNum=1000;
                float incorrectRatio=0.0;
                incorrectRatio=cnntest(cnn,testImg,testLabel,testNum);
                qDebug()<< "错误率 " <<incorrectRatio;
            }
         case CNNDATA:
            if(CNNDATA == MODEL)
            {


            }
                break;
           default:
                break;
        }

        emit mySignal();
        qDebug() << "子线程函数运行结束" ;
}
