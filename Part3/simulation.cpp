#include "simulation.h"

#include "options.h"
#include <QTime>
#include <QThread>

Simulation::Simulation(int maxSteps) :
    maxSteps(maxSteps)
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

void Simulation::setScene(QSharedPointer<QGraphicsScene> scene)
{
    this->scene = scene;
}

void Simulation::run()
{
    if (behaviour.isNull())
        return;

    stopped = false;
    int turn = 0;
    QTime timer;
    int row, col, i, antCount = Options::getInstance()->getAntCount();
    QPoint loc;
    QImage background(MAX_X, MAX_Y, QImage::Format_ARGB32_Premultiplied);
    QSP<GPAnt> ant;
    QList<QSP<GPAnt> > ants;
    bool foodCleared = false, hadFood;
    setupSim();

    for (i = 0; i < antCount; i++) {
        loc = QPoint(MAX_X/2, MAX_Y/2);
        ant = QSP<GPAnt>(new GPAnt(loc));
        ants.append(ant);
        if (!scene.isNull())
            scene->addItem(ant->getGraphicsItem());
    }
    score = 0;

    while (!stopped && !foodCleared && (maxSteps <= 0 || turn < maxSteps)) {
        turn++;
        if (!scene.isNull())
            timer.start();

        for (row = 0; row < MAX_X; row++)
            for(col = 0; col < MAX_Y; col++)
                cells[row][col].update();

        for (i = 0; i < ants.length(); i++) {
            hadFood = ants.at(i)->antHasFood();
            behaviour->eval(cells, ants.at(i));
            if (hadFood && !ants.at(i)->antHasFood())
                score++;
        }

        if (!scene.isNull()) {
            background.fill(Qt::white);
            for (row = 0; row < MAX_X; row++){
                for(col = 0; col < MAX_Y; col++) {
                    i = (cells[row][col].getPheremone()*255/MAX_PHEROMONE) / (1.0 - cells[row][col].getScent());
                    background.setPixel(col, row, qRgba(0, i, 0, i));
                }
            }
            scene->setBackgroundBrush(background);
        }

        foodCleared = true;
        for (i = 0; i < food.length(); i++) {
            if (food.at(i)->hasFood()) {
                foodCleared = false;
                break;
            }
        }
        if (!scene.isNull()) {
            QThread::msleep(qMax(0, 33 - timer.elapsed()));
        }
    }

    if (maxSteps > 0)
        score += maxSteps - turn;
    doneRunning = true;
}


bool Simulation::isDone()
{
    return doneRunning;
}

int Simulation::getScore()
{
    return score;
}

void Simulation::stop()
{
    stopped = true;
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
    if (!scene.isNull()){
        scene->clear();
        QPoint loc(MAX_X/2, MAX_Y/2);
        QGraphicsEllipseItem *anthill = new QGraphicsEllipseItem(LOC_HELPER(loc, ANTHILL_RAD));
        anthill->setPen(QPen(Qt::magenta));
        anthill->setBrush(QBrush(Qt::magenta));
        scene->addItem(anthill);
    }

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
