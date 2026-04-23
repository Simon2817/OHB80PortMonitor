#include "useraccountlabel.h"
#include "logindialog.h"
#include "usermanager.h"

#include <QPixmap>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

UserAccountLabel::UserAccountLabel(QWidget *parent)
    : QLabel(parent)
    , m_loginState(LoginState::NotLoggedIn)
    , m_contextMenu(nullptr)
    , m_iconSize(30)
{
    setAlignment(Qt::AlignCenter);

    m_originalPixmap.load(":/image/useraccount.png");
    setIconSize(30);

    // 连接 UserManager 信号
    UserManager* mgr = UserManager::instance();
    connect(mgr, &UserManager::loginSuccess, this, [this](const QString& username, UserPermission) {
        setLoginState(LoginState::LoggedIn);
        setCurrentUser(username);
    });
    connect(mgr, &UserManager::logoutSuccess, this, [this]() {
        setLoginState(LoginState::NotLoggedIn);
        setCurrentUser("");
    });
}

void UserAccountLabel::setIconSize(int size)
{
    m_iconSize = size;
    setFixedSize(size, size);
    updateDisplay();
}

void UserAccountLabel::setLoginState(LoginState state)
{
    m_loginState = state;
    updateDisplay();
}

UserAccountLabel::LoginState UserAccountLabel::loginState() const
{
    return m_loginState;
}

void UserAccountLabel::setCurrentUser(const QString& username)
{
    m_currentUser = username;
    updateDisplay();
}

QString UserAccountLabel::currentUser() const
{
    return m_currentUser;
}

void UserAccountLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        showContextMenu();
    }
    QLabel::mousePressEvent(event);
}

void UserAccountLabel::showContextMenu()
{
    if (m_contextMenu) {
        return;  // 菜单已存在，不重复创建
    }

    m_contextMenu = new QMenu(this);
    m_contextMenu->setAttribute(Qt::WA_DeleteOnClose);

    if (m_loginState == LoginState::NotLoggedIn) {
        // 未登录状态：只有"登录账号"选项
        QAction* loginAction = m_contextMenu->addAction(QStringLiteral("登录账号"));
        connect(loginAction, &QAction::triggered, this, &UserAccountLabel::onLoginRequested);
    } else {
        // 已登录状态：有"登录新账号"和"登出账号"选项
        QAction* loginNewAction = m_contextMenu->addAction(QStringLiteral("登录新账号"));
        connect(loginNewAction, &QAction::triggered, this, &UserAccountLabel::onLoginNewRequested);

        QAction* logoutAction = m_contextMenu->addAction(QStringLiteral("登出账号"));
        connect(logoutAction, &QAction::triggered, this, &UserAccountLabel::onLogoutRequested);
    }

    // 菜单关闭时清空指针
    connect(m_contextMenu, &QMenu::aboutToHide, this, [this]() {
        m_contextMenu = nullptr;
    });

    // 在 label 的右下角弹出菜单
    QPoint pos = mapToGlobal(QPoint(width(), height()));
    m_contextMenu->popup(pos);
}

void UserAccountLabel::closeContextMenu()
{
    if (m_contextMenu) {
        m_contextMenu->close();
    }
}

void UserAccountLabel::onLoginRequested()
{
    LoginDialog dlg(this);
    dlg.exec();
}

void UserAccountLabel::onLoginNewRequested()
{
    LoginDialog dlg(this);
    dlg.exec();
}

void UserAccountLabel::onLogoutRequested()
{
    UserManager::instance()->logout();
}

void UserAccountLabel::updateDisplay()
{
    if (m_loginState == LoginState::LoggedIn && !m_currentUser.isEmpty()) {
        // 已登录：显示蓝色圆形 + 用户名首字母
        setPixmap(makeAvatarPixmap(m_currentUser, m_iconSize));
    } else {
        // 未登录：显示默认图标
        if (!m_originalPixmap.isNull()) {
            setPixmap(m_originalPixmap.scaled(m_iconSize, m_iconSize,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
        }
    }
}

QPixmap UserAccountLabel::makeAvatarPixmap(const QString& name, int size) const
{
    // 获取首字母（大写）
    QString firstLetter;
    if (!name.isEmpty()) {
        firstLetter = name.left(1).toUpper();
    }

    // 创建透明背景的 pixmap
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 绘制蓝色实心圆
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(30, 144, 255));  // 道奇蓝
    painter.drawEllipse(0, 0, size, size);

    // 在圆中心绘制首字母
    if (!firstLetter.isEmpty()) {
        QFont font = painter.font();
        font.setPixelSize(size * 0.5);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(Qt::white);
        painter.drawText(QRect(0, 0, size, size), Qt::AlignCenter, firstLetter);
    }

    return pixmap;
}
