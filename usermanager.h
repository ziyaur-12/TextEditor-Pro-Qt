#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QMap>

// =====================================================
//  UserManager - Login/Register ka sara kaam karta hai
// =====================================================
class UserManager {
public:
    UserManager();

    // User register karo
    bool registerUser(const QString &username, const QString &password);

    // Login check karo
    bool loginUser(const QString &username, const QString &password);

    // User exist karta hai ya nahi
    bool userExists(const QString &username);

    // Current logged-in user
    QString currentUser() const;

    // Logout
    void logout();

private:
    QString currentUsername;
    QString usersFilePath;  // users.dat file path

    // Users file se load karo
    QMap<QString, QString> loadUsers();

    // Users file mein save karo
    void saveUsers(const QMap<QString, QString> &users);

    // Password ko hash karo (simple XOR-based for beginners)
    QString hashPassword(const QString &password);
};

#endif // USERMANAGER_H
