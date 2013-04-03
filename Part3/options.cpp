#include "options.h"

Options* Options::instance = 0;

Options::Options() :
    pheromoneDecayRate(0.9),
    antCount(100),
    pheromoneRadius(5),
    maxGenerations(51),
    elitism(10)
{
}

int Options::foodCount()
{
    return foodLocations.length();
}

Options* Options::getInstance()
{
    if (!instance)
        instance = new Options();
    return instance;
}

double Options::getPheromoneDecayRate()
{
    return pheromoneDecayRate;
}

int Options::getAntCount()
{
    return antCount;
}

int Options::getPheromoneRadius()
{
    return pheromoneRadius;
}

int Options::getMaxGenerations()
{
    return maxGenerations;
}

int Options::getElitism()
{
    return elitism;
}

const QList<QPair<QPoint, int> > &Options::getFood()
{
    return foodLocations;
}


void Options::setPheromoneDecayRate(double val)
{
    pheromoneDecayRate = val;
}

void Options::setAntCount(int val)
{
    antCount = val;
}

void Options::setPheromoneRadius(int val)
{
    pheromoneRadius = val;
}

void Options::setMaxGenerations(int val)
{
    maxGenerations = val;
}

void Options::setElitism(int val)
{
    elitism = val;
}

void Options::addFood(QPoint loc, int amount)
{
    foodLocations.append(QPair<QPoint, int>(loc, amount));
}

void Options::clearFood()
{
    foodLocations.clear();
}
