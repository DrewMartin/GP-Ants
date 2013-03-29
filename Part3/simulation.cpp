#include "simulation.h"

#include "options.h"
#include <QTime>
#include <QThread>
#include <QDebug>

Simulation::Simulation(int maxSteps) :
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

void Simulation::setScene(QSharedPointer<QGraphicsScene> scene)
{
    this->scene = scene;
}

void Simulation::run()
{
    if (behaviour.isNull())
        return;

    stopped.set(false);
    int turn = 0;
    QTime timer;
    int row, col, i, antCount = Options::getInstance()->getAntCount();
    QPoint loc;
    QImage background;
    QSP<GPAnt> ant;
    QList<QSP<GPAnt> > ants;
    bool foodCleared = false, hadFood, anyAntHasFood;
    setupSim();

    for (i = 0; i < antCount; i++) {
        loc = QPoint(MAX_X/2, MAX_Y/2);
        ant = QSP<GPAnt>(new GPAnt(loc));
        ants.append(ant);
        if (!scene.isNull())
            scene->addItem(ant->getGraphicsItem());
    }
    score = 0;

    while (!stopped.get() && !foodCleared && (maxSteps <= 0 || turn < maxSteps)) {
        turn++;
        if (!scene.isNull())
            timer.restart();

        for (row = 0; row < MAX_X; row++)
            for(col = 0; col < MAX_Y; col++)
                cells[row][col].update();

        anyAntHasFood = false;
        for (i = 0; i < ants.length(); i++) {
            hadFood = ants.at(i)->antHasFood();
            behaviour->eval(cells, ants.at(i));
            if (ants.at(i)->antHasFood())
                anyAntHasFood = true;
            else if (hadFood)
                // had food and dropped it
                score++;
        }

        if (!scene.isNull()) {
            background = QImage(MAX_X, MAX_Y, QImage::Format_ARGB32_Premultiplied);
            background.fill(Qt::white);
            for (row = 0; row < MAX_X; row++){
                for(col = 0; col < MAX_Y; col++) {
                    i = (cells[row][col].getPheremone()*255/MAX_PHEROMONE) / (1.0 - cells[row][col].getScent());
                    background.setPixel(col, row, qRgba(0, i, 0, i));
                }
            }
            scene->setBackgroundBrush(background);
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

        if (!scene.isNull()) {
            QThread::msleep(qMax(0, 33 - timer.elapsed()));
        }
    }

    if (maxSteps > 0 && turn < maxSteps)
        score += maxSteps - turn;
    doneRunning = true;
}


bool Simulation::isDone()
{
    return doneRunning;
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

void Simulation::reset()
{
    for (int i = 0; i < food.length(); i++)
        food.at(i)->reset();

    for (int row = 0; row < MAX_X; row++)
        for(int col = 0; col < MAX_Y; col++)
            cells[row][col].reset();
}

void Simulation::setupSim()
{
    preClear();

    QPoint loc(MAX_X/2, MAX_Y/2);
    if (!scene.isNull()){
        scene->clear();
        QGraphicsEllipseItem *anthill = new QGraphicsEllipseItem(LOC_HELPER(loc, ANTHILL_RAD));
        anthill->setPen(QPen(Qt::magenta));
        anthill->setBrush(QBrush(Qt::magenta));
        scene->addItem(anthill);
    }

    CIRCLE_LOOP_HELPER(MAX_X/2, MAX_Y/2,ANTHILL_RAD)
            cells[row][col].setAnthill();

    QList<QPair<QPoint, int> > foodLocs = Options::getInstance()->getFood();
    QPoint p;

    for (int i = 0; i < foodLocs.length(); i++) {
        p = foodLocs.at(i).first;
        addFood(QSP<Food>(new Food(p, foodLocs.at(i).second)));
    }

}

void Simulation::setBehaviour(QSharedPointer<AntNode> behaviour)
{
    this->behaviour = behaviour;
}

void Simulation::addFood(QSP<Food> f)
{
    QPoint point = f->getLocation();
    food.append(f);
    if (!scene.isNull())
        scene->addItem(f->getGraphicsItem());

    CIRCLE_LOOP_HELPER(point.x(), point.y(), FOOD_RAD)
            cells[row][col].setFood(f);
}

void Simulation::preClear()
{
    for (int i = 0; i < MAX_X; i++) {
        for(int j = 0; j < MAX_Y; j++) {
            cells[j][i].clear();
            cells[j][i].setScent((sq(MAX_X)/2 - sqDistance(MAX_X/2, MAX_Y/2, j, i))/((double)sq(MAX_X)/2.0));
        }
    }
    food.clear();
}
