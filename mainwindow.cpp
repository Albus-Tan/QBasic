#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 调用此函数，将会依据当前的 program 更新 CodeDisplay 中显示的代码
void MainWindow::updateCodeDisplay()
{
    ui->CodeDisplay->clear();
    for(auto it = program.program.begin(); it != program.program.end(); ++it){
        int line = it->first;
        std::string s = it->second->statement;
        ui->CodeDisplay->append(QString("%1%2").arg(line).arg(QString::fromStdString(s)));
    }
}

// 在用户输入结束之后调用此函数
void MainWindow::afterCmdLineEditFinished()
{
    QString cmd = ui->cmdLineEdit->text();
    std::string s = cmd.toStdString();
    try {
        processInput(s);
    } catch (const char* e) {
        std::cout << e << std::endl;
    } catch (int line) {
        std::cout << "Line " << line << "does not exist !" << std::endl;
    } catch (...) {
        std::cout << "Unknown error occurred !" << std::endl;
    }

    ui->cmdLineEdit->setText("");
    updateCodeDisplay();
}

void MainWindow::on_cmdLineEdit_returnPressed()
{
    if(programState == INPUT_STMT) afterCmdLineEditFinished();
    if(programState == INPUT_VAR) {  // 程序运行过程中 INPUT 语句的输入
        programState = RUNNING;  //获取输入完毕，修改状态
    }
}


void MainWindow::on_cmdLineEdit_editingFinished()
{
    // afterCmdLineEditFinished();
}


// 针对无行号的情况，判断命令种类，并立即执行
void MainWindow::processCmd(std::string input)
{
    std::stringstream ss;
    std::string cmd;
    ss << input;
    ss >> cmd;

    if(cmd == "RUN"){
        on_btnRunCode_clicked();
        return;
    }
    if(cmd == "LOAD"){
        on_btnLoadCode_clicked();
        return;
    }
    if(cmd == "CLEAR"){
        on_btnClearCode_clicked();
        return;
    }
    if(cmd == "HELP"){
        std::cout << "help" << std::endl;
        QMessageBox::information(this,"Help","Input commands and MiniBasic will interpret them");
        return;
    }
    if(cmd == "QUIT"){
        qApp->quit();
        return;
    }
    if(cmd == "LET"){
        std::shared_ptr<Statement> stmt = std::make_shared<LetStmt>(input);
        ui->ProgramResult->clear();
        ui->treeDisplay->clear();
        stmt->runStmt(ui);
        return;
    }
    if(cmd == "PRINT"){
        std::shared_ptr<Statement> stmt = std::make_shared<PrintStmt>(input);
        ui->ProgramResult->clear();
        ui->treeDisplay->clear();
        stmt->runStmt(ui);
        return;
    }
    if(cmd == "INPUT"){
        std::shared_ptr<Statement> stmt = std::make_shared<InputStmt>(input);
        ui->ProgramResult->clear();
        ui->treeDisplay->clear();
        programState = RUNNING;
        stmt->runStmt(ui);
        programState = INPUT_STMT;
        return;
    }

    // 以上情况都不是 则是错误输入，需要进行处理
    throw("illegal cmd");
}

// 用于处理新输入的一行命令，参数为输入的命令 cmd
void MainWindow::processInput(std::string cmd)
{
    std::stringstream ss;
    ss << cmd;
    int line = 0;
    ss >> line;
    std::string s;

    // 行号非法处理
    if(line < 0) throw("line number should be a positive int !");

    if(line == 0){  // 如果不是以行号开头
        processCmd(cmd);
    }
    else{  // 如果以行号开头
        getline(ss,s);
        if(s.empty()) program.program.erase(line);
        else{
            program.addStatement(line,s);
        }
    }
}

void MainWindow::on_btnLoadCode_clicked()
{
    //获取文件名称
    QString fileName = QFileDialog::getOpenFileName(this,"OpenFile",QDir::currentPath());
    if(fileName.isEmpty())
    {
         QMessageBox::information(this,"Error","Please select a txt");
         return;
    }
    else
    {
         QFile *file = new QFile;
         file->setFileName(fileName); //新建一个文件对象，并且把它设置为上面获取的文件
         bool ok = file->open(QIODevice::ReadOnly); //设置打开模式

         if(ok)//如果可以打开
         {
              //将文件与文本流相关联
              QTextStream in(file);

              //清除已有的 program
              program.program.clear();

              // load 文件内容
              in.seek(0);
              std::string s;
              while(!in.atEnd()){
                  s = (in.readLine()).toStdString();
                  processInput(s);
              }

              file->close(); //关闭文件
              delete file; //释放文件进程
          }
          else
          {
              QMessageBox::information(this,"Error Box","FileOpen Error"+file->errorString());
              delete file; //释放文件进程
          }
   }
    updateCodeDisplay();
}

void MainWindow::on_btnClearCode_clicked()
{
    program.program.clear();
    ui->ProgramResult->clear();
    ui->treeDisplay->clear();
    updateCodeDisplay();
}

void MainWindow::on_btnRunCode_clicked()
{
    programState = RUNNING;
    ui->ProgramResult->clear();
    ui->treeDisplay->clear();
    program.run(ui);
    programState = INPUT_STMT;
}

