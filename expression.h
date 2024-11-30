#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QWidget>
#include <stack>
#include "frac.h"



class Expression
{
public:
    Expression();
    Expression(QString s);
    Expression(const Expression &that);
    void setExpression(const QString s);
    void filter();
    void calculate();
    Frac res;
    bool err;
    int usedNums[4];
private:
    QString s;
    std::stack<Frac>numStack;
    std::stack<QChar>opStack;
    void stackTopCalcAux();
};

#endif // EXPRESSION_H
