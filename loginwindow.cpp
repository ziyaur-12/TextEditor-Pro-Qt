#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

// =====================================================
//  Constructor
// =====================================================
LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent) {
    userManager = new UserManager();
    setupUI();
    applyStyles();

    // Window center mein dikhao
    setFixedSize(420, 540);
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

LoginWindow::~LoginWindow() {
    delete userManager;
}

// =====================================================
//  setupUI - Main UI setup
// =====================================================
void LoginWindow::setupUI() {
    setWindowTitle("TextEditor Pro - Login");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Stacked Widget - Pages switch karne ke liye
    stackedWidget = new QStackedWidget();

    setupLoginPage();
    setupRegisterPage();

    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(registerPage);
    stackedWidget->setCurrentWidget(loginPage);

    mainLayout->addWidget(stackedWidget);
}

// =====================================================
//  setupLoginPage - Login form banana
// =====================================================
void LoginWindow::setupLoginPage() {
    loginPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    layout->setContentsMargins(50, 40, 50, 40);
    layout->setSpacing(15);

    // === HEADER ===
    QLabel *logoLabel = new QLabel("📝");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setObjectName("logoLabel");

    QLabel *titleLabel = new QLabel("TextEditor Pro");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    QLabel *subtitleLabel = new QLabel("Apne account mein login karo");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setObjectName("subtitleLabel");

    // === FORM ===
    QLabel *usernameLabel = new QLabel("Username");
    usernameLabel->setObjectName("fieldLabel");
    loginUsernameEdit = new QLineEdit();
    loginUsernameEdit->setPlaceholderText("Apna username daalo...");
    loginUsernameEdit->setObjectName("inputField");

    QLabel *passwordLabel = new QLabel("Password");
    passwordLabel->setObjectName("fieldLabel");
    loginPasswordEdit = new QLineEdit();
    loginPasswordEdit->setPlaceholderText("Apna password daalo...");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setObjectName("inputField");

    // Status message
    loginStatusLabel = new QLabel("");
    loginStatusLabel->setAlignment(Qt::AlignCenter);
    loginStatusLabel->setObjectName("statusLabel");
    loginStatusLabel->setWordWrap(true);

    // === BUTTONS ===
    loginBtn = new QPushButton("🚀  Login Karo");
    loginBtn->setObjectName("primaryBtn");
    loginBtn->setCursor(Qt::PointingHandCursor);

    QFrame *divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setObjectName("divider");

    QLabel *noAccountLabel = new QLabel("Account nahi hai?");
    noAccountLabel->setAlignment(Qt::AlignCenter);
    noAccountLabel->setObjectName("subtitleLabel");

    goToRegisterBtn = new QPushButton("✨  Naya Account Banao");
    goToRegisterBtn->setObjectName("secondaryBtn");
    goToRegisterBtn->setCursor(Qt::PointingHandCursor);

    // Layout mein add karo
    layout->addWidget(logoLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(10);
    layout->addWidget(usernameLabel);
    layout->addWidget(loginUsernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(loginPasswordEdit);
    layout->addWidget(loginStatusLabel);
    layout->addSpacing(5);
    layout->addWidget(loginBtn);
    layout->addWidget(divider);
    layout->addWidget(noAccountLabel);
    layout->addWidget(goToRegisterBtn);
    layout->addStretch();

    // Signals connect karo
    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(goToRegisterBtn, &QPushButton::clicked, this, &LoginWindow::switchToRegister);

    // Enter press karne par login ho
    connect(loginPasswordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

// =====================================================
//  setupRegisterPage - Register form banana
// =====================================================
void LoginWindow::setupRegisterPage() {
    registerPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(registerPage);
    layout->setContentsMargins(50, 30, 50, 30);
    layout->setSpacing(12);

    // === HEADER ===
    QLabel *logoLabel = new QLabel("🎉");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setObjectName("logoLabel");

    QLabel *titleLabel = new QLabel("Account Banao");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    QLabel *subtitleLabel = new QLabel("Shuru karo TextEditor Pro ke saath");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setObjectName("subtitleLabel");

    // === FORM ===
    QLabel *usernameLabel = new QLabel("Username (minimum 3 characters)");
    usernameLabel->setObjectName("fieldLabel");
    regUsernameEdit = new QLineEdit();
    regUsernameEdit->setPlaceholderText("Apna username chuno...");
    regUsernameEdit->setObjectName("inputField");

    QLabel *passwordLabel = new QLabel("Password (minimum 4 characters)");
    passwordLabel->setObjectName("fieldLabel");
    regPasswordEdit = new QLineEdit();
    regPasswordEdit->setPlaceholderText("Strong password daalo...");
    regPasswordEdit->setEchoMode(QLineEdit::Password);
    regPasswordEdit->setObjectName("inputField");

    QLabel *confirmLabel = new QLabel("Password Confirm Karo");
    confirmLabel->setObjectName("fieldLabel");
    regConfirmPasswordEdit = new QLineEdit();
    regConfirmPasswordEdit->setPlaceholderText("Password dobara daalo...");
    regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    regConfirmPasswordEdit->setObjectName("inputField");

    // Status message
    registerStatusLabel = new QLabel("");
    registerStatusLabel->setAlignment(Qt::AlignCenter);
    registerStatusLabel->setObjectName("statusLabel");
    registerStatusLabel->setWordWrap(true);

    // === BUTTONS ===
    createAccountBtn = new QPushButton("✅  Account Banao");
    createAccountBtn->setObjectName("primaryBtn");
    createAccountBtn->setCursor(Qt::PointingHandCursor);

    QFrame *divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setObjectName("divider");

    QLabel *hasAccountLabel = new QLabel("Pehle se account hai?");
    hasAccountLabel->setAlignment(Qt::AlignCenter);
    hasAccountLabel->setObjectName("subtitleLabel");

    goToLoginBtn = new QPushButton("🔑  Login Karo");
    goToLoginBtn->setObjectName("secondaryBtn");
    goToLoginBtn->setCursor(Qt::PointingHandCursor);

    // Layout mein add karo
    layout->addWidget(logoLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(5);
    layout->addWidget(usernameLabel);
    layout->addWidget(regUsernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(regPasswordEdit);
    layout->addWidget(confirmLabel);
    layout->addWidget(regConfirmPasswordEdit);
    layout->addWidget(registerStatusLabel);
    layout->addSpacing(5);
    layout->addWidget(createAccountBtn);
    layout->addWidget(divider);
    layout->addWidget(hasAccountLabel);
    layout->addWidget(goToLoginBtn);
    layout->addStretch();

    // Signals connect karo
    connect(createAccountBtn, &QPushButton::clicked, this, &LoginWindow::onCreateAccountClicked);
    connect(goToLoginBtn, &QPushButton::clicked, this, &LoginWindow::switchToLogin);
}

// =====================================================
//  Slot: Login button click
// =====================================================
void LoginWindow::onLoginClicked() {
    QString username = loginUsernameEdit->text().trimmed();
    QString password = loginPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        loginStatusLabel->setStyleSheet("color: #e74c3c; font-size: 13px;");
        loginStatusLabel->setText("❌ Username aur password dono zaroori hain!");
        return;
    }

    if (userManager->loginUser(username, password)) {
        loginStatusLabel->setStyleSheet("color: #2ecc71; font-size: 13px;");
        loginStatusLabel->setText("✅ Login ho gaya! Loading...");

        // 500ms baad main window kholo
        QTimer::singleShot(500, this, [this, username]() {
            emit loginSuccessful(username);
        });
    } else {
        loginStatusLabel->setStyleSheet("color: #e74c3c; font-size: 13px;");
        loginStatusLabel->setText("❌ Galat username ya password!");
        loginPasswordEdit->clear();
        loginPasswordEdit->setFocus();
    }
}

// =====================================================
//  Slot: Register button click (inline use)
// =====================================================
void LoginWindow::onRegisterClicked() {
    switchToRegister();
}

// =====================================================
//  Slot: Create Account button click
// =====================================================
void LoginWindow::onCreateAccountClicked() {
    QString username = regUsernameEdit->text().trimmed();
    QString password = regPasswordEdit->text();
    QString confirmPassword = regConfirmPasswordEdit->text();

    // Validation
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        registerStatusLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");
        registerStatusLabel->setText("❌ Sab fields bharna zaroori hai!");
        return;
    }

    if (username.length() < 3) {
        registerStatusLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");
        registerStatusLabel->setText("❌ Username minimum 3 characters ka hona chahiye!");
        return;
    }

    if (password.length() < 4) {
        registerStatusLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");
        registerStatusLabel->setText("❌ Password minimum 4 characters ka hona chahiye!");
        return;
    }

    if (password != confirmPassword) {
        registerStatusLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");
        registerStatusLabel->setText("❌ Dono passwords alag hain! Dobara try karo.");
        regConfirmPasswordEdit->clear();
        return;
    }

    // Registration attempt
    if (userManager->registerUser(username, password)) {
        registerStatusLabel->setStyleSheet("color: #2ecc71; font-size: 12px;");
        registerStatusLabel->setText("✅ Account ban gaya! Ab login karo.");

        // 1 second baad login page par jao
        QTimer::singleShot(1000, this, [this, username]() {
            loginUsernameEdit->setText(username);
            loginPasswordEdit->clear();
            loginPasswordEdit->setFocus();
            switchToLogin();
        });
    } else {
        registerStatusLabel->setStyleSheet("color: #e74c3c; font-size: 12px;");
        registerStatusLabel->setText("❌ Ye username already liya hua hai!");
        regUsernameEdit->setFocus();
    }
}

// =====================================================
//  switchToLogin / switchToRegister
// =====================================================
void LoginWindow::switchToLogin() {
    loginStatusLabel->clear();
    stackedWidget->setCurrentWidget(loginPage);
}

void LoginWindow::switchToRegister() {
    registerStatusLabel->clear();
    stackedWidget->setCurrentWidget(registerPage);
}

// =====================================================
//  applyStyles - Dark theme CSS apply karo
// =====================================================
void LoginWindow::applyStyles() {
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #1a1a2e;
            font-family: 'Segoe UI', Arial, sans-serif;
        }

        #logoLabel {
            font-size: 52px;
            padding: 5px;
        }

        #titleLabel {
            color: #e0e0e0;
            font-size: 24px;
            font-weight: bold;
            letter-spacing: 1px;
        }

        #subtitleLabel {
            color: #7f8c8d;
            font-size: 13px;
        }

        #fieldLabel {
            color: #a0a0b0;
            font-size: 12px;
            font-weight: 600;
            letter-spacing: 0.5px;
        }

        #inputField {
            background-color: #16213e;
            border: 2px solid #2d3561;
            border-radius: 10px;
            padding: 12px 15px;
            color: #e0e0e0;
            font-size: 14px;
            selection-background-color: #e94560;
        }

        #inputField:focus {
            border: 2px solid #e94560;
            background-color: #1e2a4a;
        }

        #primaryBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #e94560, stop:1 #c0392b);
            color: white;
            border: none;
            border-radius: 10px;
            padding: 14px;
            font-size: 15px;
            font-weight: bold;
            letter-spacing: 0.5px;
        }

        #primaryBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff6b7a, stop:1 #e94560);
        }

        #primaryBtn:pressed {
            background: #c0392b;
        }

        #secondaryBtn {
            background-color: transparent;
            color: #e94560;
            border: 2px solid #e94560;
            border-radius: 10px;
            padding: 12px;
            font-size: 14px;
            font-weight: bold;
        }

        #secondaryBtn:hover {
            background-color: rgba(233, 69, 96, 0.1);
        }

        #statusLabel {
            font-size: 13px;
            min-height: 20px;
        }

        #divider {
            color: #2d3561;
            margin: 5px 0;
        }
    )");
}
