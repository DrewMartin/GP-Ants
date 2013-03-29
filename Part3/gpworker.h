#ifndef GPWORKER_H
#define GPWORKER_H

#include <QRunnable>
#include "blockingcounter.h"
#include "blockingqueue.h"
#include "antnode.h"
#include "simulation.h"

class GPWorker : public QRunnable
{

public:
    explicit GPWorker(BlockingQueue<int> *work,
                      BlockingQueue<QSP<AntNode> > *nextGen,
                      BlockingCounter *counter,
                      QList<QSharedPointer<AntNode> > *pop);


    virtual void run();

private:
    QSP<AntNode> tournamentSelect();
    BlockingQueue<int> *work;
    BlockingQueue<QSP<AntNode> > *nextGen;
    BlockingCounter *counter;
    QList<QSharedPointer<AntNode> > *pop;
    
};

#endif // GPWORKER_H
