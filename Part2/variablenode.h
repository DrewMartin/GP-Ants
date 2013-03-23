#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include "mathnode.h"

class VariableNode : public MathNode
{
public:
    VariableNode();
    virtual ~VariableNode();

    virtual double eval(double var);
    virtual QString toString();
protected:
    virtual VariableNode *copy();
};

#endif // VARIABLENODE_H
