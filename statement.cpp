<<<<<<< HEAD
#include "statement.h"

// Debug
#include <iostream>
#include <assert.h>

// 程序所使用的解释器
Parser parser;

Statement::Statement(std::string s)
{
    statement = s;
}

RemStmt::RemStmt(std::string s) : Statement(s){
    type= REM;
    std::stringstream ss;
    ss << statement;
    std::string cmd;
    ss >> cmd;
    getline(ss, comment);
}

LetStmt::LetStmt(std::string s) : Statement(s){
    type= LET;
    std::stringstream ss;
    ss << statement;
    std::string cmd, exp;
    ss >> cmd;
    getline(ss,exp);
    ss.clear();
    this->exp = parser.generateExpressionTree(exp, true);
    if(this->exp == nullptr) throw("exp in LetStmt generate NULL expTree !");
}

PrintStmt::PrintStmt(std::string s) : Statement(s){
    type= PRINT;
    std::stringstream ss;
    ss << statement;
    std::string cmd, exp;
    ss >> cmd;
    getline(ss,exp);
    ss.clear();
    this->exp = parser.generateExpressionTree(exp);
    if(this->exp == nullptr) throw("exp in PrintStmt generate NULL expTree !");
}

InputStmt::InputStmt(std::string s) : Statement(s){
    type= INPUT;
    std::stringstream ss;
    ss << statement;
    std::string cmd;
    ss >> cmd >> varName;
}

GotoStmt::GotoStmt(std::string s) : Statement(s){
    type= GOTO;
    std::stringstream ss;
    ss << s;
    std::string cmd;
    ss >> cmd >> line;
    ss.clear();
    if(line == 0) throw("Line number after GOTO should be a positive int !");
}

IfStmt::IfStmt(std::string s) : Statement(s){
    type= IF;
    int posIf = statement.find("IF");
    int posThen = statement.find("THEN");
    if(posThen == statement.npos) throw("Illegal IF stmt grammar: no THEN !");
    std::string condition;
    std::string line;
    condition.assign(statement, posIf + 2, posThen - posIf - 2);
    line.assign(statement, posThen + 4, statement.npos);
    this->condition = parser.generateCondition(condition);
    lineNum = stoi(line);
}

EndStmt::EndStmt(std::string s) : Statement(s){
    type= END;
}

int RemStmt::runStmt(Ui::MainWindow *ui){
    return -1;
}

int LetStmt::runStmt(Ui::MainWindow *ui){
    parser.evaluateExpressionTree(this->exp);
    return -1;
}

int PrintStmt::runStmt(Ui::MainWindow *ui){
    int value = parser.evaluateExpressionTree(this->exp);
    ui->ProgramResult->append(QString::number(value));
    return -1;
}

int InputStmt::runStmt(Ui::MainWindow *ui){
    programState = INPUT_VAR;
    ui->cmdLineEdit->setText(" ? ");
    while(programState != RUNNING){
        // 等待用户输入完成， MainWindow::on_cmdLineEdit_returnPressed() 函数修改
        // programState 为 RUNNING 状态，再获取输入内容
        qApp->processEvents();  // 保证此时继续处理线程内的事件
    }
    QString input = ui->cmdLineEdit->text();
    std::string s = input.toStdString();
    std::stringstream ss;
    std::string questionMark;
    int value;
    ss << s;
    ss >> questionMark >> value;
    ss.clear();
    allVariables.setValue(varName, value);

    // 恢复 cmdLineEdit 原始状态
    ui->cmdLineEdit->setText("");
    return -1;
}

int GotoStmt::runStmt(Ui::MainWindow *ui){
    return line;
}

int IfStmt::runStmt(Ui::MainWindow *ui){
    if(condition->conditionResult()) return lineNum;
    else return -1;
}

int EndStmt::runStmt(Ui::MainWindow *ui){
    return -2;
}

void RemStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "REM";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    ui->treeDisplay->append("   " + QString::fromStdString(comment));
}

void LetStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "LET =";
    if(line != -1) firstLine = std::to_string(line) + " " + firstLine;  // 即时性指令，调用时候令参数 line 为 -1
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    parser.printPureExpressionTree(this->exp,ui);
}

void PrintStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "PRINT";
    if(line != -1) firstLine = std::to_string(line) + " " + firstLine;  // 即时性指令，调用时候令参数 line 为 -1
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    parser.printPureExpressionTree(this->exp,ui);
}

void InputStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "INPUT";
    if(line != -1) firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    ui->treeDisplay->append("    " + QString::fromStdString(varName));
}

void GotoStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "GOTO";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    ui->treeDisplay->append("    " + QString::number(this->line));
}

void IfStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "IF THEN";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    parser.printPureExpressionTree(condition->getLeftExp(), ui);
    ui->treeDisplay->append("    " + QString::fromStdString(condition->getOp()));
    parser.printPureExpressionTree(condition->getRightExp(), ui);
    ui->treeDisplay->append("    " + QString::number(lineNum));
}

void EndStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "END";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
}
=======
#include "statement.h"

// Debug
#include <iostream>
#include <assert.h>

// 程序所使用的解释器
Parser parser;

Statement::Statement(std::string s)
{
    statement = s;
}

RemStmt::RemStmt(std::string s) : Statement(s){
    type= REM;
    std::stringstream ss;
    ss << statement;
    std::string cmd;
    ss >> cmd;
    getline(ss, comment);
}

LetStmt::LetStmt(std::string s) : Statement(s){
    type= LET;
    std::stringstream ss;
    ss << statement;
    std::string cmd, exp;
    ss >> cmd;
    getline(ss,exp);
    ss.clear();
    this->exp = parser.generateExpressionTree(exp, true);
    if(this->exp == nullptr) throw("exp in LetStmt generate NULL expTree !");
}

PrintStmt::PrintStmt(std::string s) : Statement(s){
    type= PRINT;
    std::stringstream ss;
    ss << statement;
    std::string cmd, exp;
    ss >> cmd;
    getline(ss,exp);
    ss.clear();
    this->exp = parser.generateExpressionTree(exp);
    if(this->exp == nullptr) throw("exp in PrintStmt generate NULL expTree !");
}

InputStmt::InputStmt(std::string s) : Statement(s){
    type= INPUT;
    std::stringstream ss;
    ss << statement;
    std::string cmd;
    ss >> cmd >> varName;
}

GotoStmt::GotoStmt(std::string s) : Statement(s){
    type= GOTO;
    std::stringstream ss;
    ss << s;
    std::string cmd;
    ss >> cmd >> line;
    ss.clear();
    if(line == 0) throw("Line number after GOTO should be a positive int !");
}

IfStmt::IfStmt(std::string s) : Statement(s){
    type= IF;
    int posIf = statement.find("IF");
    int posThen = statement.find("THEN");
    if(posThen == statement.npos) throw("Illegal IF stmt grammar: no THEN !");
    std::string condition;
    std::string line;
    condition.assign(statement, posIf + 2, posThen - posIf - 2);
    line.assign(statement, posThen + 4, statement.npos);
    this->condition = parser.generateCondition(condition);
    lineNum = stoi(line);
}

EndStmt::EndStmt(std::string s) : Statement(s){
    type= END;
}

int RemStmt::runStmt(Ui::MainWindow *ui){
    return -1;
}

int LetStmt::runStmt(Ui::MainWindow *ui){
    parser.evaluateExpressionTree(this->exp);
    return -1;
}

int PrintStmt::runStmt(Ui::MainWindow *ui){
    int value = parser.evaluateExpressionTree(this->exp);
    ui->ProgramResult->append(QString::number(value));
    return -1;
}

int InputStmt::runStmt(Ui::MainWindow *ui){
    programState = INPUT_VAR;
    ui->cmdLineEdit->setText(" ? ");
    while(programState != RUNNING){
        // 等待用户输入完成， MainWindow::on_cmdLineEdit_returnPressed() 函数修改
        // programState 为 RUNNING 状态，再获取输入内容
        qApp->processEvents();  // 保证此时继续处理线程内的事件
    }
    QString input = ui->cmdLineEdit->text();
    std::string s = input.toStdString();
    std::stringstream ss;
    std::string questionMark;
    int value;
    ss << s;
    ss >> questionMark >> value;
    ss.clear();
    allVariables.setValue(varName, value);

    // 恢复 cmdLineEdit 原始状态
    ui->cmdLineEdit->setText("");
    return -1;
}

int GotoStmt::runStmt(Ui::MainWindow *ui){
    return line;
}

int IfStmt::runStmt(Ui::MainWindow *ui){
    if(condition->conditionResult()) return lineNum;
    else return -1;
}

int EndStmt::runStmt(Ui::MainWindow *ui){
    return -2;
}

void RemStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "REM";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    ui->treeDisplay->append("   " + QString::fromStdString(comment));
}

void LetStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "LET =";
    if(line != -1) firstLine = std::to_string(line) + " " + firstLine;  // 即时性指令，调用时候令参数 line 为 -1
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    parser.printPureExpressionTree(this->exp,ui);
}

void PrintStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "PRINT";
    if(line != -1) firstLine = std::to_string(line) + " " + firstLine;  // 即时性指令，调用时候令参数 line 为 -1
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    parser.printPureExpressionTree(this->exp,ui);
}

void InputStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "INPUT";
    if(line != -1) firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    ui->treeDisplay->append("    " + QString::fromStdString(varName));
}

void GotoStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "GOTO";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    ui->treeDisplay->append("    " + QString::number(this->line));
}

void IfStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "IF THEN";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
    parser.printPureExpressionTree(condition->getLeftExp(), ui);
    ui->treeDisplay->append("    " + QString::fromStdString(condition->getOp()));
    parser.printPureExpressionTree(condition->getRightExp(), ui);
    ui->treeDisplay->append("    " + QString::number(lineNum));
}

void EndStmt::printSyntaxTree(Ui::MainWindow *ui, int line)
{
    std::string firstLine = "END";
    firstLine = std::to_string(line) + " " + firstLine;
    ui->treeDisplay->append(QString::fromStdString(firstLine));
}
>>>>>>> a5d86ec6fb510703b518fc4a44a1b7486a6e10fe
