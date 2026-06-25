#include "filemanager.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFileInfo>

// =====================================================
//  Constructor - Har user ka apna alag folder
// =====================================================
FileManager::FileManager(const QString &username) : username(username) {
    // Har user ka apna folder: ./files/username/
    userFolderPath = QDir::currentPath() + "/files/" + username;
    ensureUserFolder();
}

// =====================================================
//  CREATE - Nai file banana
// =====================================================
bool FileManager::createFile(const QString &filename, const QString &content) {
    if (filename.isEmpty()) return false;

    QString filePath = getFilePath(filename);

    // Agar file already exist karti hai
    if (QFile::exists(filePath)) return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << content;
    file.close();
    return true;
}

// =====================================================
//  READ - File ka content padhna
// =====================================================
QString FileManager::readFile(const QString &filename) {
    QString filePath = getFilePath(filename);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ""; // File nahi mili
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    return content;
}

// =====================================================
//  UPDATE - File ka content update karna (Save)
// =====================================================
bool FileManager::updateFile(const QString &filename, const QString &newContent) {
    QString filePath = getFilePath(filename);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << newContent;
    file.close();
    return true;
}

// =====================================================
//  DELETE - File delete karna
// =====================================================
bool FileManager::deleteFile(const QString &filename) {
    QString filePath = getFilePath(filename);
    return QFile::remove(filePath);
}

// =====================================================
//  RENAME - File ka naam badalna
// =====================================================
bool FileManager::renameFile(const QString &oldName, const QString &newName) {
    if (oldName.isEmpty() || newName.isEmpty()) return false;

    QString oldPath = getFilePath(oldName);
    QString newPath = getFilePath(newName);

    if (!QFile::exists(oldPath)) return false;
    if (QFile::exists(newPath)) return false; // Naya naam already hai

    return QFile::rename(oldPath, newPath);
}

// =====================================================
//  LIST - Sari files ki list do
// =====================================================
QStringList FileManager::listFiles() {
    QDir dir(userFolderPath);
    return dir.entryList(QStringList() << "*.txt", QDir::Files, QDir::Name);
}

// =====================================================
//  fileExists - File hai ya nahi check karo
// =====================================================
bool FileManager::fileExists(const QString &filename) {
    return QFile::exists(getFilePath(filename));
}

// =====================================================
//  getUserFolderPath - User ka folder path do
// =====================================================
QString FileManager::getUserFolderPath() const {
    return userFolderPath;
}

// =====================================================
//  getFilePath - Filename se full path banana
// =====================================================
QString FileManager::getFilePath(const QString &filename) {
    // Auto .txt extension lagao agar nahi hai
    QString fname = filename;
    if (!fname.endsWith(".txt")) {
        fname += ".txt";
    }
    return userFolderPath + "/" + fname;
}

// =====================================================
//  ensureUserFolder - User ka folder exist karana
// =====================================================
void FileManager::ensureUserFolder() {
    QDir dir;
    if (!dir.exists(userFolderPath)) {
        dir.mkpath(userFolderPath);
    }
}
