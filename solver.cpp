#include "solver.h"

Solver::Solver() {}

QString Solver::getAns(int num0,int num1,int num2,int num3)
{
    return "To be solved: "+QString::number(num0)+" "+QString::number(num1)+" "+QString::number(num2)+" "+QString::number(num3);
}

void Solver::solveAll()
{

}
