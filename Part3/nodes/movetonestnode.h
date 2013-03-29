#ifndef MOVETONESTNODE_H
#define MOVETONESTNODE_H
#include "antnode.h"

class MoveToNestNode : public AntNode
{
public:
    MoveToNestNode();

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);
    virtual QString toString();

protected:
    virtual MoveToNestNode* copy();

};

#endif // MOVETONESTNODE_H
