#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QAxObject>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QButtonGroup  GroupRadio_2;
    QButtonGroup  GroupRadio_3;

    void openFile(QString strFile);

private slots:

    void on_listView_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
