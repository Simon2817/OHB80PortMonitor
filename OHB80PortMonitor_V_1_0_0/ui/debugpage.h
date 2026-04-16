#ifndef DEBUGPAGE_H
#define DEBUGPAGE_H

#include <QWidget>

namespace Ui {
class DebugPage;
}

class DebugPage : public QWidget
{
    Q_OBJECT

public:
    explicit DebugPage(QWidget *parent = nullptr);
    ~DebugPage();

private:
    Ui::DebugPage *ui;
};

#endif // DEBUGPAGE_H
