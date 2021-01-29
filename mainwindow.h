#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qmenubar.h>//菜单栏
#include <qmenu.h>
#include <QSplitter>
#include <QHBoxLayout>
#include <qaction.h>
#include <qaction.h>
#include <qstring.h>
#include <qtextedit.h>
#include <qplaintextedit.h>
#include <Qsci/qsciscintilla.h>
#include "Qsci/qscilexercpp.h"
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qsciapis.h>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QTextEdit>
#include<QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QtWidgets>
#include <QDialog>
#include "finddialog.h"
#include "debugdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QAction;
class QMenu;
class QsciScintilla;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void CreatUi();


    void setLexer(const QString &);//设置不同语言的词法分析器
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void setUtf8();
    void setAnsi();
    virtual void keyPressEvent(QKeyEvent *event);
    void showFindDialog();//显示查找框
    void searchText(const QString &text,Qt::CaseSensitivity cs);
    void replaceText(const QString &text,Qt::CaseSensitivity cs);
    void compile();
    void run();

private:
    Ui::MainWindow *ui;
    /*QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editorMenu;
    QMenu *formatMenu;
    QMenu *selectMenu;
    QMenu *helpMenu;*/

   // QTextEdit *codeEdit;
   // QPlainTextEdit  *codeEdit;
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();





    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QsciScintilla *editor;
    QsciAPIs *apis;

    QString curFile;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *codeMenu;
    QMenu *settingMenu;
    QMenu *buildMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;

    QAction *unicodeAct;
    QAction *ansiAct;
    QAction *utf8Act;

   // QAction *xkhAct;


    QAction *undoAct; //撤销
    QAction *redoAct;  //重做

    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QAction *searchAct;//查找
    QAction *replaceAct;
    //QAction *openFileAct;
   // QAction *openProAct;
    FindDialog *findDialog;//前置声明一个查找框的实例化
    DebugDialog *debugDialog;

    QAction *compileAct;
    QAction *runAct;

};

#endif // MAINWINDOW_H
