#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(600,400);
    this->setWindowTitle(tr("记事本"));
    this->setWindowIcon(QIcon(":/window.ico"));

    //文本编辑区
    textEdit = new QTextEdit();
    textEdit->setFont(QFont("微软雅黑",12));
    setCentralWidget(textEdit);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth); //默认自动换行

    //状态栏
    textInfo = new QLabel();
    textInfo->setText("StatusBar");
    connect(textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(showTextInfo()));
    statusBar()->addPermanentWidget(textInfo);

    //菜单栏
    File = new QMenu("文件(&F) ");
    Edit = new QMenu("编辑(&E) ");
    Format = new QMenu("格式(&O) ");
    View = new QMenu("查看(&V) ");
    Help = new QMenu("帮助(&H) ");
    menuBar()->addMenu(File);
    menuBar()->addMenu(Edit);
    menuBar()->addMenu(Format);
    menuBar()->addMenu(View);
    menuBar()->addMenu(Help);

    //文件
    New = new QAction("新建(N)");
    New->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    Open = new QAction("打开(O)...");
    Open->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    Save = new QAction("保存(S)");
    Save->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    SaveAs = new QAction("另存为(A)...");
    Exit = new QAction("退出(X)");

    File->addAction(New);
    File->addAction(Open);
    File->addAction(Save);
    File->addAction(SaveAs);
    File->addSeparator();
    File->addAction(Exit);
    //文件信号和槽
    connect(New,SIGNAL(triggered()),this,SLOT(slotNew()));
    connect(Open,SIGNAL(triggered()),this,SLOT(slotOpen()));
    connect(Save,SIGNAL(triggered()),this,SLOT(slotSave()));
    connect(SaveAs,SIGNAL(triggered()),this,SLOT(slotSaveAs()));
    connect(Exit,SIGNAL(triggered()),this,SLOT(slotExit()));


    //编辑
    Canel = new QAction("撤销(U)");
    Canel->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    Redo = new QAction("重做(R)");
    Redo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    Cut = new QAction("剪切(T)");
    Cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    Copy = new QAction("复制(C)");
    Copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    Paste = new QAction("粘贴(P)");
    Paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    Delete = new QAction("删除(L)");
    Delete->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));

    Edit->addAction(Canel);
    Edit->addAction(Redo);
    Edit->addSeparator();
    Edit->addAction(Cut);
    Edit->addAction(Copy);
    Edit->addAction(Paste);
    Edit->addAction(Delete);
    //编辑信号和槽
    connect(Canel,SIGNAL(triggered()),this,SLOT(slotCanel()));
    connect(Redo,SIGNAL(triggered()),this,SLOT(slotRedo()));
    connect(Cut,SIGNAL(triggered()),this,SLOT(slotCut()));
    connect(Copy,SIGNAL(triggered()),this,SLOT(slotCopy()));
    connect(Paste,SIGNAL(triggered()),this,SLOT(slotPaste()));
    connect(Delete,SIGNAL(triggered()),this,SLOT(slotDelete()));

    //格式
    AutoLineBreak = new QAction("自动换行(W)");
    AutoLineBreak->setCheckable(true);
    AutoLineBreak->setChecked(true);
    Font = new QAction("字体(F)...");

    Format->addAction(AutoLineBreak);
    Format->addAction(Font);
    //格式信号和槽
    connect(AutoLineBreak,SIGNAL(triggered()),this,SLOT(slotAutoLineBreak()));
    connect(Font,SIGNAL(triggered()),this,SLOT(slotFont()));

    //查看
    StatusBar = new QAction("状态栏(S)");
    StatusBar->setCheckable(true);
    StatusBar->setChecked(true);

    View->addAction(StatusBar);
    //查看信号和槽
    connect(StatusBar,SIGNAL(triggered()),this,SLOT(slotStatusBar()));


    //帮助
    ViewHelp = new QAction("查看帮助(H)");
    About = new QAction("关于记事本(A)");

    Help->addAction(ViewHelp);
    Help->addSeparator();
    Help->addAction(About);
    //帮助信号和槽
    connect(About,SIGNAL(triggered()),SLOT(slotAbout()));
    connect(ViewHelp,SIGNAL(triggered()),SLOT(slotViewHelp()));
}

//状态栏槽函数
void MainWindow::showTextInfo()
{
    QTextCursor textCursor = textEdit->textCursor();//获得鼠标光标
    int lineNum = textCursor.blockNumber();//行数
    int colNum = textCursor.columnNumber();//列数
    textInfo->setText(tr("行:%1,列:%2").arg(lineNum+1).arg(colNum));//显示在Label上，注意：行数是从0开始的
}


//文件槽函数
bool MainWindow::maybeSave()
{
    if(textEdit->document()->isModified())
    {//判断文件是否被修改
        QMessageBox::StandardButtons result;
        result = QMessageBox::warning(this,"记事本","是否保存文件?",
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if(result == QMessageBox::Cancel)
        {//取消
            return false;
        }
        if(result == QMessageBox::Save)
        {//保存
           return slotSave();
        }
        if(result == QMessageBox::Discard)
        {//忽略，即不保存
            return true;
        }
    }
    else
    {
        return true;
    }
}
bool MainWindow::saveFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this,"critical","cannot write file");
        return false;
    }
    else
    {
        QTextStream out(&file);
        out<<textEdit->toPlainText();
        setFileName(fileName);
        return true;
    }
}
void MainWindow::setFileName(QString fileName)
{
    currentFile = fileName;
    textEdit->document()->setModified(false);
    this->setWindowModified(false);
    fileName.isEmpty() ?
                this->setWindowFilePath("new.txt") :
                this->setWindowFilePath(fileName);
}
void MainWindow::loadFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this,"critical","cannot read file");
    }
    else
    {
        QTextStream in(&file);
        textEdit->setText(in.readAll());
        setFileName(fileName);
     }
}
//新建
void MainWindow::slotNew()
{
    if(maybeSave())
    {
        textEdit->clear();
        setFileName("");
    }
}
//打开
void MainWindow::slotOpen()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
        }
    }
}
//保存
bool MainWindow::slotSave()
{
    if(currentFile.isEmpty())
    {
        return slotSaveAs();
    }
    else
    {
        return saveFile(currentFile);
    }
}
//另存为
bool MainWindow::slotSaveAs()
{
    QString fileName =QFileDialog::getSaveFileName(this,tr("Save As"),
                                "/home/",tr("Text Files (*.txt)"));
    if(fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return saveFile(fileName);
    }
}
//退出
void MainWindow::slotExit()
{
    if(maybeSave())
    {
        this->close();//关闭所有窗口
    }
}


//编辑槽函数
void MainWindow::slotCanel()
{
    textEdit->undo();
}
void MainWindow::slotRedo()
{
    textEdit->redo();
}
void MainWindow::slotCut()
{
    textEdit->cut();
}
void MainWindow::slotCopy()
{
    textEdit->copy();
}
void MainWindow::slotPaste()
{
    textEdit->paste();
}
void MainWindow::slotDelete()
{
    textEdit->clear();
}


//格式槽函数
void MainWindow::slotAutoLineBreak()
{
    if(AutoLineBreak->isChecked())
    {
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    }
    else
    {
        textEdit->setLineWrapMode(QTextEdit::NoWrap);
    }
}
void MainWindow::slotFont()
{
    QFontDialog fontDlg; //字体选择对话框，可以选择字体，字体样式，大小
    QFont font;
    bool isChanged;
    font = fontDlg.getFont(&isChanged);
    if(isChanged)
    {
        textEdit->setFont(font);
    }
}


//查看槽函数
void MainWindow::slotStatusBar()
{
    if(StatusBar->isChecked())
    {
        statusBar()->show();
    }
    else
    {
        statusBar()->hide();
    }
}


//帮助槽函数
void MainWindow::slotAbout()
{
    QMessageBox::about(this,"关于记事本","This software was made By Georgehu.\nAll rights reserved!");
}
void MainWindow::slotViewHelp()
{
    QString URL = "http://georgehu.top/?page_id=186";
    QDesktopServices::openUrl(QUrl(URL.toLatin1()));
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(maybeSave())
    {
        e->accept();
    }
    else
    {
        e->ignore();//忽略，直接关闭
    }
}
MainWindow::~MainWindow()
{

}
