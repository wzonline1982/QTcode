#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QSerialPort>
#include <QList>
#include <QTextEdit>


namespace Ui {
class SerialWidget;
}

class SerialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialWidget(QWidget *parent = nullptr);
    ~SerialWidget();

private:
    Ui::SerialWidget *ui;

private:
    void        initData();             //数据初始化
    void        initTextBrowser();      //文本显示框初始化
    void        initTextEdit();         //发送文本显示框初始化
    void        initPushBtn();          //按钮初始化
    void        initLabel();            //标签初始化
    void        initComboBox();         //下拉菜单初始化
    void        initLayout();           //布局初始化

    void        sendCMD(QString);       //发送命令

private slots:
    void slot_connectBtnCliked();       //连接按钮响应函数
    void slot_disconnectBtnCliked();    //断开连接按钮响应函数
    void slot_sendTextBtnCliked();      //发送文本按钮响应函数
    void slot_clearBtnCliked();         //清屏按钮响应函数
    void slot_serialReadData();         //读取数据响应函数

private:
    QTextBrowser*   m_pTextShow;        ///<文本显示框
    QTextEdit*      m_pTextEdit;        ///<发送文本显示框
    QPushButton*    m_pConnectBtn;      ///<连接按钮
    QPushButton*    m_pDisconnectBtn;   ///<断开连接按钮
    QPushButton*    m_pSendTextBtn;     ///<发送数据按钮
    QPushButton*    m_pClearBtn;        ///<清屏按钮
    QLabel*         m_pPortLabel;       ///<串口号标签
    QLabel*         m_pBaudLabel;       ///<波特率标签
    QLabel*         m_pDataLabel;       ///<数据位标签
    QLabel*         m_pParityLabel;     ///<校验位标签
    QLabel*         m_pStopBitsLabel;   ///<停止位标签
    QLabel*         m_pFlowLabel;       ///<流控标签
    QComboBox*      m_pPortComboBox;    ///<串口号下拉菜单
    QComboBox*      m_pBaudComboBox;    ///<波特率下拉菜单
    QComboBox*      m_pDataComboBox;    ///<数据位下拉菜单
    QComboBox*      m_pParityComboBox;  ///<校验位下拉菜单
    QComboBox*      m_pStopBitsComboBox;///<停止位下拉菜单
    QComboBox*      m_pFlowComboBox;    ///<流控下拉菜单
    QSerialPort*    m_pSerialPort;      ///<串口

    QByteArray      m_sendBytes;        ///<发送命令
    int             m_flag;             ///<串口自动匹配标志
    QString         m_strReceive;       ///<保存收到的数据

    QList<QByteArray>   m_sendMessage;  ///<发送消息队列
    QList<QString>      m_portName;     ///<串口名队列
    QList<int>          m_baudRate;     ///<波特率队列
    QList<QString>      m_dataBits;     ///<数据位队列
    QList<QString>      m_parity;       ///<校验位队列
    QList<QString>      m_stopBits;     ///<停止位队列
    QList<QString>      m_flowControl;  ///<流控队列

};

#endif // SERIALWIDGET_H
