#include "singleargnode.h"

SingleArgNode::SingleArgNode(QSP<MathNode> child, SingleArgOp operation) :
    operation(operation)
{
    children.append(child);
}

SingleArgNode::~SingleArgNode()
{
}

SingleArgNode::SingleArgNode(SingleArgOp operation) :
    operation(operation)
{}

double SingleArgNode::eval(double var)
{
    return operation(children.at(0).staticCast<MathNode>()->eval(var));
}

QString SingleArgNode::toString()
{
    return getSymbolForSingleArgOp(operation).arg(children.at(0)->toString());
}

SingleArgNode *SingleArgNode::copy()
{
    return new SingleArgNode(operation);
}
