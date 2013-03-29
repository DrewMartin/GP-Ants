#ifndef MOVERANDOMNODE_H
#define MOVERANDOMNODE_H

#include "antnode.h"

class MoveRandomNode : public AntNode
{
public:
    MoveRandomNode();

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);
    virtual QString toString();

protected:
    virtual MoveRandomNode* copy();

};

#endif // MOVERANDOMNODE_H
