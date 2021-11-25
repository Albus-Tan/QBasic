#include "program.h"

// 全局变量，存储 program
Program program;

// 针对有行号的情况，判断 statement 的命令种类，创建该子类并在合适的位置添加 statement
void Program::addStatement(int line, std::string s)
{
    std::stringstream ss;
    ss << s;
    std::string cmd;
    ss >> cmd;

    if(cmd == "REM"){
        std::shared_ptr<Statement> stmt = std::make_shared<RemStmt>(s);
        program[line] = stmt;
        return;
    }
    if(cmd == "LET"){
        std::shared_ptr<Statement> stmt = std::make_shared<LetStmt>(s);
        program[line] = stmt;
        return;
    }
    if(cmd == "PRINT"){
        std::shared_ptr<Statement> stmt = std::make_shared<PrintStmt>(s);
        program[line] = stmt;
        return;
    }
    if(cmd == "INPUT"){
        std::shared_ptr<Statement> stmt = std::make_shared<InputStmt>(s);
        program[line] = stmt;
        return;
    }
    if(cmd == "GOTO"){
        std::shared_ptr<Statement> stmt = std::make_shared<GotoStmt>(s);
        program[line] = stmt;
        return;
    }
    if(cmd == "IF"){
        std::shared_ptr<Statement> stmt = std::make_shared<IfStmt>(s);
        program[line] = stmt;
        return;
    }
    if(cmd == "END"){
        std::shared_ptr<Statement> stmt = std::make_shared<EndStmt>(s);
        program[line] = stmt;
        return;
    }

    // 都不是，抛出命令非法
    throw("illegal cmd");
}

// 按行号执行当前所存储的整个程序
void Program::run(Ui::MainWindow *ui)
{
    int nextLineNum = -1;
    for(auto it = program.begin(); it != program.end();){
        // runStmt 如果控制流不变返回 -1，否则返回下一条将要执行语句的行号
        nextLineNum = it->second->runStmt(ui);
        it->second->printSyntaxTree(ui, it->first);  // 在界面上显示程序表达式树
        if(nextLineNum == -2) break;  // 如果遇到 END 语句，结束程序
        if(nextLineNum == -1) {++it; continue;}  // 控制流不变
        else {
            it = program.find(nextLineNum);  // 转到对应行号
            if(it == program.end() && it->first != nextLineNum) throw(nextLineNum);  // 转到的行号不存在，报错
        }
    }
}
