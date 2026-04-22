#pragma once
#include <QWidget>
#include <QVBoxLayout>

class QProgressBar;
class QLabel;
class QPushButton;

class CommLoadMoreItemWidget : public QWidget
{
    Q_OBJECT
public:
    enum class Position { Top, Bottom };
    explicit CommLoadMoreItemWidget(Position pos, QWidget *parent = nullptr);

    void setIdle();
    void setLoading();
    void setProgress(int percent);
    void setDone(const QString &timeHint);

    Position position() const { return m_pos; }

signals:
    void clicked();

private:
    Position     m_pos;
    QPushButton  *m_btn      = nullptr;
    QProgressBar *m_progress = nullptr;
    QLabel       *m_label    = nullptr;
    QVBoxLayout  *m_layout   = nullptr;
};
