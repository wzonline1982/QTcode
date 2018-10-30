#ifndef SUBWIN_H
#define SUBWIN_H

#include <QWidget>

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QPushButton>
#include <QUrl>

#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>

#include <QDialog>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QScreen>
#include <QGuiApplication>

class Subwin : public QWidget
{
    Q_OBJECT


public:
    explicit Subwin(QWidget *parent = nullptr);

    QPixmap pix; //全局变量保存绘制的图片
    QPixmap paintEventPix; //辅助画布

    QString filename;

signals:
    void subSignal(QString);

public slots:



protected:

    //拖动进入事件
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void paintEvent(QPaintEvent *event);

};

#endif // SUBWIN_H
