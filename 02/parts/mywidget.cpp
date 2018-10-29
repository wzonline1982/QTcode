#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_myButton_clicked()
{
    QString directory =
            QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("Save path"),QDir::currentPath()));
    if(!directory.isEmpty())
    {
        if(ui->directorycomboBox->findText(directory)==-1)
            ui->directorycomboBox->addItem(directory);
        ui->directorycomboBox->setCurrentIndex(ui->directorycomboBox->findText(directory));
    }
   //    qDebug() << directory;

}
