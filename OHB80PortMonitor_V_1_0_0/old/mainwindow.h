#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QStringList>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartStopClicked();
    void onWriteTimerTimeout();
    void onLogWritten(bool success);

private:
    QJsonObject generateTestRecord();

    Ui::MainWindow *ui;
    QTimer        *m_writeTimer    = nullptr;
    int            m_recordCounter = 0;
    bool           m_running       = false;

    static const QStringList kHeaders;
    static const QStringList kLevels;
};
#endif // MAINWINDOW_H
