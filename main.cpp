#include <QApplication>
#include <QTimer>
#include "loginwindow.h"
#include "mainwindow.h"

// =====================================================
//  main.cpp - Program ka Starting Point
//
//  Flow:
//  1. Login Window khulti hai
//  2. Login successful → Main Editor Window khulti hai
//  3. Logout → Wapas Login Window
// =====================================================

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // App ka naam aur icon set karo
    app.setApplicationName("TextEditor Pro");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("1st Year C++ Project");

    // =====================================================
    //  Login Window banao
    // =====================================================
    LoginWindow *loginWindow = new LoginWindow();

    // Main Editor Window (pehle hidden)
    MainWindow *mainWindow = nullptr;

    // =====================================================
    //  Connection: Login → Main Editor
    // =====================================================
    QObject::connect(loginWindow, &LoginWindow::loginSuccessful,
        [&](const QString &username) {
            // Login window band karo
            loginWindow->hide();

            // Main window banao aur kholo
            if (mainWindow) {
                delete mainWindow;
            }
            mainWindow = new MainWindow(username);

            // Main Window → Logout → Login Window wapas
            QObject::connect(mainWindow, &MainWindow::logoutRequested,
                [&]() {
                    mainWindow->hide();
                    mainWindow->deleteLater();
                    mainWindow = nullptr;

                    // Login fields clear karo
                    loginWindow->show();
                }
            );

            mainWindow->show();
        }
    );

    // Login window dikhao
    loginWindow->show();

    return app.exec();
}
