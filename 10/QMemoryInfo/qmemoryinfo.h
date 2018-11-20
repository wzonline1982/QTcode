#pragma execution_character_set("utf-8")
#ifndef QMEMORYINFO_H
#define QMEMORYINFO_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>

namespace Ui {
class QMemoryInfo;
}

class QMemoryInfo : public QWidget
{
    Q_OBJECT

public:
    explicit QMemoryInfo(QWidget *parent = nullptr);
    ~QMemoryInfo();

private:
    Ui::QMemoryInfo *ui;

    void updateInfo();

    QTimer *m_timer;

    QLabel *m_memoryUsedLabel;
    QLabel *m_totalPhysLabel;
    QLabel *m_availPhysLabel;
    QLabel *m_totalPageFileLabel;
    QLabel *m_availPageFileLabel;
    QLabel *m_totalVirtualLabel;
    QLabel *m_availVirtualLabel;
    QLabel *m_availExVirtualLabel;

    QLineEdit *m_memoryUsedEdit;
    QLineEdit *m_totalPhysEdit;
    QLineEdit *m_availPhysEdit;
    QLineEdit *m_totalPageFileEdit;
    QLineEdit *m_availPageFileEdit;
    QLineEdit *m_totalVirtualEdit;
    QLineEdit *m_availVirtualEdit;
    QLineEdit *m_availExVirtualEdit;

};

#endif // QMEMORYINFO_H
