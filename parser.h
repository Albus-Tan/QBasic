#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <cmath>
#include "expression.h"
#include "condition.h"
#include "evalstate.h"
#include "ui_mainwindow.h"

// string 类型 exp 转换为表达式树时所需要用到的操作符栈与操作数栈，定义在 evalstate 中
extern std::stack<std::shared_ptr<Expression>> operators;
extern std::stack<std::shared_ptr<Expression>> operands;

// 用于层次遍历的队列
extern std::queue<std::shared_ptr<Expression>> expNode;

extern std::map<std::string,int> presedence;

// 用于记录操作程序中所有的变量的 EvaluationContext 类实例
extern EvaluationContext allVariables;

/*
 * Class: Parser
 * -----------------
 * This class is used to convert an expression
 * in string to an expression tree.
 */

class Parser
{
public:
    Parser(){};

    // 打印表达式树
    void printPureExpressionTree(std::shared_ptr<Expression> root, Ui::MainWindow *ui);  // 打印根节点为 root 的表达式树，此表达式树为不包含 = 赋值的算数表达式
    // 生成表达式树
    std::shared_ptr<Expression> generateExpressionTree(std::string exp, bool haveAssignOp = false);  // 根据所给的参数 exp 生成对应的表达式树，此表达式树为可以包含 = 赋值的算数表达式（如果有 = 赋值，第二个参数应当为 true）
    // 计算表达式树
    int evaluateExpressionTree(std::shared_ptr<Expression> exp);  // 根据所给表达式树计算对应的表达式值（此表达式树为可以包含 = 赋值的算数表达式）

    // 生成 IF 语句的 condition
    std::shared_ptr<Condition> generateCondition(std::string condition);  // 根据所给的参数 condition 生成对应条件，包含左右两边两个表达式树以及中间的 > < = 符号的提取

private:
    void checkAndPushConstantExp(int constant);  // 创建常数 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
    void checkAndPushCompoundExp(std::string op, bool checkPresedence = true);    // 创建符号 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
    void checkAndPushIdentifierExp(std::string varName);  // 创建变量 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
};

#endif // PARSER_H
