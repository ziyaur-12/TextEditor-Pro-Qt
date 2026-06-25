#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include "usermanager.h"

// =====================================================
//  LoginWindow - Login aur Register ka UI
// =====================================================
class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    // Jab login ho jaye toh main window open karo
    void loginSuccessful(const QString &username);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onCreateAccountClicked();
    void switchToLogin();
    void switchToRegister();

private:
    UserManager *userManager;

    // Login Page Widgets
    QWidget *loginPage;
    QLineEdit *loginUsernameEdit;
    QLineEdit *loginPasswordEdit;
    QPushButton *loginBtn;
    QPushButton *goToRegisterBtn;
    QLabel *loginStatusLabel;

    // Register Page Widgets
    QWidget *registerPage;
    QLineEdit *regUsernameEdit;
    QLineEdit *regPasswordEdit;
    QLineEdit *regConfirmPasswordEdit;
    QPushButton *createAccountBtn;
    QPushButton *goToLoginBtn;
    QLabel *registerStatusLabel;

    // Stacked Widget - Login aur Register ke beech switch karna
    QStackedWidget *stackedWidget;

    // UI Setup functions
    void setupUI();
    void setupLoginPage();
    void setupRegisterPage();
    void applyStyles();
};

#endif // LOGINWINDOW_H
