#ifndef COMMUNICATEPAGE_H
#define COMMUNICATEPAGE_H

#include <QWidget>
#include "modbustcpmastermanager/modbuscommand/modbuscommand.h"

namespace Ui {
class CommunicatePage;
}

class CommunicatePage : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicatePage(QWidget *parent = nullptr);
    ~CommunicatePage();

    void initCommLoggerWidget();

private slots:
    // 处理通讯完成信号，写入日志表
    void onCommunicationCompleted(ModbusCommand cmd, QString masterId);

private:
    Ui::CommunicatePage *ui;
};

#endif // COMMUNICATEPAGE_H
