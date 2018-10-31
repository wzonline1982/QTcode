#include "widget.h"
#include "ui_widget.h"
#include "QListView"
#include "QListWidget"
#include "QListWidgetItem"
#include <QStringListModel>
#include <QFile>
#include <QTextCodec>

#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //指定为GBK  combox 初始化
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    //打开文件
    QFile file("./test.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str = codec->toUnicode(line);
            str = str.trimmed();
            qDebug()<< str;
            ui->comboBox->addItem(str);   //combox 初始化
        }
        file.close();
    }
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
    openFile("./123.xls");
    //初始化 listview
    QStringList num;
    num<<QString("0")<<QString("1")<<QString("2")<<QString("3");
    QStringListModel *model = new QStringListModel(num);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编缉

    int row_num =  ui->listView->model()->rowCount();
    qDebug()<< row_num;
    // row_idx为该行索引序号, column_idx为该列索引序号,两者都以0开始
    QString str = ui->listView->model()->index(0, 2).data().toString();
    qDebug()<< str;
    //---------------------------------------

    //radioButton
    //分组radiobutton
    GroupRadio_2.addButton(ui->radioButton);
    GroupRadio_2.addButton(ui->radioButton_2);

    GroupRadio_3.addButton(ui->radioButton_3);
    GroupRadio_3.addButton(ui->radioButton_4);

    ui->radioButton->setChecked(true);
    int select = GroupRadio_2.checkedId();
    qDebug()<< select;
    //--------------------------------
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_listView_clicked(const QModelIndex &index)
{
    qDebug()<<index.row();
    QModelIndex INDEX=ui->listView->currentIndex();
    qDebug()<<INDEX;
//    ui->listView->model()->removeRow();
//    model->insertRows(c_rowlistview-1,1)；//插入一行
//    model->removeRow(c_rowlistview+1);//删除一行
//    model->setData((model->index(c_rowlistview-1,0)),model->index(c_rowlistview+1,0).data().toString())//赋值，注意index（）第二个参数设置成0，否则赋值不成功

}

//添加 button
void Widget::on_pushButton_clicked()
{
    qDebug()<<ui->comboBox->currentIndex()<<ui->comboBox->currentText(); //获取当前索引和文本
}

//删除 button
void Widget::on_pushButton_2_clicked()
{
    QModelIndex INDEX=ui->listView->currentIndex();
    qDebug()<<INDEX;
}


//radio 冠字号 开启
void Widget::on_radioButton_clicked()
{
    int select = GroupRadio_2.checkedId();
    switch (select) {
        case -2:
             qDebug()<< "冠字号开启";
            break;
        case -3:
             qDebug()<< "冠字号关闭";
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
            break;
        case -3:
             qDebug()<< "冠字号关闭";
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


void Widget::openFile(QString strFile)
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
        int intRowStart = usedrange->property("Row").toInt();
        int intColStart = usedrange->property("Column").toInt();
        int intCols = columns->property("Count").toInt();
        int intRows = rows->property("Count").toInt();
        QAxObject * cell;
        for (int i = intRowStart; i < intRowStart + intRows; i++)
        {
            for (int j = intColStart; j < intColStart + intCols; j++)
            {
                cell = excel->querySubObject("Cells(Int, Int)", i, j );
                QVariant cellValue = cell->dynamicCall("value");
                qDebug()<< cellValue ;
            }
        }
        excel->setProperty("DisplayAlerts", 0);
        workbook->dynamicCall("Save(void)");
        workbook->dynamicCall("Close (Boolean)", false);
        excel->setProperty("DisplayAlerts",1);

        delete excel;
        qDebug() << "100" ;
}
