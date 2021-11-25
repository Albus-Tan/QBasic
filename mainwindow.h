#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "program.h"
#include "evalstate.h"

extern Program program;

// 用于表明此时程序是否在运行
extern ProgramState programState;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void processInput(std::string cmd);  // 处理用户在交互行的输入
    void processCmd(std::string cmd);  // 针对无行号的情况，判断命令种类，并立即执行
    void afterCmdLineEditFinished();  // 在用户输入结束之后调用此函数

public:
    void updateCodeDisplay();  // 更新 CodeDisplay 中显示的代码

private slots:
    void on_cmdLineEdit_editingFinished();
    void on_btnLoadCode_clicked();
    void on_btnClearCode_clicked();
    void on_btnRunCode_clicked();
    void on_cmdLineEdit_returnPressed();

public:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
