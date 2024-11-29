#ifndef SOLVER_H
#define SOLVER_H

#include <QWidget>

#include "frac.h"
#include "expression.h"
class Solver
{
public:
    Solver();
    QString getAns(int num0,int num1,int num2,int num3);
    void solveAll();
private:
    QString ans[14][14][14][14];
};

#endif // SOLVER_H
