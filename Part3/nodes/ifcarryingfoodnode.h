#ifndef IFCARRYINGFOODNODE_H
#define IFCARRYINGFOODNODE_H

#include "antnode.h"

class IfCarryingFoodNode : public AntNode
{
public:
    IfCarryingFoodNode(QSP<AntNode> thenNode, QSP<AntNode> elseNode);

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);
    virtual QString toString();

protected:
    virtual IfCarryingFoodNode* copy();

private:
    IfCarryingFoodNode();
};

#endif // IFCARRYINGFOODNODE_H
