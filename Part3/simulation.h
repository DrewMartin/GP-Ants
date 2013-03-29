#ifndef SIMULATION_H
#define SIMULATION_H

#include "cell.h"
#include <QGraphicsScene>
#include "antnode.h"
#include <QRunnable>
#include "atomicbool.h"

class Simulation : public QRunnable
{
public:
    explicit Simulation(int maxSteps = 0);
    ~Simulation();

    void setScene(QSP<QGraphicsScene> scene);

    virtual void run();

    bool isDone();
    int getScore();
    void stop();
    void setupSim();
    void setBehaviour(QSP<AntNode> behaviour);
    void addFood(QSP<Food> f);
    void preClear();
    void reset();
private:
    void dropFoodAfterMove(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant);
    QSP<QGraphicsScene> scene;
    QSP<AntNode> behaviour;
    QList<QSP<Food> > food;
    QList<QList<Cell> > cells;
    int maxSteps;
    bool doneRunning;
    int score;
    AtomicBool stopped;
    
};

#endif // SIMULATION_H
