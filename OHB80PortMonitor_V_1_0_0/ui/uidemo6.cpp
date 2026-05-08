#include "uidemo6.h"
#include "ui_uidemo6.h"
#include "quiwidget.h"
#include "homepage.h"
#include "alarmpage.h"
#include "app.h"
#include "usermanager.h"
#include "app/shareddata.h"
#include "scheduler/tasks/operation_dispatch_task.h"
#include "scheduler/tasks/alarm_dispatch_task.h"
#include "ui/customwidget/scrollingtiplabel/scrollingtiplabel.h"

UIDemo6::UIDemo6(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UIDemo6)
{
    ui->setupUi(this);
    this->initForm();
    QUIWidget::setFormInCenter(this);
    
    // 初始化双击标题栏最大化功能，默认启用
    this->doubleClickMaximize = true;
}

UIDemo6::~UIDemo6()
{
    delete ui;
}

void UIDemo6::initForm()
{
    this->max = false;
    this->location = this->geometry();
    this->setProperty("form", true);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    // 设置uidemo6主界面的名称
    ui->labTitle->setText(App::getDisplayName());
    this->setWindowTitle(App::getDisplayName());

    QStringList qss;
    qss.append("QLabel#label{color:#F0F0F0;font:50pt;}");
    qss.append("QWidget#widgetLeft>QAbstractButton{background:none;border-radius:0px;}");
    qss.append("QWidget#widgetMenu>QAbstractButton{border:0px solid #FF0000;border-radius:0px;padding:0px;margin:0px;}");
    this->setStyleSheet(qss.join(""));

    //添加自定义属性,用于切换ico用
    ui->btnHome->setProperty("icoName", "homepage");
    ui->btnSetting->setProperty("icoName", "configpage");
    ui->btnChart->setProperty("icoName", "chartpage");
    ui->btnCommunicate->setProperty("icoName", "communicatepage");
    ui->btnAlarm->setProperty("icoName", "alarmpage");
    ui->btnDebug->setProperty("icoName", "debugpage");

    // ui->btnDebug->hide();
    ui->btnChart->hide();
    ui->btnMenu_Max->hide();

    QList<QToolButton *> btns = ui->widgetLeft->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns) {
        btn->setMaximumHeight(90);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClick()));
    }

    ui->btnHome->click();

    // 为 widgetTitle 安装事件过滤器以处理双击事件
    ui->widgetTitle->installEventFilter(this);

    // 老 RunningLoggerWidget 已废弃，运行日志由 RunningLoggerTask + OperationLogDBCon 接管；
    // 老 AlarmLoggerWidget 也已废弃，警报生命周期由 AlarmDispatchTask 与 AlarmLogDBCon 接管。
    // UI 端需要实时显示请订阅 OperationLogDBCon::recordInserted / AlarmLogDBCon::recordInserted。

    // 注册控件权限
    registerWidgetPermissions();

    // 启动时全屏显示
    this->showFullScreen();
}

bool UIDemo6::eventFilter(QObject *obj, QEvent *event)
{
    if (doubleClickMaximize && obj == ui->widgetTitle && event->type() == QEvent::MouseButtonDblClick) {
        // 双击标题栏时触发最大化/还原
        emit requestMaximizeOrRestore();
        return true;
    }
    return QDialog::eventFilter(obj, event);
}

void UIDemo6::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();

    QList<QToolButton *> btns = ui->widgetLeft->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns)
    {
        QString icoName = btn->property("icoName").toString();
        if (btn != b) {
            btn->setChecked(false);
            btn->setIcon(QIcon(QString(":/image/%1.png").arg(icoName)));
        } else {
            btn->setChecked(true);
            btn->setIcon(QIcon(QString(":/image/%1_focus.png").arg(icoName)));
            if (icoName == "homepage")
                ui->stackedWidget->setCurrentIndex(0);
            else if (icoName == "configpage")
                ui->stackedWidget->setCurrentIndex(1);
            else if (icoName == "chartpage")
                ui->stackedWidget->setCurrentIndex(2);
            else if (icoName == "communicatepage")
                ui->stackedWidget->setCurrentIndex(3);
            else if (icoName == "alarmpage")
                ui->stackedWidget->setCurrentIndex(4);
            else if (icoName == "debugpage")
                ui->stackedWidget->setCurrentIndex(5);
        }
    }
}

void UIDemo6::on_btnMenu_Min_clicked()
{
    emit requestMinimize();
}

void UIDemo6::on_btnMenu_Max_clicked()
{
    emit requestMaximizeOrRestore();
}

void UIDemo6::on_btnMenu_Close_clicked()
{
    emit requestClose();
}

void UIDemo6::setDoubleClickMaximize(bool enabled)
{
    this->doubleClickMaximize = enabled;
}

bool UIDemo6::getDoubleClickMaximize() const
{
    return this->doubleClickMaximize;
}

void UIDemo6::connectTipLabelTask()
{
    // 连接 OperationDispatchTask 的操作日志插入信号
    OperationDispatchTask* operationTask = SharedData::getOperationDispatchTask();
    if (operationTask) {
        connect(operationTask, &OperationDispatchTask::operationLogInserted,
                this, [this](const OperationRecord& rec) {
                    QString logTypeStr;
                    if (rec.logType == 0) logTypeStr = "INFO";
                    else if (rec.logType == 1) logTypeStr = "WARN";
                    else if (rec.logType == 2) logTypeStr = "ERROR";
                    ui->scrollingTipLabel->submitOperationLog({rec.occurTime, logTypeStr, rec.description});
                }, Qt::QueuedConnection);
    }

    // 连接 AlarmDispatchTask 的警报日志插入信号
    AlarmDispatchTask* alarmTask = SharedData::getAlarmDispatchTask();
    if (alarmTask) {
        connect(alarmTask, &AlarmDispatchTask::alarmPublished,
                this, [this](const AlarmInfo& info) {
                    QString alarmLevelStr = QString::number(info.record.alarmLevel);
                    QString alarmTypeStr = QString::number(info.record.alarmType);
                    ui->scrollingTipLabel->submitAlarmLog(
                        {info.record.occurTime, alarmLevelStr, alarmTypeStr, info.record.description},
                        info.record.id);
                }, Qt::QueuedConnection);

        connect(alarmTask, &AlarmDispatchTask::alarmResolved,
                this, [this](const AlarmInfo& info) {
                    ui->scrollingTipLabel->submitAlarmResolved(info.record.id);
                }, Qt::QueuedConnection);
    }

    qDebug() << "[UIDemo6] OperationDispatchTask and AlarmDispatchTask signals connected to scrollingTipLabel";
}

void UIDemo6::registerWidgetPermissions()
{
    UserManager* mgr = UserManager::instance();
    if (!mgr) return;

    // Root (4): btnDebug
    mgr->registerWidget(ui->btnDebug, UserPermission::Engineer);

    // Debug (2): btnCommunicate
    mgr->registerWidget(ui->btnDebug, UserPermission::Debug);

    // Normal (1): btnSetting, btnHome, btnAlarm ,btnCommunicate(普通用户可查看)
    mgr->registerWidget(ui->btnSetting, UserPermission::Normal);
    mgr->registerWidget(ui->btnCommunicate, UserPermission::Normal);
    // 其他按钮默认所有用户可见，可省略注册
    // mgr->registerWidget(ui->btnHome, UserPermission::Normal);
    // mgr->registerWidget(ui->btnAlarm, UserPermission::Normal);
    // mgr->registerWidget(ui->btnChart, UserPermission::Normal);
}
