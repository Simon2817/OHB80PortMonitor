#include "logindialog.h"
#include "usermanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("用户登录"));
    setModal(true);
    setMinimumWidth(300);

    setupUI();
    connectSignals();
}

void LoginDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 表单布局
    QFormLayout *formLayout = new QFormLayout;

    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText(QStringLiteral("请输入用户名"));
    formLayout->addRow(QStringLiteral("用户名："), m_usernameEdit);

    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setPlaceholderText(QStringLiteral("请输入密码"));
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow(QStringLiteral("密码："), m_passwordEdit);

    mainLayout->addLayout(formLayout);

    // 错误提示标签
    m_errorLabel = new QLabel;
    m_errorLabel->setStyleSheet("color: red;");
    m_errorLabel->setVisible(false);
    mainLayout->addWidget(m_errorLabel);

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    m_loginBtn = new QPushButton(QStringLiteral("登录"));
    m_cancelBtn = new QPushButton(QStringLiteral("取消"));
    btnLayout->addStretch();
    btnLayout->addWidget(m_loginBtn);
    btnLayout->addWidget(m_cancelBtn);

    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
}

void LoginDialog::connectSignals()
{
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_cancelBtn, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
}

void LoginDialog::onLoginClicked()
{
    QString user = m_usernameEdit->text().trimmed();
    QString pwd = m_passwordEdit->text();

    if (user.isEmpty()) {
        m_errorLabel->setText(QStringLiteral("用户名不能为空"));
        m_errorLabel->setVisible(true);
        return;
    }

    if (pwd.isEmpty()) {
        m_errorLabel->setText(QStringLiteral("密码不能为空"));
        m_errorLabel->setVisible(true);
        return;
    }

    // 调用 UserManager 进行登录验证
    UserManager* mgr = UserManager::instance();
    if (mgr->login(user, pwd)) {
        accept();  // 登录成功，关闭对话框
    } else {
        m_errorLabel->setText(QStringLiteral("用户名或密码错误"));
        m_errorLabel->setVisible(true);
        m_passwordEdit->clear();
    }
}

void LoginDialog::onCancelClicked()
{
    reject();
}

QString LoginDialog::username() const
{
    return m_usernameEdit->text().trimmed();
}

QString LoginDialog::password() const
{
    return m_passwordEdit->text();
}
