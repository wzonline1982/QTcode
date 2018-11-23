#include "serialwidget.h"
#include "ui_serialwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>

SerialWidget::SerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWidget)
{
    ui->setupUi(this);

    initData();
    initTextBrowser();
    initTextEdit();
    initPushBtn();
    initLabel();
    initComboBox();
    initLayout();
}

SerialWidget::~SerialWidget()
{
    delete ui;

    m_flag = 0;
}


void SerialWidget::initData()
{
    m_sendBytes = "";
    m_flag = 0;
    m_pSerialPort = nullptr;

    m_baudRate.push_back(1200);
    m_baudRate.push_back(2400);
    m_baudRate.push_back(4800);
    m_baudRate.push_back(9600);
    m_baudRate.push_back(19200);
    m_baudRate.push_back(38400);
    m_baudRate.push_back(57600);
    m_baudRate.push_back(115200);
    m_baudRate.push_back(256000);
    m_baudRate.push_back(500000);

    m_dataBits.push_back(tr("5"));
    m_dataBits.push_back(tr("6"));
    m_dataBits.push_back(tr("7"));
    m_dataBits.push_back(tr("8"));
    m_dataBits.push_back(tr("未知"));

    m_parity.push_back(tr("无"));
    m_parity.push_back(tr("奇"));
    m_parity.push_back(tr("偶"));
    m_parity.push_back(tr("SpaceParity"));
    m_parity.push_back(tr("MarkParity"));
    m_parity.push_back(tr("未知"));

    m_stopBits.push_back(tr("1"));
    m_stopBits.push_back(tr("1.5"));
    m_stopBits.push_back(tr("2"));
    m_stopBits.push_back(tr("未知"));

    m_flowControl.push_back(tr("无"));
    m_flowControl.push_back(tr("RTS/CTS(硬件流控制)"));
    m_flowControl.push_back(tr("XON/XOFF(软件流控制)"));
    m_flowControl.push_back(tr("未知"));

}

void SerialWidget::initTextBrowser()
{
    m_pTextShow = new QTextBrowser(this);
}

void SerialWidget::initTextEdit()
{
    m_pTextEdit = new QTextEdit(this);
    m_pTextEdit->setMaximumHeight(50);
}

void SerialWidget::initPushBtn()
{
    m_pConnectBtn = new QPushButton(this);
    m_pConnectBtn->setText(tr("开始连接"));
    m_pConnectBtn->setEnabled(true);
    connect(m_pConnectBtn, &QPushButton::clicked, this, &SerialWidget::slot_connectBtnCliked);

    m_pDisconnectBtn = new QPushButton(this);
    m_pDisconnectBtn->setText(tr("断开连接"));
    m_pDisconnectBtn->setEnabled(false);
    connect(m_pDisconnectBtn, &QPushButton::clicked, this, &SerialWidget::slot_disconnectBtnCliked);

    m_pSendTextBtn = new QPushButton(this);
    m_pSendTextBtn->setText(tr("发送"));
    m_pSendTextBtn->setEnabled(false);
    connect(m_pSendTextBtn, &QPushButton::clicked, this, &SerialWidget::slot_sendTextBtnCliked);

    m_pClearBtn = new QPushButton(this);
    m_pClearBtn->setText(tr("清屏"));
    m_pClearBtn->setEnabled(false);
    connect(m_pClearBtn, &QPushButton::clicked, this, &SerialWidget::slot_clearBtnCliked);
}

void SerialWidget::initLabel()
{
    m_pPortLabel = new QLabel(this);
    m_pPortLabel->setText(tr("串口号:"));

    m_pBaudLabel = new QLabel(this);
    m_pBaudLabel->setText(tr("波特率:"));

    m_pDataLabel = new QLabel(this);
    m_pDataLabel->setText(tr("数据位:"));

    m_pParityLabel = new QLabel(this);
    m_pParityLabel->setText(tr("校验位:"));

    m_pStopBitsLabel = new QLabel(this);
    m_pStopBitsLabel->setText(tr("停止位:"));

    m_pFlowLabel = new QLabel(this);
    m_pFlowLabel->setText(tr("流控:"));
}

void SerialWidget::initComboBox()
{
    m_pPortComboBox = new QComboBox(this);
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            m_portName.push_back(serial.portName());
            serial.close();
        }
    }

    for (int i = 0; i < m_portName.size(); ++i)
    {
        m_pPortComboBox->addItem(m_portName.at(i));
    }

    m_pBaudComboBox = new QComboBox(this);
    for (int i = 0; i < m_baudRate.size(); ++i)
    {
        m_pBaudComboBox->addItem(QString::number(m_baudRate.at(i)));
    }

    m_pDataComboBox = new QComboBox(this);
    for (int i = 0; i < m_dataBits.size(); ++i)
    {
        m_pDataComboBox->addItem(m_dataBits.at(i));
    }

    m_pParityComboBox = new QComboBox(this);
    for (int i = 0; i < m_parity.size(); ++i)
    {
        m_pParityComboBox->addItem(m_parity.at(i));
    }

    m_pStopBitsComboBox = new QComboBox(this);
    for (int i = 0; i < m_stopBits.size(); ++i)
    {
        m_pStopBitsComboBox->addItem(m_stopBits.at(i));
    }

    m_pFlowComboBox = new QComboBox(this);
    for (int i = 0; i < m_flowControl.size(); ++i)
    {
        m_pFlowComboBox->addItem(m_flowControl.at(i));
    }
}

void SerialWidget::initLayout()
{
    QHBoxLayout* pComLayout = new QHBoxLayout;
    pComLayout->addWidget(m_pPortLabel);
    pComLayout->addWidget(m_pPortComboBox);

    QHBoxLayout* pBaudLayout = new QHBoxLayout;
    pBaudLayout->addWidget(m_pBaudLabel);
    pBaudLayout->addWidget(m_pBaudComboBox);

    QHBoxLayout* pDataLayout = new QHBoxLayout;
    pDataLayout->addWidget(m_pDataLabel);
    pDataLayout->addWidget(m_pDataComboBox);

    QHBoxLayout* pParityLayout = new QHBoxLayout;
    pParityLayout->addWidget(m_pParityLabel);
    pParityLayout->addWidget(m_pParityComboBox);

    QHBoxLayout* pStopBitsLayout = new QHBoxLayout;
    pStopBitsLayout->addWidget(m_pStopBitsLabel);
    pStopBitsLayout->addWidget(m_pStopBitsComboBox);

    QHBoxLayout* pFlowControlLayout = new QHBoxLayout;
    pFlowControlLayout->addWidget(m_pFlowLabel);
    pFlowControlLayout->addWidget(m_pFlowComboBox);

    QVBoxLayout* pFuncLayout = new QVBoxLayout;
    pFuncLayout->addLayout(pComLayout);
    pFuncLayout->addLayout(pBaudLayout);
    pFuncLayout->addLayout(pDataLayout);
    pFuncLayout->addLayout(pParityLayout);
    pFuncLayout->addLayout(pStopBitsLayout);
    pFuncLayout->addLayout(pFlowControlLayout);
    pFuncLayout->addWidget(m_pConnectBtn);
    pFuncLayout->addWidget(m_pDisconnectBtn);

    QHBoxLayout* pReceiveLayout = new QHBoxLayout;
    pReceiveLayout->addWidget(m_pTextShow);
    pReceiveLayout->addLayout(pFuncLayout);

    QVBoxLayout* pClearLayout = new QVBoxLayout;
    pClearLayout->addWidget(m_pSendTextBtn);
    pClearLayout->addWidget(m_pClearBtn);

    QHBoxLayout* pSendLayout = new QHBoxLayout;
    pSendLayout->addWidget(m_pTextEdit);
    pSendLayout->addLayout(pClearLayout);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->addLayout(pReceiveLayout);
    pMainLayout->addLayout(pSendLayout);

    this->setLayout(pMainLayout);
}

void SerialWidget::sendCMD(QString strMessage)
{
    QByteArray sendBytes;
    sendBytes.append(strMessage);

    m_pSerialPort->write(sendBytes);
}

void SerialWidget::slot_connectBtnCliked()
{
    m_pSerialPort = new QSerialPort(this);
    m_pSerialPort->setPortName(m_pPortComboBox->currentText());
    m_pSerialPort->setBaudRate(m_pBaudComboBox->currentText().toInt());
    switch (m_pDataComboBox->currentIndex())
    {
    case 0:
        m_pSerialPort->setDataBits(QSerialPort::Data5);
        break;
    case 1:
        m_pSerialPort->setDataBits(QSerialPort::Data6);
        break;
    case 2:
        m_pSerialPort->setDataBits(QSerialPort::Data7);
        break;
    case 3:
        m_pSerialPort->setDataBits(QSerialPort::Data8);
        break;
    case 4:
        m_pSerialPort->setDataBits(QSerialPort::UnknownDataBits);
        break;
    default:
        break;
    }

    switch (m_pParityComboBox->currentIndex())
    {
    case 0:
        m_pSerialPort->setParity(QSerialPort::NoParity);
        break;
    case 1:
        m_pSerialPort->setParity(QSerialPort::OddParity);
        break;
    case 2:
        m_pSerialPort->setParity(QSerialPort::EvenParity);
        break;
    case 3:
        m_pSerialPort->setParity(QSerialPort::SpaceParity);
        break;
    case 4:
        m_pSerialPort->setParity(QSerialPort::MarkParity);
        break;
    case 5:
        m_pSerialPort->setParity(QSerialPort::UnknownParity);
        break;
    default:
        break;
    }

    switch (m_pStopBitsComboBox->currentIndex())
    {
    case 0:
        m_pSerialPort->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        m_pSerialPort->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        m_pSerialPort->setStopBits(QSerialPort::TwoStop);
        break;
    case 3:
        m_pSerialPort->setStopBits(QSerialPort::UnknownStopBits);
        break;
    default:
        break;
    }

    switch (m_pFlowComboBox->currentIndex())
    {
    case 0:
        m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
        break;
    case 1:
        m_pSerialPort->setFlowControl(QSerialPort::HardwareControl);
        break;
    case 2:
        m_pSerialPort->setFlowControl(QSerialPort::SoftwareControl);
        break;
    case 3:
        m_pSerialPort->setFlowControl(QSerialPort::UnknownFlowControl);
        break;
    default:
        break;
    }

    if (!m_pSerialPort->open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this, "提示", "串口连接失败");
    }
    else
    {
        QMessageBox::information(this, "提示", m_pPortComboBox->currentText() + tr("连接成功"));
        m_pConnectBtn->setEnabled(false);
        m_pDisconnectBtn->setEnabled(true);
        m_pSendTextBtn->setEnabled(true);
    }
    connect(m_pSerialPort, &QSerialPort::readyRead, this, &SerialWidget::slot_serialReadData);
}

void SerialWidget::slot_disconnectBtnCliked()
{
    m_pSerialPort->clear();
    m_pSerialPort->close();
    delete m_pSerialPort;
    m_pSerialPort = nullptr;
    m_flag = 0;

    m_pConnectBtn->setEnabled(true);
    m_pDisconnectBtn->setEnabled(false);
    m_pSendTextBtn->setEnabled(false);
}

void SerialWidget::slot_sendTextBtnCliked()
{
    QString strSendMessage = m_pTextEdit->toPlainText();
    sendCMD(strSendMessage);
}

void SerialWidget::slot_clearBtnCliked()
{
    if(!m_pTextShow->toPlainText().isEmpty())
    {
        m_pTextShow->clear();
        m_pClearBtn->setEnabled(false);
    }
}

void SerialWidget::slot_serialReadData()
{
    QByteArray receiveDate;
    receiveDate = m_pSerialPort->readAll();
    if (!receiveDate.isEmpty())
    {
        QString strBuf = m_pTextShow->toPlainText();
        strBuf += receiveDate;
        m_pTextShow->clear();
        m_pTextShow->append(strBuf);
    }
    m_pClearBtn->setEnabled(true);
    receiveDate.clear();
}
