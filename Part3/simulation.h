#ifndef SIMULATION_H
#define SIMULATION_H

#include "cell.h"
#include "antnode.h"
#include <QRunnable>
#include "atomicbool.h"

class Simulation : public QRunnable
{
public:
    explicit Simulation(QSP<AntNode> behaviour, int maxSteps = 0);
    ~Simulation();

    virtual void run();

    int getScore();
    void stop();
private:
    void setupSim();
    void dropFoodAfterMove(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);
    QSP<AntNode> behaviour;
    QList<QSP<Food> > food;
    QList<QList<Cell> > cells;
    int maxSteps;
    bool doneRunning;
    int score;
    AtomicBool stopped;
    
};

#endif // SIMULATION_H
