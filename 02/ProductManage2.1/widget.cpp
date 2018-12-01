#include "widget.h"
#include "ui_widget.h"
#include "QListView"
#include "QListWidget"
#include "QListWidgetItem"
#include <QStringListModel>
#include <QFile>
#include <QTextCodec>
#include <stdlib.h>
#include <stdio.h>

#include <windows.h>
#include <iostream>
#include <cstring>
#include <string>
#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化combox
    QDir dir = QCoreApplication::applicationDirPath()+ "/data/";  //当前目录下的文件夹data下的文件
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
          dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.suffix() == "xls" || fileInfo.suffix() == "csv" )  //筛选后缀为xls的文件导入combox
        {
           ui->comboBox->addItem(fileInfo.baseName());
        }
//             qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
//             qDebug()<<fileInfo.suffix();
    }
//---------------------------------------------------------------------------

    //指定为GBK  combox 初始化
//    QTextCodec *codec = QTextCodec::codecForName("GBK");
//    //打开文件
//    QFile file("./test.txt");
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        while (!file.atEnd())
//        {
//            QByteArray line = file.readLine();
//            QString str = codec->toUnicode(line);
//            str = str.trimmed();
//            qDebug()<< str;
//            ui->comboBox->addItem(str);   //combox 初始化
//        }
//        file.close();
//    }
    //------------------------------------
/*
    //combox 初始化
    QStringList list;
    list << "A" << "B" << "C" << "D" << "E";
    ui->comboBox->addItems(list);
    ui->comboBox->addItem("F");
    int icom = 0;
    icom = ui->comboBox->count();
    //ui->comboBox->setMaxCount(icom);
    ui->comboBox->addItem("G");
*/
 //   openFile("E:/qt_work/build-ProductManage-Desktop_Qt_5_11_2_MinGW_32bit-Debug/123.xls");
    //fileName = QCoreApplication::applicationDirPath();
   // fileName = fileName+ "/data/" +"123"+".xls";

    //初始化 listview
    QStringList num;
 //   num<<QString(" "); //初始化一行，必需有数据
    QStringListModel *model = new QStringListModel(num);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编缉

    /* listview 数据操作示例*/
  //  qDebug()<< "打印初始化行数量" << ui->listView->model()->rowCount();
    // row_idx为该行索引序号, column_idx为该列索引序号,两者都以0开始
    //----------------------------------row_idx -- column_idx
 //   qDebug()<<"打印选定内容区域" << ui->listView->model()->index(1, 0).data().toString();
 //  ui->listView->model()->removeRow(1.0);  //删除row.col 先行再列 0 起始
 //   ui->listView->model()->insertRow(0);
 //   ui->listView->model()->setData(ui->listView->model()->index(0,0),"33333333");
//   QVariant data = ui->listView->model()->data(ui->listView->model()->index(0,0));
 //   qDebug() << data.toInt() ;

    //---------------------------------------

    //radioButton
    //分组radiobutton
    GroupRadio_2.addButton(ui->radioButton);
    GroupRadio_2.addButton(ui->radioButton_2);

    GroupRadio_3.addButton(ui->radioButton_3);
    GroupRadio_3.addButton(ui->radioButton_4);

    ui->radioButton->setChecked(true);
    fileHead.serialctl = 1;

    //初始化progressBar
    ui->progressBar->setValue(0);
    //--------------------------------

    /*初始化 noteList */
    noteList = (NoteList*) malloc(sizeof(NoteList));
    noteList->listHeader =(char*)  malloc(sizeof(char)*100);
    for (int i =0; i<100;i++)
    {
        noteList->cur[i] = (Cur*)  malloc(sizeof(Cur));
        noteList->cur[i]->noteFeature = (int*) calloc(100,sizeof(int));
    }
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_listView_clicked(const QModelIndex &index)
{
    //ui->comboBox->Items[2].ToString();
      qDebug()<<"当前行序号"<<index.row();
      int indexRow = index.row();
      QVariant data = ui->listView->model()->data(ui->listView->model()->index(indexRow,0)); //取当前行0列数据
      QString viewText = data.toString(); //转字符串

      int i;
      for( i=0;i<100;i++)
      {

      }

//    QModelIndex INDEX=ui->listView->currentIndex();
 //   qDebug()<<INDEX;
 //   ui->listView->model()->removeRow();
//    model->insertRows(c_rowlistview-1,1)；//插入一行
//    model->removeRow(c_rowlistview+1);//删除一行
//    model->setData((model->index(c_rowlistview-1,0)),model->index(c_rowlistview+1,0).data().toString())//赋值，注意index（）第二个参数设置成0，否则赋值不成功

}

//添加 button
void Widget::on_pushButton_clicked()
{
    //qDebug()<<QCoreApplication::applicationDirPath(); //打印当前路径
    //qDebug()<<"当前combox"<<ui->comboBox->currentIndex()<<ui->comboBox->currentText(); //获取当前索引和文本
    int comboxIndex = ui->comboBox->currentIndex();  //当前序号
    int rowCount = ui->listView->model()->rowCount(); //行数
    QString comboxText = ui->comboBox->currentText(); //当前内容
    int row = ui->listView->currentIndex().row();
    qDebug()<<"combox序号"<< comboxIndex;
    qDebug()<<"row"<< row;
    qDebug()<<"rowCount"<< rowCount;

    QString ListStr;
    int flag = 0;
    //查询是否有重复项
    for (int i = 0; i < rowCount;i++ )  //rowCount 最小值1
    {
       ListStr = ui->listView->model()->data(ui->listView->model()->index(i,0)).toString();
       if(comboxText == ListStr)flag = 1; //表示重复项
    }

    if (flag == 0) //没有重复项， 加载数据
    {
        //listview 插入数据
//        int row = ui->listView->currentIndex().row();   //获取当前行
//        if(row == -1)row =0;
        ui->listView->model()->insertRows(rowCount,1);       //当前行插入一行 row 序号
        QModelIndex index = ui->listView->model()->index(rowCount,0);
        ui->listView->model()->setData(index,comboxText); //按索引 修改内容

        //打开文件，读取内容
        fileName = QCoreApplication::applicationDirPath();
        fileName = fileName+ "/data/" +comboxText+".csv";
      //  qDebug()<< fileName;

//        QByteArray ba = comboxText.toLocal8Bit();  // 文件名存入cur->name toLocal8Bit 支持中文
//        noteList->cur[comboxIndex]->name = ba.data();  //按combox 序号
//        noteList->cur[comboxIndex]->row = row;  //listview 行号
        //方式1
     //   openFile(fileName,noteList->cur[comboxIndex]->noteFeature); //读取数据
        //方式2
        openFileCSV(fileName,noteList->cur[comboxIndex]->noteFeature);
    //  fileName.append();
    }
}

//删除 button
void Widget::on_pushButton_2_clicked()
{
    QModelIndex index=ui->listView->currentIndex();
    ui->listView->model()->removeRow(index.row());
}


//radio 冠字号 开启
void Widget::on_radioButton_clicked()
{
    int select = GroupRadio_2.checkedId();
    switch (select) {
        case -2:
             fileHead.serialctl = 1;
             qDebug()<< "冠字号开启";
            break;
        case -3:
             qDebug()<< "冠字号关闭";
            fileHead.serialctl = 0;
            break;
        default:
            break;
    }
}

//radio 冠字号 关闭
void Widget::on_radioButton_2_clicked()
{
    int select = GroupRadio_2.checkedId();
    switch (select) {
        case -2:
             qDebug()<< "冠字号开启";
            fileHead.serialctl = 1;
            break;
        case -3:
             qDebug()<< "冠字号关闭";
            fileHead.serialctl = 0;
            break;
        default:
            break;
    }
}

//红外开启
void Widget::on_radioButton_3_clicked()
{
    int select = GroupRadio_3.checkedId();
    switch (select) {
        case -2:
             qDebug()<< "红外开启";
            break;
        case -3:
             qDebug()<< "红外关闭";
            break;
        default:
            break;
    }
}

//红外关闭
void Widget::on_radioButton_4_clicked()
{
    int select = GroupRadio_3.checkedId();
    switch (select) {
        case -2:
             qDebug()<< "红外开启";
            break;
        case -3:
             qDebug()<< "红外关闭";
            break;
        default:
            break;
    }
}

//磁性个数设置
void Widget::on_spinBox_valueChanged(int arg1)
{
    qDebug() << arg1;
}


void Widget::openFile(QString strFile,int * noteFeature)
{
        QAxObject* excel = new QAxObject("Excel.Application");
        excel->setProperty("Visible", false);
        QAxObject* workbooks = excel->querySubObject("WorkBooks");
        workbooks->dynamicCall("Open (const QString&)", QString(strFile)); //filename
        QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
        QAxObject* worksheets = workbook->querySubObject("WorkSheets");
        QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1); //worksheet number
        QAxObject* usedrange = worksheet->querySubObject("UsedRange");
        QAxObject* rows = usedrange->querySubObject("Rows");
        QAxObject* columns = usedrange->querySubObject("Columns");
        int intRowStart = usedrange->property("Row").toInt();  //起始行
        int intColStart = usedrange->property("Column").toInt(); //起始列
        int intCols = columns->property("Count").toInt();  //获取列数
        int intRows = rows->property("Count").toInt();  //获取行数

        //指定行列数
        int *data;
        int k=0;
        data = noteFeature;  //获取noteFeature地址
        intRows = 10;
        intCols = 10;

        QAxObject * cell;
        for (int i = intRowStart; i < intRowStart + intRows; i++)
        {
            for (int j = intColStart; j < intColStart + intCols; j++)
            {
                //操作单元格（第i行第j列）
                cell = excel->querySubObject("Cells(Int, Int)", j, i*2 );
                QVariant cellValue = cell->dynamicCall("value");

               if (cellValue.type() == 6)
               {

                   *(data+k) = cellValue.toInt();
                   qDebug() << "int 型" << data+k;
               }
               else if(cellValue.type() == 10)
               {
                  // QString ss = cellValue.value<QString>();
                    QString str1 = cellValue.toString();
                    QByteArray ba = str1.toLatin1();
                    char *c_str2 = ba.data();
                    *(data+k) = *c_str2; // 取第一个字符

                   qDebug() << "string 型" << data+k;
               }
//                data[k] =  cellValue.type();
//                qDebug()<< cellValue.type();
//                qDebug()<< data[k];
                k++;
            }
        }
        excel->setProperty("DisplayAlerts", 0);
        workbook->dynamicCall("Save(void)");
        workbook->dynamicCall("Close (Boolean)", false);
        excel->setProperty("DisplayAlerts",1);

        delete excel;

        qDebug() << "intRowStart" << intRowStart ;
        qDebug() << "intColStart" << intColStart ;
        qDebug() << "intRows" << intRows ;
        qDebug() << "intCols" << intCols ;
}

void Widget::openFileCSV(QString strFile,int * noteFeature)
{
    FILE *fp = NULL;
    char *line,*record,*filename;
    char buffer[1024];

    wchar_t szBuf[1024];  //QString 路径 转 wchar_t
    wcscpy_s(reinterpret_cast<wchar_t*>(szBuf),
        sizeof(szBuf) / sizeof(wchar_t),
        reinterpret_cast<const wchar_t*>(strFile.utf16()));

//    QByteArray arr = strFile.toUtf8(); //中文转码
//    string str = arr.data();
//    const char* fileNameP = str.data();

////    QByteArray ba = strFile.toLatin1(); // Qstring转char
////    filename=ba.data();
//    const wchar_t* strL =char2wchar(fileNameP);
    qDebug() << strFile;
    int *data;
    int k = 0;
    data = noteFeature;  //获取noteFeature地址

    if((fp = _wfopen( szBuf,L"r")) != NULL)  //如果含中文要用 _wfopen 打开文件
    {
            qDebug()<< "读取";
            fseek(fp, 0, SEEK_SET);  //定位到第二行，每个英文字符大小为1
            char delims[] = ",";
            char *result = NULL;
            int j = 0;

            // fgets 读取一行以 \n 结束
            while ((line = fgets(buffer, sizeof(buffer), fp))!=NULL)//当没有读取到文件末尾时循环继续
            {
                record = strtok(line, ","); //取份隔符前的数据
                while (record != NULL)//读取每一行的数据
                {
                    QString strg = QString::fromLocal8Bit(record);
//                    qDebug()<< strg.toInt() ;
                    //**** 保存数据
                    if(j%2 == 1)  //取偶数位
                    {
//                        *(data+k) =  strg.toInt() ;
                        qDebug()<< *(data+k) ;
//                        QString str1 = cellValue.toString();
                        QByteArray ba = strg.toLatin1();
                        char *c_str2 = ba.data();
                        *(data+k) = *c_str2; // 取第一个字符
                        k++;
                    }
                    //****
                    record = strtok(NULL, ",");
                    j++;
                    if (j == 20)  //只需读取前20列
                        break;
                }
                j = 0;
                qDebug()<< "行数" ;
            }
            fclose(fp);
            fp = NULL;
    }
}



//生成打包文件
void Widget::on_pushButton_3_clicked()
{
    int rowCount = ui->listView->model()->rowCount(); // 行数
    FILE *fd;
    fileName = QCoreApplication::applicationDirPath();
    fileName = fileName+ "/data/" +"feature"+".bin";
    char*  ch;
    QByteArray ba = fileName.toLatin1();
    ch=ba.data();

    fd = fopen(ch,"wb");
    //fseek(fd,offset,SEEK_SET);
    //fwrite( a, sizeof(a), 1, fd);

    fileHead.num = rowCount;
    fwrite(&fileHead,sizeof(HEAD),1,fd);  //写头信息

    QVariant data;
    QString viewText;
    for (int j = 0; j < rowCount; j++)
    {
        data = ui->listView->model()->data(ui->listView->model()->index(j,0)); //取当前行0列数据
        viewText = data.toString(); //转字符串
        for (int i = 0; i < 1000; i++)  //减1 为实际行数。
        {
            if(viewText == ui->comboBox->itemText(i))
            {
                //查询到写文件
                fseek(fd,512*j+16,SEEK_SET); //头信息16字节，货币信息128字节
                //noteList->cur[i]->noteFeature
                noteList->cur[i]->noteFeature[0] = j+1;
                fwrite(noteList->cur[i]->noteFeature,400,1,fd);  //写货币信息
                break;
            }
        }
        double bar = (double)j/(double)rowCount*100;

        ui->progressBar->setValue((int)bar);

    }
    ui->progressBar->setValue(100);

    fclose(fd);  //关闭文件

    /*//int rowCount = ui->listView->model()->rowCount();
    fileName = QCoreApplication::applicationDirPath();
    fileName = fileName+ "/data/" +"feature"+".bin";
    QFile writeFile(fileName);    //设置文件名
    writeFile.open(QIODevice::WriteOnly);  //打开文件
    QDataStream out(&writeFile);

    int j=0;
    for (int i = 0; i < 100; i++)  //减1 为实际行数。
    {
        if(noteList->cur[i]->row < 100)
        {
          out.writeRawData((char*)noteList->cur[i]->noteFeature, sizeof(int)*100);
          j++;
          qDebug() << j;
        }
    }
    writeFile.close();
    */
}


wchar_t * Widget::char2wchar(const char* cchar)
{
    wchar_t *m_wchar;
    int len = MultiByteToWideChar( CP_ACP ,0,cchar ,strlen( cchar), NULL,0);
    m_wchar= new wchar_t[len+1];
    MultiByteToWideChar( CP_ACP ,0,cchar,strlen( cchar),m_wchar,len);
    m_wchar[len]= '\0' ;
    return m_wchar;
}


