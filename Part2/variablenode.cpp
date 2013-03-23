#include "variablenode.h"

VariableNode::VariableNode()
{
}

VariableNode::~VariableNode()
{
}

double VariableNode::eval(double var)
{
    return var;
}

QString VariableNode::toString()
{
    return "x";
}

VariableNode *VariableNode::copy()
{
    return new VariableNode();
}
