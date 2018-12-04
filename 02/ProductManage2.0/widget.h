#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QAxObject>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <assert.h>
#include <map>

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

struct HEAD{
    int num;
    int serialctl;
};

namespace Ui {
class Widget;
}

struct Cur
{
    int *noteFeature;
};

struct NoteList
{
    char *listHeader;  //数量
    Cur *cur[100];
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


    HEAD fileHead;
    NoteList *noteList;
    QString fileName;
    QButtonGroup  GroupRadio_2;
    QButtonGroup  GroupRadio_3;

    void openFileCSV(QString strFile,int * noteFeature);
    void openFile(QString strFile,int * noteFeature);

    wchar_t * char2wchar(const char* cchar);


private slots:

    void on_listView_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
