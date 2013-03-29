#include "cell.h"

double Cell::decay = 0.9;

Cell::Cell(QPoint &location) :
    Entity(location),
    anthill(false),
    color(255,255,255),
    scent(0.0),
    pheromone(0)
{
}

Cell::~Cell()
{

}

bool Cell::hasFood() const
{
    return !food.isNull() && food->hasFood();
}

bool Cell::atAnthill() const
{
    return anthill;
}

bool Cell::takeFood()
{
    if (!hasFood())
        return false;

    food->takeFood();
    return true;
}

double Cell::getScent() const
{
    return scent;
}

bool Cell::addPheremone(int amount)
{
    if (amount == 0)
        return true;
    pheromone = qMin(MAX_PHEROMONE, pheromone + amount);
    return false;
}

void Cell::update()
{
    if (pheromone > 0) {
        pheromone *= decay;
    }
}

void Cell::clear()
{
    reset();
    anthill = false;
    food = QSP<Food>();
}

QGraphicsItem *Cell::getGraphicsItem()
{
    return NULL;
}

void Cell::reset()
{
    pheromone = 0;

}

bool Cell::hasPheremone() const
{
    return pheromone > 0;
}

double Cell::getPheremone()
{
    return pheromone * (1.0 - scent);
}

void Cell::setAnthill()
{
    anthill = true;
}

void Cell::setFood(QSharedPointer<Food> food)
{
    this->food = food;
}


void Cell::setScent(double val)
{
    scent = val;
}

void Cell::setDecay(double decay)
{
    Cell::decay = decay;
}

