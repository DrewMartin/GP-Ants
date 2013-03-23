#include "twoargnode.h"

TwoArgNode::TwoArgNode(QSP<MathNode> left, QSP<MathNode> right, TwoArgOp operation) :
    operation(operation)
{
    children.append(left);
    children.append(right);
}

TwoArgNode::~TwoArgNode()
{
}

TwoArgNode::TwoArgNode(TwoArgOp operation) :
    operation(operation)
{
}

double TwoArgNode::eval(double var)
{
    return operation(children.at(0)->eval(var), children.at(1)->eval(var));
}

QString TwoArgNode::toString()
{
    return getSymbolForTwoArgOp(operation).arg(children.at(0)->toString()).arg(children.at(1)->toString());
}

TwoArgNode *TwoArgNode::copy()
{
    return new TwoArgNode(operation);
}
