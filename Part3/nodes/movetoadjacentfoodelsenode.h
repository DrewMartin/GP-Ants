#ifndef MOVETOADJACENTFOODELSENODE_H
#define MOVETOADJACENTFOODELSENODE_H

#include "antnode.h"

class MoveToAdjacentFoodElseNode : public AntNode
{
public:
    MoveToAdjacentFoodElseNode(QSP<AntNode> elseNode);

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);

    virtual QString toString();

protected:
    virtual MoveToAdjacentFoodElseNode* copy();

private:
    MoveToAdjacentFoodElseNode();
};

#endif // MOVETOADJACENTFOODELSENODE_H
