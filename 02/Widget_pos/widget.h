#ifndef WIDGET_H
#define WIDGET_H

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

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QPixmap pix; //全局变量保存绘制的图片
    QPoint lastPoint; //保存鼠标上一个值
    QPoint endPoint; //保存鼠标上一个值

    qreal scale;
    QPixmap tempPix; //辅助画布
    bool isDrawing;  //标志是否正在绘图


protected:
    /**********重写事件响应的虚函数****************/
    void mouseMoveEvent(QMouseEvent *m);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //拖动进入事件
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();
};

#endif // WIDGET_H
