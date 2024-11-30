#include "expression.h"

int opPri(QChar op)
{
    switch (op.unicode()) {
    case '+':
        return 1;
        break;
    case '-':
        return 1;
        break;
    case '*':
        return 2;
        break;
    case '/':
        return 2;
        break;
    default:
        break;
    }
    return 0;
}

Expression::Expression()
{
    this->s="";
}
Expression::Expression(QString s)
{
    this->s=s;
    filter();
}
Expression::Expression(const Expression &that)
{
    this->s=that.s;
    filter();
}
void Expression::setExpression(const QString s)
{
    this->s=s;
    filter();
}
void Expression::filter()
{
    QString temp=s;
    s="";
    for(auto c:temp)
        if(c.isDigit()||c=='('||c==')'||c=='+'||c=='-'||c=='*'||c=='/')
            s.append(c);
}
void Expression::calculate()
{
    bool lastIsNum=false;
    bool nowIsNum=false;
    int numCnt=0;
    err=false;
    for(auto c:"("+s+")")
    {
        nowIsNum=(c!='('&&c!=')'&&c!='+'&&c!='-'&&c!='*'&&c!='/');
        if(nowIsNum)
        {
            if(lastIsNum==true)
            {
                Frac temp=numStack.top();
                numStack.pop();
                temp=temp*Frac(10)+Frac(c.digitValue());
                numStack.push(temp);
            }
            else
            {
                numStack.push(c.digitValue());
            }

        }
        else
        {
            if(lastIsNum==false&&(c=='+'||c=='-')&&(opStack.empty()||opStack.top()=='('))
                numStack.push(Frac(0));
            if(lastIsNum==true)
            {
                if(numStack.top()<Frac(1)||numStack.top()>Frac(13))
                    err=true;
                if(numCnt<4)
                    usedNums[numCnt]=numStack.top().num;
                numCnt++;
            }
            if(c=='(')
                opStack.push(c);
            else if(c=='+'||c=='-'||c=='*'||c=='/')
            {
                while(opStack.empty()==false&&opStack.top()!='('&&opPri(c)<=opPri(opStack.top()))
                    stackTopCalcAux();
                opStack.push(c);
            }
            else if(c==')')
            {
                while(opStack.top()!='(')
                    stackTopCalcAux();
                opStack.pop();
            }

        }

        lastIsNum=nowIsNum;
    }
    if(numCnt!=4)
        err=true;
    if(numStack.empty())
        err=true;
    res=(err?Frac(0,0):numStack.top());
}
void Expression::stackTopCalcAux()
{
    Frac num1,num2,num3;
    QChar op;
    if(numStack.size()<2)
    {
        while(numStack.empty()==false)numStack.pop();
        while(opStack.empty()==false)opStack.pop();
        return;
    }
    num2=numStack.top();
    numStack.pop();
    num1=numStack.top();
    numStack.pop();
    op=opStack.top();
    opStack.pop();
    switch (op.unicode()) {
    case '+':
        num3=num1+num2;
        break;
    case '-':
        num3=num1-num2;
        break;
    case '*':
        num3=num1*num2;
        break;
    case '/':
        num3=num1/num2;
        break;
    default:
        err=true;
        break;
    }
    numStack.push(num3);
}
