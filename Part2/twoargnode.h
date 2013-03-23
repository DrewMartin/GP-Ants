#ifndef TWOARGNODE_H
#define TWOARGNODE_H

#include "mathnode.h"

class TwoArgNode : public MathNode
{
public:
    TwoArgNode(QSP<MathNode> left, QSP<MathNode> right, TwoArgOp operation);
    virtual ~TwoArgNode();

    virtual double eval(double var);
    virtual QString toString();

protected:
    virtual TwoArgNode *copy();

private:
    TwoArgNode(TwoArgOp operation);
    TwoArgOp operation;
};

#endif // TWOARGNODE_H
