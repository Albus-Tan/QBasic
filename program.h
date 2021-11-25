<<<<<<< HEAD
#ifndef PROGRAM_H
#define PROGRAM_H

#include "statement.h"
#include <map>
#include <iostream>
#include <memory>
#include <sstream>
#include "ui_mainwindow.h"

class Program
{
public:

    // 利用 map 存储 行号——语句 键值对（存入后自动按照行号进行排序）
    // 使用 shared_ptr 在 map 中存储 value
    // 定义中为指向 statement 的基类指针
    // 实际存储时存入指向特定派生类的指针
    std::map<int,std::shared_ptr<Statement>> program;

    Program(){};
    ~Program(){};
    void addStatement(int line, std::string s);  // 针对有行号的情况，判断 statement 的命令种类，创建该子类并在合适的位置添加 statement
    void run(Ui::MainWindow *ui);  // 按行号执行当前所存储的整个程序
};

#endif // PROGRAM_H
=======
#ifndef PROGRAM_H
#define PROGRAM_H

#include "statement.h"
#include <map>
#include <iostream>
#include <memory>
#include <sstream>
#include "ui_mainwindow.h"

class Program
{
public:

    // 利用 map 存储 行号——语句 键值对（存入后自动按照行号进行排序）
    // 使用 shared_ptr 在 map 中存储 value
    // 定义中为指向 statement 的基类指针
    // 实际存储时存入指向特定派生类的指针
    std::map<int,std::shared_ptr<Statement>> program;

    Program(){};
    ~Program(){};
    void addStatement(int line, std::string s);  // 针对有行号的情况，判断 statement 的命令种类，创建该子类并在合适的位置添加 statement
    void run(Ui::MainWindow *ui);  // 按行号执行当前所存储的整个程序
};

#endif // PROGRAM_H
>>>>>>> a5d86ec6fb510703b518fc4a44a1b7486a6e10fe
