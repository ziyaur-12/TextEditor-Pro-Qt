#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

// =====================================================
//  FileManager - File ke CRUD Operations
//  C = Create, R = Read, U = Update, D = Delete
// =====================================================
class FileManager {
public:
    FileManager(const QString &username);

    // CREATE - Nai file banana
    bool createFile(const QString &filename, const QString &content = "");

    // READ - File ka content padhna
    QString readFile(const QString &filename);

    // UPDATE - File ka content update karna
    bool updateFile(const QString &filename, const QString &newContent);

    // DELETE - File ko delete karna
    bool deleteFile(const QString &filename);

    // RENAME - File ka naam badalna
    bool renameFile(const QString &oldName, const QString &newName);

    // List - Sari files ki list
    QStringList listFiles();

    // Check - File exist karti hai ya nahi
    bool fileExists(const QString &filename);

    // User ka files folder path
    QString getUserFolderPath() const;

private:
    QString username;
    QString userFolderPath;

    // Full path banana
    QString getFilePath(const QString &filename);

    // User ka folder create karo agar nahi hai
    void ensureUserFolder();
};

#endif // FILEMANAGER_H
