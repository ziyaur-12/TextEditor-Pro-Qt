#include "usermanager.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>

// =====================================================
//  Constructor - users.dat file ka path set karo
// =====================================================
UserManager::UserManager() {
    // App ke saath same folder mein users.dat banao
    usersFilePath = QDir::currentPath() + "/users.dat";
}

// =====================================================
//  registerUser - Naya user banana
// =====================================================
bool UserManager::registerUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) return false;
    if (username.length() < 3) return false;
    if (password.length() < 4) return false;

    QMap<QString, QString> users = loadUsers();

    // Agar user already exist karta hai
    if (users.contains(username)) return false;

    // New user add karo
    users[username] = hashPassword(password);
    saveUsers(users);
    return true;
}

// =====================================================
//  loginUser - Login check karo
// =====================================================
bool UserManager::loginUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) return false;

    QMap<QString, QString> users = loadUsers();

    if (!users.contains(username)) return false;

    // Password match karo
    if (users[username] == hashPassword(password)) {
        currentUsername = username;
        return true;
    }
    return false;
}

// =====================================================
//  userExists - Check karo user hai ya nahi
// =====================================================
bool UserManager::userExists(const QString &username) {
    QMap<QString, QString> users = loadUsers();
    return users.contains(username);
}

// =====================================================
//  currentUser - Current user ka naam do
// =====================================================
QString UserManager::currentUser() const {
    return currentUsername;
}

// =====================================================
//  logout - User ko logout karo
// =====================================================
void UserManager::logout() {
    currentUsername = "";
}

// =====================================================
//  loadUsers - File se users load karo
// =====================================================
QMap<QString, QString> UserManager::loadUsers() {
    QMap<QString, QString> users;
    QFile file(usersFilePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return users; // File nahi hai toh empty map return

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(":");
        if (parts.size() == 2) {
            users[parts[0]] = parts[1];
        }
    }
    file.close();
    return users;
}

// =====================================================
//  saveUsers - Users ko file mein save karo
// =====================================================
void UserManager::saveUsers(const QMap<QString, QString> &users) {
    QFile file(usersFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    for (auto it = users.begin(); it != users.end(); ++it) {
        out << it.key() << ":" << it.value() << "\n";
    }
    file.close();
}

// =====================================================
//  hashPassword - Simple password hashing
// =====================================================
QString UserManager::hashPassword(const QString &password) {
    // Simple hashing for educational purposes
    // Real projects mein bcrypt ya SHA256 use karo
    QString hashed = "";
    int key = 42; // XOR key
    for (QChar ch : password) {
        hashed += QString::number(ch.unicode() ^ key) + "_";
    }
    return hashed;
}
