#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QTextEdit>
#include <QLabel>
#include <QTextCursor>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDesktopServices>
#include <QFontDialog>
#include <QCloseEvent>

#pragma execution_character_set("utf-8") //设置字符编码


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTextEdit *textEdit;
    QLabel *textInfo;
    QString currentFile;

    //菜单栏
    QMenu *File;
    QMenu *Edit;
    QMenu *Format;
    QMenu *View;
    QMenu *Help;

    //文件
    QAction *New;
    QAction *Open;
    QAction *Save;
    QAction *SaveAs;
    QAction *Exit;

    //编辑
    QAction *Canel;
    QAction *Redo;
    QAction *Cut;
    QAction *Copy;
    QAction *Paste;
    QAction *Delete;

    //格式
    QAction *AutoLineBreak;
    QAction *Font;

    //查看
    QAction *StatusBar;

    //帮助
    QAction *ViewHelp;
    QAction *About;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    void showTextInfo();//状态栏槽函数
    //文件槽函数
    bool maybeSave();
    bool saveFile(QString fileName);
    void setFileName(QString fileName);
    void loadFile(QString fileName);
    void slotNew();
    void slotOpen();
    bool slotSave();
    bool slotSaveAs();
    void slotExit();
    //编辑槽函数
    void slotCanel();
    void slotRedo();
    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotDelete();
    //格式槽函数
    void slotAutoLineBreak();
    void slotFont();
    //查看槽函数
    void slotStatusBar();
    //帮助槽函数
    void slotAbout();
    void slotViewHelp();
};

#endif // MAINWINDOW_H
