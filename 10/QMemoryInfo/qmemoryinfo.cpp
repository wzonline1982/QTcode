#include "qmemoryinfo.h"
#include "ui_qmemoryinfo.h"

#include <QGridLayout>
#include <windows.h>
#include <sysinfoapi.h>

QMemoryInfo::QMemoryInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMemoryInfo)
{
    ui->setupUi(this);

    m_memoryUsedLabel = new QLabel(this);
    m_totalPhysLabel = new QLabel(this);
    m_availPhysLabel = new QLabel(this);
    m_totalPageFileLabel = new QLabel(this);
    m_availPageFileLabel = new QLabel(this);
    m_totalVirtualLabel = new QLabel(this);
    m_availVirtualLabel = new QLabel(this);
    m_availExVirtualLabel = new QLabel(this);

    m_memoryUsedEdit = new QLineEdit(this);
    m_totalPhysEdit = new QLineEdit(this);
    m_availPhysEdit = new QLineEdit(this);
    m_totalPageFileEdit = new QLineEdit(this);
    m_availPageFileEdit = new QLineEdit(this);
    m_totalVirtualEdit = new QLineEdit(this);
    m_availVirtualEdit = new QLineEdit(this);
    m_availExVirtualEdit = new QLineEdit(this);

    m_memoryUsedLabel->setText(QObject::tr("内存使用率:"));
    m_totalPhysLabel->setText("总物理内存:");
    m_availPhysLabel->setText("可用物理内存:");
    m_totalPageFileLabel->setText("系统提交总数:");
    m_availPageFileLabel->setText("系统提交数:");
    m_totalVirtualLabel->setText("总虚拟内存:");
    m_availVirtualLabel->setText("可用虚拟内存:");
    m_availExVirtualLabel->setText("可用扩展虚拟:");

    m_memoryUsedEdit->setReadOnly(true);
    m_totalPhysEdit->setReadOnly(true);
    m_availPhysEdit->setReadOnly(true);
    m_totalPageFileEdit->setReadOnly(true);
    m_availPageFileEdit->setReadOnly(true);
    m_totalVirtualEdit->setReadOnly(true);
    m_availVirtualEdit->setReadOnly(true);
    m_availExVirtualEdit->setReadOnly(true);

    QGridLayout *pLay = new QGridLayout(this);
    int iLine = 0;
    pLay->addWidget(m_memoryUsedLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_memoryUsedEdit, iLine, 1, 1, 3);
    iLine++;
    pLay->addWidget(m_totalPhysLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_totalPhysEdit, iLine, 1, 1, 3);
    iLine++;
    pLay->addWidget(m_availPhysLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_availPhysEdit, iLine, 1, 1, 3);
    iLine++;
    pLay->addWidget(m_totalPageFileLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_totalPageFileEdit, iLine, 1, 1, 3);
    iLine++;
    pLay->addWidget(m_availPageFileLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_availPageFileEdit, iLine, 1, 1, 3);
    iLine++;
    pLay->addWidget(m_totalVirtualLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_totalVirtualEdit, iLine, 1, 1, 3);
    iLine++;
    pLay->addWidget(m_availVirtualLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_availVirtualEdit, iLine, 1, 1, 3);
    iLine++;
    pLay->addWidget(m_availExVirtualLabel, iLine, 0, 1, 1);
    pLay->addWidget(m_availExVirtualEdit, iLine, 1, 1, 3);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &QMemoryInfo::updateInfo);
    m_timer->start(100);
}

QMemoryInfo::~QMemoryInfo()
{
    delete ui;
}

void QMemoryInfo::updateInfo()
{
    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(memoryInfo);
    GlobalMemoryStatusEx(&memoryInfo);

    UINT mb = 1024 * 1024;
    m_memoryUsedEdit->setText(QString::number(memoryInfo.dwMemoryLoad).append("%"));
    m_totalPhysEdit->setText(QString::number(memoryInfo.ullTotalPhys / mb));
    m_availPhysEdit->setText(QString::number(memoryInfo.ullAvailPhys / mb));
    m_totalPageFileEdit->setText(QString::number(memoryInfo.ullTotalPageFile / mb));
    m_availPageFileEdit->setText(QString::number(memoryInfo.ullTotalPageFile / mb - memoryInfo.ullAvailPageFile / mb));
    m_totalVirtualEdit->setText(QString::number(memoryInfo.ullTotalVirtual / mb));
    m_availVirtualEdit->setText(QString::number(memoryInfo.ullAvailVirtual / mb));
    m_availExVirtualEdit->setText(QString::number(memoryInfo.ullAvailExtendedVirtual / mb));
    update();
}
