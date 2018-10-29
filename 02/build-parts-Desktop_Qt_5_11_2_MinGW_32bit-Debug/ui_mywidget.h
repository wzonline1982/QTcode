/********************************************************************************
** Form generated from reading UI file 'mywidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWIDGET_H
#define UI_MYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyWidget
{
public:
    QPushButton *myButton;
    QLabel *label;
    QGraphicsView *graphicsView;
    QComboBox *directorycomboBox;

    void setupUi(QWidget *MyWidget)
    {
        if (MyWidget->objectName().isEmpty())
            MyWidget->setObjectName(QStringLiteral("MyWidget"));
        MyWidget->resize(692, 325);
        myButton = new QPushButton(MyWidget);
        myButton->setObjectName(QStringLiteral("myButton"));
        myButton->setGeometry(QRect(320, 10, 75, 23));
        label = new QLabel(MyWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 54, 12));
        graphicsView = new QGraphicsView(MyWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 50, 391, 261));
        directorycomboBox = new QComboBox(MyWidget);
        directorycomboBox->setObjectName(QStringLiteral("directorycomboBox"));
        directorycomboBox->setGeometry(QRect(40, 10, 261, 22));

        retranslateUi(MyWidget);

        QMetaObject::connectSlotsByName(MyWidget);
    } // setupUi

    void retranslateUi(QWidget *MyWidget)
    {
        MyWidget->setWindowTitle(QApplication::translate("MyWidget", "MyWidget", nullptr));
        myButton->setText(QApplication::translate("MyWidget", "open", nullptr));
        label->setText(QApplication::translate("MyWidget", "\345\234\260\345\235\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyWidget: public Ui_MyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWIDGET_H
