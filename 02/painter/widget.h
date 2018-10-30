#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>

#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

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
    QString filename;
    bool isDrag;

protected:
    /**********重写事件响应的虚函数****************/
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //拖动进入事件
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void paintEvent(QPaintEvent *event);
};

#endif // WIDGET_H
