#ifndef DROPPHEROMONENODE_H
#define DROPPHEROMONENODE_H

#include "antnode.h"

class DropPheromoneNode : public AntNode
{
public:
    DropPheromoneNode();

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);

    virtual QString toString();

protected:
    virtual DropPheromoneNode *copy();
};

#endif // DROPPHEROMONENODE_H
