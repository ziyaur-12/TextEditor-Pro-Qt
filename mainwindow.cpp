#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>
#include <QScreen>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <QFrame>
#include <QFont>
#include <QFontDialog>
#include <QColorDialog>
#include <QDateTime>
#include <QFileDialog>

// =====================================================
//  Constructor
// =====================================================
MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), isModified(false), isNewFile(false) {

    fileManager = new FileManager(username);
    userManager = new UserManager();

    // Auto-save timer (har 2 minute mein)
    autoSaveTimer = new QTimer(this);
    autoSaveTimer->setInterval(120000); // 2 minutes
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::autoSave);

    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    applyStyles();

    // Window ka size aur position set karo
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    resize(1100, 700);
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    setWindowTitle(QString("📝 TextEditor Pro — %1 ka Workspace").arg(username));
    showStatus(QString("Welcome %1! 👋 Ek file select karo ya nai file banao.").arg(username));
}

MainWindow::~MainWindow() {
    delete fileManager;
    delete userManager;
}

// =====================================================
//  setupUI - Main Layout
// =====================================================
void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Splitter - Sidebar aur Editor ke beech
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(2);

    // Sidebar setup
    setupSidebar();
    splitter->addWidget(sidebar);

    // Editor setup
    QWidget *editorContainer = new QWidget();
    setupEditor();
    QVBoxLayout *editorLayout = new QVBoxLayout(editorContainer);
    editorLayout->setContentsMargins(0, 0, 0, 0);
    editorLayout->setSpacing(0);

    // File name header bar
    QWidget *headerBar = new QWidget();
    headerBar->setObjectName("headerBar");
    headerBar->setFixedHeight(45);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerBar);
    headerLayout->setContentsMargins(20, 5, 20, 5);

    fileNameLabel = new QLabel("📄  Koi file nahi khuli");
    fileNameLabel->setObjectName("fileNameLabel");

    wordCountLabel = new QLabel("Words: 0  |  Chars: 0");
    wordCountLabel->setObjectName("wordCountLabel");

    headerLayout->addWidget(fileNameLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(wordCountLabel);

    editorLayout->addWidget(headerBar);
    editorLayout->addWidget(textEditor);

    splitter->addWidget(editorContainer);

    // Sidebar 250px, editor baaki sab
    splitter->setSizes({250, 850});
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter);
}

// =====================================================
//  setupMenuBar - Menu banana
// =====================================================
void MainWindow::setupMenuBar() {
    // File Menu
    QMenu *fileMenu = menuBar()->addMenu("📁  File");

    newAction = fileMenu->addAction("🆕  Nai File");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

    fileMenu->addSeparator();

    saveAction = fileMenu->addAction("💾  Save Karo");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    saveAsAction = fileMenu->addAction("💾  Save As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);

    fileMenu->addSeparator();

    renameAction = fileMenu->addAction("✏️  Rename Karo");
    connect(renameAction, &QAction::triggered, this, &MainWindow::renameFile);

    deleteAction = fileMenu->addAction("🗑️  Delete Karo");
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteFile);

    fileMenu->addSeparator();

    QAction *logoutMenuAction = fileMenu->addAction("🚪  Logout");
    connect(logoutMenuAction, &QAction::triggered, this, &MainWindow::logout);

    QAction *exitAction = fileMenu->addAction("❌  Exit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    // Edit Menu
    QMenu *editMenu = menuBar()->addMenu("✏️  Edit");
    editMenu->addAction("↩  Undo", textEditor, &QTextEdit::undo, QKeySequence::Undo);
    editMenu->addAction("↪  Redo", textEditor, &QTextEdit::redo, QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction("✂️  Cut", textEditor, &QTextEdit::cut, QKeySequence::Cut);
    editMenu->addAction("📋  Copy", textEditor, &QTextEdit::copy, QKeySequence::Copy);
    editMenu->addAction("📌  Paste", textEditor, &QTextEdit::paste, QKeySequence::Paste);
    editMenu->addSeparator();
    editMenu->addAction("🔍  Select All", textEditor, &QTextEdit::selectAll, QKeySequence::SelectAll);

    // Help Menu
    QMenu *helpMenu = menuBar()->addMenu("❓  Help");
    connect(helpMenu->addAction("ℹ️  About"), &QAction::triggered, this, &MainWindow::showAbout);
}

// =====================================================
//  setupToolBar - Toolbar buttons
// =====================================================
void MainWindow::setupToolBar() {
    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);
    toolBar->setObjectName("mainToolBar");
    toolBar->setIconSize(QSize(20, 20));

    QAction *tbNew = toolBar->addAction("🆕  New");
    tbNew->setToolTip("Nai file banao (Ctrl+N)");
    connect(tbNew, &QAction::triggered, this, &MainWindow::newFile);

    toolBar->addSeparator();

    QAction *tbSave = toolBar->addAction("💾  Save");
    tbSave->setToolTip("File save karo (Ctrl+S)");
    connect(tbSave, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *tbSaveAs = toolBar->addAction("💾+  Save As");
    tbSaveAs->setToolTip("Naye naam se save karo");
    connect(tbSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);

    toolBar->addSeparator();

    QAction *tbRename = toolBar->addAction("✏️  Rename");
    tbRename->setToolTip("File ka naam badlo");
    connect(tbRename, &QAction::triggered, this, &MainWindow::renameFile);

    QAction *tbDelete = toolBar->addAction("🗑️  Delete");
    tbDelete->setToolTip("File delete karo");
    connect(tbDelete, &QAction::triggered, this, &MainWindow::deleteFile);

    toolBar->addSeparator();

    // Spacer
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(spacer);

    QAction *tbLogout = toolBar->addAction("🚪  Logout");
    tbLogout->setToolTip("Logout karo");
    connect(tbLogout, &QAction::triggered, this, &MainWindow::logout);
}

// =====================================================
//  setupSidebar - Left panel with file list
// =====================================================
void MainWindow::setupSidebar() {
    sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(250);

    QVBoxLayout *layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Sidebar header
    QWidget *sidebarHeader = new QWidget();
    sidebarHeader->setObjectName("sidebarHeader");
    sidebarHeader->setFixedHeight(50);
    QHBoxLayout *headerLayout = new QHBoxLayout(sidebarHeader);
    headerLayout->setContentsMargins(15, 10, 15, 10);

    sidebarTitle = new QLabel("📂  Meri Files");
    sidebarTitle->setObjectName("sidebarTitle");

    newFileBtn = new QPushButton("+");
    newFileBtn->setObjectName("sidebarNewBtn");
    newFileBtn->setToolTip("Nai file banao");
    newFileBtn->setFixedSize(28, 28);
    newFileBtn->setCursor(Qt::PointingHandCursor);
    connect(newFileBtn, &QPushButton::clicked, this, &MainWindow::newFile);

    headerLayout->addWidget(sidebarTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(newFileBtn);

    // File list
    fileList = new QListWidget();
    fileList->setObjectName("fileList");
    fileList->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(fileList, &QListWidget::itemClicked, this, &MainWindow::onFileSelected);

    layout->addWidget(sidebarHeader);
    layout->addWidget(fileList);

    // Files refresh karo
    refreshFileList();
}

// =====================================================
//  setupEditor - Text Editor Area
// =====================================================
void MainWindow::setupEditor() {
    textEditor = new QTextEdit();
    textEditor->setObjectName("textEditor");
    textEditor->setPlaceholderText(
        "Yahaan type karo... ✍️\n\n"
        "💡 Tips:\n"
        "  • Ctrl+N  → Nai file\n"
        "  • Ctrl+S  → Save karo\n"
        "  • Ctrl+Z  → Undo\n"
        "  • Sidebar se file open karo"
    );

    QFont editorFont("Consolas", 13);
    editorFont.setStyleHint(QFont::Monospace);
    textEditor->setFont(editorFont);

    connect(textEditor, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
}

// =====================================================
//  setupStatusBar - Bottom status bar
// =====================================================
void MainWindow::setupStatusBar() {
    statusBar()->setObjectName("mainStatusBar");
}

// =====================================================
//  SLOT: newFile - CREATE operation
// =====================================================
void MainWindow::newFile() {
    if (!confirmSave()) return;

    bool ok;
    QString filename = QInputDialog::getText(
        this,
        "Nai File Banao",
        "File ka naam daalo (bina .txt ke):",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (!ok || filename.trimmed().isEmpty()) return;

    filename = filename.trimmed();

    // Invalid characters check
    if (filename.contains('/') || filename.contains('\\') ||
        filename.contains(':') || filename.contains('*')) {
        QMessageBox::warning(this, "Galat Naam",
            "File ke naam mein / \\ : * characters use mat karo!");
        return;
    }

    if (fileManager->fileExists(filename)) {
        QMessageBox::warning(this, "File Already Hai",
            QString("'%1' naam ki file already hai!\nKoi aur naam use karo.").arg(filename));
        return;
    }

    // File create karo
    if (fileManager->createFile(filename)) {
        clearEditor();
        setCurrentFile(filename);
        isNewFile = false; // File already disk par hai
        textEditor->setFocus();
        refreshFileList();

        // Sidebar mein nai file select karo
        QList<QListWidgetItem*> items = fileList->findItems(filename + ".txt", Qt::MatchExactly);
        if (!items.isEmpty()) fileList->setCurrentItem(items.first());

        showStatus(QString("✅ '%1.txt' file ban gayi!").arg(filename));
        autoSaveTimer->start();
    } else {
        QMessageBox::critical(this, "Error", "File nahi ban saki! Dobara try karo.");
    }
}

// =====================================================
//  SLOT: openFile (sidebar click se) - READ operation
// =====================================================
void MainWindow::onFileSelected(QListWidgetItem *item) {
    if (!item) return;
    if (!confirmSave()) {
        // Purani file wapas select karo
        if (!currentFileName.isEmpty()) {
            QList<QListWidgetItem*> items = fileList->findItems(
                currentFileName + ".txt", Qt::MatchExactly);
            if (!items.isEmpty()) fileList->setCurrentItem(items.first());
        }
        return;
    }

    // Filename from list (without .txt)
    QString filename = item->text();
    if (filename.endsWith(".txt")) filename = filename.chopped(4);

    loadFileInEditor(filename);
}

// =====================================================
//  openFile - Direct file open
// =====================================================
void MainWindow::openFile() {
    // Sidebar use karna better hai; ye menu se aya toh
    showStatus("💡 Sidebar se file par click karo kholine ke liye!");
}

// =====================================================
//  loadFileInEditor - File content editor mein laao
// =====================================================
void MainWindow::loadFileInEditor(const QString &filename) {
    QString content = fileManager->readFile(filename);
    clearEditor();
    setCurrentFile(filename);
    textEditor->setPlainText(content);
    isModified = false;
    updateWordCount();
    autoSaveTimer->start();
    showStatus(QString("📂 '%1.txt' khul gayi!").arg(filename));
}

// =====================================================
//  SLOT: saveFile - UPDATE operation
// =====================================================
void MainWindow::saveFile() {
    if (currentFileName.isEmpty()) {
        saveFileAs();
        return;
    }

    QString content = textEditor->toPlainText();

    if (fileManager->updateFile(currentFileName, content)) {
        isModified = false;
        setWindowTitle(QString("📝 TextEditor Pro — %1").arg(currentFileName + ".txt"));
        showStatus(QString("💾 '%1.txt' save ho gayi! ✅").arg(currentFileName));
    } else {
        QMessageBox::critical(this, "Save Error", "File save nahi ho saki!");
    }
}

// =====================================================
//  SLOT: saveFileAs - Save with new name
// =====================================================
void MainWindow::saveFileAs() {
    bool ok;
    QString newName = QInputDialog::getText(
        this,
        "Save As",
        "Naya file naam daalo:",
        QLineEdit::Normal,
        currentFileName,
        &ok
    );

    if (!ok || newName.trimmed().isEmpty()) return;
    newName = newName.trimmed();

    if (fileManager->fileExists(newName)) {
        int ret = QMessageBox::question(this, "File Already Hai",
            QString("'%1.txt' already hai. Overwrite karo?").arg(newName),
            QMessageBox::Yes | QMessageBox::No);
        if (ret != QMessageBox::Yes) return;
    }

    // Nai file create aur save karo
    QString content = textEditor->toPlainText();
    fileManager->createFile(newName, content); // Create karo (agar nahi hai)
    fileManager->updateFile(newName, content); // Content update karo

    setCurrentFile(newName);
    isModified = false;
    refreshFileList();

    // Sidebar mein select karo
    QList<QListWidgetItem*> items = fileList->findItems(newName + ".txt", Qt::MatchExactly);
    if (!items.isEmpty()) fileList->setCurrentItem(items.first());

    showStatus(QString("💾 '%1.txt' ke naam se save ho gayi!").arg(newName));
}

// =====================================================
//  SLOT: deleteFile - DELETE operation
// =====================================================
void MainWindow::deleteFile() {
    if (currentFileName.isEmpty()) {
        QMessageBox::information(this, "Koi File Nahi",
            "Pehle sidebar se ek file select karo!");
        return;
    }

    int ret = QMessageBox::question(
        this,
        "File Delete Karo",
        QString("Kya aap '%1.txt' delete karna chahte ho?\n\n"
                "⚠️ Ye action undo nahi ho sakta!").arg(currentFileName),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (ret != QMessageBox::Yes) return;

    if (fileManager->deleteFile(currentFileName)) {
        showStatus(QString("🗑️ '%1.txt' delete ho gayi!").arg(currentFileName));
        clearEditor();
        currentFileName = "";
        fileNameLabel->setText("📄  Koi file nahi khuli");
        refreshFileList();
        autoSaveTimer->stop();
    } else {
        QMessageBox::critical(this, "Delete Error", "File delete nahi ho saki!");
    }
}

// =====================================================
//  SLOT: renameFile - Rename file
// =====================================================
void MainWindow::renameFile() {
    if (currentFileName.isEmpty()) {
        QMessageBox::information(this, "Koi File Nahi",
            "Pehle sidebar se ek file select karo!");
        return;
    }

    bool ok;
    QString newName = QInputDialog::getText(
        this,
        "File Rename Karo",
        "Naya naam daalo:",
        QLineEdit::Normal,
        currentFileName,
        &ok
    );

    if (!ok || newName.trimmed().isEmpty()) return;
    newName = newName.trimmed();

    if (newName == currentFileName) return;

    if (fileManager->fileExists(newName)) {
        QMessageBox::warning(this, "File Already Hai",
            QString("'%1.txt' naam ki file already hai!").arg(newName));
        return;
    }

    // Pehle save karo agar modified hai
    if (isModified) saveFile();

    if (fileManager->renameFile(currentFileName, newName)) {
        showStatus(QString("✏️ '%1.txt' ka naam '%2.txt' ho gaya!").arg(currentFileName).arg(newName));
        setCurrentFile(newName);
        refreshFileList();

        // Sidebar mein nai file select karo
        QList<QListWidgetItem*> items = fileList->findItems(newName + ".txt", Qt::MatchExactly);
        if (!items.isEmpty()) fileList->setCurrentItem(items.first());
    } else {
        QMessageBox::critical(this, "Rename Error", "File rename nahi ho saki!");
    }
}

// =====================================================
//  onTextChanged - Editor mein kuch type hua
// =====================================================
void MainWindow::onTextChanged() {
    if (!isModified) {
        isModified = true;
        if (!currentFileName.isEmpty()) {
            setWindowTitle(QString("📝 TextEditor Pro — %1.txt •").arg(currentFileName));
        }
    }
    updateWordCount();
}

// =====================================================
//  autoSave - Auto save karo
// =====================================================
void MainWindow::autoSave() {
    if (isModified && !currentFileName.isEmpty()) {
        fileManager->updateFile(currentFileName, textEditor->toPlainText());
        isModified = false;
        showStatus(QString("⚡ Auto-saved: %1.txt").arg(currentFileName));
        setWindowTitle(QString("📝 TextEditor Pro — %1.txt").arg(currentFileName));
    }
}

// =====================================================
//  logout - Logout karo
// =====================================================
void MainWindow::logout() {
    if (isModified) {
        int ret = QMessageBox::question(this, "Logout",
            "Unsaved changes hain. Logout se pehle save karo?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) saveFile();
        else if (ret == QMessageBox::Cancel) return;
    }

    autoSaveTimer->stop();
    emit logoutRequested();
}

// =====================================================
//  showAbout - About dialog
// =====================================================
void MainWindow::showAbout() {
    QMessageBox::about(this, "TextEditor Pro ke baare mein",
        "<h2>📝 TextEditor Pro</h2>"
        "<p><b>Version:</b> 1.0</p>"
        "<p><b>Technology:</b> C++ with Qt Framework</p>"
        "<br>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>✅ Secure Login & Registration</li>"
        "<li>✅ File Create (C)</li>"
        "<li>✅ File Read/Open (R)</li>"
        "<li>✅ File Update/Save (U)</li>"
        "<li>✅ File Delete (D)</li>"
        "<li>✅ Auto-Save</li>"
        "<li>✅ Word Count</li>"
        "</ul>"
        "<br>"
        "<p>Banaya gaya hai 1st Year C++ Project ke liye 🎓</p>"
    );
}

// =====================================================
//  updateWordCount - Word aur character count
// =====================================================
void MainWindow::updateWordCount() {
    QString text = textEditor->toPlainText();
    int chars = text.length();
    int words = text.isEmpty() ? 0 : text.split(QRegularExpression("\\s+"),
                                    Qt::SkipEmptyParts).count();
    int lines = text.isEmpty() ? 0 : text.count('\n') + 1;
    wordCountLabel->setText(
        QString("Lines: %1  |  Words: %2  |  Chars: %3")
        .arg(lines).arg(words).arg(chars)
    );
}

// =====================================================
//  closeEvent - Window band karne se pehle check
// =====================================================
void MainWindow::closeEvent(QCloseEvent *event) {
    if (!confirmSave()) {
        event->ignore();
        return;
    }
    autoSaveTimer->stop();
    event->accept();
}

// =====================================================
//  Helper: refreshFileList - Sidebar update karo
// =====================================================
void MainWindow::refreshFileList() {
    fileList->clear();
    QStringList files = fileManager->listFiles();

    if (files.isEmpty()) {
        QListWidgetItem *emptyItem = new QListWidgetItem("📭  Koi file nahi hai...");
        emptyItem->setFlags(Qt::NoItemFlags);
        emptyItem->setForeground(QColor("#666"));
        fileList->addItem(emptyItem);
    } else {
        for (const QString &file : files) {
            QListWidgetItem *item = new QListWidgetItem("📄  " + file);
            item->setData(Qt::UserRole, file); // Real name store karo
            fileList->addItem(item);
        }
    }
}

// =====================================================
//  Helper: clearEditor - Editor saaf karo
// =====================================================
void MainWindow::clearEditor() {
    textEditor->clear();
    isModified = false;
    autoSaveTimer->stop();
}

// =====================================================
//  Helper: confirmSave - Unsaved changes check
// =====================================================
bool MainWindow::confirmSave() {
    if (!isModified || currentFileName.isEmpty()) return true;

    int ret = QMessageBox::question(
        this,
        "Unsaved Changes",
        QString("'%1.txt' mein unsaved changes hain.\nSave karo?").arg(currentFileName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (ret == QMessageBox::Save) {
        saveFile();
        return true;
    } else if (ret == QMessageBox::Discard) {
        isModified = false;
        return true;
    }
    return false; // Cancel
}

// =====================================================
//  Helper: setCurrentFile - Current file set karo
// =====================================================
void MainWindow::setCurrentFile(const QString &filename) {
    currentFileName = filename;
    fileNameLabel->setText(QString("📄  %1.txt").arg(filename));
    setWindowTitle(QString("📝 TextEditor Pro — %1.txt").arg(filename));
}

// =====================================================
//  Helper: showStatus - Status bar message
// =====================================================
void MainWindow::showStatus(const QString &message) {
    statusBar()->showMessage(message, 4000);
}

// =====================================================
//  applyStyles - Complete Dark Theme
// =====================================================
void MainWindow::applyStyles() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1a1a2e;
        }

        QMenuBar {
            background-color: #16213e;
            color: #c0c0d0;
            padding: 4px;
            font-size: 13px;
            border-bottom: 1px solid #2d3561;
        }

        QMenuBar::item:selected {
            background-color: #e94560;
            color: white;
            border-radius: 4px;
        }

        QMenu {
            background-color: #16213e;
            color: #c0c0d0;
            border: 1px solid #2d3561;
            font-size: 13px;
        }

        QMenu::item:selected {
            background-color: #e94560;
            color: white;
        }

        QMenu::separator {
            height: 1px;
            background: #2d3561;
            margin: 3px 0;
        }

        #mainToolBar {
            background-color: #16213e;
            border-bottom: 2px solid #2d3561;
            spacing: 5px;
            padding: 5px;
        }

        QToolBar QToolButton {
            background-color: transparent;
            color: #c0c0d0;
            border: 1px solid transparent;
            border-radius: 6px;
            padding: 6px 12px;
            font-size: 13px;
        }

        QToolBar QToolButton:hover {
            background-color: rgba(233, 69, 96, 0.2);
            border: 1px solid #e94560;
            color: #e94560;
        }

        QToolBar::separator {
            background: #2d3561;
            width: 1px;
            margin: 5px;
        }

        #sidebar {
            background-color: #16213e;
            border-right: 2px solid #2d3561;
        }

        #sidebarHeader {
            background-color: #0f3460;
            border-bottom: 1px solid #2d3561;
        }

        #sidebarTitle {
            color: #e0e0e0;
            font-size: 14px;
            font-weight: bold;
        }

        #sidebarNewBtn {
            background-color: #e94560;
            color: white;
            border: none;
            border-radius: 14px;
            font-size: 18px;
            font-weight: bold;
        }

        #sidebarNewBtn:hover {
            background-color: #ff6b7a;
        }

        #fileList {
            background-color: #16213e;
            color: #c0c0d0;
            border: none;
            font-size: 13px;
            outline: none;
        }

        #fileList::item {
            padding: 10px 15px;
            border-bottom: 1px solid #1e2a3a;
        }

        #fileList::item:hover {
            background-color: rgba(233, 69, 96, 0.1);
            color: #e0e0e0;
        }

        #fileList::item:selected {
            background-color: rgba(233, 69, 96, 0.25);
            color: #ff6b7a;
            border-left: 3px solid #e94560;
        }

        #headerBar {
            background-color: #0f3460;
            border-bottom: 1px solid #2d3561;
        }

        #fileNameLabel {
            color: #e0e0e0;
            font-size: 14px;
            font-weight: bold;
        }

        #wordCountLabel {
            color: #7f8c8d;
            font-size: 12px;
        }

        #textEditor {
            background-color: #1a1a2e;
            color: #e0e0e0;
            border: none;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 14px;
            line-height: 1.6;
            padding: 20px;
            selection-background-color: #e94560;
            selection-color: white;
        }

        #mainStatusBar {
            background-color: #0f3460;
            color: #a0b0c0;
            font-size: 12px;
            border-top: 1px solid #2d3561;
        }

        QMessageBox {
            background-color: #1a1a2e;
            color: #e0e0e0;
        }

        QMessageBox QPushButton {
            background-color: #e94560;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 20px;
            font-size: 13px;
            min-width: 80px;
        }

        QMessageBox QPushButton:hover {
            background-color: #ff6b7a;
        }

        QInputDialog {
            background-color: #1a1a2e;
        }

        QInputDialog QLineEdit {
            background-color: #16213e;
            border: 2px solid #2d3561;
            border-radius: 8px;
            padding: 8px;
            color: #e0e0e0;
        }

        QInputDialog QPushButton {
            background-color: #e94560;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 20px;
            min-width: 80px;
        }

        QScrollBar:vertical {
            background: #1a1a2e;
            width: 10px;
        }

        QScrollBar::handle:vertical {
            background: #2d3561;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background: #e94560;
        }

        QSplitter::handle {
            background: #2d3561;
        }
    )");
}
