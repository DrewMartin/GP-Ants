#include "cell.h"

double Cell::decay = 0.9;

Cell::Cell(QPoint &location) :
    Entity(location),
    anthill(false),
    color(255,255,255),
    scent(0),
    pheromone(0),
    rect(NULL)
{
}

Cell::~Cell()
{
    if (rect) {
        delete rect;
        rect = NULL;
    }
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

bool Cell::addPheremone(int amount)
{
    if (amount == 0)
        return true;
    bool hadNoPheromone = pheromone == 0;
    if (hadNoPheromone) {
        rect = new QGraphicsRectItem(location.x(), location.y(), 1, 1);
        rect->setFlag(QGraphicsItem::ItemClipsToShape);
        rect->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        rect->setPen(QColor(0,0,0,0));
        rect->setZValue(-1);
    }
    pheromone = qMin(100, pheromone + amount);
    redraw();
    return !hadNoPheromone;
}

void Cell::update()
{
    if (pheromone > 0) {
        pheromone *= decay;
        if (pheromone > 0)
            redraw();
        else {
            delete rect;
            rect = NULL;
        }
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
    return rect;
}

void Cell::reset()
{
    pheromone = 0;
    if (rect) {
        delete rect;
        rect = NULL;
    }
}

bool Cell::hasPheremone() const
{
    return pheromone > 0;
}

int Cell::getPheremone()
{
    return pheromone;
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

void Cell::setDecay(double decay)
{
    Cell::decay = decay;
}

void Cell::redraw()
{
    int newVal = qMax(0, (100 - pheromone)*255/100);
    color.setRed(newVal);
    color.setBlue(newVal);
    rect->setBrush(QBrush(color));
}
