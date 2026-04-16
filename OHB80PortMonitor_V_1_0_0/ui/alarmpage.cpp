#include "alarmpage.h"
#include "ui_alarmpage.h"

AlarmPage::AlarmPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlarmPage)
{
    ui->setupUi(this);
}

AlarmPage::~AlarmPage()
{
    delete ui;
}
