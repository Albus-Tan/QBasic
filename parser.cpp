<<<<<<< HEAD
#include "parser.h"

// debug
#include <iostream>
//


// 根据所给的 string 参数 exp 生成对应的表达式树，此表达式 exp 可以包含 = 赋值的算数表达式
// 如果有 = 赋值，第二个参数 haveAssignOp 应当为 true （其默认值为 false）
std::shared_ptr<Expression> Parser::generateExpressionTree(std::string exp,  bool haveAssignOp)
{
    // 上次的非法表达式使得栈不空
    while (!operands.empty()) operands.pop();
    while (!operators.empty()) operators.pop();

    bool isBeforeOperator = true;  // 用于记录当前 exp 第一个符号或数字前面（上一个处理的 char）是否为运算符，此变量用于区分负数与减号（"4 -3" "4 + -3" 两种只能靠前一个符号区分）
    bool isBeforeVariable = false;  // 用于记录当前 exp 第一个符号或数字前面是不是变量，用于看使用 = 的表达式是否合法
    while(exp != "\0"){
        if(exp[0] == ' ') {
            exp.erase(0,1);
            continue;
        }
        else{
            if(exp[0] >= '0' && exp[0] <= '9'){  // 为常数
                int constant = 0;
                while(exp[0] >= '0' && exp[0] <= '9'){
                    constant *= 10;
                    constant += exp[0] - '0';
                    exp.erase(0,1);
                }
                checkAndPushConstantExp(constant);
                isBeforeOperator = false;
                isBeforeVariable = false;
                continue;
            }
            if(exp[0] == '+'){
                checkAndPushCompoundExp("+");
                exp.erase(0,1);
                isBeforeOperator = true;
                isBeforeVariable = false;
                continue;
            }
            if(exp[0] == '-'){
                exp.erase(0,1);
                if(isBeforeOperator){  // 前面为运算符，此数为负数或负的变量
                    while(exp[0] == ' ') exp.erase(0,1);
                    if(exp[0] >= '0' && exp[0] <= '9'){  // 为负数
                        int constant = 0;
                        while(exp[0] >= '0' && exp[0] <= '9'){
                            constant *= 10;
                            constant += exp[0] - '0';
                            exp.erase(0,1);
                        }
                    checkAndPushConstantExp(-constant);
                    isBeforeOperator = false;
                    isBeforeVariable = false;
                    continue;
                    }
                    else{  // 为负的变量
                        throw(" ");

                        // !!!!!!!!!

                        isBeforeOperator = false;
                        isBeforeVariable = false;  // 对负的变量赋值非法
                        continue;
                    }
                }
                else{  // 为减号
                    checkAndPushCompoundExp("-");
                    isBeforeOperator = true;
                    isBeforeVariable = false;
                    continue;
                }
            }
            if(exp[0] == '*'){
                if(exp[1] == '*'){  // 为 **


                    // ！！！运算优先级右结合


                    checkAndPushCompoundExp("**");
                    exp.erase(0,2);
                    isBeforeOperator = true;
                    isBeforeVariable = false;
                    continue;
                }
                else{  // 为 *
                    checkAndPushCompoundExp("*");
                    exp.erase(0,1);
                    isBeforeOperator = true;
                    isBeforeVariable = false;
                    continue;
                }
            }
            if(exp[0] == '/'){
                checkAndPushCompoundExp("/");
                exp.erase(0,1);
                isBeforeOperator = true;
                isBeforeVariable = false;
                continue;
            }
            if(exp[0] == '('){
                if(!isBeforeOperator) throw("illegal () use in expression");
                checkAndPushCompoundExp("(", false);
                exp.erase(0,1);
                continue;
            }
            if(exp[0] == ')'){
                if(isBeforeOperator) throw("illegal () use in expression");
                checkAndPushCompoundExp(")");
                exp.erase(0,1);
                continue;
            }
            if(haveAssignOp && exp[0] == '='){
                if(!isBeforeVariable) throw("illegal = use in expression");
                checkAndPushCompoundExp("=");
                exp.erase(0,1);
                isBeforeVariable = false;
                isBeforeOperator = true;
                continue;
            }
            if((exp[0] >= 'a' && exp[0] <= 'z') || (exp[0] >= 'A' && exp[0] <= 'Z') || exp[0] == '_'){
                std::string varName;
                varName += exp[0];
                exp.erase(0,1);
                while(exp[0] != ' ' && exp[0] != '\0'){  // 注意变量可能出现在程序末尾！需要判断 exp[0] != '\0'
                    if((exp[0] >= 'a' && exp[0] <= 'z') || (exp[0] >= 'A' && exp[0] <= 'Z') || exp[0] == '_' || (exp[0] >= '0' && exp[0] <= '9')){
                        varName += exp[0];
                        exp.erase(0,1);
                    }
                    else {
                        if(exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/' || exp[0] == '(' || exp[0] == ')' || exp[0] == '>' || exp[0] == '<' || exp[0] == '=') break;
                        else{
                            throw("illegal variable name in expression");
                        }
                    }
                }
                checkAndPushIdentifierExp(varName);
                isBeforeVariable = true;
                isBeforeOperator = false;
                continue;
            }
            throw("illegal character in expression");
        }
    }
    while(!operators.empty()){
        auto top = operators.top();
        operators.pop();
        auto rhs = operands.top();
        operands.pop();
        auto lhs = operands.top();
        operands.pop();
        top->setLRHS(lhs, rhs);
        operands.push(top);
    }
    auto root = operands.top();
    operands.pop();
    if(!operands.empty() || !operators.empty()) {
        if(!operands.empty()) {auto sth = operands.top(); std::cout << "operands stack top left: " << sth->toString() << std::endl;}
        if(!operators.empty()) {auto sth = operators.top(); std::cout << "operators stack top left: " << sth->toString() << std::endl;}
        throw("error occurred in FUNC generateExpressionTree : stack not empty !");
    }
    return root;
}

// 创建常数 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
void Parser::checkAndPushConstantExp(int constant)
{
    operands.push(std::make_shared<ConstantExp>(constant));
}

// 创建符号 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
// 参数 checkPresedence 为 true 时，需要检查符号栈优先级，否则不检查优先级
void Parser::checkAndPushCompoundExp(std::string op, bool checkPresedence)
{
    if(op == ")"){
        // 出栈直到遇到 "(" 为止
        while(!operators.empty() && operators.top()->getOperator() != "("){
            auto top = operators.top();
            operators.pop();
            auto rhs = operands.top();
            operands.pop();
            auto lhs = operands.top();
            operands.pop();
            top->setLRHS(lhs, rhs);
            operands.push(top);
        }
        operators.pop();
        // "(" 不进栈
        return;
    }
    if(checkPresedence){  // 不是 "(" 入栈的情况时
        // 如果当前符号优先级小于等于栈顶
        while(!operators.empty() && (presedence[op] <= presedence[operators.top()->getOperator()])){
            // 调整直到当前符号优先级大于栈顶
            auto top = operators.top();
            operators.pop();
            auto rhs = operands.top();
            operands.pop();
            auto lhs = operands.top();
            operands.pop();
            top->setLRHS(lhs, rhs);
            operands.push(top);
        }
    }
    operators.push(std::make_shared<CompoundExp>(op));
    return;
}

// 打印根节点为 root 的表达式树，此表达式树为不包含 = 赋值的算数表达式
void Parser::printPureExpressionTree(std::shared_ptr<Expression> root, Ui::MainWindow *ui)
{
    // 上次的非法表达式使得队列不空
    while (!expNode.empty()) expNode.pop();

    // 对表达式树进行层次遍历输出
    expNode.push(root);
    int layer = 1;  // 用于记录此时遍历到了第几层
    int nextLayerNodeNum = 1;  // 用于记录下一层应当有多少节点
    while(!expNode.empty()){
        int i = nextLayerNodeNum;
        nextLayerNodeNum *= 2;
        for(; i > 0; --i){
            std::string output = "    ";
            auto front = expNode.front();
            expNode.pop();
            int n = layer - 1;
            while(n > 0){
                output += "    ";
                --n;
            }
            if(front->toString() != "="){
                output = output + front->toString();
                ui->treeDisplay->append(QString::fromStdString(output));
            }
            else --layer;  // 含有赋值符号的表达式，= 已经打印，并让之后的 output 少缩进四个
            if(front->type() == COMPOUND) {
                expNode.push(front->getLHS());
                expNode.push(front->getRHS());
            }
            else{
                nextLayerNodeNum -= 2;
            }
        }
        ++layer;
    }
}

// 根据所给表达式树计算对应的表达式值（此表达式树为可以包含 = 赋值的算数表达式）
int Parser::evaluateExpressionTree(std::shared_ptr<Expression> exp)
{
    return exp->eval(allVariables);
}

// 创建变量 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
void Parser::checkAndPushIdentifierExp(std::string varName)
{
    operands.push(std::make_shared<IdentifierExp>(varName));
}

// 生成 IF 语句的 condition
// 根据所给的参数 condition 生成对应条件，包含左右两边两个表达式树以及中间的 > < = 符号的提取
std::shared_ptr<Condition> Parser::generateCondition(std::string condition)
{
    auto pos = condition.npos;
    std::string left;
    std::string right;
    pos = condition.find('>');
    if(pos != condition.npos){  // 含有 >
        left.assign(condition, 0, pos - 1);
        right.assign(condition, pos + 1, condition.npos);
        return std::make_shared<Condition>(">", generateExpressionTree(left), generateExpressionTree(right));
    }
    else{
        pos = condition.find('<');
        if(pos != condition.npos){  // 含有 <
            left.assign(condition, 0, pos - 1);
            right.assign(condition, pos + 1, condition.npos);
            return std::make_shared<Condition>("<", generateExpressionTree(left), generateExpressionTree(right));
        }
        else{
            pos = condition.find('=');
            if(pos != condition.npos){  // 含有 =
                left.assign(condition, 0, pos - 1);
                right.assign(condition, pos + 1, condition.npos);
                return std::make_shared<Condition>("=", generateExpressionTree(left), generateExpressionTree(right));
            }
            else throw("no legal op in condition!");
        }
    }
}
=======
#include "parser.h"

// debug
#include <iostream>
//


// 根据所给的 string 参数 exp 生成对应的表达式树，此表达式 exp 可以包含 = 赋值的算数表达式
// 如果有 = 赋值，第二个参数 haveAssignOp 应当为 true （其默认值为 false）
std::shared_ptr<Expression> Parser::generateExpressionTree(std::string exp,  bool haveAssignOp)
{
    // 上次的非法表达式使得栈不空
    while (!operands.empty()) operands.pop();
    while (!operators.empty()) operators.pop();

    bool isBeforeOperator = true;  // 用于记录当前 exp 第一个符号或数字前面（上一个处理的 char）是否为运算符，此变量用于区分负数与减号（"4 -3" "4 + -3" 两种只能靠前一个符号区分）
    bool isBeforeVariable = false;  // 用于记录当前 exp 第一个符号或数字前面是不是变量，用于看使用 = 的表达式是否合法
    while(exp != "\0"){
        if(exp[0] == ' ') {
            exp.erase(0,1);
            continue;
        }
        else{
            if(exp[0] >= '0' && exp[0] <= '9'){  // 为常数
                int constant = 0;
                while(exp[0] >= '0' && exp[0] <= '9'){
                    constant *= 10;
                    constant += exp[0] - '0';
                    exp.erase(0,1);
                }
                checkAndPushConstantExp(constant);
                isBeforeOperator = false;
                isBeforeVariable = false;
                continue;
            }
            if(exp[0] == '+'){
                checkAndPushCompoundExp("+");
                exp.erase(0,1);
                isBeforeOperator = true;
                isBeforeVariable = false;
                continue;
            }
            if(exp[0] == '-'){
                exp.erase(0,1);
                if(isBeforeOperator){  // 前面为运算符，此数为负数或负的变量
                    while(exp[0] == ' ') exp.erase(0,1);
                    if(exp[0] >= '0' && exp[0] <= '9'){  // 为负数
                        int constant = 0;
                        while(exp[0] >= '0' && exp[0] <= '9'){
                            constant *= 10;
                            constant += exp[0] - '0';
                            exp.erase(0,1);
                        }
                    checkAndPushConstantExp(-constant);
                    isBeforeOperator = false;
                    isBeforeVariable = false;
                    continue;
                    }
                    else{  // 为负的变量
                        throw(" ");

                        // !!!!!!!!!

                        isBeforeOperator = false;
                        isBeforeVariable = false;  // 对负的变量赋值非法
                        continue;
                    }
                }
                else{  // 为减号
                    checkAndPushCompoundExp("-");
                    isBeforeOperator = true;
                    isBeforeVariable = false;
                    continue;
                }
            }
            if(exp[0] == '*'){
                if(exp[1] == '*'){  // 为 **


                    // ！！！运算优先级右结合


                    checkAndPushCompoundExp("**");
                    exp.erase(0,2);
                    isBeforeOperator = true;
                    isBeforeVariable = false;
                    continue;
                }
                else{  // 为 *
                    checkAndPushCompoundExp("*");
                    exp.erase(0,1);
                    isBeforeOperator = true;
                    isBeforeVariable = false;
                    continue;
                }
            }
            if(exp[0] == '/'){
                checkAndPushCompoundExp("/");
                exp.erase(0,1);
                isBeforeOperator = true;
                isBeforeVariable = false;
                continue;
            }
            if(exp[0] == '('){
                if(!isBeforeOperator) throw("illegal () use in expression");
                checkAndPushCompoundExp("(", false);
                exp.erase(0,1);
                continue;
            }
            if(exp[0] == ')'){
                if(isBeforeOperator) throw("illegal () use in expression");
                checkAndPushCompoundExp(")");
                exp.erase(0,1);
                continue;
            }
            if(haveAssignOp && exp[0] == '='){
                if(!isBeforeVariable) throw("illegal = use in expression");
                checkAndPushCompoundExp("=");
                exp.erase(0,1);
                isBeforeVariable = false;
                isBeforeOperator = true;
                continue;
            }
            if((exp[0] >= 'a' && exp[0] <= 'z') || (exp[0] >= 'A' && exp[0] <= 'Z') || exp[0] == '_'){
                std::string varName;
                varName += exp[0];
                exp.erase(0,1);
                while(exp[0] != ' ' && exp[0] != '\0'){  // 注意变量可能出现在程序末尾！需要判断 exp[0] != '\0'
                    if((exp[0] >= 'a' && exp[0] <= 'z') || (exp[0] >= 'A' && exp[0] <= 'Z') || exp[0] == '_' || (exp[0] >= '0' && exp[0] <= '9')){
                        varName += exp[0];
                        exp.erase(0,1);
                    }
                    else {
                        if(exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/' || exp[0] == '(' || exp[0] == ')' || exp[0] == '>' || exp[0] == '<' || exp[0] == '=') break;
                        else{
                            throw("illegal variable name in expression");
                        }
                    }
                }
                checkAndPushIdentifierExp(varName);
                isBeforeVariable = true;
                isBeforeOperator = false;
                continue;
            }
            throw("illegal character in expression");
        }
    }
    while(!operators.empty()){
        auto top = operators.top();
        operators.pop();
        auto rhs = operands.top();
        operands.pop();
        auto lhs = operands.top();
        operands.pop();
        top->setLRHS(lhs, rhs);
        operands.push(top);
    }
    auto root = operands.top();
    operands.pop();
    if(!operands.empty() || !operators.empty()) {
        if(!operands.empty()) {auto sth = operands.top(); std::cout << "operands stack top left: " << sth->toString() << std::endl;}
        if(!operators.empty()) {auto sth = operators.top(); std::cout << "operators stack top left: " << sth->toString() << std::endl;}
        throw("error occurred in FUNC generateExpressionTree : stack not empty !");
    }
    return root;
}

// 创建常数 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
void Parser::checkAndPushConstantExp(int constant)
{
    operands.push(std::make_shared<ConstantExp>(constant));
}

// 创建符号 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
// 参数 checkPresedence 为 true 时，需要检查符号栈优先级，否则不检查优先级
void Parser::checkAndPushCompoundExp(std::string op, bool checkPresedence)
{
    if(op == ")"){
        // 出栈直到遇到 "(" 为止
        while(!operators.empty() && operators.top()->getOperator() != "("){
            auto top = operators.top();
            operators.pop();
            auto rhs = operands.top();
            operands.pop();
            auto lhs = operands.top();
            operands.pop();
            top->setLRHS(lhs, rhs);
            operands.push(top);
        }
        operators.pop();
        // "(" 不进栈
        return;
    }
    if(checkPresedence){  // 不是 "(" 入栈的情况时
        // 如果当前符号优先级小于等于栈顶
        while(!operators.empty() && (presedence[op] <= presedence[operators.top()->getOperator()])){
            // 调整直到当前符号优先级大于栈顶
            auto top = operators.top();
            operators.pop();
            auto rhs = operands.top();
            operands.pop();
            auto lhs = operands.top();
            operands.pop();
            top->setLRHS(lhs, rhs);
            operands.push(top);
        }
    }
    operators.push(std::make_shared<CompoundExp>(op));
    return;
}

// 打印根节点为 root 的表达式树，此表达式树为不包含 = 赋值的算数表达式
void Parser::printPureExpressionTree(std::shared_ptr<Expression> root, Ui::MainWindow *ui)
{
    // 上次的非法表达式使得队列不空
    while (!expNode.empty()) expNode.pop();

    // 对表达式树进行层次遍历输出
    expNode.push(root);
    int layer = 1;  // 用于记录此时遍历到了第几层
    int nextLayerNodeNum = 1;  // 用于记录下一层应当有多少节点
    while(!expNode.empty()){
        int i = nextLayerNodeNum;
        nextLayerNodeNum *= 2;
        for(; i > 0; --i){
            std::string output = "    ";
            auto front = expNode.front();
            expNode.pop();
            int n = layer - 1;
            while(n > 0){
                output += "    ";
                --n;
            }
            if(front->toString() != "="){
                output = output + front->toString();
                ui->treeDisplay->append(QString::fromStdString(output));
            }
            else --layer;  // 含有赋值符号的表达式，= 已经打印，并让之后的 output 少缩进四个
            if(front->type() == COMPOUND) {
                expNode.push(front->getLHS());
                expNode.push(front->getRHS());
            }
            else{
                nextLayerNodeNum -= 2;
            }
        }
        ++layer;
    }
}

// 根据所给表达式树计算对应的表达式值（此表达式树为可以包含 = 赋值的算数表达式）
int Parser::evaluateExpressionTree(std::shared_ptr<Expression> exp)
{
    return exp->eval(allVariables);
}

// 创建变量 ExpNode，并检查此时栈的条件，进行调整，最终完成进栈
void Parser::checkAndPushIdentifierExp(std::string varName)
{
    operands.push(std::make_shared<IdentifierExp>(varName));
}

// 生成 IF 语句的 condition
// 根据所给的参数 condition 生成对应条件，包含左右两边两个表达式树以及中间的 > < = 符号的提取
std::shared_ptr<Condition> Parser::generateCondition(std::string condition)
{
    auto pos = condition.npos;
    std::string left;
    std::string right;
    pos = condition.find('>');
    if(pos != condition.npos){  // 含有 >
        left.assign(condition, 0, pos - 1);
        right.assign(condition, pos + 1, condition.npos);
        return std::make_shared<Condition>(">", generateExpressionTree(left), generateExpressionTree(right));
    }
    else{
        pos = condition.find('<');
        if(pos != condition.npos){  // 含有 <
            left.assign(condition, 0, pos - 1);
            right.assign(condition, pos + 1, condition.npos);
            return std::make_shared<Condition>("<", generateExpressionTree(left), generateExpressionTree(right));
        }
        else{
            pos = condition.find('=');
            if(pos != condition.npos){  // 含有 =
                left.assign(condition, 0, pos - 1);
                right.assign(condition, pos + 1, condition.npos);
                return std::make_shared<Condition>("=", generateExpressionTree(left), generateExpressionTree(right));
            }
            else throw("no legal op in condition!");
        }
    }
}
>>>>>>> a5d86ec6fb510703b518fc4a44a1b7486a6e10fe
