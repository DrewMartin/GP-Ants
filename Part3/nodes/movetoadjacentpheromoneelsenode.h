#ifndef MOVETOADJACENTPHEROMONEELSENODE_H
#define MOVETOADJACENTPHEROMONEELSENODE_H

#include "antnode.h"

class MoveToAdjacentPheromoneElseNode : public AntNode
{
public:
    MoveToAdjacentPheromoneElseNode(QSP<AntNode> elseNode);

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);

    virtual QString toString();

protected:
    virtual MoveToAdjacentPheromoneElseNode* copy();

private:
    MoveToAdjacentPheromoneElseNode();
};

#endif // MOVETOADJACENTPHEROMONEELSENODE_H
