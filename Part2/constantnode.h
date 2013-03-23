#ifndef CONSTANTNODE_H
#define CONSTANTNODE_H

#include "mathnode.h"

class ConstantNode : public MathNode
{
public:
    ConstantNode(int constant);
    virtual ~ConstantNode();

    virtual double eval(double var);
    virtual QString toString();

protected:
    virtual ConstantNode *copy();
private:
    int constant;
};

#endif // CONSTANTNODE_H
