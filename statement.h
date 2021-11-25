#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <sstream>
#include <memory>
#include "expression.h"
#include "parser.h"
#include "ui_mainwindow.h"
#include "condition.h"

/*
 * Type: StatementType
 * --------------------
 * This enumerated type is used to differentiate seven different
 * statement types: REM, LET, PRINT, INPUT, GOTO, IF and END.
 */

enum StatementType { REM, LET, PRINT, INPUT, GOTO, IF, END };

extern Parser parser;

// 用于记录操作程序中所有的变量的 EvaluationContext 类实例
extern EvaluationContext allVariables;

extern ProgramState programState;

/*
 * Class: Statement
 * -----------------
 * This class is used to represent a statement in program.
 * Statement itself is an abstract class.  Every Statement object
 * is therefore created using one of the seven concrete subclasses:
 * RemStmt, LetStmt, PrintStmt, InputStmt, GotoStmt, IfStmt and EndStmt.
 */

class Statement
{

public:

    StatementType type;  // 该行命令的种类
    std::string statement = "\0";  // 存储不含行号，包含命令名称的 statement （首个字符为空格）

    Statement(){};
    Statement(std::string s);
    virtual int runStmt(Ui::MainWindow *ui) = 0;  // 执行该行 statement，如果控制流不变返回 -1，否则返回下一条将要执行语句的行号，如果为 END 语句返回 -2
    virtual void printSyntaxTree(Ui::MainWindow *ui, int line = -1) = 0;  // 在页面上显示该行 statement 的表达式树
    // virtual bool isLegal() = 0;
    // 判断当前 statement 的形式否合法

};


/*
 * Class: RemStmt
 * ------------------
 * This subclass represents a REM statement.
 * REM statement is used for comments.
 * Any text on the line after the keyword REM is ignored.
 */

class RemStmt : public Statement
{

public:

    RemStmt(){};
    RemStmt(std::string s);
    int runStmt(Ui::MainWindow *ui);
    void printSyntaxTree(Ui::MainWindow *ui, int line = -1);

private:
    std::string comment;
};


/*
 * Class: LetStmt
 * ------------------
 * LET statement is BASIC’s assignment statement. The LET keyword
 * is followed by a variable name, an equal sign, and an expression.
 * As in C++, the effect of this statement is to assign the value of
 * the expression to the variable, replacing any previous value.
 * In BASIC, assignment is not an operator and may not be nested
 * inside other expressions.
 */

class LetStmt : public Statement
{

public:

    LetStmt(){};
    LetStmt(std::string s);
    int runStmt(Ui::MainWindow *ui);
    void printSyntaxTree(Ui::MainWindow *ui, int line = -1);

private:
    std::shared_ptr<Expression> exp;
};


/*
 * Class: PrintStmt
 * ------------------
 * PRINT statement has the form: "PRINT exp" where exp is an expression.
 * The effect of this statement is to print the value of the expression
 * on the console and then print a newline character so that the output
 * from the next PRINT statement begins on a new line.
 */

class PrintStmt : public Statement
{

public:

    PrintStmt(){};
    PrintStmt(std::string s);
    int runStmt(Ui::MainWindow *ui);
    void printSyntaxTree(Ui::MainWindow *ui, int line = -1);

private:
    std::shared_ptr<Expression> exp;
};


/*
 * Class: InputStmt
 * ------------------
 * INPUT statement has the form: "INPUT var"
 * where var is a variable read in from the user.
 * The effect of this statement is to print a prompt
 * consisting of the string " ? " and then to read
 * in a value to be stored in the variable.
 * (The string " ? " should display in the command
 * input edit box in GUI.)
 */

class InputStmt : public Statement
{

public:

    InputStmt(){};
    InputStmt(std::string s);
    int runStmt(Ui::MainWindow *ui);
    void printSyntaxTree(Ui::MainWindow *ui, int line = -1);

private:
    std::string varName;
};


/*
 * Class: GotoStmt
 * ------------------
 * This statement has the syntax "GOTO n" and forces an
 * unconditional change in the control flow of the program.
 * When the program hits this statement, the program continues
 * from line n instead of continuing with the next statement.
 * Your program should report an error if line n does not exist.
 */

class GotoStmt : public Statement
{

public:

    GotoStmt(){};
    GotoStmt(std::string s);
    int runStmt(Ui::MainWindow *ui);
    void printSyntaxTree(Ui::MainWindow *ui, int line = -1);

private:
    int line = 0;  // 记录跳转到第几行，值为 0 时表明出现问题
};


/*
 * Class: IfStmt
 * ------------------
 * This statement provides conditional control.
 * The syntax for this statement is: "IF exp1 op exp2 THEN n"
 * where exp1 and exp2 are expressions and op is one of the
 * conditional operators =, <, or >. If the condition holds,
 * the program should continue from line n just as in the GOTO statement.
 * If not, the program continues on to the next line. Note that
 * the conditional operators (=, <, >) are not parts of expressions.
 */

class IfStmt : public Statement
{

public:

    IfStmt(){};
    IfStmt(std::string s);
    int runStmt(Ui::MainWindow *ui);
    void printSyntaxTree(Ui::MainWindow *ui, int line = -1);

private:
    std::shared_ptr<Condition> condition;
    int lineNum;
};


/*
 * Class: EndStmt
 * ------------------
 * Marks the end of the program. Execution halts when
 * this line is reached. This statement is usually optional
 * in BASIC programs because execution also stops if the
 * program continues past the last numbered line.
 */

class EndStmt : public Statement
{

public:

    EndStmt(){};
    EndStmt(std::string s);
    int runStmt(Ui::MainWindow *ui);
    void printSyntaxTree(Ui::MainWindow *ui, int line = -1);
};


#endif // STATEMENT_H
