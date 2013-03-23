#ifndef SINGLEARGNODE_H
#define SINGLEARGNODE_H

#include "mathnode.h"

class SingleArgNode : public MathNode
{
public:
    SingleArgNode(QSP<MathNode> child, SingleArgOp operation);
    virtual ~SingleArgNode();

    virtual double eval(double var);
    virtual QString toString();


protected:

    virtual SingleArgNode *copy() ;

private:
    SingleArgNode(SingleArgOp operation);
    SingleArgOp operation;
};

#endif // SINGLEARGNODE_H
