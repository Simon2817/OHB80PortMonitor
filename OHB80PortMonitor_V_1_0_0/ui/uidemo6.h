#ifndef UIDEMO6_H
#define UIDEMO6_H

#include <QDialog>

namespace Ui {
class UIDemo6;
}

class UIDemo6 : public QDialog
{
    Q_OBJECT

public:
    explicit UIDemo6(QWidget *parent = 0);
    ~UIDemo6();

Q_SIGNALS:
    void requestMinimize();
    void requestMaximizeOrRestore();
    void requestClose();

private:
    Ui::UIDemo6 *ui;
    bool max;
    QRect location;
    bool doubleClickMaximize;  // 是否允许双击标题栏最大化/还原

public:
    // 设置是否允许双击标题栏最大化/还原
    void setDoubleClickMaximize(bool enabled);
    // 获取是否允许双击标题栏最大化/还原
    bool getDoubleClickMaximize() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void initForm();
    void buttonClick();

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
};

#endif // UIDEMO6_H
