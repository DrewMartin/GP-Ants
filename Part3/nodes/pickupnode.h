#ifndef PICKUPNODE_H
#define PICKUPNODE_H

#include "antnode.h"

class PickUpNode : public AntNode
{
public:
    PickUpNode();

    virtual void eval(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);

    virtual QString toString();

protected:
    virtual PickUpNode* copy();

};

#endif // PICKUPNODE_H
