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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pushButton_read;
    QPushButton *pushButton_init;
    QPushButton *pushButton;

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
        pushButton->setGeometry(QRect(370, 30, 75, 23));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        pushButton_read->setText(QApplication::translate("Widget", "\350\257\273\345\217\226Minst", nullptr));
        pushButton_init->setText(QApplication::translate("Widget", "\345\210\235\345\247\213\345\214\226CNN", nullptr));
        pushButton->setText(QApplication::translate("Widget", "\345\205\263\351\227\255CNN", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
