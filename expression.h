#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QWidget>
#include "frac.h"

class Expression
{
public:
    Expression();
    Expression(QString s);
    Expression(const Expression &that);
    void setExpression(const QString s);
    Frac calculate();
private:
    QString s;
    bool err;
};

#endif // EXPRESSION_H
