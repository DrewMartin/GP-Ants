#ifndef SIMULATION_H
#define SIMULATION_H

#include "cell.h"
#include <QGraphicsScene>
#include "antnode.h"
#include <QRunnable>

class Simulation : public QRunnable
{
public:
    explicit Simulation(int maxSteps = 0);

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
    QSP<QGraphicsScene> scene;
    QSP<AntNode> behaviour;
    QList<QSP<Food> > food;
    QList<QList<Cell> > cells;
    int maxSteps;
    bool doneRunning;
    int score;
    bool stopped;
    
};

#endif // SIMULATION_H
