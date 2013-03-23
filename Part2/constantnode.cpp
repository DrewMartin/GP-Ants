#include "constantnode.h"

ConstantNode::ConstantNode(int constant) :
    constant(constant)
{
}

ConstantNode::~ConstantNode()
{
}

double ConstantNode::eval(double)
{
    return constant;
}

QString ConstantNode::toString()
{
    return QString::number(constant);
}

ConstantNode* ConstantNode::copy()
{
    return new ConstantNode(constant);
}
