#ifndef CELL_H
#define CELL_H

#include "food.h"
#include "pheremone.h"

class Cell
{
public:
    Cell();

    bool hasFood() const;
    bool atAnthill() const;
    bool takeFood();
    int getScent() const;

    bool addPheremone(QSP<Pheremone> pheremone);
    void update();
    void clear();
    void reset();

    bool hasPheremone() const;
    QSP<Pheremone> getPheremone();

    void setAnthill();
    void setFood(QSP<Food> food);
    void setScent(int val);

private:
    QSP<Food> food;
    QSP<Pheremone> pheremone;
    bool anthill;
    int scent;
};

#endif // CELL_H
