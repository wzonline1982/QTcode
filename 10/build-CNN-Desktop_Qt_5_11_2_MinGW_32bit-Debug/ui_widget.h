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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QTextBrowser *textBrowser;
    QLabel *label;
    QLineEdit *lineEdit_insize;
    QLabel *label_2;
    QLineEdit *lineEdit_outsize;
    QLabel *label_3;
    QLineEdit *lineEdit_alpha;
    QLabel *label_4;
    QLineEdit *lineEdit_trainNum;
    QPushButton *pushButton_5;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(592, 354);
        comboBox = new QComboBox(Widget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(20, 20, 411, 22));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(470, 20, 75, 23));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(470, 60, 75, 23));
        pushButton_3 = new QPushButton(Widget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(470, 100, 75, 23));
        pushButton_4 = new QPushButton(Widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(470, 130, 75, 23));
        textBrowser = new QTextBrowser(Widget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 171, 421, 171));
        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 120, 54, 12));
        lineEdit_insize = new QLineEdit(Widget);
        lineEdit_insize->setObjectName(QStringLiteral("lineEdit_insize"));
        lineEdit_insize->setGeometry(QRect(100, 120, 71, 20));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(180, 120, 54, 12));
        lineEdit_outsize = new QLineEdit(Widget);
        lineEdit_outsize->setObjectName(QStringLiteral("lineEdit_outsize"));
        lineEdit_outsize->setGeometry(QRect(240, 120, 113, 20));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 150, 54, 12));
        lineEdit_alpha = new QLineEdit(Widget);
        lineEdit_alpha->setObjectName(QStringLiteral("lineEdit_alpha"));
        lineEdit_alpha->setGeometry(QRect(100, 150, 71, 20));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(180, 150, 54, 12));
        lineEdit_trainNum = new QLineEdit(Widget);
        lineEdit_trainNum->setObjectName(QStringLiteral("lineEdit_trainNum"));
        lineEdit_trainNum->setGeometry(QRect(240, 150, 113, 20));
        pushButton_5 = new QPushButton(Widget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(470, 170, 75, 23));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        pushButton->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        pushButton_2->setText(QApplication::translate("Widget", "\345\257\274\345\205\245\346\225\260\346\215\256", nullptr));
        pushButton_3->setText(QApplication::translate("Widget", "\345\210\235\345\247\213\345\214\226CNN", nullptr));
        pushButton_4->setText(QApplication::translate("Widget", "\350\256\255\347\273\203CNN", nullptr));
        label->setText(QApplication::translate("Widget", "\350\276\223\345\205\245Size", nullptr));
        lineEdit_insize->setText(QApplication::translate("Widget", "1", nullptr));
        label_2->setText(QApplication::translate("Widget", "\350\276\223\345\207\272Size", nullptr));
        label_3->setText(QApplication::translate("Widget", "\345\255\246\344\271\240\351\200\237\347\216\207", nullptr));
        lineEdit_alpha->setText(QApplication::translate("Widget", "1.0", nullptr));
        label_4->setText(QApplication::translate("Widget", "\350\256\255\347\273\203\346\254\241\346\225\260", nullptr));
        lineEdit_trainNum->setText(QApplication::translate("Widget", "1000", nullptr));
        pushButton_5->setText(QApplication::translate("Widget", "\346\265\213\350\257\225\346\225\260\346\215\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
