/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pushButton_read;
    QPushButton *pushButton_init;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(713, 429);
        pushButton_read = new QPushButton(Widget);
        pushButton_read->setObjectName(QStringLiteral("pushButton_read"));
        pushButton_read->setGeometry(QRect(50, 30, 75, 23));
        pushButton_init = new QPushButton(Widget);
        pushButton_init->setObjectName(QStringLiteral("pushButton_init"));
        pushButton_init->setGeometry(QRect(140, 30, 75, 23));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(600, 30, 75, 23));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(230, 30, 75, 23));
        pushButton_3 = new QPushButton(Widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(320, 30, 75, 23));
        pushButton_4 = new QPushButton(Widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(410, 30, 75, 23));
        pushButton_5 = new QPushButton(Widget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(510, 30, 75, 23));
        textBrowser = new QTextBrowser(Widget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(30, 140, 471, 261));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        pushButton_read->setText(QApplication::translate("Widget", "\350\257\273\345\217\226Minst", nullptr));
        pushButton_init->setText(QApplication::translate("Widget", "\345\210\235\345\247\213\345\214\226CNN", nullptr));
        pushButton->setText(QApplication::translate("Widget", "\345\205\263\351\227\255CNN", nullptr));
        pushButton_2->setText(QApplication::translate("Widget", "\350\256\255\347\273\203CNN", nullptr));
        pushButton_3->setText(QApplication::translate("Widget", "testCNN", nullptr));
        pushButton_4->setText(QApplication::translate("Widget", "savecnn", nullptr));
        pushButton_5->setText(QApplication::translate("Widget", "loadcnn", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
