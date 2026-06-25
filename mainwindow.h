#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QSplitter>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include "filemanager.h"
#include "usermanager.h"

// =====================================================
//  MainWindow - Main Text Editor UI
// =====================================================
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Window band karne se pehle check karo
    void closeEvent(QCloseEvent *event) override;

private slots:
    // File Operations (CRUD)
    void newFile();          // CREATE
    void openFile();         // READ (file list se)
    void saveFile();         // UPDATE
    void saveFileAs();       // UPDATE with new name
    void deleteFile();       // DELETE
    void renameFile();       // RENAME

    // Sidebar file click
    void onFileSelected(QListWidgetItem *item);

    // Editor changes track karna
    void onTextChanged();

    // Auto-save
    void autoSave();

    // Logout
    void logout();

    // About
    void showAbout();

    // Word count update
    void updateWordCount();

signals:
    void logoutRequested();

private:
    FileManager *fileManager;
    UserManager *userManager;

    // Current file tracking
    QString currentFileName;
    bool isModified;          // Unsaved changes hain ya nahi
    bool isNewFile;           // Nai file hai ya existing

    // --- UI Components ---

    // Left Sidebar - File List
    QWidget *sidebar;
    QListWidget *fileList;
    QLabel *sidebarTitle;
    QPushButton *newFileBtn;

    // Center - Text Editor
    QTextEdit *textEditor;
    QLabel *fileNameLabel;    // Current file ka naam
    QLabel *wordCountLabel;   // Word count

    // Toolbar actions
    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *deleteAction;
    QAction *renameAction;
    QAction *logoutAction;

    // Auto-save timer
    QTimer *autoSaveTimer;

    // --- Setup Functions ---
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupSidebar();
    void setupEditor();
    void setupStatusBar();
    void applyStyles();

    // Helper functions
    void refreshFileList();
    void loadFileInEditor(const QString &filename);
    void clearEditor();
    bool confirmSave();       // Unsaved changes ke baare mein poochho
    void setCurrentFile(const QString &filename);
    void showStatus(const QString &message);
};

#endif // MAINWINDOW_H
