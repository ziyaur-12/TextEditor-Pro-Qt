# 📝 TextEditor Pro — C++ Qt Project

## 🎓 1st Year C++ College Project

**Technology:** C++ with Qt 5/6 Framework  
**Features:** Login System + File CRUD Operations + GUI

---

## 📁 Project Files

```
TextEditorProject/
├── main.cpp           → Program ka starting point
├── loginwindow.h      → Login/Register window header
├── loginwindow.cpp    → Login/Register UI aur logic
├── mainwindow.h       → Main text editor header
├── mainwindow.cpp     → Text editor UI aur CRUD operations
├── usermanager.h      → User authentication header
├── usermanager.cpp    → Login/Register logic
├── filemanager.h      → File CRUD operations header
├── filemanager.cpp    → File Create/Read/Update/Delete
└── TextEditor.pro     → Qt project file
```

---

## 🚀 Setup aur Run Kaise Kare

### Option 1: Qt Creator (Sabse Aasan)

1. **Qt install karo:**
   - https://www.qt.io/download-qt-installer se Qt 5.15+ ya Qt 6.x install karo
   - "MinGW" ya "MSVC" compiler select karo

2. **Project kholo:**
   - Qt Creator open karo
   - File → Open File or Project
   - `TextEditor.pro` file select karo

3. **Run karo:**
   - Green "Play" button dabao (Ctrl+R)

---

### Option 2: Command Line (Linux/Mac)

```bash
# Qt install karo
sudo apt-get install qt5-default qtbase5-dev  # Ubuntu
# ya
brew install qt5  # Mac

# Project folder mein jao
cd TextEditorProject

# Build karo
qmake TextEditor.pro
make

# Run karo
./TextEditor
```

---

### Option 3: Command Line (Windows)

```cmd
# Qt Creator install karo from qt.io
# PATH mein Qt tools add karo

cd TextEditorProject
qmake TextEditor.pro
mingw32-make

TextEditor.exe
```

---

## 🔐 Features Detail

### 1. LOGIN SYSTEM
- **Register:** Naya account banao (username + password)
- **Login:** Existing account se login karo
- **Security:** Password hashing (XOR-based)
- **Validation:** Empty fields, short password check
- **Persistence:** `users.dat` file mein save hota hai

### 2. FILE CRUD OPERATIONS

| Operation | Matlab | Kaise Karo |
|-----------|--------|------------|
| **C** - Create | Nai file banana | "New" button ya Ctrl+N |
| **R** - Read | File padhna/kholna | Sidebar mein file par click |
| **U** - Update | File save karna | "Save" button ya Ctrl+S |
| **D** - Delete | File delete karna | "Delete" button |
| Extra | Rename | "Rename" button |

### 3. ADDITIONAL FEATURES
- ✅ Auto-Save (har 2 minute mein)
- ✅ Word Count, Line Count, Character Count
- ✅ Undo/Redo (Ctrl+Z / Ctrl+Y)
- ✅ Cut/Copy/Paste support
- ✅ Unsaved changes warning
- ✅ Har user ka apna alag folder (`/files/username/`)
- ✅ Dark Theme GUI

---

## 📂 Data Storage

Program run karne ke baad ye files/folders bante hain:

```
TextEditorProject/
├── users.dat          → Sab users ka data (auto-created)
└── files/
    ├── alice/         → Alice ki files
    │   ├── notes.txt
    │   └── homework.txt
    └── bob/           → Bob ki files
        └── todo.txt
```

---

## 🏗️ Project Architecture

```
┌─────────────────────────────────────┐
│           main.cpp                  │
│   (App start, Window management)    │
└──────────────┬──────────────────────┘
               │
    ┌──────────┴──────────┐
    │                     │
    ▼                     ▼
┌─────────────┐    ┌─────────────┐
│ LoginWindow │    │ MainWindow  │
│  (Login UI) │    │ (Editor UI) │
└──────┬──────┘    └──────┬──────┘
       │                  │
       ▼                  ▼
┌─────────────┐    ┌─────────────┐
│ UserManager │    │ FileManager │
│ (Auth Logic)│    │(CRUD Logic) │
└─────────────┘    └─────────────┘
       │                  │
       ▼                  ▼
  users.dat           /files/
  (User data)      (User folders)
```

---

## 📚 C++ Concepts Used

1. **Classes & Objects** - UserManager, FileManager, etc.
2. **Inheritance** - QMainWindow se inherit karna
3. **Signals & Slots** - Qt ka event system
4. **File I/O** - QFile, QTextStream
5. **STL** - QString, QMap, QList, QStringList
6. **Polymorphism** - Virtual functions (closeEvent)
7. **Memory Management** - new/delete, smart pointers
8. **OOP Principles** - Encapsulation, Abstraction

---

*Banaya gaya: 1st Year C++ Project — TextEditor Pro*
