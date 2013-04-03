#include "simulation.h"

#include "options.h"
#include <QTime>
#include <QThread>
#include <QDebug>

Simulation::Simulation(QSharedPointer<AntNode> behaviour, int maxSteps) :
    behaviour(behaviour),
    maxSteps(maxSteps),
    stopped(false)
{
    QPoint loc;
    QList<Cell> thisRow;
    for (int row = 0; row < MAX_Y; row++) {
        thisRow = QList<Cell>();
        for (int col = 0; col < MAX_X; col++) {
            loc = QPoint(col, row);
            thisRow.append(Cell(loc));
        }
        cells.append(thisRow);
    }
    setAutoDelete(false);
}

Simulation::~Simulation()
{
}


void Simulation::dropFoodAfterMove(QList<QList<Cell> > &cells, const QSharedPointer<GPAnt> &ant)
{
    if (!ant->antHasFood())
        return;

    QPoint p = ant->getLocation();
    if (cells[p.y()][p.x()].atAnthill())
        ant->dropFood();
}

void Simulation::run()
{
    if (behaviour.isNull())
        return;

    stopped.set(false);
    int turn = 0;
    int row, col, i, antCount = Options::getInstance()->getAntCount();
    QPoint loc;
    QSP<GPAnt> ant;
    QList<QSP<GPAnt> > ants;
    bool foodCleared = false, hadFood, anyAntHasFood;
    setupSim();

    for (i = 0; i < antCount; i++) {
        loc = QPoint(MAX_X/2, MAX_Y/2);
        ant = QSP<GPAnt>(new GPAnt(loc));
        ants.append(ant);
    }
    score = 0;

    while (!stopped.get() && !foodCleared && (maxSteps <= 0 || turn < maxSteps)) {
        turn++;

        for (row = 0; row < MAX_X; row++)
            for(col = 0; col < MAX_Y; col++)
                cells[row][col].update();

        anyAntHasFood = false;
        for (i = 0; i < ants.length(); i++) {
            hadFood = ants.at(i)->antHasFood();
            behaviour->eval(cells, ants.at(i));
            ants.at(i)->moveForward();
            dropFoodAfterMove(cells, ants.at(i));
            if (ants.at(i)->antHasFood())
                anyAntHasFood = true;
            else if (hadFood)
                // had food and dropped it
                score++;
        }

        if (!anyAntHasFood) {
            foodCleared = true;
            for (i = 0; i < food.length(); i++) {
                if (food.at(i)->hasFood()) {
                    foodCleared = false;
                    break;
                }
            }
        }
    }

    if (maxSteps > 0 && turn < maxSteps)
        score += maxSteps - turn;
    doneRunning = true;
}

int Simulation::getScore()
{
    if (!doneRunning)
        throw "Not yet scored";
    return score;
}

void Simulation::stop()
{
    stopped.set(true);
}

void Simulation::setupSim()
{
    for (int i = 0; i < MAX_X; i++) {
        for(int j = 0; j < MAX_Y; j++) {
            cells[j][i].clear();
            cells[j][i].setScent((sq(MAX_X)/2 - sqDistance(MAX_X/2, MAX_Y/2, j, i))/((double)sq(MAX_X)/2.0));
        }
    }
    food.clear();

    CIRCLE_LOOP_HELPER(MAX_X/2, MAX_Y/2,ANTHILL_RAD)
            cells[row][col].setAnthill();

    QList<QPair<QPoint, int> > foodLocs = Options::getInstance()->getFood();
    QPoint p;
    QSP<Food> f;

    for (int i = 0; i < foodLocs.length(); i++) {
        p = foodLocs.at(i).first;
        f = QSP<Food>(new Food(p, foodLocs.at(i).second));
        food.append(f);

        CIRCLE_LOOP_HELPER(p.x(), p.y(), FOOD_RAD)
                cells[row][col].setFood(f);
    }

}

