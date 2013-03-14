#include "cell.h"

Cell::Cell() :
    anthill(false),
    scent(0)
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

int Cell::getScent() const
{
    return scent;
}

bool Cell::addPheremone(QSharedPointer<Pheremone> otherPheremone)
{
    if (pheremone.isNull() || pheremone->getStrength() < otherPheremone->getStrength()) {
        this->pheremone = otherPheremone;
        return true;
    }
    return false;
}

void Cell::update()
{
    if (!pheremone.isNull()) {
        if (!pheremone->update()) {
            pheremone = QSP<Pheremone>();
        }
    }
}

void Cell::clear()
{
    pheremone = QSP<Pheremone>();
    anthill = false;
    food = QSP<Food>();
}

void Cell::reset()
{
    pheremone = QSP<Pheremone>();
}

bool Cell::hasPheremone() const
{
    return !pheremone.isNull();
}

QSP<Pheremone> Cell::getPheremone()
{
    return pheremone;
}

void Cell::setAnthill()
{
    anthill = true;
}

void Cell::setFood(QSharedPointer<Food> food)
{
    this->food = food;
}

void Cell::setScent(int val)
{
    scent = val;
}
