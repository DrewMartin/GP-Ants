#ifndef IFFOODHERENODE_H
#define IFFOODHERENODE_H

#include "antnode.h"

class IfFoodHereNode : public AntNode
{
public:
    IfFoodHereNode(QSP<AntNode> thenNode, QSP<AntNode> elseNode);

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);

    virtual QString toString();

protected:
    virtual IfFoodHereNode* copy();

private:
    IfFoodHereNode();
};

#endif // IFFOODHERENODE_H
