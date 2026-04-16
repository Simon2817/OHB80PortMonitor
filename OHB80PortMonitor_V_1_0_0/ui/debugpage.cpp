#include "debugpage.h"
#include "ui_debugpage.h"

DebugPage::DebugPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugPage)
{
    ui->setupUi(this);
}

DebugPage::~DebugPage()
{
    delete ui;
}
