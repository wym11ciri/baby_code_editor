#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QVBoxLayout"
#include <QPlainTextEdit>
#include <iconv.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    editor = new QsciScintilla;//编辑器
    setCentralWidget(editor);
    CreatUi();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();



    readSettings();

    connect(editor, SIGNAL(textChanged()),
            this, SLOT(documentWasModified()));

    setCurrentFile("");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::CreatUi()
{

    setWindowTitle(QString::fromLocal8Bit("**CODE EDITOR**"));
    this->resize(QSize(600,500));   //设置初始窗口大小
    QsciLexerCPP *textLexer=new QsciLexerCPP;
   // QsciLexerPython *textLexer = new QsciLexerPython;
    textLexer->setColor(QColor(0,100,0),QsciLexerCPP::CommentLine);    //设置自带的注释行为绿色
    textLexer->setFoldComments(true);//设置可以折叠多行注释
       editor->setLexer(textLexer);
        //行号提示
       editor->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
       editor->setMarginLineNumbers(0,true);//对该页边启用行号
       editor->setMarginWidth(0,30);//设置页边宽度
       editor->setMarginsForegroundColor(QColor(234, 234, 234));//边栏字体颜色
       editor->setMarginsBackgroundColor(QColor(72, 61, 139));//边栏字体背景
     //  editor->setAutoIndent(True);  // 换行后自动缩进

       editor->setFont(QFont("Courier 10 Pitch"));//设置显示字体
       //光标
       editor->setCaretWidth(2);//光标宽度，0表示不显示光标
       editor->setCaretLineVisible(true); //是否高亮显示光标所在行
       editor->setCaretLineBackgroundColor(QColor(255, 240, 245));//光标所在行背景颜色
       //折叠
       editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);//折叠样式
       editor->setFoldMarginColors(Qt::gray,Qt::lightGray);//折叠栏颜色

    //   editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);
       editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,
                            QsciScintilla::SC_CP_UTF8);

       //自动补全
       apis = new QsciAPIs(textLexer);
       if(!apis->load(QString("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/api_cpp.txt")))
             QMessageBox::warning(this,QString("warning"),QString("Failed to read code completion file"));
          else
             apis->prepare();
       editor->setAutoCompletionCaseSensitivity(true);
       editor->setAutoCompletionSource(QsciScintilla::AcsAll);//自动补全。对于所有Ascii字符
       editor->setAutoCompletionThreshold(1);//设置每输入一个字符就会出现自动补全的提示

       editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);//括号匹配
       editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SCI_GETSTATUS);
}





void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        editor->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);

}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("Created by 841 domitory ^_^\n"
               "The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void MainWindow::documentWasModified()
{
    setWindowModified( editor->isModified());
}

void MainWindow::showFindDialog()
{    findDialog = new FindDialog;
     connect(findDialog, SIGNAL(findPrevious(const QString &,Qt::CaseSensitivity)), this, SLOT(searchText(const QString &,Qt::CaseSensitivity)));
      connect(findDialog, SIGNAL(findNext(const QString &,Qt::CaseSensitivity)), this, SLOT(searchText(const QString &,Qt::CaseSensitivity)));
       connect(findDialog, SIGNAL(emitReplaceSignal(const QString &,Qt::CaseSensitivity)), this, SLOT(replaceText(const QString &,Qt::CaseSensitivity)));
       findDialog->show();
}

void MainWindow::searchText(const QString &text,Qt::CaseSensitivity cs)
{    // qDebug()<<"用到了";
      editor->findFirst(text,true,cs,true,true);
}
void MainWindow::replaceText(const QString &text,Qt::CaseSensitivity cs)
{     qDebug()<<"huan到了";
      editor->replace(text);
}


void MainWindow::setUtf8()//打开ansi文件可以显示中文，打开utf8文件中文是乱码
{

   editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,
                         //932);
                         QsciScintilla::SC_CP_UTF8);
   QMessageBox::about(this, tr("About Application"),
            tr("已经设为了utf8"));
   qDebug()<<"utf8bianle";
}

void MainWindow::setAnsi(){

    editor->SendScintilla(QsciScintilla::SCI_STYLESETCHARACTERSET,
                          6,
                          QsciScintilla::SC_CHARSET_ANSI);

    qDebug()<<"ansibianle";
}


void MainWindow::createActions()
{
    newAct = new QAction(QIcon("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/images/new.png"), tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), editor, SLOT(cut()));

    copyAct = new QAction(QIcon("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/images//copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()),  editor, SLOT(copy()));

    pasteAct = new QAction(QIcon("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()),  editor, SLOT(paste()));

    undoAct = new QAction( tr("&undo"), this);
    undoAct->setShortcut(tr("Ctrl+Z"));
    connect(undoAct, SIGNAL(triggered()),  editor, SLOT(undo()));

    redoAct = new QAction( tr("&Redo"), this);
    connect(redoAct, SIGNAL(triggered()),  editor, SLOT(redo()));

    aboutAct = new QAction(QIcon("E:/qt_projects/cross_platform_code_editor/BabyCodeEditor/images/about.png"),tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the code edtior's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    searchAct = new QAction(tr("search"), this);
    searchAct->setShortcut(tr("Ctrl+F"));
    connect(searchAct, SIGNAL(triggered()), this, SLOT(showFindDialog()));

    ansiAct = new QAction( tr("&ANSI"), this);
    connect(ansiAct, SIGNAL(triggered()), this, SLOT(setAnsi()));

    unicodeAct = new QAction( tr("&UNICODE"), this);

    utf8Act = new QAction( tr("&Utf8"), this);
    utf8Act->setStatusTip(tr("set utf8"));
    connect(utf8Act, SIGNAL(triggered()), this, SLOT(setUtf8()));

//    xkhAct = new QAction(  this);
//    xkhAct->setShortcut(tr("Shift+9"));
//    connect(xkhAct, SIGNAL(triggered()), this, SLOT(xkh()));
    compileAct = new QAction( tr("&Compile"), this);
    connect(compileAct, SIGNAL(triggered()), this, SLOT(compile()));

    runAct = new QAction( tr("&Run"), this);
    runAct->setShortcut(tr("Ctrl+R"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect( editor, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect( editor, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addAction(searchAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);



    settingMenu=menuBar()->addMenu(tr("&Setting"));
    QMenu* codeMenu = settingMenu->addMenu("setcode");
    codeMenu->addAction(utf8Act);
    codeMenu->addAction(ansiAct);
    codeMenu->addAction("Unicode",this,SLOT(actionsSlot()));
    QMenu* syntaxMenu = settingMenu->addMenu("setSyntax");
    syntaxMenu->addAction("C++",this,SLOT(actionsSlot()));

    buildMenu = menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(compileAct);
    buildMenu->addAction(runAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("Trolltech", "BabyCodeEditor");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("Trolltech", "BabyCodeEditor");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSave()
{
    if (editor->isModified()) {
        int ret = QMessageBox::warning(this, tr("BabyCodeEditor"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("BabyCodeEditor"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    editor->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("BabyCodeEditor"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << editor->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
     editor->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("BabyCodeEditor")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
//
void MainWindow::keyPressEvent(QKeyEvent *event){
    setFocusPolicy(Qt::StrongFocus);
 // qDebug()<<event->key()<<"lll";
    int line, index;
  if(event->key()==Qt::Key_ParenLeft)
  {
      editor->insert(tr("()"));

      editor->getCursorPosition(&line,&index);
      qDebug()<<line<<index<<endl;
      editor->setCursorPosition(line,index+1);
      qDebug()<<line<<index<<endl;
      //this->moveCursor(QTextCursor::PreviousCharacter);
    }
  event->ignore();
}

void MainWindow::compile()
{


    debugDialog = new DebugDialog;


    QFileInfo fileInfo;
    QString file, fileName, fileSuffix, filePath;
    fileInfo = QFileInfo(curFile);
    QString ss;

        //文件名
            fileName = fileInfo.fileName();
            //文件后缀
            fileSuffix = fileInfo.suffix();
            //绝对路径
            filePath = fileInfo.absolutePath();
         //   qDebug() << filePath <<endl;
            ss+=filePath;
            ss+="/";
            QString ename = fileName.replace(".cpp",".exe");
            ss+=ename;
    QString srcname = curFile;

    QString destname = srcname.replace(".cpp",".exe");
    QString command = "g++ "+curFile+" -o "+destname+" 2>debug.txt";
     QFile file0("debug.txt");
    qDebug()<<command;
    QProcess process(this);
    process.setProgram("cmd");
    QStringList argument;
    argument<<"/c"<<command;
    process.setArguments(argument);
        process.start();
        process.waitForStarted(); //等待程序启动
        process.waitForFinished();//等待程序关闭
        QString displayString;
        if(!file0.open(QIODevice::ReadOnly | QIODevice::Text))

        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while(!file0.atEnd())
        {
            QByteArray line = file0.readLine();
            QString str(line);
            qDebug()<< str;
            displayString.append(str);
        }
       // QString temp=QString::fromLocal8Bit(process.readAllStandardOutput()); //程序输出信息
        debugDialog->show();
        debugDialog->debugtext->setText(displayString);
}


void MainWindow::run()
{
    qDebug()<<"我在run";
    QString tname = curFile;
    QString exefile = curFile.replace(".cpp",".exe");
    system(exefile.toStdString().data());
    curFile = tname;
}


